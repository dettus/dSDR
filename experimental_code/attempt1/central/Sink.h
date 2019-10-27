#ifndef	SINK_H
#define	SINK_H
#include "Datatypes.h"
class Sink
{
	public:
		virtual void onNewSamples(tSComplex* iqSamples,int n) = 0;
};
#endif
