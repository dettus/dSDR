#include "SimpleShifter.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SimpleShifter::SimpleShifter(int samplerate,int freqHz)
{
	int i;
	mSinLUT=new double[samplerate];
	mCosLUT=new double[samplerate];
	mTimeIdx=0;
	for (i=0;i<samplerate;i++)
	{
		mSinLUT[i]= sin(2*M_PI*(double)i*double(freqHz)/double(samplerate));	
		mCosLUT[i]= cos(2*M_PI*(double)i*double(freqHz)/double(samplerate));	
	}
	mSampleRate=samplerate;
	mFrequency=freqHz;
}
SimpleShifter::~SimpleShifter()
{
	if (mSinLUT!=nullptr) delete(mSinLUT);
	if (mCosLUT!=nullptr) delete(mCosLUT);
	if (mOutput!=nullptr) delete(mOutput);
}
#define	CMPLX_MULR(ar,ai, br,bi)  ((ar)*(br)-(ai)*(bi))
#define	CMPLX_MULI(ar,ai, br,bi)  ((ar)*(bi)+(ai)*(br))
void SimpleShifter::process(tSComplex* input,int n)
{
	int i;
	int t;
	double yr,yi;
	if (mOutput!=nullptr)
		delete(mOutput);
	mOutput=new tSComplex[n];
	mSampleCnt=n;


	t=mTimeIdx;	
	for (i=0;i<n;i++)
	{
		yr=CMPLX_MULR(input[i].real,input[i].imag,mCosLUT[t],mSinLUT[t]);
		yi=CMPLX_MULI(input[i].real,input[i].imag,mCosLUT[t],mSinLUT[t]);
		t=(t+1)%mSampleRate;

		mOutput[i].real=yr;
		mOutput[i].imag=yi;
	}
	mTimeIdx=t;
}
void SimpleShifter::getResult(tSComplex* output)
{
	int i;
	if (mOutput!=nullptr)
	{
		for (i=0;i<mSampleCnt;i++)
		{
			output[i]=mOutput[i];
		}
	} else {
		for (i=0;i<mSampleCnt;i++)
		{
			output[i].real=0;
			output[i].imag=0;
		}
	}	
}
void SimpleShifter::process(tSComplex* input,tSComplex* output,int n)
{
	int i;
	int t;
	double xr,xi;
	double yr,yi;
	mSampleCnt=n;


	t=mTimeIdx;	
	for (i=0;i<n;i++)
	{
		xr=input[i].real;
		xi=input[i].imag;
		yr=CMPLX_MULR(xr,xi,mCosLUT[t],mSinLUT[t]);
		yi=CMPLX_MULI(xr,xi,mCosLUT[t],mSinLUT[t]);
		t=(t+1)%mSampleRate;

		output[i].real=yr;
		output[i].imag=yi;
	}
	
	mTimeIdx=t;

}

