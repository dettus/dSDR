#include "AudioMain.h"
AudioMain::AudioMain()
{
//	mAudioDeviceInfo=QAudioDeviceInfo::defaultOutputDevice();
	mAudioIODevice=new AudioIODevice(this);
//	mAudioIODevice->setAudioDevice(QAudioDeviceInfo::defaultOutputDevice());
	mAudioIODevice->start();
	mBuffer.clear();
}
void AudioMain::stop()
{
	mStopped=true;
}
void AudioMain::run()
{
	QAudioFormat format;
	format=mFormat;
	while (!mStopped)
	{
		QThread::msleep(1000);
		mMutex.lock();
		if (format!=mFormat)
		{	
			mAudioIODevice->setAudioConfig(16,mFormat.channelCount(),mFormat.sampleRate());
			mAudioIODevice->start();
		}
		mAudioIODevice->newSamples((signed short*)mBuffer.data(),mBuffer.length()/sizeof(short));
		printf("AUDIO len:%d\n",mBuffer.length());
		mBuffer.clear();
		mMutex.unlock();
		format=mFormat;
	}
}
void AudioMain::setAudioFormat(QAudioFormat format)
{
	mMutex.lock();
	if (format!=mFormat && format.sampleRate())
	{
		mFormat=format;
	}
	mMutex.unlock();
		
}
void AudioMain::onNewPcmSamples(signed short* pcmBuf,int num)
{
	mMutex.lock();
	mBuffer.append((char*)pcmBuf,num*sizeof(short));
	mMutex.unlock();
}

