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
}
void WSpectrum::resizeEvent(QResizeEvent *event)
{
	mWidth=this->width();
	mHeight=this->height();
	repaint();
}
void WSpectrum::paintEvent(QPaintEvent *event)
{
	QPainter painter1(this);
	int i;
	int x;
	int y;
	double dx;
	double dy;

	

	x=0;
	y=mHeight;
	dx=(double)mWidth/(double)mFftSize;
	dy=(mMax-mMin)/(double)mHeight;

	painter1.fillRect(0,0,mWidth,mHeight,QColor(32,32,32,255));
	painter1.setPen(QColor(255,255,255,255));

	for (i=0;i<mFftSize;i++)
	{
		int nx;
		int ny;

		nx=(int)((double)i*dx);
		ny=mHeight-(int)((mSpectrum[i]-mMin)/dy);
		painter1.drawLine(x,y,nx,ny);
		x=nx;
		y=ny;
	}
}


void WSpectrum::mousePressEvent(QMouseEvent *event)
{

}

void WSpectrum::setFFTsize(int size)
{
	mFftSize=size;
	repaint();
}

void WSpectrum::plotSpectrum(double* spectrum, int n)
{
	int i;
	
	mMin=mMax=spectrum[0];
	for (i=0;i<n;i++)
	{
		mSpectrum[i]=spectrum[i];
		if (mSpectrum[i]<mMin) mMin=mSpectrum[i];
		if (mSpectrum[i]>mMax) mMax=mSpectrum[i];
	}
	repaint();
}
