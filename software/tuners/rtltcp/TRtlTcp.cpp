#include "TRtlTcp.h"



#define NUM_TUNER_GAIN_VALUES   30

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

// the constructor initialized the GUI.
TRtlTcp::TRtlTcp(QWidget* parent): Tuners(parent)
{
	char tmp[32];
	snprintf(tmp,32,"%d",mPort);
	mLayout=new QVBoxLayout;
	mHostnameInput=new QLineEdit(mHostname);
	mPortInput=new QLineEdit(tmp);
	mConnect=new QPushButton("connect");
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
	mLayout->addWidget(new QLabel("RTL TCP Settings"));
	mLayout->addWidget(mHostnameInput);
	mLayout->addWidget(mPortInput);
	mLayout->addWidget(mConnect);
	mLayout->addWidget(mGainUp);
	mLayout->addWidget(mGainDown);
	mLayout->addWidget(mFreqUp);
	mLayout->addWidget(mFreqInput);
	mLayout->addWidget(mFreqDown);
	setLayout(mLayout);


	connect(mConnect,SIGNAL(released()),this,SLOT(handleConnect()));
	connect(mGainUp,SIGNAL(released()),this,SLOT(handleGainUp()));
	connect(mGainDown,SIGNAL(released()),this,SLOT(handleGainDown()));
	connect(mFreqUp,SIGNAL(released()),this,SLOT(handleFreqUp()));
	connect(mFreqInput,SIGNAL(returnPressed()),this,SLOT(handleFreqInput()));
	connect(mFreqDown,SIGNAL(released()),this,SLOT(handleFreqDown()));



	
}
void TRtlTcp::initialize()
{

}

void TRtlTcp::process()
{
	// read 1 second of samples from the file
	// readyRead() is being called implicitly.
	// so nothing to do here.
}

int TRtlTcp::timeToWait()
{
	return 50;	// wait for 50 milliseconds between process calls
}

void TRtlTcp::getSamples(tIQSamplesBlock *pIQSamplesBlock)
{
	// copy samples from the buffer into the return structure
	mLock2.lock();
	pIQSamplesBlock->sampleRate=mSampleRate;
	pIQSamplesBlock->centerFreq=mFrequency;
	pIQSamplesBlock->gain=mGain;
	if (mSamples!=nullptr)
	{
		int i,n;
		pIQSamplesBlock->pData=mSamples;
		n=mBufLevel/2;
		pIQSamplesBlock->sampleNum=n;

		for (i=0;i<n;i++)
		{
			mSamples[i].real=mBuf[2*i+0]-127;
			mSamples[i].imag=mBuf[2*i+1]-127;
		}
		if (mBufLevel%2==1)
		{
			mBuf[0]=mBuf[mBufLevel-1];
		}
		mBufLevel%=2;
	} else {
		pIQSamplesBlock->sampleNum=0;
	}

	mLock2.unlock();
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
	char tmp[32];
	if (freqHz<mSampleRate/2) return false;
	sendCmd(RTLTCP_CMD_SET_FREQUENCY,freqHz);
	mFrequency=freqHz;
	snprintf(tmp,32,"%d",mFrequency);
	mFreqInput->setText(tmp);
	
	return true;
}
bool TRtlTcp::setGain(int gainCB)
{
	int i;
	bool found;
	
	found=false;
	if (mTunerType>=1 && mTunerType<=6 && gainCB!=-1)
	{
		for (i=0;i<mMaxGainIdx;i++)
		{
			if (RtlTcpClientLegalGains[mTunerType][i]==gainCB) 
			{
				found=true;
				mGainIdx=i;
			}
		}
	}
	if (found) sendCmd(RTLTCP_CMD_SET_GAIN_VALUE,gainCB);
	return found;
}
bool TRtlTcp::gainUp()
{
	
	if (mGainIdx!=mMaxGainIdx && mTunerType>=1 && mTunerType<=6)
	{
		mGainIdx+=1;
		sendCmd(RTLTCP_CMD_SET_GAIN_VALUE,RtlTcpClientLegalGains[mTunerType][mGainIdx]);
		return true;
	}
	return false;
}
bool TRtlTcp::gainDown()
{
	
	if (mGainIdx!=0 && mTunerType>=1 && mTunerType<=6)
	{
		mGainIdx-=1;
		sendCmd(RTLTCP_CMD_SET_GAIN_VALUE,RtlTcpClientLegalGains[mTunerType][mGainIdx]);
		return true;
	}
	return false;
}
int TRtlTcp::minValue()
{
	return -127;
}
int TRtlTcp::maxValue()
{
	return 127;
}

bool TRtlTcp::openConnection()
{
	mLock.lock();
	if (mSocket!=nullptr)
		mSocket->close();
	mSocket=new QTcpSocket(this);
	if (mSocket==nullptr) 
	{
		mLock.unlock();
		return false;
	}
	mSocket->connectToHost(mHostname,mPort);

//        connect(mSocket, SIGNAL(connected()), this, SLOT(connected()));
//        connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
//        connect(mSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
	mLock.unlock();
	return true;
}
// taken from https://www.bogotobogo.com/cplusplus/sockets_server_client_QT.php
bool TRtlTcp::sendCmd(unsigned char cmd,int value)
{
        unsigned char buf[5];
        unsigned int x;
 
        buf[0]=cmd;
        //x=htonl(value);        assuming LITTLE ENDIAN HERE
        //buf[1]=x&0xff;x>>=8;
        //buf[2]=x&0xff;x>>=8;
        //buf[3]=x&0xff;x>>=8;
        //buf[4]=x&0xff;x>>=8;
        x=(unsigned int)value;
        buf[4]=x&0xff;x>>=8;
        buf[3]=x&0xff;x>>=8;
        buf[2]=x&0xff;x>>=8;
        buf[1]=x&0xff;x>>=8;

        // send(buf);
        mSocket->write((const char*)buf,5);


        return true;
}




void TRtlTcp::handleConnect()
{
	if (mHostname!=mHostnameInput->text() || mPort!=mPortInput->text().toInt() || mSocket==nullptr)
	{
		mLock2.lock();
		mHostname=mHostnameInput->text();
		mPort=mPortInput->text().toInt();
		openConnection();
		if (mBuf!=nullptr)
		{
			delete(mBuf);
		}
		if (mSamples!=nullptr)
		{
			delete(mSamples);
		}
		mBuf=new signed char[mSampleRate*2];
		mSamples=new tSComplex[mSampleRate];

		mBufLevel=0;
		mLock2.unlock();
	}
}
void TRtlTcp::handleGainUp()
{
	gainUp();
}
void TRtlTcp::handleGainDown()
{
	gainDown();
}
void TRtlTcp::handleFreqUp()
{
	int frequency;
	frequency=mFrequency+100000;
	setFrequency(frequency);
}
void TRtlTcp::handleFreqInput()
{
	int frequency;
	frequency=mFreqInput->text().toInt();
	if (frequency>mSampleRate/2)
		setFrequency(frequency);
	
}
void TRtlTcp::handleFreqDown()
{
	int frequency;
	frequency=mFrequency-100000;
	if (frequency>mSampleRate/2)
		setFrequency(frequency);
}


void TRtlTcp::readyRead()
{
	mLock.lock();
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
		mMaxGainIdx=mGainIdx;

		sendCmd(RTLTCP_CMD_SET_FREQUENCY,mFrequency);
		sendCmd(RTLTCP_CMD_SET_SAMPLERATE,mSampleRate);

		sendCmd(RTLTCP_CMD_SET_DIRECT_SAMPLING,0);
		sendCmd(RTLTCP_CMD_SET_OFFSET_TUNING,0);
		// some rtl tcp sticks have an issue with the gain control. apparently, it works to set it to the lowest and the maximum settings directly afterwards.
		sendCmd(RTLTCP_CMD_SET_GAIN_MODE,1);    // manual gain
		sendCmd(RTLTCP_CMD_SET_AGC_MODE,0);     // manual gain, agc off
		sendCmd(RTLTCP_CMD_SET_GAIN_VALUE,RtlTcpClientLegalGains[mTunerType][0]);
		sendCmd(RTLTCP_CMD_SET_GAIN_VALUE,RtlTcpClientLegalGains[mTunerType][mGainIdx]);


		mGain=RtlTcpClientLegalGains[mTunerType][mGainIdx];
	} else {
		mLock2.lock();
		if (mBuf!=nullptr)
		{
			memcpy(&mBuf[mBufLevel],newbuf.data(),newbuf.size());
			mBufLevel+=newbuf.size();
		}
		mLock2.unlock();


	}

	mLock.unlock();
}
