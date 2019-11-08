#include "Upsampler.h"
#include "Filter.h"
#include <stdio.h>

#define	TAPNUM	65
Upsampler::Upsampler(int inSamplerate,int outSamplerate)
{
	int i;
	mInSamplerate=inSamplerate;
	mOutSamplerate=outSamplerate;
	mAccu=0;
	mCnt=0;

	mFilter=new Filter(outSamplerate,TAPNUM,inSamplerate,0);
	mFilter->apply_blackman();
	
}

int Upsampler::process(tIQSamplesBlock* input,tIQSamplesBlock* output)
{
	int i,j,k;

	output->sampleRate=mOutSamplerate;
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
			output->pData[j].real=0;
			output->pData[j].imag=0;
			j++;
			mAccu+=mInSamplerate;
		}
		mAccu-=mOutSamplerate;
		mFilter->process(&output->pData[k],&output->pData[k],j-k);
	}
	output->sampleNum=j;
	
}
