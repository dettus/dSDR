#include "WSpectrum2.h"


WSpectrum::WSpectrum(QWidget* parent): QWidget(parent)
{
	int i;
	mWaterfallImage1=new QImage(mFftSize,WATERFALLHEIGHT,QImage::Format_ARGB32);
	mWaterfallImage2=new QImage(mFftSize,WATERFALLHEIGHT,QImage::Format_ARGB32);
	mFft=new SimpleFft(mFftSize);
	mFftCnt=mFftAvg;

#if 1			// grey scale to greenish cyan
	for (i=0;i<WATERFALLNUANCES;i++)
	{
		if (i<64)
		{
			mPalette[i]=QColor(i,i,i,255);

		} else {
			mPalette[i]=QColor((int)((double)i*0.2),(int)((double)i*0.8),128,255);
		}
	}
#else		// blue/red
	for (i=0;i<WATERFALLNUANCES;i++)
	{
		mPalette[i]=QColor((i*255)/WATERFALLNUANCES,0,255-((i*255)/WATERFALLNUANCES),255);
	}
#endif
	mSampleBufLevel=0;
	mSampleBuf=new tSComplex[mFftSize];
	mSpectrum=new double[mFftSize];
	mSpectrumPlot=new double[mFftSize];
	QPainter waterfallPainter1(mWaterfallImage1);
	QPainter waterfallPainter2(mWaterfallImage2);

	waterfallPainter1.fillRect(0,0,mFftSize,WATERFALLHEIGHT,mPalette[0]);
	waterfallPainter2.fillRect(0,0,mFftSize,WATERFALLHEIGHT,mPalette[0]);
}
void WSpectrum::setFFTsize(int fftSize)
{
	if (fftSize!=mFftSize && (
				fftSize==  256||
				fftSize==  512||
				fftSize== 1024||
				fftSize== 2048||
				fftSize== 4096||
				fftSize== 8192||
				fftSize==16384||
				fftSize==32768||
				fftSize==65536)
	   )
	{
		mFftSize=fftSize;
		mFftCnt=mFftAvg;
		if (mFft!=nullptr) delete(mFft);
		mFft=new SimpleFft(mFftSize);
		mFftCnt=mFftAvg;
		if (mWaterfallImage1!=nullptr) delete(mWaterfallImage1);
		if (mWaterfallImage2!=nullptr) delete(mWaterfallImage2);
		mWaterfallImage1=new QImage(mFftSize,WATERFALLHEIGHT,QImage::Format_ARGB32);
		mWaterfallImage2=new QImage(mFftSize,WATERFALLHEIGHT,QImage::Format_ARGB32);
		mWaterfallLine=0;
		QPainter waterfallPainter1(mWaterfallImage1);
		QPainter waterfallPainter2(mWaterfallImage2);

		waterfallPainter1.fillRect(0,0,mFftSize,WATERFALLHEIGHT,QColor(255,0,0,255));
		waterfallPainter2.fillRect(0,0,mFftSize,WATERFALLHEIGHT,QColor(0,255,0,255));
		if (mSpectrum!=nullptr) delete(mSpectrum);
		if (mSpectrumPlot!=nullptr) delete(mSpectrumPlot);
		if (mSampleBuf!=nullptr) delete(mSampleBuf);
		mSampleBufLevel=0;
		mSampleBuf=new tSComplex[mFftSize];
		mSpectrum=new double[mFftSize];
		mSpectrumPlot=new double[mFftSize];
		mLeft=0;
		mRight=mFftSize;
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
			mFftCnt--;
			if (mFftCnt<=0)
			{
				for (j=0;j<mFftSize;j++)
				{
//					mSpectrum[j]=1000*log(sqrt(mSpectrum[j]))/log(10);
					mSpectrumPlot[j]=1000*log((mSpectrum[j]))/log(10);
				}
				mFftCnt=mFftAvg;
				updateWaterfall();
				for (j=0;j<mFftSize;j++)
				{
					mSpectrum[j]=0;
				}
			}
			mSampleBufLevel=0;
		}
	}
	update();
}

void WSpectrum::updateWaterfall()
{
	int min,max,i;

	

	// the waterfall moves DOWNWARDS.
	// a new line is added to the top.
	// then the whole waterfall is moved.
	// the way this is being realized is by
	// creating two images in the background,
	// and having the paintevent draw them when
	// it has the time.
	// a new line is added to THE BOTTOM of one
	// of the images. the paintevent is drawing
	// the lower lines ON TOP later.	
	QPainter waterfallPainter1(mWaterfallImage1);
	QPainter waterfallPainter2(mWaterfallImage2);

	min=max=0;
	for (i=0;i<mFftSize;i++)
	{
		if ((i==0) || min>mSpectrumPlot[i]) min=mSpectrumPlot[i];
		if ((i==0) || max<mSpectrumPlot[i]) max=mSpectrumPlot[i];
	}
	for (i=0;i<mFftSize;i++)
	{
		double y;
		y=((mSpectrumPlot[i]-min)*WATERFALLNUANCES)/(max-min);

		if (mWaterfallLine<WATERFALLHEIGHT)
		{
			if (y>=0 && y<WATERFALLNUANCES) waterfallPainter1.setPen(mPalette[(int)y]);
			waterfallPainter1.drawPoint(i,WATERFALLHEIGHT-mWaterfallLine-1);	// fill the image BOTTOM UP
		} else {
			if (y>=0 && y<WATERFALLNUANCES) waterfallPainter2.setPen(mPalette[(int)y]);
			waterfallPainter2.drawPoint(i,2*WATERFALLHEIGHT-mWaterfallLine-1);	// fill the image BOTTOM UP
		}
	}
	mWaterfallLine=(mWaterfallLine+1)%(2*WATERFALLHEIGHT);;
}	
void WSpectrum::paintEvent(QPaintEvent *event)
{
	int width=this->width();
	int height=this->height();
	int yupper_spectrum =(int)(0.0*height);
	int ylower_spectrum =(int)(0.3*height);
	int yupper_waterfall=(int)(0.3*height);
	int ylower_waterfall=(int)(1.0*height);
	QPainter painter(this);

	// draw the waterfall diagram, scaled
	{
		int delta;
		delta=mRight-mLeft;
		// first, cut out the portion that is to be scaled
		QImage		tmpImage(delta,WATERFALLHEIGHT,QImage::Format_ARGB32);
		QPainter	tmpPainter(&tmpImage);

		if (mWaterfallLine<WATERFALLHEIGHT)	// case 1: draw the lower lines of image1 on top, and image 2 below
		{
			// mWaterfallLine is between 0... and WATERFALLHEIGHT-1
			QRectF	source1(mLeft,WATERFALLHEIGHT-mWaterfallLine,delta,mWaterfallLine);	// select the bottom lines
			QRectF	target1(0,0,delta,mWaterfallLine);					// are being painted to the top
			QRectF	source2(mLeft,0,delta,WATERFALLHEIGHT-mWaterfallLine);			// the highest lines from the older image
			QRectF	target2(0,mWaterfallLine,delta,WATERFALLHEIGHT-mWaterfallLine-1);	// are being painted on the bottom
			tmpPainter.drawImage(target1,*mWaterfallImage1,source1);
			tmpPainter.drawImage(target2,*mWaterfallImage2,source2);
		} else {	// case 2: draw image2 on top, and image 1 below it.
			// mWaterfallLine is between WATERFALLHEIGHT and 2*WATERFALLHEIGHT-1
			QRectF	source2(mLeft,2*WATERFALLHEIGHT-mWaterfallLine,delta,mWaterfallLine-WATERFALLHEIGHT);	// select the bottom lines
			QRectF	target2(0,0,delta,mWaterfallLine-WATERFALLHEIGHT);					// are being painted to the top
			QRectF	source1(mLeft,0,delta,2*WATERFALLHEIGHT-mWaterfallLine);			// the highest lines from the older image
			QRectF	target1(0,mWaterfallLine-WATERFALLHEIGHT,delta,2*WATERFALLHEIGHT-mWaterfallLine-1);	// are being painted on the bottom
			tmpPainter.drawImage(target1,*mWaterfallImage1,source1);
			tmpPainter.drawImage(target2,*mWaterfallImage2,source2);
		}


		// second: scale the cut out portion up.
		QSize		size(width,ylower_waterfall-yupper_waterfall);
		QRectF		source2(0,0,width,(ylower_waterfall-yupper_waterfall));
		QRectF		target2(0,yupper_waterfall,width,(ylower_waterfall-yupper_waterfall));
		painter.drawImage(target2,tmpImage.scaled(size),source2);
	}

	// draw the fft spectrum
	{
		double min,max;
		int i;
		int x,y;
		double dx,dy;

		painter.fillRect(0,yupper_spectrum,width,ylower_spectrum-yupper_spectrum,QColor(32,32,32,255));

		

		min=max=0;
		for (i=0;i<mFftSize;i++)
		{
			if (i==0 || min>mSpectrumPlot[i]) min=mSpectrumPlot[i];
			if (i==0 || max<mSpectrumPlot[i]) max=mSpectrumPlot[i];
		}
		x=y=0;	
		
		dx=(double)(width)/(double)(mRight-mLeft);
		dy=(double)(max-min)/(double)(ylower_spectrum-yupper_spectrum);

		painter.setPen(QColor(0,255,255,255));
		for (i=mLeft;i<mRight;i++)
		{
			int nx,ny;

			nx=(int)((double)(i-mLeft)*dx);
			ny=ylower_spectrum-(int)((mSpectrumPlot[i]-(double)min)/dy);
			if (i==mLeft)
			{
				x=nx;
				y=ny;
			}
			painter.drawLine(x,y,nx,ny);
			x=nx;
			y=ny;
		}
	}	

	// the finishing touch: draw the scale

	if (mSampleRate)
	{
	        QFont font=painter.font();

		int samplerate=mSampleRate;
		double nyquist=samplerate/(double)mFftSize;
		int freqleft=(int)(nyquist*(double)mLeft);
		int freqright=(int)(nyquist*(double)mRight);
		int freq,freqinc;;
		signed long long d;
		signed long long carrierleft,carrierright;
		int i;
		int w;
		w=width/10;
		font.setPointSize(w/10);
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
			x=(i-mLeft)*width/(mRight-mLeft);

			if (x>=0 && x<=width)
			{
				painter.drawLine(x,0,x,height);

				snprintf(tmp,64,"%dHz",freq+mCenterFreq);

				painter.drawText(QRectF((double)x-w/2,0.0,w,20),Qt::AlignCenter,tmp);
			}
			freq+=freqinc;
		}


	}	
}
QSize WSpectrum::sizeHint() const
{
	return QSize(100, 110);
}

void WSpectrum::resizeEvent(QResizeEvent *event) {}
void WSpectrum::mousePressEvent(QMouseEvent *event) {}
void WSpectrum::mouseReleaseEvent(QMouseEvent *event) {}
void WSpectrum::mouseMoveEvent(QMouseEvent *event) {}
void WSpectrum::wheelEvent(QWheelEvent *event) 
{
	QPoint curPoint = event->pos();
	double x,y;
	double cy;
	double d;
	double fact;
	int width=this->width();
	int height=this->height();
	int left=mLeft;
	int right=mRight;



	cy=curPoint.y();
	x=(double)curPoint.x()/(double)width;
	y=(double)cy/(0.3*(double)height);
	d=right-left;

	fact=(right-left)/10;
	if (event->delta()>0) // mousewheel up
	{
		if (event->modifiers() & Qt::ControlModifier && cy<(0.3*(double)height))
		{
			//fact=(mUpper-mLower);
			//			mLower+=(double)(fact*y);
			//mUpper-=(double)(fact*(1.0-y));
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
			//fact=(mUpper-mLower)/10;
			//mLower-=(double)fact;
			//mUpper+=(double)fact;
		} else {
			fact=(right-left)/10;
			left-=(int)fact;
			right+=(int)fact;
		}
	}
	if (left<0) left=0;
	if (left>mFftSize-32) left=mFftSize-32;

	if (right>mFftSize || right<=left) right=mFftSize;
	//if (mLower<0) mLower=0;
	//if (mLower>(1e16-32)) mLower=(1e16-32);

	//if (mUpper>1e16 || mUpper<=mLower) mUpper=1e16;
	//printf("++++ mUpper:%10.f mLower:%10.f  left:%d right:%d\n",mUpper,mLower,left,right);
	mLeft=left;
	mRight=right;	
	update();

	
}

