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
void AudioIODevice::newSamples(signed short* pcm,int numSamples)
{
	mMutex.lock();
	mBuffer.append((char*)pcm,numSamples*sizeof(short));
	mMutex.unlock();
}
qint64 AudioIODevice::readData(char* data,qint64 maxlen)
{
	int outlen=0;
	int i;
	signed short *ptri,*ptro;	// PCM samples are 16 bit signed integer

	
	if (maxlen<2) 
		return 0;
	mMutex.lock();
	outlen=mBuffer.length();
	if (outlen>maxlen) outlen=maxlen;

	// apply the volume here
	ptri=(signed short*)mBuffer.data();
	ptro=(signed short*)data;
	for (i=0;i<outlen/sizeof(short);i++)
	{
		double o;
		o=ptri[i];
		o*=mVolume;
		if (o>32768) o=32767;
		if (o<-32767) o=-32767;
		
		ptro[i]=(signed short)o;
	}	
	mBuffer.remove(0,outlen);	
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
