#ifndef TUNERS_H
#define TUNERS_H
#include <QWidget>
#include "Sink.h"


// this is an abstract tuner class.
// its purpose is to provide a uniform interface, no matter what hardware is being used.

// specific parameters that are unique to the tuner can be changed in a special widget panel.

class Tuners
{
	// any tuner has to implement the following functions, to provide a uniform "look&feel". no matter what the actual hardware is.
	public:
		virtual	QWidget* getWidget() = 0;	// this one should return a QT widget, in which specific parameters can be selected. there will be a panel in the main gui reserved for this.
							// in case the tuner should not have such a widget, this function returns with a nullptr.
		virtual int getFrequency() = 0;		// this function returns the current frequency (in Hertz)
		virtual int getGain() = 0;		// this function returns the gain in centibel. 1cB=0.1dB
		virtual int getSamplerate() = 0;	// this function returns the current samplerate in Hertz.
		
	
		// the following functions are returning "true", if the selected parameter is valid for the actual tuner. there might be tuners that can only receive UHF transmissions, so setting them to 5000 kHz is not possible for them.
		virtual bool setFrequency(int freqHz) = 0;	// calling this function causes a retune.
		virtual bool setGain(int gaincB) = 0;		// calling this function will change the gain.
		virtual bool setSamplerate(int sampleRate) = 0;	// some tuners have the possibilty to set their samplingrate

		virtual void gainUp() = 0;	// to provide a uniform "look&feel", the tuner's gain should be increased by this function.
		virtual void gainDown() = 0;	// to provide a uniform "look&feel", the tuner's gain should be reduced by this function.

		virtual void stop() = 0;	// tuners will be running in threads. there has to be a mechanism to shut them down.

		void setSink(Sink* pSink)
		{
			mSink=pSink;
		}
		

	protected:
		virtual void run() = 0;		// tuners will be running in threads. 
		Sink *mSink=nullptr;
};
#endif
