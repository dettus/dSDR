#include "TDummy.h"

void TDummy::initialize()
{
	mSampleBuf=new tSComplex[4*mSampleRate];	// set up a buffer for at least 4 seconds of samples.
	if (mFptr) fclose(mFptr);
	mFptr=nullptr;
}
void TDummy::process()
{
	// read 1 second of samples from the file
	if (mFptr!=nullptr && mRunning)
	{
		mLock.lock();
		if (feof(mFptr))
		{
			fseek(mFptr,0,SEEK_SET);
		}
		fread(&mSampleBuf[mSampleBufWriteLevel%(4*mSampleRate)],sizeof(tSComplex),mSampleRate,mFptr);
		mSampleBufWriteLevel=(mSampleBufWriteLevel+mSampleRate)%(8*mSampleRate);
		mLock.unlock();
	}
}

int TDummy::timeToWait()
{
	return 1000;	// wait for 1000 milliseconds between process calls
}

void TDummy::getSamples(tIQSamplesBlock *pIQSamplesBlock)
{
	// copy samples from the buffer into the return structure
	mLock.lock();
	pIQSamplesBlock->pData=&mSampleBuf[mSampleBufReadLevel];	// TODO: memcpy??
	pIQSamplesBlock->sampleNum=mSampleBufWriteLevel-mSampleBufReadLevel;
	pIQSamplesBlock->sampleRate=mSampleRate;
	pIQSamplesBlock->centerFreq=mFrequency;
	pIQSamplesBlock->gain=mGain;
	if (mSampleBufReadLevel!=mSampleBufWriteLevel)
	{
		mSampleBufWriteLevel%=(4*mSampleRate);
		mSampleBufReadLevel=mSampleBufWriteLevel;
	}
	mLock.unlock();
}

int TDummy::getSampleRate()
{
	return mSampleRate;
}

int TDummy::getFrequency()
{
	return mFrequency;
}

bool TDummy::setFrequency(int freqHz)
{
	// this tuner should be fixed to a specific frequency. thus, the setFrequency returns false, whenever a differnt one is requested.
	return (mFrequency==freqHz);
}
bool TDummy::setGain(int gainCB)
{
	// this tuner has only a single gain setting. 
	return (mGain==gainCB);
}
bool TDummy::gainUp()
{
	return false;
}
bool TDummy::gainDown()
{
	return false;
}
/////////
/////////
//////// the GUI is being set up in here
TDummy::TDummy(QWidget* parent): Tuners(parent)
{
	mLock.lock();
	mLayout=new QVBoxLayout;
	
	mOpen=new QPushButton("Open Signal File...");

	setAutoFillBackground(true);
	QPalette pal = palette();
	pal.setColor(QPalette::Background, QColor(255,255,255,255));
	setPalette(pal);


	mStartStop=new QPushButton("Start");
	mLayout->setAlignment(Qt::AlignTop);
	mLayout->addWidget(new QLabel("Dummy Settings"));
	mLayout->addWidget(mOpen);
	mLayout->addWidget(mStartStop);
	setLayout(mLayout);

	connect(mOpen,SIGNAL(released()),this,SLOT(handleOpen()));
	connect(mStartStop,SIGNAL(released()),this,SLOT(handleStartStop()));
	mLock.unlock();
}
// pushing the "open" button should reveal the "file io" dialog
void TDummy::handleOpen()
{
	mLock.lock();
	QString filename=QFileDialog::getOpenFileName(nullptr,"Open...","Signal Files (*.iq2048);;All Files (*)");
	if (mFptr!=nullptr) fclose(mFptr);
	mFptr=fopen(filename.toLocal8Bit().data(),"rb");
	mLock.unlock();
}
void TDummy::handleStartStop()
{
	if (mRunning)
	{
		mStartStop->setText("Start");
		mRunning=false;
	} else {
		mStartStop->setText("Pause");
		mRunning=true;
	}
}
int TDummy::minValue()
{
	return -32767;
}
int TDummy::maxValue()
{
	return 32768;
}
