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
	mTapNum2=mTapNum1+1;
	
	mTaps1=new double[tapNum+mTapNum1];
	mTaps2=new double[tapNum+mTapNum2];
	

	Filter::generate_lowpass(taps,tapNum,bandwidth);
	Filter::apply_hamming(taps,tapNum);

	mSum1=mSum2=0;

	// so... the lowpass filter and the decimation can in fact be performed with a single
	// summation. this has the advantage that the output samplerate is the driving factor
	// of the mips performance.
	for (i=0;i<tapNum+mTapNum1;i++)
	{
		mTaps1[i]=0;
		for (j=0;j<mTapNum1;j++)
		{
			if ((i-j)>=0 && (i-j)<tapNum) mTaps1[i]+=taps[i-j];
		}
		mSum1+=mTaps1[i];
	}
	for (i=0;i<tapNum+mTapNum2;i++)
	{
		mTaps2[i]=0;
		for (j=0;j<mTapNum2;j++)
		{
			if ((i-j)>=0 && (i-j)<tapNum) mTaps2[i]+=taps[i-j];
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
			double *pTaps;
			int len;
			double sum;
			int ridx;
			double sumre;
			double sumim;
			

			if (mCnt==mTapNum1)
			{
				len=mTapNum1+251;
				pTaps=mTaps1;
				sum=mSum1;
			} else {
				len=mTapNum2+251;
				pTaps=mTaps1;
				sum=mSum2;
			}
			
			ridx=mBufIdx;
			for (i=0;i<len;i++)
			{
				if (ridx==0) ridx=mBufLen;
				ridx--;

				sumre+=pTaps[i]*mBuf[ridx].real;	
				sumim+=pTaps[i]*mBuf[ridx].imag;	
			}
			sumre/=(sum*mCnt);
			sumim/=(sum*mCnt);
			pOutput->pData[outidx].real=sumre;
			pOutput->pData[outidx].imag=sumim;

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
