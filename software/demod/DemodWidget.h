#ifndef	DEMODWIDGET_H
#define	DEMODWIDGET_H

#include <QtWidgets>
#include <QRadioButton>
#include <QVBoxLayout>

class DemodWidget: public QWidget
{
	Q_OBJECT

	public:
		DemodWidget(QWidget *parent=nullptr);
		QSize sizeHint();
	
	public slots:
		void handleToggled();

	private:
		QVBoxLayout *mVLayout=nullptr;	
		QHBoxLayout *mHLayout=nullptr;
		QStackedLayout *mStackedLayout=nullptr;
		#define	BUTTON_NUM	6
		QRadioButton *demod_buttons[BUTTON_NUM]={nullptr};
		QWidget	*demod_labels[BUTTON_NUM];

		int mDemodMode=0;
	
};
#endif
