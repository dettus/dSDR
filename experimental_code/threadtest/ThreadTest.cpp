#include "ThreadTest.h"
#include <stdio.h>

ThreadTest::ThreadTest(int id)
{
	mId=id;
	mStopped=false;
}
void ThreadTest::stop()
{
	mStopped=true;
}
void ThreadTest::run()
{
	while (!mStopped)
	{
		printf("%d\n",mId);
		QThread::msleep(mId*20);
	}
	mStopped=false;
	
}
