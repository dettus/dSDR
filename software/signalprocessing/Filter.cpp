#include "Filter.h"
#include <math.h>
#include <stdio.h>

Filter::Filter(int samplerate,int tapnum,int upper, int lower)
{
	double u,l;
	mSampleRate=samplerate;
	mTapnum=tapnum;
	mUpperFreq=upper;
	mLowerFreq=lower;
	if (!(mTapnum&1)) mTapnum++;

	mTaps=new double[tapnum];


	u=convertFreq(samplerate,upper);
	l=convertFreq(samplerate,lower);
	if (upper==samplerate)
	{
		generate_highpass(mTaps,mTapnum,l);
	} else if (lower==0 && upper!=0)
	{
		generate_lowpass(mTaps,mTapnum,u);
	} else if (lower<upper)
	{
		Filter::generate_bandpass(mTaps,mTapnum,u,l);
	} else if (lower>upper) 
	{
		generate_bandstop(mTaps,mTapnum,l,u);
	} else generate_hilbert(mTaps,mTapnum);

	apply_hamming(mTaps,mTapnum);

	mBufLen=mTapnum*2;	
	mBuf=new tSComplex[mBufLen];

}

Filter::~Filter()
{
	if (mTaps!=nullptr) delete(mTaps);
}


// The filter generation was described by Gerhard Krucker.
void Filter::generate_bandpass(double *pTaps,int tapnum,double u,double l)
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
void Filter::generate_bandstop(double *pTaps,int tapnum,double u,double l)
{
	int i,m;
	for (i=0;i<tapnum;i++)
	{
		m=i-(tapnum-1)/2;
		if (m==0) 
			pTaps[i]=(M_PI-u+1)/M_PI;
		else
			pTaps[i]=(sin(m*M_PI)-sin(m*u)+sin(m*l))/(m*M_PI);
	}
}
void Filter::generate_lowpass(double *pTaps,int tapnum,double u)
{
	int i,m;
	for (i=0;i<tapnum;i++)
	{
		m=i-(tapnum-1)/2;
		if (m==0) 
			pTaps[i]=u/M_PI;
		else
			pTaps[i]=sin(m*u)/(m*M_PI);
	}

}

void Filter::generate_highpass(double *pTaps,int tapnum,double l)
{
	int i,m;
	for (i=0;i<tapnum;i++)
	{
		m=i-(tapnum-1)/2;
		if (m==0) 
			pTaps[i]=1-1/M_PI;
		else
			pTaps[i]=sin(m*l)/(m*M_PI);
	}

}
void Filter::generate_hilbert(double *pTaps,int tapnum)
{
	int i,m;
	for (i=0;i<tapnum;i++)
	{
		m=i-(tapnum-1)/2;
		if (i&1)
			pTaps[i]=2/(m*M_PI);
		else
			pTaps[i]=0;
	}

}
double Filter::apply_blackman(double *pTaps,int tapnum)
{
	int i,m;
	double s;
	s=0;
	for (i=0;i<tapnum;i++)
	{
		double a;
		m=i-(tapnum-1)/2;
		a=0.42+0.5*cos((2*M_PI*m)/(tapnum-1))+0.08*cos((4*M_PI*m)/(tapnum-1));   
		pTaps[i]*=a;
		s+=pTaps[i];
	}
	return s;
}

double Filter::apply_bartlett(double *pTaps,int tapnum)
{
	int i,m;
	double s;
	s=0;
	for (i=0;i<tapnum;i++)
	{
		double a;
		m=i-(tapnum-1)/2;
		if (m<0) m=-m;	// ABS
		a=1-2*(m)/(tapnum-1);
		pTaps[i]*=a;
		s+=pTaps[i];
	}
	return s;
}


double Filter::apply_hamming(double *pTaps,int tapnum)
{
	int i,m;
	double s;
	s=0;
	for (i=0;i<tapnum;i++)
	{
		double a;
		m=i-(tapnum-1)/2;
		a=0.54+0.46*cos((2*M_PI*m)/(tapnum-1));
		pTaps[i]*=a;
		s+=pTaps[i];
	}
	return s;

}

double Filter::apply_hanning(double *pTaps,int tapnum)
{
	int i,m;
	double s;
	s=0;
	for (i=0;i<tapnum;i++)
	{
		double a;
		m=i-(tapnum-1)/2;
		a=0.5+0.5*cos((2*M_PI*m)/(tapnum-1));
		pTaps[i]*=a;
		s+=pTaps[i];
	}
	return s;
}
double Filter::apply_kaiser(double *pTaps,int tapnum,double gamma)
{
	int i;
	double s;
	s=0;
	for (i=0;i<tapnum;i++)
	{
		double a;
		//m=i-(tapnum-1)/2;
		a=(10*gamma*sqrt(i*(tapnum-i-1))/(tapnum-1))/(10*gamma);
		pTaps[i]*=a;
		s+=pTaps[i];
	}
	return s;
}

double Filter::convertFreq(int samplerate,int freq)
{
	return (double)freq*M_PI/(double)samplerate;	
}
void Filter::process(tSComplex* input,tSComplex* output,int n)
{
	int i;
	int j;
	int ridx;
	double sumr,sumi;

	
	for (i=0;i<n;i++)
	{
		mBuf[mBufIdx]=input[i];
		mBufIdx=(mBufIdx+1)%mBufLen;
		ridx=mBufIdx;
		sumr=sumi=0;
		for (j=0;j<mTapnum;j++)
		{
			if (ridx==0) ridx=mBufLen;
			ridx--;
			sumr+=mTaps[j]*mBuf[ridx].real;
			sumi+=mTaps[j]*mBuf[ridx].imag;
		}
	//	sumr/=mBufLen;
	//	sumi/=mBufLen;

		output[i].real=(signed short)sumr;
		output[i].imag=(signed short)sumi;
	}
}
