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
	Central *central=new Central(&mainwin,nullptr);


	central->start();

	return app.exec();
}
