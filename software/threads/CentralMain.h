#ifndef	CENTRALMAIN_H
#define	CENTRALMAIN_H
#include <QThread>
#include <QMutex>
#include "StartupDialog.h"
class CentralMain: public QThread
{
	Q_OBJECT

	public:
		CentralMain();
		void stop();
		
	protected:
		void run();
	
	private:
		void startup();
		bool mStopped=false;
		QMutex mMutex;
		StartupDialog *mStartupDialog;
};
#endif
