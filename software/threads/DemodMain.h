#ifndef	DEMODMAIN_H
#define	DEMODMAIN_H
#include <QThread>
#include <QMutex>
#include "DataTypes.h"
#include "DemodWidget.h"
class DemodMain: public QThread
{
	Q_OBJECT

	public:
		DemodMain(DemodWidget* demodWidget);
		void onNewSamples(tIQSamplesBlock* pSamples);
		void stop();
		void setDemodFreq(int freqHz);
		QWidget* getDemodWidget() {return mDemodWidget;};
		
	protected:
		void run();
	
	private:
		bool mStopped=false;
		QMutex mMutex;
		tSComplex *samples;
		tIQSamplesBlock mIqSamples;
		DemodWidget *mDemodWidget;
		int mFrequency=0;
};
#endif
