#include "TDummy.h"
#include <QLabel>

#define	DUMMY_SAMPLERATE	2048000
#define	DUMMY_FREQUENCY		101200000
#define	DUMMY_GAINCB		123

TDummy::TDummy()
{
	mStopped=false;
	f=fopen("/dev/urandom","rb");
	mLabel=new QLabel("dummy");
	mSamplesBuf=new signed short[2*DUMMY_SAMPLERATE];
}
void TDummy::stop()
{
	mStopped=true;
}
QWidget* TDummy::getWidget()
{
	return (QWidget*)mLabel;
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
		if (feof(f))
		{
			fseek(f,0,SEEK_SET);
		}
		n=fread(mSamplesBuf,sizeof(short),2*DUMMY_SAMPLERATE,f);
		cnt+=n;
		printf("%d\n",cnt);
		snprintf(tmp,8,"%d",cnt);
		mLabel->setText(tmp);
		
		
		if (mSink!=nullptr)
		{
			mSink->onNewSamples((tSComplex*)mSamplesBuf,2*DUMMY_SAMPLERATE);
		}	
		
		//mPSignalSink(iqsamples,n);
	}
}

