#include "TDummy.h"
#include "CBDummy.h"
#include <QFileDialog>
#include <QLabel>

#define	DUMMY_SAMPLERATE	2048000
#define	DUMMY_FREQUENCY		101200000
#define	DUMMY_GAINCB		123

TDummy::TDummy()
{
	mStopped=false;
	f=nullptr;
	mWDummy=new WDummy();
	mWDummy->setCallback(this);
	mSamplesBuf=new signed short[2*DUMMY_SAMPLERATE];
}
void TDummy::stop()
{
	mStopped=true;
}
QWidget* TDummy::getWidget()
{
	return (QWidget*)mWDummy;
}
int TDummy::getFrequency()
{
	return DUMMY_FREQUENCY;
}
int TDummy::getGain()
{
	return DUMMY_GAINCB;
}
int TDummy::getSamplerate()
{
	return DUMMY_SAMPLERATE;
}
bool TDummy::setFrequency(int freqHz)
{
	return (freqHz==DUMMY_FREQUENCY);
}
bool TDummy::setGain(int gaincB)
{
	return (gaincB==DUMMY_GAINCB);
}
bool TDummy::setSamplerate(int samplerate)
{
	return (samplerate==DUMMY_SAMPLERATE);
}
void TDummy::gainUp()
{

}
void TDummy::gainDown()
{

}

void TDummy::run()
{
	int cnt=0;
	while (!mStopped)
	{
		int n;
		char tmp[8];
		QThread::msleep(1000);
		if (f!=nullptr && mStarted)
		{
			if (feof(f))
			{
				fseek(f,0,SEEK_SET);
			}
			n=fread(mSamplesBuf,sizeof(tSComplex),DUMMY_SAMPLERATE,f);
			cnt+=n;
			printf("%d\n",cnt);


			if (mSink!=nullptr)
			{
				mSink->onNewSamples((tSComplex*)mSamplesBuf,n);
			}	
		}	
		//mPSignalSink(iqsamples,n);
	}
}
void TDummy::startStop(bool started)
{
	mStarted=started;
}
void TDummy::fileOpen(char* filename)
{
	if (f!=nullptr) fclose(f);
	f=fopen(filename,"rb");
}
