#ifndef	DOWNSAMPLER_H
#define	DOWNSAMPLER_H
#include "DataTypes.h"
// the purpose of this class is to provide a downsampler
//
// $Id$

class Downsampler
{
	public:
		Downsampler(int inSamplerate,int outSamplerate,int bandwidth);
		int process(tIQSamplesBlock* input,tIQSamplesBlock* output);
		
	private:
		int mInSamplerate=0;
		int mOutSamplerate=0;
		int mAccu=0;
		int mCnt=0;
		double *mTaps1=nullptr;
		double *mTaps2=nullptr;
		int mTapNum1=0;
		int mTapNum2=0;
		double mSum1=0;
		double mSum2=0;

		int mBufLen=2048;
		int mBufIdx=0;
		tSComplex *mBuf=nullptr;
		
		
};

#endif
