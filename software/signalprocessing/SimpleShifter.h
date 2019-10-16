#ifndef SIMPLESHIFTER_H
#define	SIMPLESHIFTER_H


// the purpose of this module is to provide an easy frequency shifter.
class SimpleShifter
{
	public:
		SimpleShifter(int samplerate,int freqHz);
		void process(signed short* input,int n);
		void process(signed short* input,signed short* output,int n);
		void getResult(signed short* output);
			

	private:
		double	*mSinLUT;
		double	*mCosLUT;
		
		signed short *mOutput;
		int	mTimeIdx;
		int	mSampleRate;
		int	mSampleCnt;
};

#endif
		
