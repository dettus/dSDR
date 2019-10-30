// The purpose of this Widget is to draw the nice spectrum and waterfall
// diagram people are used to from other SDR software packages.
//
// $Id$
//
#ifndef	WSPECTRUM_H
#define	WSPECTRUM_H
#include <QtWidgets>
#include "SimpleFft.h"
#include "DataTypes.h"
#include <QImage>

#define WATERFALLWIDTH  32768
#define WATERFALLHEIGHT 256
#define	WATERFALLNUANCES	256

class WSpectrum: public QWidget
{
	Q_OBJECT
	
	public:
		WSpectrum(QWidget* parent=nullptr);
		void setFFTsize(int fftsize);
		void onNewSamples(tIQSamplesBlock *pIqSamples);
		QSize sizeHint() const;

	protected:
		void paintEvent(QPaintEvent *event) override;
		void resizeEvent(QResizeEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;
		void mouseReleaseEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
		void wheelEvent(QWheelEvent *event) override;

	private:
		void drawSpectrum(QPainter *painter,int yupper,int ylower,double min,double max);
		void drawWaterfall(QPainter *painter,int yupper,int ylower,double min,double max);
		int mWidth=0;
		int mHeight=0;
		int mFftSize=1024;
		int mSampleBufLevel=0;
		tSComplex *mSampleBuf;
		double *mSpectrum;
		double *mSpectrumPlot;
		SimpleFft *mFft=nullptr;
		int mFftAvgLen=10;
		int mFftCallcnt=mFftAvgLen;

		int mSampleRate=2048000;
		int mCenterFreq=0;
		int mGain=0;

		int mLeft=0;
		int mRight=mFftSize;
		int mUpper=400000000;
		int mLower=0;

		QImage *mWaterfallImage=nullptr;
		QColor mRgbPalette[WATERFALLNUANCES];
};
#endif
