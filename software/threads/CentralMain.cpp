#include <QApplication>
#include "CentralMain.h"
#include "Tuners.h"



CentralMain::CentralMain(TunerMain* tunerMain)
{
	mTunerMain=tunerMain;
	mHLayout=new QHBoxLayout;
	mVLayout=new QVBoxLayout;
	mainWin=new QWidget(nullptr);
	mWSpectrum=new WSpectrum(nullptr);
	mRecordButton=new QPushButton("Record");
	connect(mRecordButton,SIGNAL(released()),this,SLOT(handleRecord()));
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
	//mWSpectrum->setFFTsize(32768);
	mWSpectrum->setFFTsize(4096);
	mVLayout->addWidget(tuner);
	mVLayout->addWidget(mRecordButton);
	mHLayout->addLayout(mVLayout);
	mHLayout->addWidget(mWSpectrum);
	mHLayout->setStretch(0,10);
	mHLayout->setStretch(1,30);
	mainWin->setLayout(mHLayout);
	mainWin->showMaximized();

	while (!mStopped)
	{
		QThread::msleep(100);
		tuner->getSamples(&iqSamples);
		if (iqSamples.sampleNum)
		{
			mWSpectrum->onNewSamples(&iqSamples);
		}
		mLock.lock();
		if (mRecord && mFptr!=nullptr)
		{
			fwrite(iqSamples.pData,sizeof(tSComplex),iqSamples.sampleNum,mFptr);
			
		}
		mLock.unlock();
	}

	QApplication::quit();
}
void CentralMain::handleRecord()
{
	if (mRecord==false)
	{
		QString filename=QFileDialog::getSaveFileName(nullptr,"Record as...","signal.iq2048");
		mLock.lock();
		if (mFptr!=nullptr) 
		{
			fclose(mFptr);
		} 
		mFptr=fopen(filename.toLocal8Bit().data(),"wb");
		mRecord=true;
		if (mFptr!=nullptr) 
		{
			mRecordButton->setText("Stop Recording");
		}
		mLock.unlock();
	}else {
		mLock.lock();
		if (mFptr!=nullptr) 
		{
			fclose(mFptr);
		}
		mRecord=false;
		mRecordButton->setText("Record");
		mLock.unlock();
	}
}

