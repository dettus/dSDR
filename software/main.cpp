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
	AudioMain	*audioMain=new AudioMain();
	DemodMain	*demodMain=new DemodMain(demodWidget,audioMain);
	CentralMain	*centralMain=new CentralMain(tunerMain,demodMain);
#if 0
	{
		QAudioFormat            mFormat;
		QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
		AudioIODevice	*audioIODevice=new AudioIODevice(nullptr);
		
		mFormat.setSampleSize(16);
		mFormat.setChannelCount(2);
		mFormat.setSampleRate(48000);
		mFormat.setCodec("audio/pcm");
		mFormat.setByteOrder(QAudioFormat::LittleEndian);
		mFormat.setSampleType(QAudioFormat::SignedInt);


		if (info.isFormatSupported(mFormat)) printf("\x1b[1;34m format supported \x1b[0m\n");
		QAudioOutput *test=new QAudioOutput(mFormat);
		audioIODevice->start();
		test->start(audioIODevice);		
	}

#endif

	tunerMain->start();
	demodMain->start();
	audioMain->start();
	centralMain->start();

	return app.exec();
	
}
