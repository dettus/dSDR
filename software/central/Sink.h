#ifndef	SINK_H
#define	SINK_H
class Sink
{
	public:
		virtual void onNewSamples(signed short* iqSamples,int n) = 0;
};
#endif
