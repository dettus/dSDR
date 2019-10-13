#ifndef	WWATERFALL_H
#define	WWATERFALL_H

#include <QtWidgets>
#include <QImage>

class WWaterfall : public QWidget
{
	Q_OBJECT

public:
	WWaterfall(QWidget *parent = nullptr);
	void setFFTsize(int size);
	void plotWaterfall(double* spectrum,int n);

	
protected:
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

private:
	int mWidth;
	int mHeight;
	int mFftSize;
	QImage* mWaterfallImage=nullptr;

	int mUpper,mLower;
	int mLeft,mRight;

	double mSpectrum[65536];
	double mMin,mMax;
	
};

#endif
