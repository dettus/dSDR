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
//					mSpectrumPlot[j]=1000*log(sqrt(mSpectrum[j]))/log(10);
					mSpectrumPlot[j]=1000*log((mSpectrum[j]))/log(10);
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

	int samplerate=mSampleRate;
	double nyquist=samplerate/(double)mFftSize;
	int freqleft=(int)(nyquist*(double)mLeft);
	int freqright=(int)(nyquist*(double)mRight);
	int freq,freqinc;;
	signed long long d;
	signed long long carrierleft,carrierright;
	freqleft-=samplerate/2;
	freqright-=samplerate/2;
	d=freqright-freqleft;
	
	if (d>=10000000) d=1000000;
	else if (d>=  250000) d=100000;
	else if (d>=   25000) d= 10000;
	else if (d>=    2500) d=  1000;
	else d=100;

	carrierleft=freqleft/d-1;
	carrierright=freqright/d+2;
	carrierleft*=d;
	carrierright*=d;
	
	freq=carrierleft;
	freqinc=d;

	carrierleft*=mFftSize;
	carrierright*=mFftSize;

	carrierleft/=samplerate;
	carrierright/=samplerate;
			
	d*=mFftSize;
	d/=samplerate;

	carrierleft+=mFftSize/2;
	carrierright+=mFftSize/2;
	
	painter.setPen(QColor(255,255,255,255));
	for (i=carrierleft;i<carrierright;i+=d)
	{
		int x;
		char tmp[64];
		x=(i-mLeft)*mWidth/(mRight-mLeft);

		if (x>=0 && x<=mWidth)
		{
			painter.drawLine(x,0,x,mHeight);

			snprintf(tmp,64,"%dHz",freq+mCenterFreq);

			painter.drawText(QRectF((double)x-50,0.0,100,20),Qt::AlignCenter,tmp);
		}
		freq+=freqinc;
	}

}
void WSpectrum::resizeEvent(QResizeEvent *event)
{
	mHeight=this->height();
	mWidth=this->width();
}
void WSpectrum::mousePressEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton)	)
	{
		mLastMoveEventX=event->pos().x();
		mLastMoveEventY=event->pos().y();
	}

}
void WSpectrum::mouseReleaseEvent(QMouseEvent *event)
{
	mLastMoveEventX=-1;
	mLastMoveEventY=-1;

}
void WSpectrum::mouseMoveEvent(QMouseEvent *event)
{
	int x,y;
	double dx,dy;
	int left,right;
	double upper,lower;

	dx=((double)mRight-(double)mLeft)/(double)mWidth;
	dy=((double)mUpper-(double)mLower)/(double)mHeight;
	left=mLeft;
	right=mRight;
	upper=mUpper;
	lower=mLower;
	if ((event->buttons() & Qt::LeftButton)	)
	{
		x=event->pos().x();
		if (mLastMoveEventX!=-1)
		{
			left+=(mLastMoveEventX-x)*dx;
			right+=(mLastMoveEventX-x)*dx;
		}
		y=event->pos().y();
		if (mLastMoveEventY!=-1)
		{
			upper+=(y-mLastMoveEventY)*dy;
			lower+=(y-mLastMoveEventY)*dy;
		}
		if (left>0&&left<mFftSize && right>0&&right<mFftSize)
		{
			mLeft=left;
			mRight=right;	
		}
		if (upper>=0 && upper<=1e16 && lower>=0 && lower<=1e16-32)
		{
			mLower=lower;
			mUpper=upper;
		}
		mLastMoveEventX=x;	
		mLastMoveEventY=y;
		update();
	}

}
void WSpectrum::wheelEvent(QWheelEvent *event)
{
	QPoint curPoint = event->pos();
	double x,y;
	double d;
	double fact;
	int left=mLeft;
	int right=mRight;



	x=(double)curPoint.x()/(double)mWidth;
	y=(double)curPoint.y()/(double)mHeight;
	d=right-left;

	fact=(right-left)/10;
	if (event->delta()>0) // mousewheel up
	{
		if (event->modifiers() & Qt::ControlModifier)
		{
			fact=(mUpper-mLower);
			//			mLower+=(double)(fact*y);
			mUpper-=(double)(fact*(1.0-y));
		} else {
			fact=(right-left)/10;
			left+=(int)(fact*x);
			right-=(int)(fact*(1.0-x));
		}
	}
	else if (event->delta()<0) // mousewheel down
	{
		if (event->modifiers() & Qt::ControlModifier)
		{
			fact=(mUpper-mLower)/10;
			mLower-=(double)fact;
			mUpper+=(double)fact;
		} else {
			fact=(right-left)/10;
			left-=(int)fact;
			right+=(int)fact;
		}
	}
	if (left<0) left=0;
	if (left>mFftSize-32) left=mFftSize-32;

	if (right>mFftSize || right<=left) right=mFftSize;
	if (mLower<0) mLower=0;
	if (mLower>(1e16-32)) mLower=(1e16-32);

	if (mUpper>1e16 || mUpper<=mLower) mUpper=1e16;
	printf("++++ mUpper:%10.f mLower:%10.f  left:%d right:%d\n",mUpper,mLower,left,right);
	mLeft=left;
	mRight=right;	
	update();

}
void WSpectrum::drawSpectrum(QPainter *painter,int yupper,int ylower,double min,double max)
{
	int i;
	int x,y;

	int right,left;
	double upper,lower;
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

