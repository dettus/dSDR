#ifndef	DEMODWIDGET_H
#define	DEMODWIDGET_H

#include <QtWidgets>
#include <QRadioButton>
#include <QVBoxLayout>
#include "DataTypes.h"
#include "Downsampler.h"
#include "SimpleShifter.h"

class DemodWidget: public QWidget
{
	Q_OBJECT

	public:
		DemodWidget(QWidget *parent=nullptr);
		QSize sizeHint();
		void onNewSamples(tIQSamplesBlock* pSamples);
		void setDemodFreq(int freqHz);
	
	public slots:
		void handleToggled();

	private:
		QVBoxLayout *mVLayout=nullptr;	
		QHBoxLayout *mHLayout=nullptr;
		QStackedLayout *mStackedLayout=nullptr;
		#define	BUTTON_NUM	6
		QRadioButton *demod_buttons[BUTTON_NUM]={nullptr};
		QWidget	*demod_labels[BUTTON_NUM];
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
