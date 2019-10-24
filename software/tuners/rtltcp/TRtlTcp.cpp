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
TRtlTcp::RtlTcp()
{
	mutex.lock();
	mBuf=new unsigned char[1<<19];
	connect(socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
	mutex.unlock();
}
TRtlTcp::~RtlTcp()
{
	if (mBuf!=nullptr)
		delete(mBuf);
}
bool TRtlTcp::openConnection(char* hostname,int port)
{
	if (mSocket!=nullptr)
		mSocket->close();
	mSocket=new QTcpSocket(this);
	mSocket->connectToHost(hostname,port);

	// socket->waitForReadyRead(3000)
	// socket->bytesAvailable();	
		
}
// taken from https://www.bogotobogo.com/cplusplus/sockets_server_client_QT.php
bool TRtlTcp::sendCmd(unsigned char cmd,int value)
{
	unsigned char buf[5];
	unsigned int x;

	x=htonl(value);
	buf[0]=cmd;
	buf[1]=x&0xff;x>>=8;
	buf[2]=x&0xff;x>>=8;
	buf[3]=x&0xff;x>>=8;
	buf[4]=x&0xff;x>>=8;

	// send(buf);
	mSocket->write(buf);


	return true;
}
void TRtlTcp::connected()
{
}
void TRtlTcp::disconnected()
{
}
void TRtlTcp::bytesWritten(qint64 bytes)
{
}
void TRtlTcp::readyRead()
{
	mutex.lock();
	
	QByteArray newbuf=mSocket->readAll();
	memcpy(&mBuf[mBufLevel],newbuf.data(),newbuf.size());
	mBufLevel+=newbuf.size();
	

	mutex.unlock();
}

void TRtlTcp::run()
{
	while (!mStopped)
	{
		int n;

		QThread::msleep(10);
		mutex.lock();
		if (mBufLevel)
		{
			int i;
			int n;

			n=mBufLevel/2;

			for (i=0;i<n;i++)
			{
				mSamplesBuf[i].re=(signed short)mBuf[i*2+0]-127;
				mSamplesBuf[i].im=(signed short)mBuf[i*2+1]-127;
			}

			if (mBufLevel%2)
			{
				mBuf[0]=mBuf[mBufLevel-1];
				mBufLevel=1;				
			}
			mSink->onNewSamples(mSamplesBuf,n);
			
		}
		mutex.unlock();
	}
}
