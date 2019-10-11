#include "WFrequncy.h"

WFrequency::WFrequency()
{
	curFreq=0;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
	
}
void MainWindow::paintEvent(QPaintEvent *event)
{
	QPainter painter1(this);
	int i;
	int x;
	unsigned long long freq;
	QFont font=painter1.font();

	curWidth=painter1.width();
	curHeight=painter1.height();
	
	freq=curFreq;


	painter1.setPen(QColor(255,0,0,0));
	painter1.drawRect(0,0,curWidth,curHeight);
	painter1.setPen(QColor(255,0,0,255));
	painter1.setFont(font);
	

	for (i=0;i<10;i++)
	{
		char tmp[8];
		snprintf(tmp,8,"%d",freq%10);
		painter.drawText(QRectF(x,0,curWidth/10,curHeight),Qt::AlignCenter,tmp);
		freq/=10;
	}
}



