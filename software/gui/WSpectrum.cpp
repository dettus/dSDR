#include "WSpectrum.h"

WSpectrum::WSpectrum(QWidget* parent):QWidget(parent)
{
	int i;
	mFft=new SimpleFft(mFftSize);
	mSampleBuf=new tSComplex[mFftSize];
	mSpectrum=new double[mFftSize];
	for (i=0;i<mFftSize;i++) mSpectrum[i]=0;
}
void WSpectrum::setFFTsize(int fftsize)
{
	int i;
	if (fftsize!=mFftSize && (
		fftsize==  256||
		fftsize==  512||
		fftsize== 1024||
		fftsize== 2048||
		fftsize== 4096||
		fftsize== 8192||
		fftsize==16384||
		fftsize==32768||
		fftsize==65536)
	)
	{
		if (mFft!=nullptr) delete(mFft);
		if (mSampleBuf!=nullptr) delete(mSampleBuf);
		if (mSpectrum!=nullptr) delete(mSpectrum);
		mFftSize=fftsize;
		mFft=new SimpleFft(mFftSize);
		mSampleBufLevel=0;
		mSampleBuf=new tSComplex[mFftSize];
		mSpectrum=new double[mFftSize];
		for (i=0;i<mFftSize;i++) mSpectrum[i]=0;
	}
}
void WSpectrum::onNewSamples(tIQSamplesBlock *pIqSamples)
{
	int i;

	mSampleRate=pIqSamples->sampleRate;
	mCenterFreq=pIqSamples->centerFreq;
	mGain=pIqSamples->gain;
	for (i=0;i<pIqSamples->sampleNum;i++)
	{
		mSampleBuf[mSampleBufLevel]=pIqSamples->pData[i];	
		mSampleBufLevel++;
		if (mSampleBufLevel==mFftSize)
		{
			mFft->process(mSampleBuf);
			mFft->addSpectrum(mSpectrum);
			mFftCallcnt--;
			if (mFftCallcnt<=0)
			{
				// plot	
				mFftCallcnt=mFftAvgLen;
			}
			printf("update\n");
			mSampleBufLevel=0;
			update();
		}
	}	
}

void WSpectrum::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	mHeight=this->height();
	mWidth=this->width();
	drawSpectrum(&painter,0,mHeight*0.3);
	drawWaterfall(&painter,0.3*mHeight,mHeight*1.0);
}
void WSpectrum::resizeEvent(QResizeEvent *event)
{
	mHeight=this->height();
	mWidth=this->width();
}
void WSpectrum::mousePressEvent(QMouseEvent *event)
{

}
void WSpectrum::mouseReleaseEvent(QMouseEvent *event)
{

}
void WSpectrum::mouseMoveEvent(QMouseEvent *event)
{

}
void WSpectrum::wheelEvent(QWheelEvent *event)
{

}
void WSpectrum::drawSpectrum(QPainter *painter,int yupper,int ylower)
{
	int i;
	double max,min;
	double x,y;
	int oldx,oldy;
	painter->fillRect(0,yupper,mWidth,(ylower-yupper),QColor(32,32,32,255));
	min=max=mSpectrum[0];
	for (i=1;i<mFftSize;i++)
	{
		if (mSpectrum[i]<min) min=mSpectrum[i];
		if (mSpectrum[i]>max) max=mSpectrum[i];
	}
	oldx=0;
	oldy=mSpectrum[i];

	painter->setPen(QColor(255,255,255,255));	
	for (i=0;i<mFftSize;i++)
	{
		x=(double)i*(double)mWidth/(double)mFftSize;
		y=(mSpectrum[i]-min);
		y*=(ylower-yupper);
		y/=(max-min);
		y=ylower-y;

		painter->drawLine(oldx,oldy,x,y);
	
		oldx=(int)x;
		oldy=(int)y;
	}	
}
void WSpectrum::drawWaterfall(QPainter *painter,int yupper,int ylower)
{

}

