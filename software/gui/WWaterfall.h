#ifndef	WWaterfall_H
#define	WWaterfall_H

#include <QtWidgets>
#include "Zoomback.h"

class WWaterfall : public QWidget,public Zoomback
{
	Q_OBJECT

public:
	WWaterfall(QWidget *parent = nullptr);
	void setFFTsize(int size);
	void plotWaterfall(double* spectrum,int n);
	void setZoom(int left,int right,double upper,double lower);
	void setZoom(int left,int right);
	void zoomFit();
	void setZoomback(Zoomback* zoomback);

	
protected:
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

private:
	int mWidth;
	int mHeight;
	int mFftSize;
	QImage* mWaterfallImage=nullptr;
	double mMin,mMax;

	int mUpper,mLower;
	int mLeft,mRight;
	int mLastMoveEventX=-1;
	Zoomback* mZoomback=nullptr;
};

#endif
