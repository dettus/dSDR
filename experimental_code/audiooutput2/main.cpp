#include <QApplication>
#include "MyAudioOutput.h"

int main(int argc,char** argv)
{
	int i;
	signed short s;
	signed short pcm[48000];
	QApplication app(argc,argv);
	MyAudioOutput audioOutput;
	
	audioOutput.init(16,1,48000);
	audioOutput.setDevice(QAudioDeviceInfo::defaultOutputDevice());	
	for (i=0;i<48000;i++) pcm[i]=(i*32)%1024;
	QByteArray ba((char*)pcm,sizeof(pcm));

	audioOutput.output(ba);
	audioOutput.start();	
	audioOutput.output(ba);
	
	return app.exec();	
}
