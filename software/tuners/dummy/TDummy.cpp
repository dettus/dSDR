#include "TDummy.h"

TDummy::TDummy(QWidget* parent): Tuners(parent)
{
	
}

void TDummy::process()
{
	// read 1 second of samples from the file
}

int TDummy::timeToWait()
{
	return 1000;	// wait for 1000 milliseconds between process calls
}

void TDummy::getSamples(tIQSamplesBlock *pIQSamplesBlock)
{
	// copy samples from the buffer into the return structure
}

int TDummy::getSampleRate()
{
	return mSampleRate;
}

int TDummy::getFrequency()
{
	return mFrequency;
}

bool TDummy::setFrequency(int freqHz)
{
	// this tuner should be fixed to a specific frequency. thus, the setFrequency returns false, whenever a differnt one is requested.
	return (mFrequency==freqHz);
}
bool TDummy::setGain(int gainCB)
{
	// this tuner has only a single gain setting. 
	return (mGain==gainCB);
}
bool TDummy::gainUp()
{
	return false;
}
bool TDummy::gainDown()
{
	return false;
}

