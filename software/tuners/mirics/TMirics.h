#ifndef	TMIRICS_H
#define	TMIRICS_H
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>

#include "DataTypes.h"
#include "Tuners.h"
// the purpose of this class is to provide an interface to the mirics sdrplay tuners
#include "mirsdrapi-rsp.h"

class TMirics: public Tuners
{
	Q_OBJECT

	public:
		TMirics(QWidget* parent=nullptr);
		~TMirics();
		void initialize();
		void process();
		int timeToWait() {return 100;}
		int minValue() {return -2048;}
		int maxValue() {return  2048;}
		void getSamples(tIQSamplesBlock *pIQSamplesBlock);
		int getSampleRate() {return mSampleRate;};
		int getFrequency() {return mFrequency;};

		bool setFrequency(int freqHz);
		bool setGain(int gainCB);
		bool gainUp();
		bool gainDown();


		////// widgets for the GUI start here
		QVBoxLayout	*mLayout;
		QPushButton	*mGainUp;
		QPushButton	*mGainDown;
		QPushButton	*mFreqUp;
		QLineEdit	*mFreqInput;
		QPushButton	*mFreqDown;

	
		// callbacks for the mirics tuner
//		static void static_gainCallback(unsigned int gRdB, unsigned int lnaGRdB, void *cbContext);
//
//		static void static_streamCallback(short *xi, short *xq, unsigned int firstSampleNum,
//				int grChanged, int rfChanged, int fsChanged, unsigned int numSamples,
//				unsigned int reset, unsigned int hwRemoved, void *cbContext);
		void gainCallback(unsigned int gRdB, unsigned int lnaGRdB);
		void streamCallback(short *xi, short *xq, unsigned int firstSampleNum,
			int grChanged, int rfChanged, int fsChanged, unsigned int numSamples,
			unsigned int reset, unsigned int hwRemoved);

		

	private:
		mir_sdr_Bw_MHzT mBandwidth=mir_sdr_BW_1_536;
		int mRspLNA=0;
		mir_sdr_SetGrModeT mGrMode=mir_sdr_USE_SET_GR_ALT_MODE;
		double mIfFreq=0;	// 450,1620,2048
#define	MAX_DEVICE_NUM		4
		mir_sdr_DeviceT mDevices[MAX_DEVICE_NUM];
		unsigned int mNumDevices=0;
		int mDevIdx;
		double mPpmOffset=0;
		int mGRdB=20;
		int mGRdBsystem;
		int mSamplesPacket=0;
		int mNumDevs;
		int mHWVersion;
		mir_sdr_AgcControlT mAgcControl=mir_sdr_AGC_100HZ;
		int msetPoint=-40;


		int mFrequency=87800000;
		int mSampleRate=2048000;
	
		QMutex mMutex;	

		QByteArray mBufRe;
		QByteArray mBufIm;
		tSComplex *mSamples;
		mir_sdr_LoModeT mLoMode=mir_sdr_LO_Undefined; 
	public slots:
		// GUI slots
		void handleGainUp();
		void handleGainDown();
		void handleFreqUp();
		void handleFreqInput();
		void handleFreqDown();
		


};

#endif
