#ifndef	CENTRALMAIN_H
#define	CENTRALMAIN_H
#include <QThread>
#include <QMutex>
class CentralMain: public QThread
{
	Q_OBJECT

	public:
		CentralMain();
		void stop();
		
	protected:
		void run();
	
	private:
		bool mStopped=false;
		QMutex mMutex;
};
#endif
