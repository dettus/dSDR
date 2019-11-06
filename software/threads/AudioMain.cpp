#include "AudioMain.h"
AudioMain::AudioMain()
{
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
	if (format!=mFormat)
	{
		mFormat=format;
	}	
}
void AudioMain::onNewPcmSamples(signed short* pcmBuf,int num)
{
	static FILE *f=nullptr;
	if (f==nullptr) f=fopen("audioout.pcm","wb");

	fwrite(pcmBuf,sizeof(short),num,f);
}
