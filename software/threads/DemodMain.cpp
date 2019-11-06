#include "DemodMain.h"
DemodMain::DemodMain(DemodWidget* demodWidget)
{
	samples=new tSComplex[8192000];
	mIqSamples.pData=samples;
	mIqSamples.sampleNum=0;
	mIqSamples.sampleRate=0;
	mIqSamples.centerFreq=0;
	mIqSamples.gain=0;

	mDemodWidget=demodWidget;
	mFrequency=0;
		
}
void DemodMain::stop()
{
	mStopped=true;
}
void DemodMain::run()
{
	while (!mStopped)
	{
		QThread::msleep(1000);
		mMutex.lock();
		mDemodWidget->setDemodFrequency(mFrequency);
		mDemodWidget->onNewSamples(&mIqSamples);
		
		mIqSamples.sampleNum=0;
		mMutex.unlock();
	}
}
void DemodMain::onNewSamples(tIQSamplesBlock* pSamples)
{
	int i;
	int j;
	mMutex.lock();
	if (pSamples->sampleRate!=mIqSamples.sampleRate || pSamples->centerFreq!=mIqSamples.centerFreq )
	{
		mIqSamples.sampleNum=0;
	}
	j=mIqSamples.sampleNum;
	for (i=0;i<pSamples->sampleNum;i++)
	{
		mIqSamples.pData[j]=pSamples->pData[i];
		j++;
	}
	mIqSamples.sampleNum=j;
	mIqSamples.sampleRate=pSamples->sampleRate;
	mIqSamples.centerFreq=pSamples->centerFreq;
	mIqSamples.gain=pSamples->gain;
	mMutex.unlock();
}
void DemodMain::setDemodFreq(int freqHz)
{
	mFrequency=freqHz;
}
