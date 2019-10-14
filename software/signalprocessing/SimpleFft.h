#ifndef SIMPLEFFT_H
#define	SIMPLEFFT_H

class SimpleFFT
{
	public:
		SimpleFft(int fftsize);
		void process(signed short* input);
		void getResult(signed short &output,bool flipSpectrum=true);
		void getResult(double &output,bool flipSpectrum=true);
		void getSpectrum(double &output,bool flipSpectrum=true);
		int getSize();

	private:
		double	*twiddle_r;
		double	*twiddle_i;

		double	*tmp_r;
		double	*tmp_i;	

		int mFftSize;
		int mSigmas;
};


#endif
