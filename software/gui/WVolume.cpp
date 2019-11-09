#include "WVolume.h"
#include <QLinearGradient>

WVolume::WVolume(QWidget* parent): QWidget(parent)
{
	this->setMouseTracking(false);
}
void WVolume::paintEvent(QPaintEvent *event)
{
	int width;
	int height;
	int y;
	int x;
	int ax,ay,bx,by;
	double mx,my;
	int i;

	// a speaker symbol consists basically of a rectangle, and a trapeze. those are the X,Y coordinates. 
	int speaker_symbol[8][2]={
		{1,1},{0,1},{0,2},{1,2},{1,1},	// rectangle. note that it starts and ends where the trapeze will begin
		{2,0},{2,3},{1,2}		// trapeze
	};

	width=this->width();
	height=this->height();
	QPainter painter(this);

	QLinearGradient linearGradient(0,0,width/2,height);
	linearGradient.setColorAt(0.8,QColor(0,255,0,255));
	linearGradient.setColorAt(0.5,QColor(128,255,0,255));
	linearGradient.setColorAt(0.0,QColor(255,0,0,255));

	y=height-mVolume*height/255;
	painter.fillRect(0,y,width/3,height,linearGradient);		
	painter.setPen(QColor(0,0,0,255));	// black

	mx=(width/2)/3;
	my=height/3;	// multiplier for the y-coordinates of the speaker symbol


	mx=4;
	my=5;
	ax=speaker_symbol[0][0]*mx;
	ay=speaker_symbol[0][1]*my;
	x=width/2;	
	for (i=1;i<8;i++)
	{
		bx=speaker_symbol[i][0]*mx;
		by=speaker_symbol[i][1]*my;

		painter.drawLine(x+ax,ay+y,x+bx,by+y);
		ax=bx;
		ay=by;	
	}	
}

void WVolume::mousePressEvent(QMouseEvent *event) 
{
	mouseMoveEvent(event);
}
void WVolume::mouseReleaseEvent(QMouseEvent *event) 
{
	mouseMoveEvent(event);
}
void WVolume::mouseMoveEvent(QMouseEvent *event) 
{
	int height;
	int newvolume;
	QPoint curPoint = event->pos();
	int y;
	
	height=this->height();
	y=curPoint.y();
	newvolume=255-y*255/height;
	if (newvolume<0) newvolume=0;
	if (newvolume>255) newvolume=255;

	mVolume=newvolume;
	update();

}
void WVolume::wheelEvent(QWheelEvent *event) 
{
	int newvolume=mVolume;
	if (event->delta()>0) // mousewheel up
	{
		newvolume+=16;
	} else {
		newvolume-=16;
	}
	if (newvolume>=255) newvolume=255;
	if (newvolume<=0) newvolume=0;
	
	mVolume=newvolume;
	update();
}
QSize WVolume::sizeHint() const
{
	return QSize(20, 110);
}

