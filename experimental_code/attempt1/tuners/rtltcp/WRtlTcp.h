#ifndef	WRtlTcp_H
#define	WRtlTcp_H
#include <QtWidgets>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include "CBRtlTcp.h"

class WRtlTcp: public QWidget
{
	Q_OBJECT

	public:
		WRtlTcp();
		void setCallback(CBRtlTcp* cbRtlTcp);
		bool setFrequency(int freqHz,bool performCB=false);
	
	private slots:
		void handleGainUp();
		void handleGainDown();
		void handleFreqUp();
		void handleFreqDown();
		void handleFreqInput();
		void handleConnect();

	private:
		QVBoxLayout *mLayout;
		QLineEdit   *mHostnameInput;
		QLineEdit   *mPortInput;
		QPushButton *mConnect;
		QPushButton *mGainUp;
		QPushButton *mGainDown;
		QPushButton *mFreqUp;
		QPushButton *mFreqDown;
		QLineEdit   *mFreqInput;
		QLabel *mLabel;
		bool mStarted=false;
		CBRtlTcp *mCbRtlTcp=nullptr;

		int mFrequency=87900000;
		QString mHostname="127.0.0.1";
		int mPort=1234;
		
};
#endif
