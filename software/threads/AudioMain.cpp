#include "AudioMain.h"
AudioMain::AudioMain()
{
	mBuffer.clear();
	mAudioIODevice=new AudioIODevice(nullptr);
	mAudioIODevice->start();
	mFormat.setSampleSize(16);
	mFormat.setChannelCount(2);
	mFormat.setSampleRate(48000);
	mFormat.setCodec("audio/pcm");
	mFormat.setByteOrder(QAudioFormat::LittleEndian);
	mFormat.setSampleType(QAudioFormat::SignedInt);

	mAudioOutput=new QAudioOutput(mFormat);
	mAudioOutput->start(mAudioIODevice);		
	{
		QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
		

		if (info.isFormatSupported(mFormat)) printf("\x1b[1;34m format supported \x1b[0m\n");
	}
	mPcmBuf=new tSComplex[65536];
}
void AudioMain::stop()
{
	mStopped=true;
}
void AudioMain::run()
{
	QAudioFormat format;
	format=mFormat;
	while (!mStopped)
	{
		QThread::msleep(100);
		mMutex.lock();

		if (mFormat.sampleRate()!=48000)
		{
			tIQSamplesBlock input;
			tIQSamplesBlock output;
			if (format!=mFormat || (mDownSampler==nullptr && mUpSampler==nullptr))
			{	

				//			mAudioIODevice->setAudioConfig(16,mFormat.channelCount(),mFormat.sampleRate());
				//			mAudioIODevice->start();
				if (mDownSampler!=nullptr) delete(mDownSampler);
				if (mUpSampler!=nullptr) delete(mUpSampler);

				mDownSampler=nullptr;
				mUpSampler=nullptr;

				if (mFormat.sampleRate()>48000)
				{
					mDownSampler=new Downsampler(mFormat.sampleRate(),48000,24000);
				}  else {
					mUpSampler=new Upsampler(mFormat.sampleRate(),48000);
				}
			}
			input.pData=(tSComplex*)mBuffer.data();
			input.sampleNum=mBuffer.length()/sizeof(tSComplex);
			input.sampleRate=mFormat.sampleRate();
			input.centerFreq=0;
			input.gain=0;
			output.pData=mPcmBuf;
			if (mUpSampler!=nullptr) 
			{
				mUpSampler->process(&input,&output);	
				input.pData=mPcmBuf;
				input.sampleNum=output.sampleNum;
			}
			if (mDownSampler!=nullptr)
			{
				mDownSampler->process(&input,&output);	
			}
			mAudioIODevice->newSamples((signed short*)output.pData,output.sampleNum*2);
		}
		else 
		{	
			mAudioIODevice->newSamples((signed short*)mBuffer.data(),mBuffer.length()/sizeof(short));
		}
//		printf("AUDIO len:%d\n",mBuffer.length());
		mBuffer.clear();
		mMutex.unlock();
		format=mFormat;
	}
}
void AudioMain::setAudioFormat(QAudioFormat format)
{
	mMutex.lock();
	if (format!=mFormat && format.sampleRate())
	{
		mFormat=format;
	}
	mMutex.unlock();
		
}
void AudioMain::onNewPcmSamples(signed short* pcmBuf,int num)
{
	mMutex.lock();
	mBuffer.append((char*)pcmBuf,num*sizeof(short));
	mMutex.unlock();
}

