#include <QApplication>
#include "TDummy.h"
#include "Central.h"
#include "MainWindow.h"

int main(int argc,char* argv[])
{
	QApplication app(argc,argv);
	MainWindow mainwin(nullptr);
	TDummy *tdummy=new TDummy();
	Central *central=new Central(&mainwin,tdummy);


	mainwin.showMaximized();
	central->setTuner(tdummy);
	central->start();

	tdummy->setSink(central);
	tdummy->start();
	return app.exec();
}
