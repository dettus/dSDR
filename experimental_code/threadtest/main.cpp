#include <QApplication>
#include "ThreadTest.h"

int main(int argc, char *argv[])
{
	QApplication app(argc,argv);
	ThreadTest *threadtest1=new ThreadTest(7);
	ThreadTest *threadtest2=new ThreadTest(13);

	threadtest1->start();
	threadtest2->start();

	return app.exec();
}
