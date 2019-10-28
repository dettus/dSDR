#include "TunerMain.h"
TunerMain::TunerMain()
{
	mMutex.unlock();	
}
void TunerMain::stop()
{
	mStopped=true;
}
void TunerMain::run()
{
	while (!mStopped)
	{
		QThread::msleep(1000);
	}
}
