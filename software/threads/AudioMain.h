#ifndef	AUDIOMAIN_H
#define	AUDIOMAIN_H
#include <QThread>
#include <QMutex>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include "AudioIODevice.h"

class AudioMain: public QThread
{
	Q_OBJECT

	public:
		AudioMain();
		void stop();
		void setAudioFormat(QAudioFormat format);
		void onNewPcmSamples(signed short* pcmBuf,int num);



	protected:
		void run();

	private:
		bool mStopped=false;
		QMutex mMutex;
		QAudioFormat mFormat;
		QByteArray	mBuffer;

		//QIODevice	*mIoDevice=nullptr;
		AudioIODevice	*mAudioIODevice=nullptr;

	//	QAudioDeviceInfo mAudioDeviceInfo;
		QAudioOutput *mAudioOutput=nullptr;
};
#endif
