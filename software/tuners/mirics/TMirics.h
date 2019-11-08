#ifndef	TMIRICS_H
#define	TMIRICS_H

#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "mirsdrapi-rsp.h"

#include "DataTypes.h"
#include "Tuners.h"
// the purpose of this class is to provide an interface to the mirics sdrplay tuners

class TMirics: public Tuners
{
	Q_OBJECT

	public:
		TMirics(QWidget* parent=nullptr);
		~TMirics();
		void initialize();
		void process();
		int timeToWait() {return 100;}
		int minValue() {return -2048;}
		int maxValue() {return  2048;}
		void getSamples(tIQSamplesBlock *pIQSamplesBlock);
		int getSampleRate();
		int getFrequency();

		bool setFrequency(int freqHz);
		bool setGain(int gainCB);
		bool gainUp();
		bool gainDown();

	
		// callbacks for the mirics tuner
//		static void static_gainCallback(unsigned int gRdB, unsigned int lnaGRdB, void *cbContext);
//
//		static void static_streamCallback(short *xi, short *xq, unsigned int firstSampleNum,
//				int grChanged, int rfChanged, int fsChanged, unsigned int numSamples,
//				unsigned int reset, unsigned int hwRemoved, void *cbContext);
		void gainCallback(unsigned int gRdB, unsigned int lnaGRdB);
		void streamCallback(short *xi, short *xq, unsigned int firstSampleNum,
			int grChanged, int rfChanged, int fsChanged, unsigned int numSamples,
			unsigned int reset, unsigned int hwRemoved);

};

#endif
