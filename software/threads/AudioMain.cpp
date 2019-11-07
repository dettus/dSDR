#include "AudioMain.h"
AudioMain::AudioMain()
{
	mBuffer.clear();
	mAudioIODevice=new AudioIODevice(nullptr);
	mAudioIODevice->start();
	mFormat.setSampleSize(16);
	mFormat.setChannelCount(2);
	mFormat.setSampleRate(192000);
	mFormat.setCodec("audio/pcm");
	mFormat.setByteOrder(QAudioFormat::LittleEndian);
	mFormat.setSampleType(QAudioFormat::SignedInt);

	mAudioOutput=new QAudioOutput(mFormat);
	mAudioOutput->start(mAudioIODevice);		
	{
		QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
		

		if (info.isFormatSupported(mFormat)) printf("\x1b[1;34m format supported \x1b[0m\n");
	}

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
		QThread::msleep(100);
		mMutex.lock();
		if (format!=mFormat)
		{	
//			mAudioIODevice->setAudioConfig(16,mFormat.channelCount(),mFormat.sampleRate());
//			mAudioIODevice->start();
		}
		mAudioIODevice->newSamples((signed short*)mBuffer.data(),mBuffer.length()/sizeof(short));
//		printf("AUDIO len:%d\n",mBuffer.length());
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

