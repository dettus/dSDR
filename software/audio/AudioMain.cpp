#include "AudioMain.h"
AudioMain::AudioMain()
{
	mMutex.unlock();	
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
