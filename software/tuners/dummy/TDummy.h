#ifndef	TDUMMY_H
#define	TDUMMY_H
#include <QWidget>
#include <QThread>
#include <QLabel>
#include <stdio.h>

#include "Tuners.h"
class TDummy : public QThread, public Tuners
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
	
	protected:
		void run();
	
	private:
		bool mStopped;
		// Psignalsink mPSignalSink();
		FILE *f;
		QLabel *mLabel;
		signed short *mSamplesBuf;
};
#endif
