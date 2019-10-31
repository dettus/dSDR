#include "WSpectrum.h"

WSpectrum::WSpectrum(QWidget* parent):QWidget(parent)
{
	int i;
	mFft=new SimpleFft(mFftSize);
	mSampleBuf=new tSComplex[mFftSize];
	mSpectrum=new double[mFftSize];
	mSpectrumPlot=new double[mFftSize];
	for (i=0;i<mFftSize;i++) mSpectrum[i]=0;

	mWaterfallImage=new QImage(WATERFALLWIDTH,WATERFALLHEIGHT,QImage::Format_ARGB32);
	QPainter waterfallPainter(mWaterfallImage);
	for (i=0;i<WATERFALLNUANCES;i++)
	{
#if 1
		if (i<256)
		{
			mRgbPalette[i]=QColor(i,0,255-i,255);

		} else {
			mRgbPalette[i]=QColor(255,i-256,i-256,255);
		}
#else
		mRgbPalette[i]=QColor(i/2,i/2,i/2,255);
#endif
	}
	waterfallPainter.fillRect(0,0,WATERFALLWIDTH,WATERFALLHEIGHT,mRgbPalette[0]);

}
QSize WSpectrum::sizeHint() const
{
	return QSize(100, 110);
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
		if (mSpectrumPlot!=nullptr) delete(mSpectrumPlot);
		mFftSize=fftsize;
		mFft=new SimpleFft(mFftSize);
		mSampleBufLevel=0;
		mSampleBuf=new tSComplex[mFftSize];
		mSpectrum=new double[mFftSize];
		mSpectrumPlot=new double[mFftSize];
		for (i=0;i<mFftSize;i++) mSpectrum[i]=0;

		mLeft=0;
		mRight=mFftSize;
		mUpper=400000000;
		mLower=0;
	}
}
void WSpectrum::onNewSamples(tIQSamplesBlock *pIqSamples)
{
	int i,j;
	int samplenum=pIqSamples->sampleNum;
	mSampleRate=pIqSamples->sampleRate;
	mCenterFreq=pIqSamples->centerFreq;
	mGain=pIqSamples->gain;
	for (i=0;i<samplenum;i++)
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
				for (j=0;j<mFftSize;j++)
				{
					mSpectrumPlot[j]=100*log(sqrt(mSpectrum[j]))/log(10);
					mSpectrum[j]=0;	
				}
				mFftCallcnt=mFftAvgLen;
				update();
			}
			mSampleBufLevel=0;
		}
	}	
}

void WSpectrum::paintEvent(QPaintEvent *event)
{
	int i;
	double min,max;
	QPainter painter(this);
	mHeight=this->height();
	mWidth=this->width();
	min=max=0;
	for (i=0;i<mFftSize;i++)
	{
		if (i==0 || min<mSpectrumPlot[i]) min=mSpectrumPlot[i];
		if (i==0 || max>mSpectrumPlot[i]) max=mSpectrumPlot[i];
	}
	drawSpectrum(&painter,0,mHeight*0.3,min,max);
	drawWaterfall(&painter,0.3*mHeight,mHeight*1.0,min,max);

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
void WSpectrum::drawSpectrum(QPainter *painter,int yupper,int ylower,double min,double max)
{
	int i;
	int x,y;

	int upper,lower,right,left;
	double dx,dy;

	right=mRight;
	if (right>mFftSize) right=mFftSize;
	left=mLeft;
	upper=mUpper;
	lower=mLower;
	if (left>(right-32)) left=right-32;
	if (left<0) left=0;
	if (lower<0) lower=0;

	painter->fillRect(0,yupper,mWidth,(ylower-yupper),QColor(32,32,32,255));
	upper=max;
	lower=min;
	printf("%f max   %f min\n",max,min);
	dx=(double)mWidth/(double)(right-left);
	dy=(double)(upper-lower)/(double)(ylower-yupper);


	painter->setPen(QColor(255,255,255,255));	
	x=0;
	y=0;
	for (i=left;i<right;i++)
	{
		int nx,ny;

		nx=(int)((double)(i-left)*dx);
		ny=yupper+(int)((mSpectrumPlot[i]-(double)lower)/dy);
		if (i==left) 
		{
			x=nx;
			y=ny;
		}	
	
		painter->drawLine(x,y, nx,ny);
	
		x=nx;
		y=ny;
	}
	
}
void WSpectrum::drawWaterfall(QPainter *painter,int yupper,int ylower,double min,double max)
{
	int i;
	QImage tmpImage1(WATERFALLWIDTH,WATERFALLHEIGHT,QImage::Format_ARGB32);
	QPainter tmpPainter1(&tmpImage1);
	QPainter waterfallPainter(mWaterfallImage);

	double delta;

		
	if (max!=min)
	{

		// with each new spectrum, the waterfall moves DOWN
		QRectF target1(0,1, WATERFALLWIDTH,WATERFALLHEIGHT-1);
		QRectF source1(0,0, WATERFALLWIDTH,WATERFALLHEIGHT-1);
		tmpPainter1.drawImage(target1, *mWaterfallImage, source1); // move the image 1 pixel up
		waterfallPainter.drawImage(source1, tmpImage1, source1);
		waterfallPainter.setPen(QColor(0,255,0,255));
		
		// the upper line is now free to paint the new spectrum


		for (i=0;i<WATERFALLWIDTH;i++)
		{
			double y;

			y=((mSpectrumPlot[i]-min)*WATERFALLNUANCES)/(max-min);
			waterfallPainter.setPen(mRgbPalette[WATERFALLNUANCES-1-(int)y]);
			waterfallPainter.drawPoint(i,0);
		}

		// 
		delta=((double)(mRight-mLeft)*WATERFALLWIDTH)/(double)mFftSize;
		QImage tmpImage2((int)delta,WATERFALLHEIGHT,QImage::Format_ARGB32);
		QPainter tmpPainter2(&tmpImage2);
		QRectF source2(mLeft,0,delta,WATERFALLHEIGHT);
		QRectF target2(0,0,delta,WATERFALLHEIGHT);
		QRectF source3(0,0,mWidth,(ylower-yupper));
		QRectF target3(0,yupper,mWidth,(ylower-yupper));
		QSize size(mWidth,ylower-yupper);
		tmpPainter2.drawImage(target2,*mWaterfallImage,source2);

		painter->drawImage(target3,tmpImage2.scaled(size),source3);

	}
}

