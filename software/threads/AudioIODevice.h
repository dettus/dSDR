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
		explicit AudioIODevice(QObject *parent=nullptr);
		bool setAudioConfig(int bits,int channels,int sampleRate);
		bool setAudioDevice(QAudioDeviceInfo deviceInfo);
		void start();
		void stop();
		void setVolume(double volume);
		void newSamples(short* pcmSamples,int samples);
		
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
