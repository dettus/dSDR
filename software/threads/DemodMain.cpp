#include "DemodMain.h"
DemodMain::DemodMain()
{
	mMutex.unlock();	
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
	}
}
