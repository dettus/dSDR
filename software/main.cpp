#include <QApplication>
#include "TunerMain.h"
#include "CentralMain.h"
#include "DemodMain.h"
#include "AudioMain.h"

#include "DemodWidget.h"
int main(int argc,char** argv)
{
	QApplication app(argc,argv);
	DemodWidget *demodWidget=new DemodWidget();
	TunerMain	*tunerMain=new TunerMain();
	DemodMain	*demodMain=new DemodMain(demodWidget);
	CentralMain	*centralMain=new CentralMain(tunerMain,demodMain);
	AudioMain	*audioMain=new AudioMain();





	tunerMain->start();
	demodMain->start();
	audioMain->start();
	centralMain->start();

	return app.exec();
	
}
