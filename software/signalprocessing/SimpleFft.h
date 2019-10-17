#ifndef SIMPLEFFT_H
#define	SIMPLEFFT_H

#include "Datatypes.h"

// the purpose of this class is to provide a first, rudimentary FFT.
// it can be instantiated with an fftsize of 256,512,1024,2048,4096,8192,16384,32768,65536
// 
// a new block of time domain input can be processed.
// since the output format is flexible, it can be returned as signed short, double.
// 
// another output possibility is the spectrum. which can be summed up over time with the addSpectrum() function.
class SimpleFft
{
	public:
		SimpleFft(int fftsize);
		void process(tSComplex* input);
		void getResult(tSComplex* output,bool flipSpectrum=true);
		void getResult(tDComplex *output,bool flipSpectrum=true);
		void getSpectrum(double *output,bool flipSpectrum=true);
		void addSpectrum(double *output,bool flipSpectrum=true);
		int getSize();

	private:
		double	*mTwiddle_r;
		double	*mTwiddle_i;

		double	*mTmp_r;
		double	*mTmp_i;	

		int mFftSize;
		int mSigmas;
		int mOmegaMask;
		int mNotMask;
		int *mPermute;
};


#endif
