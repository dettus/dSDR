#ifndef	CENTRAL_H
#define	CENTRAL_H
#include <QThread>
#include <QMutex>
#include <QWidget>
#include "Datatypes.h"
#include "Sink.h"
#include "WSpectrum.h"
#include "WWaterfall.h"
#include "SimpleFft.h"
#include "MainWindow.h"
#include "Tuners.h"

class Central: public QThread,public Sink,public Zoomback
{

	Q_OBJECT

	public:
		Central(MainWindow* mainwin,Tuners* tuner);
		void onNewSamples(tSComplex* iqSamples,int n);
		void stop();
		void setFftSize(int size);
		void setTuner(Tuners* tuner);
		void setZoom(int left,int right);
	
	protected:
		void run();

	private:
		MainWindow* mMainwin;
		tSComplexBuf	iqBuf;
		bool mStopped;
		QMutex mMutex;
		QMutex mMutex2;
		WSpectrum *mWSpectrum;
		WWaterfall *mWaterfall;
		Tuners	*mTuner;
		int	mFftSize=32768;
		SimpleFft *mFft=nullptr;
	
};
#endif
