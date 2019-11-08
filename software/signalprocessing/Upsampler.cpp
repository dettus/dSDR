#include "Upsampler.h"
#include "Filter.h"
#include <stdio.h>

#define	TAPNUM	251
Upsampler::Upsampler(int inSamplerate,int outSamplerate)
{
	int i;
	mInSamplerate=inSamplerate;
	mOutSamplerate=outSamplerate;
	mAccu=0;
	mCnt=0;

	mTapNum=TAPNUM;
	mBufIdx=0;
	mBuf=new tSComplex[mBufLen];
	for (i=0;i<mBufLen;i++)
	{
		mBuf[i].real=0;
		mBuf[i].imag=0;
	}

	mTaps=new double[TAPNUM];
	mFilter=new Filter(outSamplerate,TAPNUM,inSamplerate/2,0);
	
}

int Upsampler::process(tIQSamplesBlock* input,tIQSamplesBlock* output)
{
	int i,j,k;

	output->sampleRate=outSamplerate;
	output->centerFreq=input->centerFreq;
	output->gain=input->gain;

	j=0;
	for (i=0;i<input->sampleNum;i++)
	{
		k=j;
		output->pData[j++]=input->pData[i];
		mAccu+=mInSamplerate;
		while (mAccu<mOutSamplerate)
		{
			output->pData[j++].real=0;
			output->pData[j++].imag=0;
			mAccu+=mInSamplerate;
		}
		mAccu-=mOutSamplerate;
		Filter->process(&output->pData[k],&output->pData[k],k-j);
	}
	output->sampleNum=j;
	
}
