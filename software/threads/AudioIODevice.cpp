#include "AudioIODevice.h"
AudioIODevice::AudioIODevice(QObject* parent):QIODevice(parent)
{
}
void AudioIODevice::start()
{
	open(QIODevice::ReadOnly);	
}
void AudioIODevice::stop()
{
	close();
}
qint64 AudioIODevice::readData(char* data,qint64 maxlen)
{
	int outlen=0;
	int i;
	signed short *ptr;

	ptr=(signed short*)data;
	mMutex.lock();
//	outlen=maxlen;
	printf("maxlen:%d\n",maxlen);
	for (i=0;i<outlen/sizeof(short);i++)
	{
		ptr[i]=i;
	}
	mMutex.unlock();


	return outlen;
}
qint64 AudioIODevice::writeData(const char *data,qint64 len)
{
	Q_UNUSED(data);	
	Q_UNUSED(len);
	return 0;
}
qint64 AudioIODevice::bytesAvailable() const
{
	qint64 retval;
	retval=mBuffer.length()+QIODevice::bytesAvailable();
	return retval;
}
void AudioIODevice::notified() {}
void AudioIODevice::stateChanged(QAudio::State state)
{
}
