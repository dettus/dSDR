#ifndef	DEMOD_H
#define	DEMOD_H
#include <QtWidgets>
#include "DataTypes.h"

class Demod: public QWidget
{
	Q_OBJECT

	public:
		Demod(QWidget* parent):QWidget(parent) {};
		virtual void onNewSamples(tIQSamplesBlock *pSamples,signed short* pcmBuf,int pcmBufSize,int* pcmNum,int* sampleRate)=0;
		virtual int getBandwidth()=0;
		virtual int getSampleRate()=0;
		virtual int getFreqOffset()=0;
		virtual int getRaster()=0;
};

#endif
