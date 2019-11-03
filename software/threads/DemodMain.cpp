#include "DemodMain.h"
DemodMain::DemodMain()
{
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
