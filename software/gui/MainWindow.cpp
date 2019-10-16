#include "MainWindow.h"
#include <QPalette>
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
	double width;
	double height;

	width=this->width();
	height=this->height();

//	painter.fillRect((int)(0.00*width),(int)(0.20*height),(int)(0.20*width),(int)(0.75*height),QColor(0,255,0,255));
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	double width;
	double height;

	width=this->width();
	height=this->height();

	// first element: demod selector (AM/FM/DRM)
	painter.fillRect((int)(0.00*width),(int)(0.00*height),(int)(0.20*width),(int)(0.20*height),QColor(255,0,0,255));
	
	// right of it: the demod widget
	painter.fillRect((int)(0.20*width),(int)(0.00*height),(int)(0.80*width),(int)(0.20*height),QColor(128,128,0,255));

	// left: the tuner widget
//	painter.fillRect((int)(0.00*width),(int)(0.20*height),(int)(0.20*width),(int)(0.75*height),QColor(0,255,0,255));
	
	//below: the "record" button
	painter.fillRect((int)(0.00*width),(int)(0.95*height),(int)(0.20*width),(int)(0.05*height),QColor(128,128,0,255));


	// below the "demod": freq/gain
	painter.fillRect((int)(0.20*width),(int)(0.20*height),(int)(0.80*width),(int)(0.05*height),QColor(0,0,255,255));

	// below the "freq/gain": FFT spectrum
	painter.fillRect((int)(0.20*width),(int)(0.25*height),(int)(0.80*width),(int)(0.20*height),QColor(64,64,255,255));

	// and the big waterfall
	painter.fillRect((int)(0.20*width),(int)(0.45*height),(int)(0.80*width),(int)(0.55*height),QColor(128,0,255,255));

	if (mWTuner!=nullptr)
	{	
		mWTuner->move((int)(0.00*width),(int)(0.20*height));
		mWTuner->resize((int)(0.20*width),(int)(0.75*height));
	}
}

void MainWindow::setWTuner(QWidget* wTuner)
{
	mWTuner=wTuner;
	if (mWTuner!=nullptr) 
	{
		mWTuner->setParent(this);
		mWTuner->show();
	}
	
}
