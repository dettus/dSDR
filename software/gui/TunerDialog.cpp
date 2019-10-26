#include "TunerDialog.h"
#include <QThread>

TunerDialog::TunerDialog()
{

}
TunerDialog::~TunerDialog()
{
	int i;
	if (mWindow!=nullptr) delete(mWindow);
	for (i=0;i<NUMBUTTONS;i++)
	{
		if (mButton[i]!=nullptr) delete(mButton[i]);
	}
}
int TunerDialog::askForTuner()
{
	int i;
	mLayout=new QVBoxLayout;
	mWindow=new QWidget(nullptr);
	for (i=0;i<NUMBUTTONS;i++)
	{
		mButton[i]=new QPushButton(cTunerButtons[i].text);
		mLayout->addWidget(mButton[i]);
		connect(mButton[i],SIGNAL(released()),this,SLOT(buttonReleased()));
	}	
	mWindow->setLayout(mLayout);
	mWindow->show();	
	while (mTunerIdReturnvalue==-1)
	{
		QThread::msleep(10);
	}
	return mTunerIdReturnvalue;
}
void TunerDialog::buttonReleased()
{
	int i;
	int retval;
	QString tmp;
	tmp=(QPushButton*)sender()->text();
	retval=mTunerIdReturnvalue;
	for (i=0;i<NUMBUTTONS;i++)
	{
		if (tmp==cTunerButtons[i].text) retval=i;	
	}
	mTunerIdReturnvalue=retval;
}
