#include "TDummy.h"

#define	DUMMY_SAMPLERATE	2048000
#define	DUMMY_FREQUENCY		101200000
#define	DUMMY_GAINCB		123

TDummy::TDummy()
{
	mStopped=false;
	f=fopen("/dev/urandom","rb");
	mWidget=nullptr;
	mSamplesBuf=new signed short[2*DUMMY_SAMPLERATE];
}
void TDummy::stop()
{
	mStopped=true;
}
QWidget* TDummy::getWidget()
{
	return mWidget;
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
bool TDummy:setGain(int gaincB)
{
	return (gaincB==DUMMY_GAIN);
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
	while (!mStopped)
	{
		int n;
		QThread::msleep(1000);
		if (feof(f))
		{
			fseek(f,0,SEEK_SET);
		}
		n=fread(iqsamples,sizeof(short),2*DUMMY_SAMPLERATE,f);
		
		
		//mPSignalSink(iqsamples,n);
	}
}

