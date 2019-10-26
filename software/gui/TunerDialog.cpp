#include "TunerDialog.h"
#include <QThread>

TunerDialog::TunerDialog()
{
	int i;
	mLayout=new QVBoxLayout;
	mWindow=new QWidget(nullptr);
	for (i=0;i<NUMBUTTONS;i++)
	{
		mButton[i]=new QPushButton(cTunerButtons[i].text);
		mLayout->addWidget(mButton[i]);
		//connect(mButton[i],SIGNAL(released()),this,SLOT(buttonReleased(i)));
		connect(mButton[i],SIGNAL(released()),this,SLOT(buttonReleased()));
	}	
	mWindow->setLayout(mLayout);
	mWindow->show();	
	QSize mSize = mWindow->sizeHint(); // here's what you want, not m.width()/height()
//	QRect screenRect = QDesktopWidget().screen()->rect();
	QRect screenRect = QGuiApplication::primaryScreen()->geometry();
	mWindow->move( QPoint( screenRect.width()/2 - mSize.width()/2,
				screenRect.height()/2 - mSize.height()/2 ) );
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
enum eTunerId TunerDialog::getTunerValue()
{
	return mTunerIdReturnvalue;
}

void TunerDialog::buttonReleased()
{
	int i;
	eTunerId retval;
	QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
	retval=mTunerIdReturnvalue;
	printf("RELEASED [%s]\n",buttonSender->text().data());
	for (i=0;i<NUMBUTTONS;i++)
	{
		if (buttonSender->text()==mButton[i]->text()) retval=cTunerButtons[i].id;
	}
	mTunerIdReturnvalue=retval;
}
