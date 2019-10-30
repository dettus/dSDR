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
		void drawSpectrum(QPainter *painter,int yupper,int ylower);
		void drawWaterfall(QPainter *painter,int yupper,int ylower);
		int mWidth=0;
		int mHeight=0;
		int mFftSize=1024;
		int mSampleBufLevel=0;
		tSComplex *mSampleBuf;
		double *mSpectrum;
		SimpleFft *mFft=nullptr;
		int mFftAvgLen=100;
		int mFftCallcnt=mFftAvgLen;

		int mSampleRate=2048000;
		int mCenterFreq=0;
		int mGain=0;
};
#endif
