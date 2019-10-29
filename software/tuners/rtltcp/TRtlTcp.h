// this class is am implementation of the tuner class.
// it is a client to the RTL TCP server, to be able
// to access many cheap tuners.
//
// 
// $Id$

#ifndef	TRtlTcp_H
#define	TRtlTcp_H
#include "Tuners.h"
class TRtlTcp: public Tuners
{
	Q_OBJECT
	
	public:
		TRtlTcp(QWidget* parent=nullptr);
		void initialize();
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
