#include "TMirics.h"
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>


TMirics::TMirics(QWidget* parent): Tuners(parent)
{
	char tmp[32];
	mLayout=new QVBoxLayout;
	mGainUp=new QPushButton("gain up");
	mGainDown=new QPushButton("gain down");
	mFreqUp=new QPushButton("Frequency up");
	snprintf(tmp,32,"%d",mFrequency);
	mFreqInput=new QLineEdit(tmp);
	mFreqDown=new QPushButton("Frequency down");
	setAutoFillBackground(true);
	QPalette pal = palette();
	pal.setColor(QPalette::Background, QColor(255,255,255,255));
	setPalette(pal);
	mLayout->setAlignment(Qt::AlignTop);
	mLayout->addWidget(new QLabel("Mirics Settings"));
	mLayout->addWidget(mGainUp);
	mLayout->addWidget(mGainDown);
	mLayout->addWidget(mFreqUp);
	mLayout->addWidget(mFreqInput);
	mLayout->addWidget(mFreqDown);

	setLayout(mLayout);


	connect(mGainUp,SIGNAL(released()),this,SLOT(handleGainUp()));
	connect(mGainDown,SIGNAL(released()),this,SLOT(handleGainDown()));
	connect(mFreqUp,SIGNAL(released()),this,SLOT(handleFreqUp()));
	connect(mFreqInput,SIGNAL(returnPressed()),this,SLOT(handleFreqInput()));
	connect(mFreqDown,SIGNAL(released()),this,SLOT(handleFreqDown()));

//	

	

}
TMirics::~TMirics()
{
#if 0
        mir_sdr_StreamUninit();
        mir_sdr_ReleaseDeviceIdx();

#endif
}

static void static_gainCallback(unsigned int gRdB, unsigned int lnaGRdB, void *cbContext) 
{
	TMirics* context=(TMirics*)cbContext;
	context->gainCallback(gRdB,lnaGRdB);
}
static void static_streamCallback(short *xi, short *xq, unsigned int firstSampleNum,
		int grChanged, int rfChanged, int fsChanged, unsigned int numSamples,
		unsigned int reset, unsigned int hwRemoved, void *cbContext)
{
	TMirics* context=(TMirics*)cbContext;
	context->streamCallback(xi,xq,firstSampleNum,grChanged,rfChanged,fsChanged,numSamples,reset, hwRemoved);
}

void TMirics::initialize() 
{
	mir_sdr_GetDevices(&mDevices[0],&mNumDevices,MAX_DEVICE_NUM);
	mDevIdx=0;
	mHWVersion=mDevices[mDevIdx].hwVer;
	mir_sdr_SetDeviceIdx(mDevIdx);
	mir_sdr_SetPpm(mPpmOffset);
	if (mHWVersion==2)
	{
		mir_sdr_RSPII_AntennaControl(mir_sdr_RSPII_ANTENNA_A);
		mir_sdr_AmPortSelect(0);
	}
	mir_sdr_StreamInit(&mGRdB, (double)mSampleRate/1000000.0f,(double)mFrequency/1000000.0f,mBandwidth,mir_sdr_IF_Zero, mRspLNA, &mGRdBsystem,mGrMode, &mSamplesPacket, static_streamCallback,static_gainCallback,this);
        mir_sdr_AgcControl(mAgcControl,msetPoint, 0,0,0,0, mRspLNA);
	
        if (mHWVersion==2)
        {
                mir_sdr_RSPII_ExternalReferenceControl(0);
                mir_sdr_RSPII_RfNotchEnable(0);
                mir_sdr_RSPII_BiasTControl(0);
        }
	mBufRe.clear();
	mBufIm.clear();
#if 0
        int bandwidth=mir_sdr_BW_1_536;
        int rspLNA=0;
        int grMode=mir_sdr_USE_SET_GR_ALT_MODE;
        double ifFreq=0;        // 450, 1620, 2048
        mir_sdr_DeviceT devices[4];
        int i;
        int devIdx;
        double ppmOffset;
        int gRdB=20;
        int gRdBsystem;
        int samplesPacket;
        unsigned int numDevs;
        int hwVer;
        int agcControl=1;
        int setPoint=-30;
        int mode;

	mir_sdr_GetDevices(&devices[0], &numDevs, 4);
        for (i=0;i<numDevs;i++)
        {
                fprintf(stderr,"%i> hwVer:%i\n",i,devices[i].hwVer);
        }
        devIdx=0;

        hwVer=devices[devIdx].hwVer;
        mir_sdr_SetDeviceIdx(devIdx);
        mir_sdr_SetPpm(ppmOffset);

        if (hwVer==2)
        {
                mir_sdr_RSPII_AntennaControl(mir_sdr_RSPII_ANTENNA_A);
                mir_sdr_AmPortSelect(0);
        }
	mir_sdr_StreamInit(&gRdB, samplerate/1000000.0f,frequency/1000000.0f,bandwidth,mir_sdr_IF_Zero, rspLNA, &gRdBsystem,grMode, &samplesPacket, TMirics::static_streamCallback, TMirics::static_gainCallback,this);
        mir_sdr_AgcControl(agcControl,setPoint, 0,0,0,0, rspLNA);
        if (hwVer==2)
        {
                mir_sdr_RSPII_ExternalReferenceControl(0);
                mir_sdr_RSPII_RfNotchEnable(0);
                mir_sdr_RSPII_BiasTControl(0);
        }

#endif
	mSamples=new tSComplex[mSampleRate*2];
}
void TMirics::process() {}
void TMirics::getSamples(tIQSamplesBlock *pIQSamplesBlock) 
{
	int i;
	signed short *ptrre;
	signed short *ptrim;

	pIQSamplesBlock->pData=mSamples;
	pIQSamplesBlock->sampleRate=mSampleRate;
	pIQSamplesBlock->centerFreq=mFrequency;
	pIQSamplesBlock->gain=msetPoint*10;
	pIQSamplesBlock->sampleNum=mBufRe.length()/sizeof(short);	
	mMutex.lock();
	ptrre=(signed short*)mBufRe.data();
	ptrim=(signed short*)mBufIm.data();



	for (i=0;i<pIQSamplesBlock->sampleNum;i++)
	{
		pIQSamplesBlock->pData[i].real=ptrre[i];
		pIQSamplesBlock->pData[i].imag=ptrim[i];
	}
	mBufRe.remove(0,pIQSamplesBlock->sampleNum*sizeof(short));
	mBufIm.remove(0,pIQSamplesBlock->sampleNum*sizeof(short));
	mMutex.unlock();
}

bool TMirics::setFrequency(int freqHz) 
{
	char tmp[32];
	mir_sdr_Reinit(&mGRdB, (double)mSampleRate/1000000.0f,(double)mFrequency/1000000.0f,mBandwidth,mir_sdr_IF_Zero, mLoMode,mRspLNA, &mGRdBsystem,mGrMode, &mSamplesPacket,mir_sdr_CHANGE_RF_FREQ);

	mFrequency=freqHz;
	snprintf(tmp,32,"%d",mFrequency);
	mFreqInput->setText(tmp);
	mir_sdr_Reinit(&mGRdB, (double)mSampleRate/1000000.0f,(double)mFrequency/1000000.0f,mBandwidth,mir_sdr_IF_Zero, mLoMode,mRspLNA, &mGRdBsystem,mGrMode, &mSamplesPacket,mir_sdr_CHANGE_RF_FREQ);
}
bool TMirics::setGain(int gainCB) 
{
	if (gainCB<=0 && gainCB>=-600)
	{
		msetPoint=gainCB/10;
		mir_sdr_AgcControl(mAgcControl,msetPoint, 0,0,0,0, mRspLNA);
	}

}
bool TMirics::gainUp() 
{
	setGain((msetPoint+1)*10);
}
bool TMirics::gainDown() 
{
	setGain((msetPoint-1)*10);
}
void TMirics::gainCallback(unsigned int gRdB, unsigned int lnaGRdB) {}
void TMirics::streamCallback(short *xi, short *xq, unsigned int firstSampleNum,
		int grChanged, int rfChanged, int fsChanged, unsigned int numSamples,
		unsigned int reset, unsigned int hwRemoved) 
{
	mMutex.lock();
	mBufRe.append((char*)xi,numSamples*sizeof(short));
	mBufIm.append((char*)xq,numSamples*sizeof(short));
	mMutex.unlock();
}

void TMirics::handleGainUp()
{
	gainUp();
}
void TMirics::handleGainDown()
{
	gainDown();
}
void TMirics::handleFreqUp()
{
	int frequency;
	frequency=mFrequency+100000;
	setFrequency(frequency);
}
void TMirics::handleFreqInput()
{
	int frequency;
	frequency=mFreqInput->text().toInt();
	if (frequency>mSampleRate/2)
		setFrequency(frequency);
	
}
void TMirics::handleFreqDown()
{
	int frequency;
	frequency=mFrequency-100000;
	if (frequency>mSampleRate/2)
		setFrequency(frequency);
}


