#ifndef	AUDIOMAIN_H
#define	AUDIOMAIN_H
#include <QThread>
#include <QMutex>
class AudioMain: public QThread
{
	Q_OBJECT

	public:
		AudioMain();
		void stop();
		
	protected:
		void run();
	
	private:
		bool mStopped=false;
		QMutex mMutex;
};
#endif
