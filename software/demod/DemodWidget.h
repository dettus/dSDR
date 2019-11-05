#ifndef	DEMODWIDGET_H
#define	DEMODWIDGET_H

#include <QtWidgets>

class DemodWidget: public QWidget
{
	Q_OBJECT

	public:
		DemodWidget(QWidget *parent=nullptr);
		QSize sizeHint();
	
};
#endif
