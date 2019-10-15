#ifndef	THREADTEST_H
#define	THREADTEST_H
#include <QThread>
class ThreadTest : public QThread
{
	Q_OBJECT

	public:
		ThreadTest(int id);
		void stop();

	protected:
		void run();
	
	private:
		int mId;
		volatile bool mStopped;
	
};
#endif
