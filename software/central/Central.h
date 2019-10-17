#ifndef	CENTRAL_H
#define	CENTRAL_H
#include <QThread>
#include "Datatypes.h"

class Central: public Sink,public QThread
{

	Q_OBJECT

	public:
		Central();
		void onNewSamples(tSComplex* iqSamples,int n);
		void stop();
	
	protected:
		void run();

	private:
		tSComplexBuf	iqBuf;
		bool mStopped;
		QMutexLocker locker;
	
};
#endif
