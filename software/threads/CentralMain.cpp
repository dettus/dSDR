#include <QApplication>
#include "CentralMain.h"
#include "Tuners.h"


CentralMain::CentralMain(TunerMain* tunerMain)
{
	mMutex.unlock();
	mTunerMain=tunerMain;
	mLayout=new QHBoxLayout;
	mainWin=new QWidget(nullptr);
	mainWin->hide();
}
void CentralMain::stop()
{
	mStopped=true;
}
void CentralMain::run()
{
	Tuners* tuner=nullptr;
	tIQSamplesBlock iqSamples;
	while (!mStopped && tuner==nullptr)
	{
		QThread::msleep(100);
		tuner=mTunerMain->getTuner();
	}
	tuner->initialize();
	mLayout->addWidget(tuner);
	mainWin->setLayout(mLayout);
	mainWin->showMaximized();
	while (!mStopped)
	{
		QThread::msleep(10);
		tuner->getSamples(&iqSamples);
		if (iqSamples.sampleNum)
			printf("sampleNum:%d\n",iqSamples.sampleNum);	
	}

	QApplication::quit();
}
