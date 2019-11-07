#include "DemodAM.h"
#include <math.h>


DemodAM::DemodAM(QWidget* parent):Demod(parent)
{
	mLabel=new QLabel("AMPLITUDE DEMODULATION");
	mHLayout=new QHBoxLayout;
	mHLayout->addWidget(mLabel);
	setLayout(mHLayout);
}
void DemodAM::onNewSamples(tIQSamplesBlock *pSamples,signed short* pcmBuf,int pcmBufSize,int* pcmNum,int *sampleRate)
{
	int i;
	double s;
	double rpart,ipart;
	signed short pcm;
	for (i=0;i<pSamples->sampleNum;i++)
	{
		rpart=pSamples->pData[i].real;
		ipart=pSamples->pData[i].imag;
		pcm=(signed short)(sqrt(rpart*rpart+ipart*ipart));
		if (mCnt==0 || pcm<mMin) mMin=pcm;
		if (mCnt==0 || pcm>mMax) mMax=pcm;

		pcm-=mBias;
		pcm*=mFact;

		mCnt++;
		if (mCnt==5000)
		{
			mBias=(mMin+mMax)/2;	
			if (mMax!=mMin) 
				mFact=8192/(mMax-mMin); 
			else 
				mFact=1;
			mMin=mMax=0;
			mCnt=0;
		}
		// mono to stereo conversion
		if (i<pcmBufSize/2)
		{
			pcmBuf[i*2+0]=pcm;
			pcmBuf[i*2+1]=pcm;
		}
	}
	*pcmNum=pSamples->sampleNum*2;
	*sampleRate=pSamples->sampleRate;
}

