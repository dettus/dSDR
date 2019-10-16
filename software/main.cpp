#include <QApplication>
#include "TDummy.h"
#include "MainWindow.h"

int main(int argc,char* argv[])
{
	QApplication app(argc,argv);
	MainWindow mainwin(nullptr);
	
	TDummy *tdummy=new TDummy();

	mainwin.show();
	return app.exec();
}
