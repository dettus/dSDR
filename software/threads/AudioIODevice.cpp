#include "AudioIODevice.h"	

AudioIODevice::AudioIODevice()
{
	mPcmBufData=new char[mPcmBufSize];
}
void AudioIODevice::onNewPcmSamples(signed short* pcmBuf,int num)
{
	char *ptr;
	int i;
	int l;
	ptr=(char*)pcmBuf;
	mMutex.lock();
	
	l=mPcmBufLevel%mPcmBufSize;
	for (i=0;i<num*sizeof(short)/sizeof(char);i++)
	{
		mPcmBufData[l]=ptr[i];
		l=(l+1)%mPcmBufSize;
	}
	mPcmBufLevel+=num*sizeof(short)/sizeof(char);
	printf("new bytes:%d\n",num*sizeof(short)/sizeof(char));
	mMutex.unlock();
	
}

qint64 AudioIODevice::readData(char *data, qint64 maxlen)
{
	int n;
	int i;
	int j;
	mMutex.lock();
	n=mPcmBufLevel-mPcmBufUsed;
	if (n>maxlen) n=maxlen;
	j=mPcmBufUsed%mPcmBufSize;
	for (i=0;i<n;i++)
	{
		data[i]=mPcmBufData[j];
		j=(j+1)%mPcmBufSize;
	}
	mPcmBufUsed+=n;
	if (mPcmBufUsed>mPcmBufSize && mPcmBufLevel>mPcmBufSize) 
	{
		mPcmBufUsed %=mPcmBufSize;
		mPcmBufLevel%=mPcmBufSize;
	}
	printf("read %d bytes\n",n);
	mMutex.unlock();	
	return n;
}
qint64 AudioIODevice::writeData(const char *data, qint64 len)
{
	Q_UNUSED(data);
	Q_UNUSED(len);
	return 0;
}
qint64 AudioIODevice::bytesAvailable() const
{
	int bytes;
	printf("bytes available %d\n",mPcmBufUsed-mPcmBufLevel);
	bytes=0;
	return bytes;
}
