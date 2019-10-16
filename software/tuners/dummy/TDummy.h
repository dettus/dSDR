#ifndef	TDUMMY_H
#define	TDUMMY_H
#include <QWidget>
#include <QThread>
#include <stdio.h>
class TDummy : public QThread
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
		QWidget *mWidget;
		signed short *mSamplesBuf;
};
#endif
