#include "AudioIODevice.h"
#define	UNDERRUN_SIZE	256000
#define	OVERRUN_SIZE	2*UNDERRUN_SIZE

AudioIODevice::AudioIODevice(QObject* parent):QObject(parent)
{
	mDeviceInfo=QAudioDeviceInfo::defaultOutputDevice();
}
bool AudioIODevice::setAudioDevice(QAudioDeviceInfo deviceInfo)
{
	bool restartRequired;

	restartRequired=false;
	if (mOutput!=nullptr)
	{
		if (mOutput->state() != QAudio::StoppedState)
		{
			restartRequired=true;
		}
		delete(mOutput);
	}
	stop();
	mOutput=new QAudioOutput(mDeviceInfo,mFormat,this);

	connect(mOutput,SIGNAL(notify()),	SLOT(notified()));
	connect(mOutput,SIGNAL(stateChanged(QAudio::State)),	SLOT(stateChanged(QAudio::State)));
	if (restartRequired)
	{
		start();
	}

	mDeviceInfo=deviceInfo;
	return true;
}
bool AudioIODevice::setAudioConfig(int bits,int channels,int sampleRate)
{
	if (bits!=16) return false;
	mFormat.setSampleSize(bits);
	mFormat.setChannelCount(channels);
	mFormat.setSampleRate(sampleRate);
	mFormat.setCodec("audio/pcm");
	mFormat.setByteOrder(QAudioFormat::LittleEndian);
	mFormat.setSampleType(QAudioFormat::SignedInt);
	if (mDeviceInfo.isFormatSupported(mFormat))
	{
		return setAudioDevice(mDeviceInfo);
	}
	return false;
}
void AudioIODevice::start()
{
	if (mOutput!=nullptr)
	{
		if (mOutput->state()!=QAudio::StoppedState)
			return;
	} else return;
	this->open(QIODevice::ReadOnly);
	mBuffer.clear();
	mOutput->start(this);
	mOutput->suspend();
}
void AudioIODevice::stop()
{
	if (mOutput!=nullptr)
	{
		if (mOutput->state()==QAudio::StoppedState)
			return;
	} else return;
	mOutput->stop();
	mBuffer.clear();
	this->close();
}
void AudioIODevice::setVolume(double volume)
{
	mVolume=volume;
}
void AudioIODevice::newSamples(short* pcmSamples,int sampleNum)
{
	if (mOutput!=nullptr)
	{
		int newBytes;
		int delta;
		newBytes=sampleNum*sizeof(short);
	
		if (mOutput->state()==QAudio::StoppedState)
			return;		
		mMutex.lock();
		delta=2*OVERRUN_SIZE-mBuffer.length()-newBytes;
		if (delta<0)	// just truncate samples that might overflow the buffer
		{
			newBytes+=delta;
		}
		if (newBytes>=0) mBuffer.append((char*)pcmSamples,newBytes);
		mMutex.unlock();
		if (mOutput->state()==QAudio::SuspendedState)
		{
			if (mBuffer.length>=UNDERRUN_SIZE)
				mOutput->resume();
		}
	}
}

qint64 AudioIODevice::readData(char* data,qint64 maxlen)
{
	mMutex.lock();
	signed short *ptr;
	int i;
	int outlen;

	if (maxlen<2) return 0;

	if ((maxlen%2)!=0) maxlen-=1;
	outlen=mBuffer.length();
	if (maxlen<outlen) outlen=maxlen;
	
	memcpy(data,mBuffer.data(),outlen);
	ptr=data;
	for (i=0;i<outlen/2;i++) ptr[i]=(signed short)((double)ptr[i]*mVolume);	
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
	mMutex.lock();
	retval=mBuffer.length()+QIODevice::bytesAvailable();
	mMutex.unlock();
	return retval;
}
void AudioIODevice::notified() {}
void AudioIODevice::stateChanged(QAudio::State state)
{
	// apparently, those checks are required if somebody plans on running the software on windows or Mac OSX.
	if (state==QAudio::IdleState && mOutput->error()==QAudio::UnderrunError)
	{
		if (mBuffer.length()<UNDERRUN_SIZE)
		{
			mOutput->suspend();
		}
	}
}
