#ifndef	DEMODFM_H
#define	DEMODFM_H
#include <QtWidgets>
#include <QHBoxLayout>
#include "Demod.h"

class DemodFM: public Demod
{
	Q_OBJECT
	public:
		DemodFM(QWidget* parent=nullptr);
		void onNewSamples(tIQSamplesBlock *pSamples,signed short* pcmBuf,int pcmBufSize,int* pcmNum,int* sampleRate);
		int getBandwidth() {return 192000/2;};
		int getSampleRate() {return 192000;};
		int getFreqOffset() {return 0;};
		int getRaster() {return 100000;};

	private:
		double mAngle=0;
		
		QHBoxLayout *mHLayout=nullptr;
		QLabel *mLabel=nullptr;
		
};

#endif
