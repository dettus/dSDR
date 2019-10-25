#include "WRtlTcp.h"

WRtlTcp::WRtlTcp()
{
	mLayout=new QVBoxLayout;
	mGainUp=new QPushButton("gain up");
	mGainDown=new QPushButton("gain down");
	mFreqUp=new QPushButton("Frequency up");
	mFreqInput=new QLineEdit("");
	mFreqDown=new QPushButton("Frequency down");

	mLayout->addWidget(mGainUp);
	mLayout->addWidget(mGainDown);
	mLayout->addWidget(mFreqUp);
	mLayout->addWidget(mFreqInput);
	mLayout->addWidget(mFreqDown);
	setLayout(mLayout);

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
