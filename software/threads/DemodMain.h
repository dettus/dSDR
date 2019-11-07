#ifndef	DEMODMAIN_H
#define	DEMODMAIN_H
#include <QThread>
#include <QMutex>
#include "DataTypes.h"
#include "DemodWidget.h"
#include "AudioMain.h"
class DemodMain: public QThread
{
	Q_OBJECT

	public:
		DemodMain(DemodWidget* demodWidget,AudioMain* audioMain);
		void onNewSamples(tIQSamplesBlock* pSamples);
		void stop();
		void setDemodFreq(int freqHz);
		QWidget* getDemodWidget() {return mDemodWidget;};
		void getDemodParams(int* demodFreq,int* demodBW,bool* demodOn)
		{
			if (mDemodWidget!=nullptr)
			{
				mDemodWidget->getDemodParams(demodFreq,demodBW,demodOn);
			} else {
				*demodFreq=0;
                                *demodBW=0;
                                *demodOn=false;
			}	
		};
	protected:
		void run();
	
	private:
		bool mStopped=false;
		QMutex mMutex;
		tSComplex *samples;
		tIQSamplesBlock mIqSamples;
		DemodWidget *mDemodWidget=nullptr;
		int mFrequency=0;
		AudioMain	*mAudioMain;
		signed short *mPcmBuf;
};
#endif
