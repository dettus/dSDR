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
		static void generate_bandpass(double* pTaps,int tapnum,double u,double l);

	private:
		void generate_bandstop(double u,double l);
		void generate_lowpass(double u);
		void generate_highpass(double l);
		void generate_hilbert();
		double apply_blackman();
		double apply_bartlett();
		double apply_hamming();
		double apply_hanning();
		double apply_kaiser(double gamma);

		int mTapnum=0;
		int mUpperFreq=0;
		int mLowerFreq=0;
		int mSampleRate=0;
		double *mTaps=nullptr;
		
};


#endif
