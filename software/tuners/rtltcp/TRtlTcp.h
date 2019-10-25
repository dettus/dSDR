#ifndef	TRTLTCP_H
#define	TRTLTCP_H
#include <QWidget>
#include <QThread>
#include <QLabel>
#include <QMutex>
#include <QTcpSocket>
#include <stdio.h>

#include "Tuners.h"
class TRtlTcp : public QThread, public Tuners
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
		tSComplex samplesBuf[1<<19];
		int mBufLevel=0;
		QTcpSocket *mSocket;
		bool sendCmd(unsigned char cmd,int value);
		QLabel *mWidget;

		int mFrequency=101200000;
		int mGainIdx=0;
		int mSamplerate=2048000;
		int mTunerType=-1;
		
};
#endif
