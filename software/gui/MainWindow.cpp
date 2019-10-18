#include "MainWindow.h"
#include <QPalette>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>



MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
	
	
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
	double width;
	double height;

	width=(double)this->width();
	height=(double)this->height();
	if (width>300 && height>300 && mWWaterfall!=nullptr && mWSpectrum!=nullptr && mWTuner!=nullptr)
	{

		mWTuner->move((int)(0.00*width),(int)(0.00*height));
		mWTuner->resize((int)(0.20*width),(int)(1.00*height));
		mWSpectrum->move((int)(0.20*width),(int)(0.00*height));
		mWSpectrum->resize((int)(0.80*width),(int)(0.20*height));
		mWWaterfall->move((int)(0.20*width),(int)(0.20*height));
		mWWaterfall->resize((int)(0.80*width),(int)(0.80*height));
	}

}

void MainWindow::paintEvent(QPaintEvent *event)
{
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
void MainWindow::setWSpectrum(QWidget* wSpectrum)
{
	mWSpectrum=wSpectrum;
	if (mWSpectrum!=nullptr)
	{
		mWSpectrum->show();	
	}
}
void MainWindow::setWWaterfall(QWidget* wWaterfall)
{
	mWWaterfall=wWaterfall;
	if (mWWaterfall!=nullptr)
	{
		mWWaterfall->show();	
	}
}
