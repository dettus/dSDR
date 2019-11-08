#ifndef	FILTER_H
#define	FILTER_H
// the purpose of this class is to provide filter functionality in the time domain.
//
// $Id$
#include "DataTypes.h"

class Filter
{
	public:
		Filter(int samplerate,int tapnum,int upper,int lower);
		~Filter();
		void process(tSComplex* input,tSComplex* output,int n);
		static void generate_bandpass(double* pTaps,int tapnum,double u,double l);
		static void generate_bandstop(double *pTaps,int tapnum,double u,double l);
		static void generate_lowpass(double *pTaps,int tapnum,double u);
		static void generate_highpass(double *pTaps,int tapnum,double l);
		static void generate_hilbert(double *pTaps,int tapnum);
		static double apply_blackman(double *pTaps,int tapnum);
		static double apply_bartlett(double *pTaps,int tapnum);
		static double apply_hamming(double *pTaps,int tapnum);
		static double apply_hanning(double *pTaps,int tapnum);
		static double apply_kaiser(double *pTaps,int tapnum,double gamma);
		static double convertFreq(int samplerate,int freq);

		double apply_blackman()	{return apply_blackman(mTaps,mTapnum);};
		double apply_bartlett()	{return apply_bartlett(mTaps,mTapnum);};
		double apply_hamming()	{return apply_hamming(mTaps,mTapnum);};
		double apply_hanning()	{return apply_hanning(mTaps,mTapnum);};
		double apply_kaiser(double gamma)	{return apply_kaiser(mTaps,mTapnum,gamma);};

	private:

		int mTapnum=0;
		int mUpperFreq=0;
		int mLowerFreq=0;
		int mSampleRate=0;
		double *mTaps=nullptr;
		tSComplex *mBuf;
		int mBufIdx=0;
		int mBufLen=0;
		
};


#endif
