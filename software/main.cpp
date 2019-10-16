#include <QApplication>
#include "TDummy.h"

int main(int argc,char* argv[])
{
	QApplication app(argc,argv);
	TDummy *tdummy=new TDummy();

	return app.exec();
}
