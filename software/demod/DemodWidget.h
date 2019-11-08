#ifndef	DEMODWIDGET_H
#define	DEMODWIDGET_H

#include <QtWidgets>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QMutex>
#include "DataTypes.h"
#include "Downsampler.h"
#include "SimpleShifter.h"
#include "Demod.h"


class DemodWidget: public QWidget
{
	Q_OBJECT

	public:
		DemodWidget(QWidget *parent=nullptr);
		QSize sizeHint();
		void onNewSamples(tIQSamplesBlock *pSamples,signed short* pcmBuf,int pcmBufSize,int* pcmNum,int* sampleRate);
		void setDemodFrequency(int freqHz);

		void getDemodParams(int* demodFreq,int* demodBW,int* raster,bool* demodOn)
		{
			mMutex.lock();
			if (mDemodMode==0 || demod_modules[mDemodMode]==nullptr)
			{
				*demodFreq=0;
				*demodBW=0;
				*raster=0;
				*demodOn=false;
			} else {
				*demodFreq=mDemodFreq;
				*demodBW=demod_modules[mDemodMode]->getBandwidth();
				*raster=demod_modules[mDemodMode]->getRaster();
				*demodOn=true;
			}
			mMutex.unlock();
		};
	
	public slots:
		void handleToggled();

	private:
		QMutex	mMutex;
		QVBoxLayout *mVLayout=nullptr;	
		QHBoxLayout *mHLayout=nullptr;
		QStackedLayout *mStackedLayout=nullptr;
		#define	BUTTON_NUM	6
		QRadioButton *demod_buttons[BUTTON_NUM]={nullptr};
		QWidget	*demod_labels[BUTTON_NUM];
		Demod   *demod_modules[BUTTON_NUM]={nullptr};
		int mInSamplerate=0;
		int mOutSamplerate=0;
		int mDemodFreq=0;
		int mShiftFreq=0;
		int mBandwidth=192000;
		

		int mDemodMode=0;

		SimpleShifter *mSimpleShifter=nullptr;
		Downsampler *mDownsampler=nullptr;
	
};
#endif
