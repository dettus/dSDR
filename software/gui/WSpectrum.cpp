#include "WSpectrum.h"

WSpectrum::WSpectrum(QWidget* parent):QWidget(parent)
{
	int i;
	mFft=new SimpleFft(mFftSize);
	mSampleBuf=new tSComplex[mFftSize];
	mSpectrum=new double[mFftSize];
	for (i=0;i<65536;i++) mSpectrum[i]=0;
}
void WSpectrum::setFFTsize(int fftsize)
{
	int i;
	if (fftsize!=mFftSize && (
		fftsize==256  ||
		fftsize==512  ||
		fftsize==1024 ||
		fftsize==2048 ||
		fftsize==4096 ||
		fftsize==8192 ||
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
		for (i=0;i<65536;i++) mSpectrum[i]=0;
	}
}
void WSpectrum::onNewSamples(tIQSamplesBlock *pIqSamples)
{
	int i;

	mSampleRate=pIqSamples->sampleRate;
	mCenterFreq=pIqSamples->frequency;
	mGain=pIqSamples->gain;
	for (i=0;i<pIqSamples->sampleNum;i++)
	{
		mSampleBuf[mSampleBufLevel]=pIqSamples->pData[i];	
		mSampleBufLevel++;
		if (mSampleBufLevel==mFftSize)
		{
			mFft->process(mSampleBuf);
			mFft->addSpectrum(mSpectrum);
			mFft->mFftCallcnt--;
			if (mFft->mFftCallcnt<=0)
			{
				// plot	
				mFft->mFftCallcnt=mFftAvgLen;
			}
			mSampleBufLevel=0;
		}
	}	
}

