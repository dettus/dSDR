#include "TRtlTcp.h"

TRtlTcp::TRtlTcp(QWidget* parent): Tuners(parent)
{
	
}

void TRtlTcp::process()
{
	// read 1 second of samples from the file
}

int TRtlTcp::timeToWait()
{
	return 1000;	// wait for 1000 milliseconds between process calls
}

void TRtlTcp::getSamples(tIQSamplesBlock *pIQSamplesBlock)
{
	// copy samples from the buffer into the return structure
}

int TRtlTcp::getSampleRate()
{
	return mSampleRate;
}

int TRtlTcp::getFrequency()
{
	return mFrequency;
}

bool TRtlTcp::setFrequency(int freqHz)
{
	// this tuner should be fixed to a specific frequency. thus, the setFrequency returns false, whenever a differnt one is requested.
	return (mFrequency==freqHz);
}
bool TRtlTcp::setGain(int gainCB)
{
	// this tuner has only a single gain setting. 
	return (mGain==gainCB);
}
bool TRtlTcp::gainUp()
{
	return false;
}
bool TRtlTcp::gainDown()
{
	return false;
}

