#include <QApplication>
#include "TRtlTcp.h"
#include "TDummy.h"
#include "Central.h"
#include "MainWindow.h"
#include "TunerDialog.h"


int main(int argc,char* argv[])
{
	QApplication app(argc,argv);
	MainWindow mainwin(nullptr);
	
	Tuners *tuner;
	TunerDialog *tunerdialog=new TunerDialog();
	TRtlTcp *tRtlTcp=new TRtlTcp();
//	TDummy *tdummy=new TDummy();
//	tRtlTcp->openConnection("127.0.0.1",1234);
	tuner=(Tuners*)tRtlTcp;
	Central *central=new Central(&mainwin,tuner);


	mainwin.showMaximized();
	central->setTuner(tuner);
	central->start();

	tuner->setSink(central);
	tRtlTcp->start();
//	tdummy->start();
	return app.exec();
}
