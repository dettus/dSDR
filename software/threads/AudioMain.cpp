#include "AudioMain.h"
AudioMain::AudioMain()
{
	mAudioDeviceInfo=QAudioDeviceInfo::defaultOutputDevice();
	mAudioIODevice=new AudioIODevice();
}
void AudioMain::stop()
{
	mStopped=true;
}
void AudioMain::run()
{
	while (!mStopped)
	{
		QThread::msleep(1000);
	}
}
void AudioMain::setAudioFormat(QAudioFormat format)
{
	if (format!=mFormat && format.sampleRate())
	{
		mFormat=format;
		printf("new audio format. samplerate:%d \n",mFormat.sampleRate());
		if (mAudioOutput!=nullptr)
		{
			mAudioOutput->stop();
			delete(mAudioOutput);
		}
		printf("format supported:%d\n",(int)mAudioDeviceInfo.isFormatSupported(mFormat));
		mAudioOutput=new QAudioOutput(mAudioDeviceInfo,mFormat);
		
		mAudioOutput->start(mAudioIODevice);
	}
		
}
void AudioMain::onNewPcmSamples(signed short* pcmBuf,int num)
{
	mAudioIODevice->onNewPcmSamples(pcmBuf,num);
}

