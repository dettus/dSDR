#ifndef SIMPLEFFT_H
#define	SIMPLEFFT_H

class SimpleFft
{
	public:
		SimpleFft(int fftsize);
		void process(signed short* input);
		void getResult(signed short *output,bool flipSpectrum=true);
		void getResult(double *output,bool flipSpectrum=true);
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
