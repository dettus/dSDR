#include "CentralMain.h"
#include "StartupDialog.h"
CentralMain::CentralMain()
{
	mMutex.unlock();	
	mStartupDialog=new StartupDialog(nullptr);
}
void CentralMain::stop()
{
	mStopped=true;
}
void CentralMain::run()
{
	startup();
	while (!mStopped)
	{
		QThread::msleep(1000);
	}
	QApplication::quit();
}
void CentralMain::startup()
{
	eTunerType tunerType=TUNER_UNDEF;
	do
	{
		//QThread::wait(10);
		//requestInterruption();
		msleep(10);
		tunerType=mStartupDialog->getSelectedTunerType();
	}
	while (!mStopped && tunerType==TUNER_UNDEF);
	mStartupDialog->hide();
	if (tunerType==TUNER_QUIT)
	{
		mStopped=true;
	}

}
