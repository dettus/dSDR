#ifndef	DEMODMAIN_H
#define	DEMODMAIN_H
#include <QThread>
#include <QMutex>
class DemodMain: public QThread
{
	Q_OBJECT

	public:
		DemodMain();
		void stop();
		
	protected:
		void run();
	
	private:
		bool mStopped=false;
		QMutex mMutex;
};
#endif
