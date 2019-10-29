// tuners are Hardware Components. Each one of them has a different Layout, a different
// design and a different API. 
// the purpose of this class is to provide a minimal API, shared among tuner hardware.
// unique configurations can be set within a Tuner Widget Window.
//
// $Id$

#ifndef	TUNERS_H
#define	TUNERS_H



#include "DataTypes.h"
#include <QtWidgets>

class Tuners: public QWidget
{
	Q_OBJECT

	public:
		Tuners(QWidget* parent): QWidget(parent){};
		// this function is called from the tuner thread.
		// it handles the hardware access in the background

		virtual void initialize()=0;		// this function is supposed to be called AFTER the constructor. it contains mallocs().
							// the difference is that the constructor is supposed to set up the GUI widgets.
		virtual void process()=0;		// is called periodically
		virtual int timeToWait()=0;		// how long should the thread wait before calling it?
	


	
		// this function is called from the signal processing
		virtual void getSamples(tIQSamplesBlock *pIQSamplesBlock)=0;	// this function is used to poll the samples

		// those functions are the minimum, that each tuner has to provide.
		// the rest of them are going to be part of the specific tuner widgets
		virtual int getSampleRate()=0;		// returns the number of samples per second
		virtual int getFrequency()=0;		// returns the center frequency
	
		// some tuners do not like certain gain settings. Thus, the API functions return true upon success
		virtual bool setFrequency(int freqHz)=0;
		virtual bool setGain(int gainCB)=0;
		virtual bool gainUp()=0;		// more gain
		virtual bool gainDown()=0;		// less gain
		
};
#endif
