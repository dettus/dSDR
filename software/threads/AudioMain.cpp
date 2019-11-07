#include "AudioMain.h"
AudioMain::AudioMain()
{
	mAudioDeviceInfo=QAudioDeviceInfo::defaultOutputDevice();
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
		
		mIoDevice=mAudioOutput->start();
	}
		
}
void AudioMain::onNewPcmSamples(signed short* pcmBuf,int num)
{
	if (mIoDevice!=nullptr)
		mIoDevice->write((char*)pcmBuf,num*sizeof(short));
}

