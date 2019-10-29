#include "TunerDialog.h"

TunerDialog::TunerDialog(QWidget* parent): QWidget(parent)
{
	int i;
	mLayout=new QVBoxLayout;

	mTunerDummy=new TDummy();
	mTunerRtlTcp=new TRtlTcp();
	for (i=0;i<NUM_TUNER_BUTTONS;i++)
	{
		mButton[i]=new QPushButton(cTunerButtons[i].buttonlabeltext);	
		mLayout->addWidget(mButton[i]);
		connect(mButton[i],SIGNAL(released()),this,SLOT(buttonReleased()));
	}

	this->setLayout(mLayout);
	this->show();
	

	// to show this dialog in the center of the current screen,
	// we have to know what the size of the window AND the size
	// of the screen is. 
	QSize dialogSize=this->sizeHint();
	QRect screenRect=QGuiApplication::primaryScreen()->geometry(); 
	this->move(QPoint(	screenRect.width() /2 - dialogSize.width() /2,
				screenRect.height()/2 - dialogSize.height()/2));

	mLock.unlock();	
}
TunerDialog::~TunerDialog()
{
	
}
enum eTunerType TunerDialog::getSelectedTunerType()
{
	enum eTunerType retval;
	mLock.lock();
	retval=mTunerType;
	mLock.unlock();
	return retval;
}
Tuners* TunerDialog::getTuner()
{
	Tuners* retval;
	mLock.lock();
	retval=mTuner;
	mLock.unlock();
	return retval;
}
void TunerDialog::buttonReleased()
{
	int i;
	eTunerType retval;

	printf("RELEASED\n");

	retval=TUNER_UNDEF;
	// when one of the buttons has been pressed, find out which one it was.
	// the next time, the getSelectedTunerType() is being called, it will return
	// the last selected type.
	for (i=0;i<NUM_TUNER_BUTTONS;i++)
	{
		if (qobject_cast<QPushButton*>(sender())->text()==mButton[i]->text()) 
		{
			retval=cTunerButtons[i].type;
		}
	}

	// --> new tuners here
	mLock.lock();
	if (retval!=TUNER_UNDEF && retval!=TUNER_QUIT)
	{
		switch (retval)
		{
			case TUNER_DUMMY:
				mTuner=mTunerDummy;
				delete(mTunerRtlTcp);
				mTunerRtlTcp=nullptr;
				break;	
			case TUNER_RTLTCP:
				mTuner=mTunerRtlTcp;
				delete(mTunerDummy);
				mTunerDummy=nullptr;
				break;
			default:
				break;
		}	
	}
	// --> until HERE
	mTunerType=retval;
	mLock.unlock();
}
