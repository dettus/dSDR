#ifndef	DemodAM_H
#define	DemodAM_H
#include <QtWidgets>
#include <QHBoxLayout>
#include "Demod.h"

class DemodAM: public Demod
{
	Q_OBJECT
	public:
		DemodAM(QWidget* parent=nullptr);
		void onNewSamples(tIQSamplesBlock *pSamples,signed short* pcmBuf,int pcmBufSize,int* pcmNum,int* sampleRate);
		int getBandwidth() {return 192000/2;};
		int getSampleRate() {return 192000;};
//		int getBandwidth() {return 10000/2;};
//		int getSampleRate() {return 10000;};
		int getFreqOffset() {return 0;};
		int getRaster() {return 1000;};

	private:
		double mAngle=0;
		
		QHBoxLayout *mHLayout=nullptr;
		QLabel *mLabel=nullptr;

		int mCnt=0;
		signed short mMin=0;
		signed short mMax=0;
		signed short mBias=0;
		signed short mFact=1;
		
};

#endif
