#include "WFrequency.h"

WFrequency::WFrequency()
{
	curFreq=0;
	drawPlus=-1;
	drawMinus=-1;
	setMouseTracking(true);
}
void WFrequency::closeEvent(QCloseEvent *event)
{

}
void WFrequency::resizeEvent(QResizeEvent *event)
{

}
void WFrequency::paintEvent(QPaintEvent *event)
{
	QPainter painter1(this);
	int i;
	int x;
	unsigned long long freq;
	QFont font=painter1.font();

	curWidth=this->width();
	curHeight=this->height();
	font.setPointSize(curWidth/10);

	freq=curFreq;


	//	painter1.setPen(QColor(255,0,0,0));
	//	painter1.drawRect(0,0,curWidth,curHeight);
	painter1.fillRect(0,0,curWidth,curHeight,QColor(0,0,0,255));
	if (drawPlus!=-1)
	{
		painter1.fillRect(drawPlus*curWidth/10,0,curWidth/10,curHeight/2,QColor(0,64,32,255));
	}
	if (drawMinus!=-1)
	{
		painter1.fillRect(drawMinus*curWidth/10,curHeight/2,curWidth/10,curHeight/2,QColor(64,0,32,255));
	}

	painter1.setPen(QColor(128,128,255,255));
	painter1.setFont(font);

	x=curWidth;
	for (i=0;i<10;i++)
	{
		char tmp[8];
		x-=curWidth/10;
		snprintf(tmp,8,"%d",freq%10);
		painter1.drawText(QRectF(x,0,curWidth/10,curHeight),Qt::AlignCenter,tmp);
		freq/=10;
	}

}

void WFrequency::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		int x,y;
		QPoint clickPoint = event->pos();
		x=clickPoint.x();
		y=clickPoint.y();

		if (y>curHeight/2) calculateFreq(x,-1);
		else calculateFreq(x,1);

		this->repaint();
	}
}
void WFrequency::wheelEvent(QWheelEvent *event)
{
	QPoint curPoint = event->pos();

	int x=curPoint.x();

	if (event->delta()>0) calculateFreq(x,1);
	else calculateFreq(x,-1);
	this->repaint();
}
void WFrequency::calculateFreq(int x,int up1down_1)
{
	int i;
	signed long long modifier;
	//		printf("clicked at %d %d  width:%d height:%d\n",x,y,curWidth,curHeight);
	x*=10;
	x/=curWidth;
	modifier=up1down_1;
	for (i=9;i>x;i--)
	{
		modifier*=10;
	}
	printf("x:%d modifier:%lld\n",x,modifier);
	curFreq+=modifier;
	if (curFreq<0) curFreq=0;
	if (curFreq>9999999999ull) curFreq=9999999999ull;
	printf("curFreq:%lld\n",curFreq);

}

void WFrequency::mouseMoveEvent(QMouseEvent *event) 
{
	QPoint curPoint = event->pos();
	int x=curPoint.x();
	int y=curPoint.y();
	int newplus,newminus;


	newplus=drawPlus;
	newminus=drawMinus;	
	if (x>=0 && x<curWidth && y>=0 && y<curHeight)
	{
		if (y>curHeight/2) 
		{
			drawMinus=(x*10)/curWidth;
			drawPlus=-1;
		} else {
			drawPlus=(x*10)/curWidth;
			drawMinus=-1;
		}
	} else {
		drawPlus=-1;
		drawMinus=-1;
	}
	if (newplus!=drawPlus || newminus!=drawMinus)
	{	
		this->repaint();
	}

}
void WFrequency::mouseReleaseEvent(QMouseEvent *event)
{


}




