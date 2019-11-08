#ifndef	UPSAMPLER_H
#define	UPSAMPLER_H
#include "DataTypes.h"
#include "Filter.h"
// the purpose of this class is to provide an upsampler
// 
// $Id$

class Upsampler
{
	
	public:
		Upsampler(int inSamplerate,int outSamplerate);
		int process(tIQSamplesBlock* input,tIQSamplesBlock* output);

	private:
		int mInSamplerate=0;
		int mOutSamplerate=0;
		int mAccu=0;
		int mCnt=0;
		Filter *mFilter;
	

};
#endif
