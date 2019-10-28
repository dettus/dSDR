#include "StartupDialog.h"

StartupDialog::StartupDialog(QWidget* parent): QWidget(parent)
{
	int i;
	mLayout=new QVBoxLayout;
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
}
StartupDialog::~StartupDialog()
{
	
}
enum eTunerType StartupDialog::getSelectedTunerType()
{
	return mTunerType;
}
void StartupDialog::buttonReleased()
{
	int i;
	eTunerType retval;

	printf("RELEASED\n");

	retval=mTunerType;
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
	mTunerType=retval;
}
