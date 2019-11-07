#include "DemodWidget.h"
#include "DataTypes.h"
#include "DemodAM.h"
#include "DemodFM.h"

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
//	demod_labels[1]=(QWidget*)(new QLabel("Amplitude Modulation"));
//	demod_labels[2]=(QWidget*)(new QLabel("Frequency Modulation"));
	demod_labels[3]=(QWidget*)(new QLabel("Digital Radio Mondiale Mode A-D"));
	demod_labels[4]=(QWidget*)(new QLabel("Digital Radio Mondiale Mode E"));
	demod_labels[5]=(QWidget*)(new QLabel("Digital Audio Broadcasting"));

	demod_modules[1]=new DemodAM();
	demod_labels[1]=(QWidget*)demod_modules[1];
	demod_modules[2]=new DemodFM();
	demod_labels[2]=(QWidget*)demod_modules[2];


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
	mMutex.lock();
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
	mMutex.unlock();
}
void DemodWidget::onNewSamples(tIQSamplesBlock *pSamples,signed short* pcmBuf,int pcmBufSize,int* pcmNum,int* sampleRate)
{
	bool newResampler=false;
	int shiftfreq;
	shiftfreq=mShiftFreq;
	*pcmNum=0;
	*sampleRate=0;
	mMutex.lock();
	if (mDemodFreq!=0 && mDemodMode!=0 && demod_modules[mDemodMode]!=nullptr)
	{
		tSComplex shiftedSamples[2048];
		tSComplex downSamples[2048];
		int ridx;
		int n;
		tIQSamplesBlock shiftedBlock=*pSamples;
		tIQSamplesBlock downBlock=*pSamples;
		downBlock.pData=downSamples;
		if (pSamples->sampleRate!=mInSamplerate)
		{
			mInSamplerate=pSamples->sampleRate;
			newResampler=true;
		}
		shiftfreq=pSamples->centerFreq-mDemodFreq;
		if (mDemodMode!=0 && demod_modules[mDemodMode]!=nullptr)
		{
			shiftfreq+=demod_modules[mDemodMode]->getFreqOffset();
			if (demod_modules[mDemodMode]->getSampleRate()!=mOutSamplerate || demod_modules[mDemodMode]->getBandwidth()!=mBandwidth)
			{
				mOutSamplerate=demod_modules[mDemodMode]->getSampleRate();
				mBandwidth=demod_modules[mDemodMode]->getBandwidth();
				newResampler=true;
			}
			
		}
		if (newResampler || mDownsampler==nullptr)
		{
			if (mDownsampler!=nullptr) 
				delete(mDownsampler);
			mDownsampler=new Downsampler(mInSamplerate,mOutSamplerate,mBandwidth);
		}
		if (shiftfreq>=-mInSamplerate/2 && shiftfreq<=mInSamplerate/2)
		{
			if (shiftfreq!=mShiftFreq || mSimpleShifter==nullptr)
			{
				mShiftFreq=shiftfreq;
				if (mSimpleShifter!=nullptr)
					delete(mSimpleShifter);
				printf("\x1b[1;32mSHIFTFREQ:%d \x1b[0m\n",mShiftFreq);
				mSimpleShifter=new SimpleShifter(mInSamplerate,mShiftFreq);
			}
		}

		ridx=0;
		while (ridx<pSamples->sampleNum)
		{
			int num;
			n=pSamples->sampleNum-ridx;
			if (n>2048) n=2048;
			shiftedBlock.pData=shiftedSamples;
			shiftedBlock.centerFreq=pSamples->centerFreq-mShiftFreq;
			shiftedBlock.sampleNum=n;
			mSimpleShifter->process(&pSamples->pData[ridx],shiftedBlock.pData,n);
			mDownsampler->process(&shiftedBlock,&downBlock);
#if 0
			{
				static FILE *f1=nullptr;
				static FILE *f2=nullptr;

				if (f1==nullptr) f1=fopen("shifted.iq2048","wb");
				if (f2==nullptr) f2=fopen("downsampled.iq192","wb");
				fwrite(shiftedBlock.pData,sizeof(tSComplex),shiftedBlock.sampleNum,f1);
				fwrite(downBlock.pData,sizeof(tSComplex),downBlock.sampleNum,f2);
			}
#endif
			demod_modules[mDemodMode]->onNewSamples(&downBlock,&pcmBuf[*pcmNum],pcmBufSize,&num,sampleRate);
			pcmBufSize-=num;
			*pcmNum+=num;
			ridx+=n;

		}

	}
	mMutex.unlock();	
}
void DemodWidget::setDemodFrequency(int freqHz)
{
	int raster=100000;
	if (freqHz>=0) freqHz+=raster/2;
	else freqHz-=raster/2;
	freqHz-=(freqHz%raster);
	if (freqHz!=mDemodFreq)
	{
		mDemodFreq=freqHz;	
		printf("new demodfreq!i %dHz\n",mDemodFreq);
	}
}
