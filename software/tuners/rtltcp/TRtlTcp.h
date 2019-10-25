#ifndef	TRTLTCP_H
#define	TRTLTCP_H
#include <QWidget>
#include <QThread>
#include <QLabel>
#include <QMutex>
#include <QTcpSocket>
#include <stdio.h>

#include "Tuners.h"
#include "WRtlTcp.h"
#include "CBRtlTcp.h"

#define	SAMPLESBUF	(1<<22)
class TRtlTcp : public QThread, public Tuners, public CBRtlTcp
{
	Q_OBJECT

	public:
		TRtlTcp();
		~TRtlTcp();
		void stop();
		QWidget* getWidget();
		int getFrequency();
		int getGain();
		int getSamplerate();

		bool setFrequency(int freqHz);
		bool setGain(int gaincB);
		bool setSamplerate(int sampleRate);
		
		void gainUp();
		void gainDown();


		bool openConnection(char* hostname,int port);


	public slots:
		void connected();
		void disconnected();
		void bytesWritten(qint64 bytes);
		void readyRead();

	protected:
		void run();
	
	private:
		bool mStopped=false;
		QMutex mutex;
		unsigned char* mBuf=nullptr;
		tSComplex *mSamplesBuf=nullptr;
		int mBufLevel=0;
		QTcpSocket *mSocket=nullptr;
		bool sendCmd(unsigned char cmd,int value);
		WRtlTcp *mWidget;

//		int mFrequency=178352000;//87900000;
		int mFrequency=87900000;
		int mGainIdx=0;
		int mSamplerate=2048000;
		int mTunerType=-1;
		
};
#endif
