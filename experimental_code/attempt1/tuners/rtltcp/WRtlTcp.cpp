#include "WRtlTcp.h"

WRtlTcp::WRtlTcp()
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
	mFreqInput=new QLineEdit("");
	mFreqDown=new QPushButton("Frequency down");

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
void WRtlTcp::setCallback(CBRtlTcp* cbRtlTcp)
{
	mCbRtlTcp=cbRtlTcp;
}
bool WRtlTcp::setFrequency(int freqHz,bool performCB)
{
	char tmp[64];
	if (mCbRtlTcp!=nullptr && performCB)
	{
		if (!mCbRtlTcp->setFrequency(freqHz))
			return false;
	}
	mFrequency=freqHz;

	
	snprintf(tmp,64,"%d",mFrequency);
	mFreqInput->setText(tmp);
	return true;
}

void WRtlTcp::handleGainUp()
{
	if (mCbRtlTcp!=nullptr) mCbRtlTcp->gainUp();
}
void WRtlTcp::handleGainDown()
{
	if (mCbRtlTcp!=nullptr) mCbRtlTcp->gainDown();
}
void WRtlTcp::handleFreqUp()
{
	setFrequency(mFrequency+100000,true);
}
void WRtlTcp::handleFreqDown()
{
	setFrequency(mFrequency-100000,true);
}
void WRtlTcp::handleFreqInput()
{
	int freqHz;
	freqHz=mFreqInput->text().toInt();
	printf("freqHz:%d\n",freqHz);
	if (freqHz)
	{
		setFrequency(freqHz,true);	
	}
}
void WRtlTcp::handleConnect()
{
	int port;
	port=mPortInput->text().toInt();
	if (port>=1024 && port<65536 && port!=mPort)
	{
		mPort=port;
	}
	mHostname=mHostnameInput->text();
	if (mCbRtlTcp!=nullptr)
	{
		mCbRtlTcp->openConnection(mHostname,mPort);
	}
}
