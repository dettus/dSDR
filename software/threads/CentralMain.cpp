#include <QApplication>
#include "CentralMain.h"
#include "Tuners.h"



CentralMain::CentralMain(TunerMain* tunerMain,DemodMain* demodMain,AudioMain* audioMain)
{
	int i;
	int fftsize;
	char tmp[32];
	mTunerMain=tunerMain;
	mDemodMain=demodMain;
	mAudioMain=audioMain;
	mV1Layout=new QVBoxLayout;
	mV2Layout=new QVBoxLayout;
	mV3Layout=new QVBoxLayout;
	mH1Layout=new QHBoxLayout;
	mH2Layout=new QHBoxLayout;
	mH3Layout=new QHBoxLayout;
	mainWin=new QWidget(nullptr);
	mWVolume=new WVolume(nullptr);
	mWSpectrum=new WSpectrum(nullptr);
	mRecordButton=new QPushButton("Record");
	mDemodWidget=nullptr;

	connect(mRecordButton,SIGNAL(released()),this,SLOT(handleRecord()));
	mainWin->hide();
	mDemodWidget=demodMain->getDemodWidget();

	mH3Layout->setAlignment(Qt::AlignLeft);
	mH3Layout->addWidget(new QLabel("FFT size:"));

	fftsize=256;
	for (i=0;i<8;i++)
	{
		snprintf(tmp,32,"%d",fftsize);
		bFFT[i]=new QPushButton(tmp);
		bFFT[i]->setFlat(false);
		connect(bFFT[i]   ,SIGNAL(clicked()),this,SLOT(handleFFTclicked()));
		mH3Layout->addWidget(bFFT[i]);
		fftsize*=2;
	}	

	mV3Layout->setStretch(0,1000);
	mV3Layout->setStretch(1,100);	

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
//	mWSpectrum->setFFTsize(32768);
	mWSpectrum->setFFTsize(8192);
	bFFT[5]->setFlat(true);
//	mWSpectrum->setFFTsize(4096);

	mH2Layout->addWidget(mWVolume);
	mH2Layout->addWidget(mDemodWidget);
	mH1Layout->setStretch(0,5);
	mH1Layout->setStretch(1,30);
	mV1Layout->addLayout(mH2Layout);

	mV2Layout->addWidget(tuner);
	mV2Layout->addWidget(mRecordButton);
	mH1Layout->addLayout(mV2Layout);
	mV3Layout->addWidget(mWSpectrum);
	mV3Layout->addLayout(mH3Layout);
	mH1Layout->addLayout(mV3Layout);
	mH1Layout->setStretch(0,10);
	mH1Layout->setStretch(1,30);
	mV1Layout->addLayout(mH1Layout);

	mV1Layout->setStretch(0,5);
	mV1Layout->setStretch(1,50);
	mainWin->setLayout(mV1Layout);
	mainWin->showMaximized();

	while (!mStopped)
	{
		QThread::msleep(100);
		tuner->getSamples(&iqSamples);
		if (iqSamples.sampleNum)
		{
			int demodFreq;
			int demodBw;
			int raster;
			bool demodOn;
			mDemodMain->setDemodFreq(mWSpectrum->getLastFreq());
			mDemodMain->getDemodParams(&demodFreq,&demodBw,&raster,&demodOn);
			
			mDemodMain->onNewSamples(&iqSamples);
			mWSpectrum->onNewSamples(&iqSamples);
			mWSpectrum->setDemodParams(demodFreq,demodBw,raster,demodOn);

		}
		mAudioMain->setVolume((double)mWVolume->getVolume()/255.0f);
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
void CentralMain::handleFFTclicked()
{
	int i;
	int fftsize;
	QPushButton *sender = (QPushButton*)QObject::sender();

	fftsize=256;
	for (i=0;i<8;i++)
	{
		if (sender==bFFT[i]) 
		{
			mWSpectrum->setFFTsize(fftsize);
			bFFT[i]->setFlat(true);
		} else bFFT[i]->setFlat(false);
		fftsize*=2;
	}	
}
