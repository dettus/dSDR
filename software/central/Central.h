#ifndef	CENTRAL_H
#define	CENTRAL_H
#include <QThread>
#include <QMutex>
#include "Datatypes.h"
#include "Sink.h"

class Central: public QThread,public Sink
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
		QMutex mMutex;
	
};
#endif
