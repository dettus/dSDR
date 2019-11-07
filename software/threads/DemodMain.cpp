#include "DemodMain.h"
#define	PCMBUF_SIZE	512000
DemodMain::DemodMain(DemodWidget* demodWidget,AudioMain* audioMain)
{
	samples=new tSComplex[8192000];
	mIqSamples.pData=samples;
	mIqSamples.sampleNum=0;
	mIqSamples.sampleRate=0;
	mIqSamples.centerFreq=0;
	mIqSamples.gain=0;

	mDemodWidget=demodWidget;
	mFrequency=0;
	mAudioMain=audioMain;
	

	mPcmBuf=new signed short[PCMBUF_SIZE];	
}
void DemodMain::stop()
{
	mStopped=true;
}
void DemodMain::run()
{
	int i,j;
	int n;
	int sampleRate;
	// 1 khz sine @48000hz
//	double pcm[48]={ 0.00000,0.13329,0.26420,0.39039,0.50962,0.61975,0.71882,0.80507,0.87695,0.93318,0.97276,0.99498,0.99944,0.98607,0.95510,0.90709,0.84289,0.76365,0.67078,0.56595,0.45101,0.32802,0.19919,0.06679,-0.06679,-0.19919,-0.32802,-0.45101,-0.56595,-0.67078,-0.76365,-0.84289,-0.90709,-0.95510,-0.98607,-0.99944,-0.99498,-0.97276,-0.93318,-0.87695,-0.80507,-0.71882,-0.61975,-0.50962,-0.39039,-0.26420,-0.13329,-0.0000};
	QAudioFormat format;
	format.setSampleRate(48000);
	format.setChannelCount(2);
	format.setSampleSize(16);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	mIqSamples.sampleNum=0;
	
	while (!mStopped)
	{
		int pcmNum;
		QThread::msleep(10);
		mMutex.lock();
		n=mIqSamples.sampleNum;
		mDemodWidget->setDemodFrequency(mFrequency);
		mDemodWidget->onNewSamples(&mIqSamples,mPcmBuf,PCMBUF_SIZE,&pcmNum,&sampleRate);
		mIqSamples.sampleNum=0;
		mMutex.unlock();
		if (pcmNum)
		{
			if (sampleRate!=format.sampleRate())
			{
				format.setSampleRate(sampleRate);
				mAudioMain->setAudioFormat(format);
			}
			mAudioMain->onNewPcmSamples(mPcmBuf,pcmNum);
		}
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
