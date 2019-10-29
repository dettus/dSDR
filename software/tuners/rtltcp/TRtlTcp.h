// this class is am implementation of the tuner class.
// it is a client to the RTL TCP server, to be able
// to access many cheap tuners.
//
// 
// $Id$

#ifndef	TRtlTcp_H
#define	TRtlTcp_H
#include "Tuners.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTcpSocket>
#include <QMutex>
class TRtlTcp: public Tuners
{
	Q_OBJECT
	
	public:
		TRtlTcp(QWidget* parent=nullptr);
		void initialize();
		void process();
		int timeToWait();

		void getSamples(tIQSamplesBlock *pIQSamplesBlock);
		int getSampleRate();
		int getFrequency();
		
		bool setFrequency(int freqHz);
		bool setGain(int gainCB);
		bool gainUp();
		bool gainDown();
		int minValue();
		int maxValue();

	private:
		bool openConnection();
		bool sendCmd(unsigned char cmd,int value);
		int mSampleRate=2048000;
		int mFrequency=87900000;
		int mGain=0;
		int mTunerType=-1;	// 1..6
		int mGainIdx=0;
		int mMaxGainIdx=0;

		QTcpSocket *mSocket=nullptr;

		QString mHostname="127.0.0.1";
		int mPort=1234;
		
		////// widgets for the GUI start here
		QVBoxLayout	*mLayout;
		QLineEdit	*mHostnameInput;
		QLineEdit	*mPortInput;
		QPushButton	*mConnect;
		QPushButton	*mGainUp;
		QPushButton	*mGainDown;
		QPushButton	*mFreqUp;
		QLineEdit	*mFreqInput;
		QPushButton	*mFreqDown;
		QMutex		mLock;
		signed char	*mBuf=nullptr;
		tSComplex	*mSamples=nullptr;
		int		mBufLevel=0;
		QMutex		mLock2;

	public slots:
		// GUI slots
		void handleConnect();
		void handleGainUp();
		void handleGainDown();
		void handleFreqUp();
		void handleFreqInput();
		void handleFreqDown();

		// network slots
		void readyRead();


		
		
};
#endif
