// The purpose of this class is to make QT happy

#ifndef	AUDIOIODEVICE_H
#define	AUDIOIODEVICE_H
#include <QIODevice>
#include <QMutex>
class AudioIODevice: public QIODevice
{
	Q_OBJECT

	public:
		AudioIODevice();
		void onNewPcmSamples(signed short* pcmBuf,int num);
		qint64 readData(char *data, qint64 maxlen) override;
		qint64 writeData(const char *data, qint64 len) override;
		qint64 bytesAvailable() const override;

	private:
		char *mPcmBufData;
		int mPcmBufLevel=0;
		int mPcmBufUsed=0;
		int mPcmBufSize=(1<<20);
		QMutex mMutex;
		
};
#endif
