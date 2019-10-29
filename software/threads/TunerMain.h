#ifndef	TUNERMAIN_H
#define	TUNERMAIN_H
#include <QThread>
#include <QMutex>
#include "TunerDialog.h"
#include "Tuners.h"
class TunerMain: public QThread
{
	Q_OBJECT

	public:
		TunerMain();
		void stop();
		Tuners* getTuner();
		
	protected:
		void run();
	
	private:
		void startup();
		bool mStopped=false;
		QMutex mMutex;
		Tuners* mTuner=nullptr;
		TunerDialog *mTunerDialog=nullptr;
};
#endif
