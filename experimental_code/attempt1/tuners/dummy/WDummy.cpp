#include "WDummy.h"

WDummy::WDummy()
{
	mLayout=new QVBoxLayout;


	mStartStop=new QPushButton("Start");
	mOpen=new QPushButton("Open File...");
	mLabel=new QLabel("Please Select a Filename");
	mLayout->addWidget(mStartStop);
	mLayout->addWidget(mOpen);
	mLayout->addWidget(mLabel);
	setLayout(mLayout);

	connect(mStartStop,SIGNAL(released()),this,SLOT(handleStartStop()));
	connect(mOpen,SIGNAL(released()),this,SLOT(handleOpen()));
}

void WDummy::handleStartStop()
{
	if (mCbDummy!=nullptr)
	{
		if (!mStarted)
		{
			mStarted=true;
			mStartStop->setText("Pause");	
		} else {
			mStarted=false;
			mStartStop->setText("Start");	
		}
		mCbDummy->startStop(mStarted);
	}
}
void WDummy::handleOpen()
{
	if (mCbDummy!=nullptr)
	{
		QString fileName=QFileDialog::getOpenFileName(nullptr,"Open...","Signal Files (*.iq2048);;All Files (*)");
		mLabel->setText(fileName);
		mCbDummy->fileOpen(fileName.toLocal8Bit().data());	
	}
}
void WDummy::setCallback(CBDummy* cbDummy)
{
	mCbDummy=cbDummy;
}
