#include <QApplication>
#include "TRtlTcp.h"
#include "TDummy.h"
#include "Central.h"
#include "MainWindow.h"


int main(int argc,char* argv[])
{
	QApplication app(argc,argv);
	QThread *tunerThread=new QThread;
	Tuners *tuner;
	MainWindow mainwin(nullptr);
	TDummy *tunerDummy=new TDummy();
	TRtlTcp *tunerRtl=new TRtlTcp();
	Central *central;

	tunerDummy->start();
	tuner=tunerDummy;
	
	central=new Central(&mainwin,tuner);

	central->start();

	return app.exec();
}
