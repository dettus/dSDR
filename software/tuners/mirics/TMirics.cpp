#include "TMirics.h"

TMirics::TMirics(QWidget* parent): QWidget(parent)
{

}

void TMirics::initialize() {}
void TMirics::process() {}
void TMirics::getSamples(tIQSamplesBlock *pIQSamplesBlock) {}
int TMirics::getSampleRate() {}
int TMirics::getFrequency() {}

bool TMirics::setFrequency(int freqHz) {}
bool TMirics::setGain(int gainCB) {}
bool TMirics::gainUp() {}
bool TMirics::gainDown() {}

static void TMirics::static_gainCallback(unsigned int gRdB, unsigned int lnaGRdB, void *cbContext) 
{
	TMirics* context=(TMirics*)cbContext;
	context->gainCallback(gRdB,lnaGRdB);
}
static void TMirics::static_streamCallback(short *xi, short *xq, unsigned int firstSampleNum,
		int grChanged, int rfChanged, int fsChanged, unsigned int numSamples,
		unsigned int reset, unsigned int hwRemoved, void *cbContext)
{
	TMirics* context=(TMirics*)cbContext;
	context->streamCallback(xi,xq,firstSampleNum,grChanged,rfChanged,fsChanged,numSamples,reset, hwRemoved);
}

void TMirics::gainCallback(unsigned int gRdB, unsigned int lnaGRdB, void *cbContext) {}
void TMirics::streamCallback(short *xi, short *xq, unsigned int firstSampleNum,
		int grChanged, int rfChanged, int fsChanged, unsigned int numSamples,
		unsigned int reset, unsigned int hwRemoved) {}


