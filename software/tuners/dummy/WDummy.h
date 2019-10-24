#ifndef	WDUMMY_H
#define	WDUMMY_H
#include <QtWidgets>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "CBDummy.h"

class WDummy: public QWidget
{
	Q_OBJECT

	public:
		WDummy();
		void setCallback(CBDummy* cbDummy);
	
	private slots:
		void handleStartStop();
		void handleOpen();

	private:
		QVBoxLayout *mLayout;
		QPushButton *mStartStop;
		QPushButton *mOpen;
		QLabel *mLabel;
		bool mStarted=false;
		CBDummy *mCbDummy=nullptr;
		
};
#endif
