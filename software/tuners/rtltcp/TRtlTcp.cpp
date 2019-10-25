#include "TRtlTcp.h"

#define	NUM_TUNER_GAIN_VALUES	30

#define RTLTCP_CMD_SET_FREQUENCY        0x01
#define RTLTCP_CMD_SET_SAMPLERATE       0x02
#define RTLTCP_CMD_SET_GAIN_MODE        0x03    // 0=auto, 1=manual
#define RTLTCP_CMD_SET_GAIN_VALUE       0x04
#define RTLTCP_CMD_SET_FREQ_CORRECTION  0x05
#define RTLTCP_CMD_SET_IF_STAGE_GAIN    0x06    // msb: stage lsb: gain value
#define RTLTCP_CMD_SET_TEST_MODE        0x07    
#define RTLTCP_CMD_SET_AGC_MODE         0x08    // 0=auto, 1=manual
#define RTLTCP_CMD_SET_DIRECT_SAMPLING  0x09    
#define RTLTCP_CMD_SET_OFFSET_TUNING    0x0a
#define RTLTCP_CMD_SET_RTL_XTAL         0x0b
#define RTLTCP_CMD_SET_TUNER_XTAL       0x0c
#define RTLTCP_CMD_SET_GAIN_IDX         0x0d
const signed short RtlTcpClientLegalGains[7][NUM_TUNER_GAIN_VALUES]={
        {0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // unknown
        { -10, 0,15, 40, 65, 90, 115, 140, 165, 190, 215, 240, 290, 340, 420,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},   // e4000
        {-99, -40, 0,71, 179, 192 ,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//FC0012
        {-99, -73, -65, -63, -60, -58, -54, 0,58, 61, 63, 65, 67, 68, 70, 71, 179, 181, 182, 184, 186, 188, 191, 197 ,-1,0,0,0,0,0},// FC0013
        {0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //FC2580
        {0, 9, 14, 27, 37, 77, 87, 125, 144, 157, 166, 197, 207, 229, 254, 280, 297, 328, 338, 364, 372, 386, 402, 421, 434, 439, 445, 480, 496 ,-1},   // R820T
        {0, 9, 14, 27, 37, 77, 87, 125, 144, 157, 166, 197, 207, 229, 254, 280, 297, 328, 338, 364, 372, 386, 402, 421, 434, 439, 445, 480, 496 ,-1}    // R828D
};

        const char RtlTcpClientTunerNames[7][8]={
                "unknown",
                "E4000  ",
                "FC0012 ",
                "FC0013 ",
                "FC2580 ",
                "R820T  ",
                "R828D  "
        };
TRtlTcp::TRtlTcp()
{
	mutex.lock();
	mBuf=new unsigned char[SAMPLESBUF];
	mSamplesBuf=new tSComplex[SAMPLESBUF];
	mWidget=new QLabel("rtltcp client");
	mutex.unlock();
}
TRtlTcp::~TRtlTcp()
{
	if (mBuf!=nullptr)
		delete(mBuf);
	if (mSamplesBuf!=nullptr)
		delete(mSamplesBuf);
}
bool TRtlTcp::openConnection(char* hostname,int port)
{
	if (mSocket!=nullptr)
		mSocket->close();
	mSocket=new QTcpSocket(this);
	mSocket->connectToHost(hostname,port);
	connect(mSocket, SIGNAL(connected()), this, SLOT(connected()));
	connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(mSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));

	// socket->waitForReadyRead(3000)
	// socket->bytesAvailable();	
	
	
}
// taken from https://www.bogotobogo.com/cplusplus/sockets_server_client_QT.php
bool TRtlTcp::sendCmd(unsigned char cmd,int value)
{
	unsigned char buf[5];
	unsigned int x;

	buf[0]=cmd;
	//x=htonl(value);	 assuming LITTLE ENDIAN HERE
	//buf[1]=x&0xff;x>>=8;
	//buf[2]=x&0xff;x>>=8;
	//buf[3]=x&0xff;x>>=8;
	//buf[4]=x&0xff;x>>=8;
	x=value;
	buf[4]=x&0xff;x>>=8;
	buf[3]=x&0xff;x>>=8;
	buf[2]=x&0xff;x>>=8;
	buf[1]=x&0xff;x>>=8;

	// send(buf);
	mSocket->write((const char*)buf,5);


	return true;
}
void TRtlTcp::connected()
{
}
void TRtlTcp::disconnected()
{
	mTunerType=-1;
}
void TRtlTcp::bytesWritten(qint64 bytes)
{
}
void TRtlTcp::readyRead()
{
	mutex.lock();
	
	QByteArray newbuf=mSocket->readAll();

	if (mTunerType==-1 && newbuf.size())
	{
		int i;
		unsigned char* ptr;
		bool done;
		ptr=(unsigned char*)newbuf.data();
		mTunerType =((unsigned int)ptr[4])&0xff;mTunerType<<=8;
		mTunerType|=((unsigned int)ptr[5])&0xff;mTunerType<<=8;
		mTunerType|=((unsigned int)ptr[6])&0xff;mTunerType<<=8;
		mTunerType|=((unsigned int)ptr[7])&0xff;

		if (mTunerType<1 || mTunerType>6) 
		{
			// ERROR
		}
		mGainIdx=0;
		done=false;
		for (i=0;i<NUM_TUNER_GAIN_VALUES && !done;i++)
		{
			
			if (RtlTcpClientLegalGains[mTunerType][i]!=-1) mGainIdx=i;
			else done=true;
		}

		sendCmd(RTLTCP_CMD_SET_FREQUENCY,mFrequency);
		sendCmd(RTLTCP_CMD_SET_SAMPLERATE,mSamplerate);
		
		sendCmd(RTLTCP_CMD_SET_DIRECT_SAMPLING,0);
		sendCmd(RTLTCP_CMD_SET_OFFSET_TUNING,0);
	// some rtl tcp sticks have an issue with the gain control. apparently, it works to set it to the lowest and the maximum settings directly afterwards.
		sendCmd(RTLTCP_CMD_SET_GAIN_MODE,1);	// agc1 
		sendCmd(RTLTCP_CMD_SET_AGC_MODE,0);	// agc1 
		sendCmd(RTLTCP_CMD_SET_GAIN_VALUE,RtlTcpClientLegalGains[mTunerType][0]);
		sendCmd(RTLTCP_CMD_SET_GAIN_VALUE,RtlTcpClientLegalGains[mTunerType][mGainIdx]);
	


	} else {
		if (mBuf!=nullptr)
		{
			memcpy(&mBuf[mBufLevel],newbuf.data(),newbuf.size());
			mBufLevel+=newbuf.size();
		}
	}

	mutex.unlock();
}

void TRtlTcp::run()
{
	while (!mStopped)
	{
		int n;

		QThread::msleep(10);
		mutex.lock();
		char tmp[64];
		snprintf(tmp,64,"Samples:%d",mBufLevel);
		mWidget->setText(tmp);
		if (mBufLevel)
		{
			int i;
			int n;
			n=mBufLevel/2;

			for (i=0;i<n;i++)
			{
				mSamplesBuf[i].real=(signed short)mBuf[i*2+0]-127;
				mSamplesBuf[i].imag=(signed short)mBuf[i*2+1]-127;
			}

			if (mBufLevel%2)
			{
				mBuf[0]=mBuf[mBufLevel-1];
				mBufLevel=1;				
			} else mBufLevel=0;
			mSink->onNewSamples(mSamplesBuf,n);
			
		}
		mutex.unlock();
	}
}

void TRtlTcp::stop()
{
	mStopped=true;
}
QWidget* TRtlTcp::getWidget()
{
	return (QWidget*)mWidget;
}
int TRtlTcp::getFrequency()
{
	return mFrequency;
}
int TRtlTcp::getGain()
{
	if (mTunerType>=1 && mTunerType<=6)
		return RtlTcpClientLegalGains[mTunerType][mGainIdx];
	else
		return -1;
}
bool TRtlTcp::setFrequency(int freqHz)
{
	mFrequency=freqHz;
	return sendCmd(RTLTCP_CMD_SET_FREQUENCY,freqHz);
}
int TRtlTcp::getSamplerate()
{
	return mSamplerate;
}
bool TRtlTcp::setGain(int gaincB)
{
	int i;
	if (mTunerType>=1 && mTunerType<=6)
	{
		for (i=0;i<NUM_TUNER_GAIN_VALUES;i++)
		{
			int gain;
			gain=RtlTcpClientLegalGains[mTunerType][i];
			if (gain==gaincB)
			{
				mGainIdx=i;
				sendCmd(RTLTCP_CMD_SET_GAIN_VALUE,gain);				
			}
		}
	} 
	return false;
}
bool TRtlTcp::setSamplerate(int sampleRate)
{
	return (sampleRate==mSamplerate);
}
void TRtlTcp::gainUp()
{
	if (mTunerType>=1 && mTunerType<=6)
	{
		if (mGainIdx<(NUM_TUNER_GAIN_VALUES-1))
		{
			if (RtlTcpClientLegalGains[mTunerType][mGainIdx+1]!=-1)
			{
				mGainIdx++;
			}
		}
		sendCmd(RTLTCP_CMD_SET_GAIN_VALUE,RtlTcpClientLegalGains[mTunerType][mGainIdx]);
	}
}
void TRtlTcp::gainDown()
{
	if (mTunerType>=1 && mTunerType<=6)
	{
		if (mGainIdx>0)
		{
			mGainIdx--;
		}
		sendCmd(RTLTCP_CMD_SET_GAIN_VALUE,RtlTcpClientLegalGains[mTunerType][mGainIdx]);
	}
}
