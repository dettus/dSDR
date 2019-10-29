#include <QApplication>
#include "TunerMain.h"

TunerMain::TunerMain()
{
	mMutex.unlock();
	mTunerDialog=new TunerDialog();
}
void TunerMain::stop()
{
	mStopped=true;
}
void TunerMain::run()
{
	int timeToWait;
	startup();
	if (mTuner!=nullptr)
	{
		timeToWait=mTuner->timeToWait();
		while (!mStopped)
		{
			QThread::msleep(timeToWait);
			mTuner->process();
		}
	}
	QApplication::quit();
}
void TunerMain::startup()
{
	enum eTunerType tunerType;
	mTunerDialog->show();
	do
	{
		QThread::msleep(100);
		tunerType=mTunerDialog->getSelectedTunerType();

	}
	while (!mStopped && tunerType==TUNER_UNDEF);
	mTuner=mTunerDialog->getTuner();
	mTunerDialog->hide();
}
