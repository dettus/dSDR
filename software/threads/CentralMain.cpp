#include "CentralMain.h"
CentralMain::CentralMain()
{
	mMutex.unlock();	
}
void CentralMain::stop()
{
	mStopped=true;
}
void CentralMain::run()
{
	while (!mStopped)
	{
		QThread::msleep(1000);
	}
}
