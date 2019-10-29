// this class is am implementation of the tuner class.
// its purpose is to illustrate, in the simplest way,
// how to provide widgets for new tuner hardware
//
// samples are being read from a file once every second.
// 
// $Id$

#ifndef	TDUMMY_H
#define	TDUMMY_H
#include "Tuners.h"
class TDummy: public Tuners
{
	Q_OBJECT
	
	public:
		TDummy(QWidget* parent=nullptr);
		void process();
		int timeToWait();

		void getSamples(tIQSamplesBlock *pIQSamplesBlock);
		int getSampleRate();
		int getFrequency();
		
		bool setFrequency(int freqHz);
		bool setGain(int gainCB);
		bool gainUp();
		bool gainDown();

	private:
		int mSampleRate=2048000;
		int mFrequency=87900000;
		int mGain=0;	
};
#endif
