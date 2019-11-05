#include <QApplication>
#include "TunerMain.h"
#include "CentralMain.h"
#include "DemodMain.h"
#include "AudioMain.h"

#include "DemodWidget.h"
int main(int argc,char** argv)
{
	QApplication app(argc,argv);
	TunerMain	*tunerMain=new TunerMain();
	CentralMain	*centralMain=new CentralMain(tunerMain);
	DemodMain	*demodMain=new DemodMain();
	AudioMain	*audioMain=new AudioMain();

	DemodWidget *demodWidget=new DemodWidget();




	tunerMain->start();
	centralMain->start();
	centralMain->setDemodWidget(demodWidget);
	demodMain->start();
	audioMain->start();

	return app.exec();
	
}
