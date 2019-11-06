#include "DemodWidget.h"

DemodWidget::DemodWidget(QWidget *parent):QWidget(parent)
{
	int i;
	mHLayout=new QHBoxLayout();
	mVLayout=new QVBoxLayout();
	mStackedLayout=new QStackedLayout();
	demod_buttons[0]=new QRadioButton("Demodulation off");
	demod_buttons[1]=new QRadioButton("AM");
	demod_buttons[2]=new QRadioButton("FM");
	demod_buttons[3]=new QRadioButton("** DRM-AM **");//	demod_buttons[3]->setCheckable(false);
	demod_buttons[4]=new QRadioButton("** DRM-FM **");//	demod_buttons[4]->setCheckable(false);
	demod_buttons[5]=new QRadioButton("** DAB+ **");//	demod_buttons[5]->setCheckable(false);

	// some dummy widgets, before the "real" ones are being implemented.
	demod_labels[0]=(QWidget*)(new QLabel("Demodulation deactivated"));
	demod_labels[1]=(QWidget*)(new QLabel("Amplitude Modulation"));
	demod_labels[2]=(QWidget*)(new QLabel("Frequency Modulation"));
	demod_labels[3]=(QWidget*)(new QLabel("Digital Radio Mondiale Mode A-D"));
	demod_labels[4]=(QWidget*)(new QLabel("Digital Radio Mondiale Mode E"));
	demod_labels[5]=(QWidget*)(new QLabel("Digital Audio Broadcasting"));


	for (i=0;i<BUTTON_NUM;i++)
	{	
		mVLayout->addWidget(demod_buttons[i]);
		connect(demod_buttons[i],SIGNAL(released()),this,SLOT(handleToggled()));
		mStackedLayout->addWidget(demod_labels[i]);
	}
	mHLayout->addLayout(mVLayout);
	mHLayout->addLayout(mStackedLayout);
	mHLayout->setStretch(0,10);
	mHLayout->setStretch(1,30);
	
	mStackedLayout->setCurrentIndex(mDemodMode);
	demod_buttons[mDemodMode]->setChecked(true);
		

	setLayout(mHLayout);

}

QSize DemodWidget::sizeHint()
{
	return QSize(500,50);
}

void DemodWidget::handleToggled()
{
	int i;
	int demodmode;
	demodmode=mDemodMode;
	for (i=0;i<BUTTON_NUM;i++)
	{
		if (demod_buttons[i]->isChecked()) demodmode=i;
	}
	if (demodmode!=mDemodMode)
	{
		printf("new demod mode:%d\n",demodmode);
	}
	mDemodMode=demodmode;
	mStackedLayout->setCurrentIndex(mDemodMode);
	
}
