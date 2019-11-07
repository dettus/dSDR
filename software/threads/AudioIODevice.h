#ifndef	AUDIOIODEVICE_H
#define	AUDIOIODEVICE_H
#include <QObject>
#include <QIODevice>
#include <QByteArray>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QMutex>

class AudioIODevice: public QIODevice
{
	Q_OBJECT
	
	public:
		AudioIODevice(QObject *parent=nullptr);
		void newSamples(signed short* pcm,int numSamples);

		void start();
		void stop();
		
	// interface to the audio device
		qint64 readData(char* data,qint64 maxlen);
		qint64 writeData(const char *data,qint64 len);
		qint64 bytesAvailable() const;
		bool isSequential() const {return true;};

	private:
		QAudioOutput		*mOutput=nullptr;
		QByteArray		mBuffer;
		bool			mValid=false;
		QAudioFormat		mFormat;
		QAudioDeviceInfo	mDeviceInfo;
		double			mVolume=1.0f;
		QMutex			mMutex;

	private slots:
		void notified();
		void stateChanged(QAudio::State state);	

};
#endif
