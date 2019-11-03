#include "WSpectrum2.h"


WSpectrum::WSpectrum(QWidget* parent): QWidget(parent)
{
	int i;
	mWaterfallImage1=new QImage(mFftSize,WATERFALLHEIGHT,QImage::Format_ARGB32);
	mWaterfallImage2=new QImage(mFftSize,WATERFALLHEIGHT,QImage::Format_ARGB32);
	mFft=new SimpleFft(mFftSize);
	mFftCnt=mFftAvg;

	for (i=0;i<WATERFALLNUANCES;i++)
	{
		mPalette[i]=QColor((i*255)/WATERFALLNUANCES,0,255-((i*255)/WATERFALLNUANCES),255);
	}
	mSampleBufLevel=0;
	mSampleBuf=new tSComplex[mFftSize];
	mSpectrum=new double[mFftSize];
	QPainter waterfallPainter1(mWaterfallImage1);
	QPainter waterfallPainter2(mWaterfallImage2);

	waterfallPainter1.fillRect(0,0,mFftSize,WATERFALLHEIGHT,QColor(255,0,0,255));
	waterfallPainter2.fillRect(0,0,mFftSize,WATERFALLHEIGHT,QColor(0,255,0,255));
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
		if (mSampleBuf!=nullptr) delete(mSampleBuf);
		mSampleBufLevel=0;
		mSampleBuf=new tSComplex[mFftSize];
		mSpectrum=new double[mFftSize];
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
					mSpectrum[j]=1000*log((mSpectrum[j]))/log(10);
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
		if ((i==0) || min>mSpectrum[i]) min=mSpectrum[i];
		if ((i==0) || max<mSpectrum[i]) max=mSpectrum[i];
	}
	for (i=0;i<mFftSize;i++)
	{
		double y;
		y=((mSpectrum[i]-min)*WATERFALLNUANCES)/(max-min);

		if (mWaterfallLine<WATERFALLHEIGHT)
		{
			waterfallPainter1.setPen(mPalette[(int)y]);
			waterfallPainter1.drawPoint(i,WATERFALLHEIGHT-mWaterfallLine-1);	// fill the image BOTTOM UP
		} else {
			waterfallPainter2.setPen(mPalette[(int)y]);
			waterfallPainter2.drawPoint(i,2*WATERFALLHEIGHT-mWaterfallLine-1);	// fill the image BOTTOM UP
		}
	}
	mWaterfallLine=(mWaterfallLine+1)%(2*WATERFALLHEIGHT);;
}	
void WSpectrum::paintEvent(QPaintEvent *event)
{
	int yupper_spectrum =(int)(0.0*this->height());
	int ylower_spectrum =(int)(0.3*this->height());
	int yupper_waterfall=(int)(0.3*this->height());
	int ylower_waterfall=(int)(1.0*this->height());
	int width=this->width();
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
			QRectF	target1(mLeft,0,delta,mWaterfallLine);					// are being painted to the top
			QRectF	source2(mLeft,0,delta,WATERFALLHEIGHT-mWaterfallLine);			// the highest lines from the older image
			QRectF	target2(mLeft,mWaterfallLine,delta,WATERFALLHEIGHT-mWaterfallLine-1);	// are being painted on the bottom
			tmpPainter.drawImage(target1,*mWaterfallImage1,source1);
			tmpPainter.drawImage(target2,*mWaterfallImage2,source2);
		} else {	// case 2: draw image2 on top, and image 1 below it.
			// mWaterfallLine is between WATERFALLHEIGHT and 2*WATERFALLHEIGHT-1
			QRectF	source2(mLeft,2*WATERFALLHEIGHT-mWaterfallLine,delta,mWaterfallLine-WATERFALLHEIGHT);	// select the bottom lines
			QRectF	target2(mLeft,0,delta,mWaterfallLine-WATERFALLHEIGHT);					// are being painted to the top
			QRectF	source1(mLeft,0,delta,2*WATERFALLHEIGHT-mWaterfallLine);			// the highest lines from the older image
			QRectF	target1(mLeft,mWaterfallLine-WATERFALLHEIGHT,delta,2*WATERFALLHEIGHT-mWaterfallLine-1);	// are being painted on the bottom
			tmpPainter.drawImage(target1,*mWaterfallImage1,source1);
			tmpPainter.drawImage(target2,*mWaterfallImage2,source2);
		}


		// second: scale the cut out portion up.
		QSize		size(width,ylower_waterfall-yupper_waterfall);
		QRectF		source2(0,0,width,(ylower_waterfall-yupper_waterfall));
		QRectF		target2(0,yupper_waterfall,width,(ylower_waterfall-yupper_waterfall));
		painter.drawImage(target2,tmpImage.scaled(size),source2);
	}

	// the finishing touch: draw the scale
}
QSize WSpectrum::sizeHint() const
{
	return QSize(100, 110);
}

void WSpectrum::resizeEvent(QResizeEvent *event) {}
void WSpectrum::mousePressEvent(QMouseEvent *event) {}
void WSpectrum::mouseReleaseEvent(QMouseEvent *event) {}
void WSpectrum::mouseMoveEvent(QMouseEvent *event) {}
void WSpectrum::wheelEvent(QWheelEvent *event) {}

