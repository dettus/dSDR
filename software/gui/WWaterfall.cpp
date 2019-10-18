#include "WWaterfall.h"

WWaterfall::WWaterfall(QWidget* parent) : QWidget(parent)
{
	int i;

	mFftSize=1024;
	mMin=mMax=0;
	mUpper=5*65536;
	mLower=0;
	mLeft=0;
	mRight=mFftSize;
	if (mWaterfallImage!=nullptr) delete(mWaterfallImage);
	mWaterfallImage=nullptr;

}
void WWaterfall::resizeEvent(QResizeEvent *event)
{
//	if (mWaterfallImage!=nullptr) delete(mWaterfallImage);
//	mWaterfallImage=new QImage(mWidth,mHeight,QImage::Format_ARGB32);
//	painter.fillRect(0,0,mWidth,mHeight,QColor(32,32,32,255));
	if (mWidth!=this->width() || (mHeight!=this->height()))
	{
		mWidth=this->width();
		mHeight=this->height();
		if (mWaterfallImage!=nullptr) delete(mWaterfallImage);
		mWaterfallImage=new QImage(mWidth,mHeight,QImage::Format_ARGB32); 
	}
}
void WWaterfall::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	if (mWidth!=this->width() || (mHeight!=this->height()))
	{
		mWidth=this->width();
		mHeight=this->height();
		if (mWaterfallImage!=nullptr) delete(mWaterfallImage);
		mWaterfallImage=new QImage(mWidth,mHeight,QImage::Format_ARGB32); 
	}
	QRectF full(0,0, mWidth,mHeight);
	painter.drawImage(full,*mWaterfallImage,full);

}


void WWaterfall::mousePressEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton)	)
	{
		mLastMoveEventX=event->pos().x();
	}
	if ((event->buttons() & Qt::RightButton)	)
	{
		zoomFit();
	}
}

void WWaterfall::mouseMoveEvent(QMouseEvent *event)
{
	int x,y;
	double dx;
	int left,right;

	dx=((double)mRight-(double)mLeft)/(double)mWidth;
	left=mLeft;
	right=mRight;
	if ((event->buttons() & Qt::LeftButton)	)
	{
		x=event->pos().x();
		if (mLastMoveEventX!=-1)
		{
			left+=(mLastMoveEventX-x)*dx;
			right+=(mLastMoveEventX-x)*dx;
		}
		if (left>0&&left<mFftSize && right>0&&right<mFftSize)
		{
			mLeft=left;
			mRight=right;
		}
		mLastMoveEventX=x;	
		update();
	}
}


void WWaterfall::setFFTsize(int size)
{
	mFftSize=size;
	mLeft=0;
	mRight=mFftSize;
	update();
}

void WWaterfall::plotWaterfall(double* spectrum, int n)
{
	int right,left;

	right=mRight;
	if (right>mFftSize) right=mFftSize;
	left=mLeft;
	if (left>(right-32)) left=right-32;
	if (left<0) left=0;

	if (mWaterfallImage!=nullptr)
	{
		QImage tmpImage(mWidth,mHeight,QImage::Format_ARGB32);
		QPainter tmpPainter(&tmpImage);
		QPainter waterfallPainter(mWaterfallImage);
		int i;
		int x;
		double dx;
		double dy;
		double maxy;


		mMin=mMax=sqrt(sqrt(spectrum[0]));
		for (i=0;i<n;i++)
		{
			double s;
			s=sqrt(sqrt(spectrum[i]));
			if (s<mMin) mMin=s;
			if (s>mMax) mMax=s;
		}
		// with each new spectrum, the waterfall moves UP. 
		QRectF target(0,0, mWidth,mHeight-1);
		QRectF source(0,0, mWidth,mHeight);
		tmpPainter.drawImage(target, *mWaterfallImage, source);	// move the image 1 pixel up
		waterfallPainter.drawImage(source, tmpImage, source);
		// the last line is now free to paint the new spectrum

		x=0;
		dx=(double)mWidth/(double)(mRight-mLeft);
		dy=(mMax-mMin);
		maxy=0;
		for (i=mLeft;i<mRight;i++)
		{
			double nx;
			double y;

			y=sqrt(sqrt(spectrum[i]))-mMin;

			nx=dx*(i-mLeft);
			if (y>maxy) maxy=y;
			if ((int)nx!=x)
			{
				double r,g,b;
				r=(maxy*255.0)/dy;
				g=64-(maxy*64.0)/dy;
				b=255-(maxy*255.0)/dy;
				waterfallPainter.setPen(QColor((int)r,(int)g,(int)b,255));
				waterfallPainter.drawLine(x,mHeight-1, (int)nx,mHeight-1);
				x=(int)nx;
				maxy=0;
			}

		}
		update();
	} else {
	}
}
void WWaterfall::setZoom(int left,int right,double upper,double lower)
{
	mUpper=upper;
	mLower=lower;
	mRight=right;
	mLeft=left;
	update();
}
void WWaterfall::setZoom(int left,int right)
{
	mRight=right;
	mLeft=left;
	update();

}
void WWaterfall::zoomFit()
{
	setZoom(0,mFftSize,mMax,0);
}
void WWaterfall::wheelEvent(QWheelEvent *event)
{
	QPoint curPoint = event->pos();
	double x;
	double d;
	double fact;

	
	x=(double)curPoint.x()/(double)mWidth;
	d=mRight-mLeft;

	fact=(mRight-mLeft)/10;
	if (event->delta()>0) // mousewheel up
	{

		fact=(mRight-mLeft)/10;
		mLeft+=(int)(fact*x);
		mRight-=(int)(fact*(1.0-x));
	}
	else if (event->delta()<0) // mousewheel down
	{
		fact=(mRight-mLeft)/10;
		mLeft-=(int)fact;
		mRight+=(int)fact;
	}
	if (mLeft<0) mLeft=0;
	if (mLeft>mFftSize-32) mLeft=mFftSize-32;

	if (mRight>mFftSize || mRight<=mLeft) mRight=mFftSize;

	update();
}


