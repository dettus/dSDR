#include <QtWidgets>
#include "mainwindow.h"

MainWindow::MainWindow()
{
	setWindowTitle(tr("hello world"));
	resize(500,500);
	mainimage=new QImage(500,500,QImage::Format_ARGB32);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	int i;
	QRectF target(0, 0, 500, 500);
	QRectF source(0, 0, 500, 500);
	QPainter painter1(this);
	QPainter painter2(mainimage);
	QFont font=painter2.font();
	//const QRect rect = event->rect();

	for (i=0;i<255;i++)
	{
		painter2.setPen(QColor(0,255-i,i,255));
		painter2.drawLine(0,i,100,i);
	}
	painter2.setPen(QColor(255,0,0,255));
	painter2.drawLine(0,0,200,300);
	painter2.setPen(QColor(255,245,0,255));
	painter2.drawLine(0,50,200,350);
	painter2.setPen(QColor(0,0,255,255));
	painter2.drawLine(0,20,200,320);
	painter2.setPen(QColor(0,0,0,255));
	font.setPixelSize(50);
	painter2.setFont(font);	
	painter2.drawText(QRectF(50,50,200,200),Qt::AlignCenter,tr("Hello World"));
	painter1.drawImage(target, *mainimage, source);

}

void MainWindow::resizeEvent(QResizeEvent *event)
{

}
