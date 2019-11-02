#include "Filter.h"
#include <math.h>

Filter::Filter(int samplerate,int tapnum,int upper, int lower)
{
	double u,l;
	mSampleRate=samplerate;
	mTapnum=tapnum;
	mUpperFreq=upper;
	mLowerFreq=lower;
	if (!(mTapnum&1)) mTapnum++;

	mTaps=new double[tapnum];


	u=(double)upper*2*M_PI/(double)samplerate;	
	l=(double)lower*2*M_PI/(double)samplerate;	
	if (upper==samplerate)
	{
		generate_highpass(l);
	} else if (lower==0 && upper!=0)
	{
		generate_lowpass(u);
	} else if (lower<upper)
	{
		Filter::generate_bandpass(mTaps,mTapnum,u,l);
	} else if (lower>upper) 
	{
		generate_bandstop(l,u);
	} else generate_hilbert();

	apply_hamming();

}

Filter::~Filter()
{
	if (mTaps!=nullptr) delete(mTaps);
}


// The filter generation was described by Gerhard Krucker.
static void Filter::generate_bandpass(double *pTaps,int tapum,double u,double l)
{
	int i,m;
	for (i=0;i<tapnum;i++)
	{
		m=i-(tapnum-1)/2;
		if (m==0) 
			pTaps[i]=(u-l)/M_PI;
		else
			pTaps[i]=(sin(m*u)-sin(m*l))/(m*M_PI);
	}
}
void Filter::generate_bandstop(double u,double l)
{
	int i,m;
	for (i=0;i<mTapnum;i++)
	{
		m=i-(mTapnum-1)/2;
		if (m==0) 
			mTaps[i]=(M_PI-u+1)/M_PI;
		else
			mTaps[i]=(sin(m*M_PI)-sin(m*u)+sin(m*l))/(m*M_PI);
	}
}
void Filter::generate_lowpass(double u)
{
	int i,m;
	for (i=0;i<mTapnum;i++)
	{
		m=i-(mTapnum-1)/2;
		if (m==0) 
			mTaps[i]=u/M_PI;
		else
			mTaps[i]=sin(m*u)/(m*M_PI);
	}

}

void Filter::generate_highpass(double l)
{
	int i,m;
	for (i=0;i<mTapnum;i++)
	{
		m=i-(mTapnum-1)/2;
		if (m==0) 
			mTaps[i]=1-1/M_PI;
		else
			mTaps[i]=sin(m*l)/(m*M_PI);
	}

}
void Filter::generate_hilbert()
{
	int i,m;
	for (i=0;i<mTapnum;i++)
	{
		m=i-(mTapnum-1)/2;
		if (i&1)
			mTaps[i]=2/(m*M_PI);
		else
			mTaps[i]=0;
	}

}
double Filter::apply_blackman()
{
	int i,m;
	double s;
	s=0;
	for (i=0;i<mTapnum;i++)
	{
		double a;
		m=i-(mTapnum-1)/2;
		a=0.42+0.5*cos((2*M_PI*m)/(mTapnum-1))+0.08*cos((4*M_PI*m)/(mTapnum-1));   
		mTaps[i]*=a;
		s+=mTaps[i];
	}
	return s;
}
double Filter::apply_bartlett()
{
	int i,m;
	double s;
	s=0;
	for (i=0;i<mTapnum;i++)
	{
		double a;
		m=i-(mTapnum-1)/2;
		a=1-2*(abs(m))/(mTapnum-1);
		mTaps[i]*=a;
		s+=mTaps[i];
	}
	return s;
}


double Filter::apply_hamming()
{
	int i,m;
	double s;
	s=0;
	for (i=0;i<mTapnum;i++)
	{
		double a;
		m=i-(mTapnum-1)/2;
		a=0.54+0.46*cos((2*M_PI*m)/(mTapnum-1));
		mTaps[i]*=a;
		s+=mTaps[i];
	}
	return s;

}

double Filter::apply_hanning()
{
	int i,m;
	double s;
	s=0;
	for (i=0;i<mTapnum;i++)
	{
		double a;
		m=i-(mTapnum-1)/2;
		a=0.5+0.5*cos((2*M_PI*m)/(mTapnum-1));
		mTaps[i]*=a;
		s+=mTaps[i];
	}
	return s;
}
double Filter::apply_kaiser(double gamma)
{
	int i;
	double s;
	s=0;
	for (i=0;i<mTapnum;i++)
	{
		double a;
		//m=i-(mTapnum-1)/2;
		a=(10*gamma*sqrt(i*(mTapnum-i-1))/(mTapnum-1))/(10*gamma);
		mTaps[i]*=a;
		s+=mTaps[i];
	}
	return s;
}

