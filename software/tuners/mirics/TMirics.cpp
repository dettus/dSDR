#include "TMirics.h"

TMirics::TMirics(QWidget* parent): QWidget(parent)
{

}
TMirics::~TMirics()
{
#if 0
        mir_sdr_StreamUninit();
        mir_sdr_ReleaseDeviceIdx();

#endif
}
void TMirics::initialize() 
{
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
}
void TMirics::process() {}
void TMirics::getSamples(tIQSamplesBlock *pIQSamplesBlock) {}
int TMirics::getSampleRate() {}
int TMirics::getFrequency() {}

bool TMirics::setFrequency(int freqHz) 
{
#if 0

instead of 

mir_sdr_StreamInit(int *gRdB, double fsMHz, double rfMHz, mir_sdr_Bw_MHzT bwType, mir_sdr_If_kHzT ifType, int LNAstate, int *gRdBsystem, mir_sdr_SetGrModeT setGrMode, int *samplesPerPacket, mir_sdr_StreamCallback_t StreamCbFn, mir_sdr_GainChangeCallback_t GainChangeCbFn, void *cbContext);

call
typedef mir_sdr_ErrT (*mir_sdr_Reinit_t)(int *gRdB, double fsMHz, double rfMHz, mir_sdr_Bw_MHzT bwType, mir_sdr_If_kHzT ifType, mir_sdr_LoModeT loMode, int LNAstate, int *gRdBsystem, mir_sdr_SetGrModeT setGrMode, int *samplesPerPacket, mir_sdr_ReasonForReinitT reasonForReinit);


mir_sdr_CHANGE_RF_FREQ
#endif
}
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


