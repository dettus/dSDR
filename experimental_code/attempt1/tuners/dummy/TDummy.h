#ifndef	TDUMMY_H
#define	TDUMMY_H
#include <QWidget>
#include <QThread>
#include <QLabel>
#include <stdio.h>

#include "Tuners.h"
#include "CBDummy.h"
#include "WDummy.h"
class TDummy : public QThread, public Tuners, public CBDummy
{
	Q_OBJECT

	public:
		TDummy();
		void stop();
		QWidget* getWidget();
		int getFrequency();
		int getGain();
		int getSamplerate();
		bool setFrequency(int freqHz);
		bool setGain(int gaincB);
		bool setSamplerate(int sampleRate);
		
		void gainUp();
		void gainDown();

		void startStop(bool started);
		void fileOpen(char* filename);
	
	protected:
		void run();
	
	private:
		bool mStopped;
		bool mStarted;
		// Psignalsink mPSignalSink();
		FILE *f=nullptr;
		signed short *mSamplesBuf;
		WDummy *mWDummy;
};
#endif
