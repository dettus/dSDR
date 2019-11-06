#ifndef	CENTRALMAIN_H
#define	CENTRALMAIN_H
#include <stdio.h>
#include <QThread>
#include <QMutex>
#include "TunerMain.h"
#include "DemodMain.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMutex>
#include "WSpectrum2.h"
#include "Downsampler.h"
#include "SimpleShifter.h"
class CentralMain: public QThread
{
	Q_OBJECT

	public:
		CentralMain(TunerMain *tunerMain,DemodMain *demodMain);
		void stop();
		
	protected:
		void run();
	
	private:
		bool mStopped=false;
		QMutex mMutex;

		TunerMain *mTunerMain;
		DemodMain *mDemodMain;
		QVBoxLayout *mV1Layout;
		QHBoxLayout *mHLayout;
		QVBoxLayout *mV2Layout;
		QWidget *mainWin=nullptr;
		QPushButton *mRecordButton=nullptr;
		QWidget *mDemodWidget=nullptr;
		QMutex	mLock;
		FILE *mFptr=nullptr;
		bool mRecord=false;

		WSpectrum *mWSpectrum;


// testing

	public slots:
		void handleRecord();

};
#endif
