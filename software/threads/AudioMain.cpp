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
