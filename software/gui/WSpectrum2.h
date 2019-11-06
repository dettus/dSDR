#ifndef	WSPECTRUM_H
#define	WSPECTRUM_H
#include <QtWidgets>
#include "SimpleFft.h"
#include "DataTypes.h"
#include <QImage>

// the purpose of this class is to draw the nice spectrum
// and waterfall diagram people are used to from other SDR
// software packages.
#define WATERFALLWIDTH  32768
#define WATERFALLHEIGHT 512
#define	WATERFALLNUANCES	384
class WSpectrum: public QWidget
{
	Q_OBJECT

	public:
		WSpectrum(QWidget* parent=nullptr);
		void setFFTsize(int fftsize);
		void onNewSamples(tIQSamplesBlock *pIqSamples);
		QSize sizeHint() const;
		int getLastFreq() {return mLastFreq;};

	protected:
		void paintEvent(QPaintEvent *event) override;
		void resizeEvent(QResizeEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;
		void mouseReleaseEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
		void wheelEvent(QWheelEvent *event) override;

	private:
		tSComplex *mSampleBuf=nullptr;
		double	*mSpectrum=nullptr;
		double	*mSpectrumPlot=nullptr;
		int mSampleBufLevel=0;
		void updateWaterfall();
		int mFftSize=32768;
		QImage *mSpectrumImage=nullptr;
		QImage *mWaterfallImage1=nullptr;
		QImage *mWaterfallImage2=nullptr;
		int mWaterfallLine=0;
		SimpleFft* mFft=nullptr;
		int mFftAvg=20;			// average the spectrum out over 10 fft calls
		int mFftCnt=mFftAvg;
		QColor mPalette[WATERFALLNUANCES];
		int mLeft=0;
		int mRight=mFftSize;

		int mSampleRate=0;
		int mCenterFreq=0;
		int mGain=0;
		int mLastFreq=0;
	
};

#endif
