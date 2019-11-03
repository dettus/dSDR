#include "Downsampler.h"
#include "Filter.h"

Downsampler::Downsampler(int inSamplerate,int outSamplerate,int bandwidth)
{
	int tapNum=251;
	double taps[tapNum];
	int i,j;
	mInSamplerate=inSamplerate;
	mOutSamplerate=outSamplerate;
	mAccu=0;
	mCnt=0;

	mTapNum1=(inSamplerate/outSamplerate);
	mTapNum2=tapNum1+1;
	
	mTaps1=new double[tapnum+mTapNum1];
	mTaps2=new double[tapnum+mTapNum2];
	

	Filter::generate_lowpass(taps,tapnum,bandwidth);
	Filter::apply_hamming(taps,tapnum);

	mSum1=mSum2=0;

	// so... the lowpass filter and the decimation can in fact be performed with a single
	// summation. this has the advantage that the output samplerate is the driving factor
	// of the mips performance.
	for (i=0;i<tapnum+mTapNum1;i++)
	{
		mTaps1[i]=0;
		for (j=0;j<mTapNum1;j++)
		{
			if ((i-j)>=0 && (i-j)<tapNum) mTaps1[i]+=taps[i-j];
		}
		mSum1+=mTaps1[i];
	}
	for (i=0;i<tapnum+mTapNum2;i++)
	{
		mTaps2[i]=0;
		for (j=0;j<mTapNum2;j++)
		{
			if ((i-j)>=0 && (i-j)<tapNum) mTap2[i]+=taps[i-j];
		}	
		mSum2+=mTaps2[i];
	}

	mBuf=new tSComplex[mBufLen];
	mBufIdx=0;
}
int Downsampler::process(tIQSamplesBlock* pInput,tIQSamplesBlock* pOutput)
{
	int i;
	int outidx;


	outidx=0;
	for (i=0;i<pInput->sampleNum;i++)
	{
		mBuf[mBufIdx]=pInput->pData[i];
		mBufIdx++;
		mAccu+=mInSamplerate;
		mCnt++;
		if (mAccu>=mOutSamplerate)
		{
			double *pTap;
			int len;
			double sum;
			int ridx;
			double sumre;
			double sumim;
			

			if (mCnt==mTapNum1)
			{
				len=mTapNum1+251;
				pTap=mTaps1;
				sum=mSum1;
			} else {
				len=mTapNum2+251;
				pTap=mTaps1;
				sum=mSum2;
			}
			
			ridx=mBufIdx;
			for (i=0;i<len;i++)
			{
				if (ridx==0) ridx=mBufLen;
				ridx--;

				sumre+=pTabs[i]*mBuf[ridx].real;	
				sumim+=pTabs[i]*mBuf[ridx].imag;	
			}
			sumre/=(sum*mCnt);
			sumim/=(sum*mCnt);
			pOutput->data[outidx].real=sumre;
			pOutput->data[outidx].imag=sumim;

			outidx++;
				
			
			mAccu-=mOutSamplerate;
			mCnt=0;
		}
	}
	pOutput->sampleNum=outidx;
	pOutput->sampleRate=mOutSamplerate;
	pOutput->centerFreq=pInput->centerFreq;
	pOutput->gain=pInput->gain;
}
