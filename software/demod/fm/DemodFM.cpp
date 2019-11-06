#include "DemodFM.h"
#include <math.h>


DemodFM::DemodFM(QWidget* parent):Demod(parent)
{
	mLabel=new QLabel("FREQUENCY DEMODULATION");
	mHLayout=new QHBoxLayout;
	mHLayout->addWidget(mLabel);
	setLayout(mHLayout);
}
void DemodFM::onNewSamples(tIQSamplesBlock *pSamples)
{
	int i;
	double angle;
	double delta;
	signed short pcm;
	for (i=0;i<pSamples->sampleNum;i++)
	{
		angle=atan2(pSamples->pData[i].imag,pSamples->pData[i].real);
		delta=angle-mAngle;
		mAngle=angle;
		if (delta<-M_PI) delta+=2*M_PI;
		if (delta> M_PI) delta-=2*M_PI;
		pcm=(signed short)(delta*8192.0/M_PI);
	}
}

