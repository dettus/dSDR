#include "Central.h"
#define	BUFSIZE	(1<<23)

Central::Central()
{
	iqBuf.samples=new tSCOmplex[BUFSIZE];
	iqBuf.size=BUFSIZE;
	iqBuf.fillcur=0;
	iqBuf.fillnext=0;
	iqBuf.used=0;
	
	mStopped=false;
}
void Central::onNewSamples(tSComplex* iqSamples,int n)
{
	int i;
	int j;
	int mask;
	// lock here
	
	j=iqBuf.fillnext;
	mask=iqBuf.size-1;
	for (i=0;i<n;i++)
	{
		iqBuf.samples[j&mask]=iqSamples[i];
		j++;
	}
	iqBuf.fillnext=j;

	// unlock here
}

void Central::run()
{
	while (!mStopped)
	{
		QThread::msleep(1);
		// lock
		if (iqBuf.fillnext!=iqBuf.fillcur)
		{
			iqBuf.fillcur=iqBuf.fillnext;
		}
		if (iqBuf.fillcur>=iqBuf.size && iqBuf.fillnext>=iqBuf.size && iqBuf.used>=iqBuf.size)
		{
			iqBuf.fillcur-=size;
			iqBuf.fillnext-=size;
			iqBuf.used-=size;
		}
		// unlock



		iqBuf.used=iqBuf.fillcur;
	}
}
void Central::stop()
{
	mStopped=true;
}