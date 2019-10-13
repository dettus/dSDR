#include "WWaterfall.h"

WWaterfall::WWaterfall(QWidget* parent) : QWidget(parent)
{
	mFftSize=1024;
	mUpper=5*65536;
	mLower=0;
	mLeft=0;
	mRight=mFftSize;
	if (mWaterfallImage!=nullptr) delete(mWaterfallImage);
	mWaterfallImage=nullptr;

// FIXME: remove the next few lines.

	mWaterfallImage=new QImage(1000,500,QImage::Format_ARGB32); 
	mWidth=1000;
	mHeight=500;
}
void WWaterfall::resizeEvent(QResizeEvent *event)
{
	mWidth=this->width();
	mHeight=this->height();
	if (mWaterfallImage!=nullptr) delete(mWaterfallImage);
	mWaterfallImage=new QImage(mWidth,mHeight,QImage::Format_ARGB32);
	QPainter painter(mWaterfallImage);
	
	repaint();
}
void WWaterfall::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QRectF full(0,0, mWidth,mHeight);
	painter.drawImage(full,*mWaterfallImage,full);
	
}

void WWaterfall::plotWaterfall(double* spectrum, int n)
{
	if (mWaterfallImage!=nullptr)
	{
		QImage tmpImage(mWidth,mHeight,QImage::Format_ARGB32);
		QPainter tmpPainter(&tmpImage);
		QPainter waterfallPainter(mWaterfallImage);
		int i;
		int x;
		double dx;



		mMin=mMax=spectrum[0];
		for (i=0;i<n;i++)
		{
			mSpectrum[i]=spectrum[i];
			if (mSpectrum[i]<mMin) mMin=mSpectrum[i];
			if (mSpectrum[i]>mMax) mMax=mSpectrum[i];
		}
		// with each new spectrum, the waterfall moves UP. 
		QRectF target(0,0, mWidth,mHeight-1);
		QRectF source(0,1, mWidth,mHeight  );
		tmpPainter.drawImage(target, *mWaterfallImage, source);	// move the image 1 pixel up
		waterfallPainter.drawImage(source, tmpImage, source);
		// the last line is now free to paint the new spectrum

		x=0;
		dx=(double)(mRight-mLeft)/(double)mWidth;
		for (i=mLeft;i<mRight;i++)
		{
			double nx;
			double r,g,b;

			r=((mSpectrum[i]-(double)mMin)*255.0)/(double)mMax;
			g=255-((mSpectrum[i]-(double)mMin)*255.0)/(double)mMax;
			b=((mSpectrum[i]-(double)mMin)*255.0)/(double)mMax;
			r=255;g=0;b=129;
			nx=dx*i;
			waterfallPainter.setPen(QColor((int)r,(int)g,(int)b,255));
			waterfallPainter.drawLine(x,mHeight-1, (int)nx,mHeight-1);
			x=(int)nx;
		}
		repaint();
	} else {
		printf("width:%d height:%d\n",mWidth,mHeight);
	}
}


void WWaterfall::setFFTsize(int size)
{
	mFftSize=size;
	repaint();
}


