#include "WSpectrum.h"

WSpectrum::WSpectrum(QWidget* parent) : QWidget(parent)
{
	int i;

	mFftSize=1024;
	mMin=mMax=0;
	for (i=0;i<mFftSize;i++)
	{
		mSpectrum[i]=0;	
	}
	mUpper=65536*5;
	mLower=0;
	mLeft=0;
	mRight=mFftSize;
}
void WSpectrum::resizeEvent(QResizeEvent *event)
{
	mWidth=this->width();
	mHeight=this->height();
//	update();
}
void WSpectrum::paintEvent(QPaintEvent *event)
{
	QPainter painter1(this);
	int i;
	int x;
	int y;
	double dx;
	double dy;
	int upper,lower,right,left;

	right=mRight;
	if (right>mFftSize) right=mFftSize;
	left=mLeft;
	if (left>(right-32)) left=right-32;
	if (left<0) left=0;
	if (lower<0) lower=0;
	
	

	x=0;
	y=mHeight;
	dx=(double)mWidth/(double)(right-left);
//	dy=(mMax-mMin)/(double)mHeight;
	dy=(double)(mUpper-mLower)/(double)mHeight;

	painter1.fillRect(0,0,mWidth,mHeight,QColor(32,32,32,255));
	painter1.setPen(QColor(255,255,255,255));

	for (i=left;i<right;i++)
	{
		int nx;
		int ny;

		nx=(int)((double)(i-left)*dx);
		ny=mHeight-(int)((mSpectrum[i]-(double)mLower)/dy);
		painter1.drawLine(x,y,nx,ny);
		x=nx;
		y=ny;
	}
}


void WSpectrum::mousePressEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton)	)
	{
		mLastMoveEventX=event->pos().x();
		mLastMoveEventY=event->pos().y();
	}
	if ((event->buttons() & Qt::RightButton)	)
	{
		zoomFit();
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

void WSpectrum::setFFTsize(int size)
{
	mFftSize=size;
	//update();
}

void WSpectrum::plotSpectrum(double* spectrum, int n)
{
	int i;
	
	mMin=mMax=spectrum[0];
	for (i=0;i<n;i++)
	{
		mSpectrum[i]=sqrt(spectrum[i]);
		if (mSpectrum[i]<mMin) mMin=mSpectrum[i];
		if (mSpectrum[i]>mMax) mMax=mSpectrum[i];
	}
	update();
}
void WSpectrum::setZoom(int left,int right,double upper,double lower)
{
	mUpper=upper;
	mLower=lower;
	mRight=right;
	mLeft=left;
	update();
}
void WSpectrum::zoomFit()
{
	setZoom(0,mFftSize,mMax,0);
}

void WSpectrum::wheelEvent(QWheelEvent *event)
{
	QPoint curPoint = event->pos();
	double x,y;
	double d;
	double fact;

	
	x=(double)curPoint.x()/(double)mWidth;
	y=(double)curPoint.y()/(double)mHeight;
	d=mRight-mLeft;

	printf("mUpper:%10.f mLower:%10.f  left:%d right:%d\n",mUpper,mLower,mLeft,mRight);
	fact=(mRight-mLeft)/10;
	if (event->delta()>0) // mousewheel up
	{
		if (event->modifiers() & Qt::ControlModifier)
		{
			fact=(mUpper-mLower);
//			mLower+=(double)(fact*y);
			mUpper-=(double)(fact*(1.0-y));
		} else {
			fact=(mRight-mLeft)/10;
			mLeft+=(int)(fact*x);
			mRight-=(int)(fact*(1.0-x));
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
			fact=(mRight-mLeft)/10;
			mLeft-=(int)fact;
			mRight+=(int)fact;
		}
	}
	if (mLeft<0) mLeft=0;
	if (mLeft>mFftSize-32) mLeft=mFftSize-32;

	if (mRight>mFftSize || mRight<=mLeft) mRight=mFftSize;
	if (mLower<0) mLower=0;
	if (mLower>(1e16-32)) mLower=(1e16-32);

	if (mUpper>1e16 || mUpper<=mLower) mUpper=1e16;
	printf("++++ mUpper:%10.f mLower:%10.f  left:%d right:%d\n",mUpper,mLower,mLeft,mRight);
	update();
}
void WSpectrum::getZoom(int *left,int *right)
{
	*left=mLeft;
	*right=mRight;
}

