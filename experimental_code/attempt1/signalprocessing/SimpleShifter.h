	#ifndef SIMPLESHIFTER_H
	#define	SIMPLESHIFTER_H
	#include "Datatypes.h"

	// the purpose of this module is to provide an easy frequency shifter.
	class SimpleShifter
	{
		public:
			SimpleShifter(int samplerate,int freqHz);
			~SimpleShifter();
			void process(tSComplex* input,int n);
			void process(tSComplex* input,tSComplex* output,int n);
			void getResult(tSComplex* output);
				

		private:
			double	*mSinLUT;
			double	*mCosLUT;
			
			tSComplex *mOutput;
			int	mTimeIdx;
		int	mSampleRate;
		int	mSampleCnt;
};

#endif
		
