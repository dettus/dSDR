#ifndef	TUNERMAIN_H
#define	TUNERMAIN_H
#include <QThread>
#include <QMutex>
class TunerMain: public QThread
{
	Q_OBJECT

	public:
		TunerMain();
		void stop();
		
	protected:
		void run();
	
	private:
		bool mStopped=false;
		QMutex mMutex;
};
#endif
