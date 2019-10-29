#include <QApplication>
#include "TunerMain.h"
#include "CentralMain.h"
#include "DemodMain.h"
#include "AudioMain.h"

int main(int argc,char** argv)
{
	QApplication app(argc,argv);
	TunerMain	*tunerMain=new TunerMain();
	CentralMain	*centralMain=new CentralMain();
	DemodMain	*demodMain=new DemodMain();
	AudioMain	*audioMain=new AudioMain();




	tunerMain->start();
	centralMain->start();
	demodMain->start();
	audioMain->start();

	return app.exec();
	
}
