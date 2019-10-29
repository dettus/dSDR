#ifndef	CENTRALMAIN_H
#define	CENTRALMAIN_H
#include <QThread>
#include <QMutex>
#include "TunerMain.h"
#include <QHBoxLayout>
class CentralMain: public QThread
{
	Q_OBJECT

	public:
		CentralMain(TunerMain *tunerMain);
		void stop();
		
	protected:
		void run();
	
	private:
		bool mStopped=false;
		QMutex mMutex;

		TunerMain *mTunerMain;
		QHBoxLayout *mLayout;
		QWidget *mainWin=nullptr;
};
#endif
