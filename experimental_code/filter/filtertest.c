#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// filter generation, according to Gerhard Krucker.

int generate_bandpass(double *pTaps,int tapnum,int lower,int upper,int samplerate)
{
	int i,m;
	double u,l;

	if (!(tapnum&1)) return 0;
	l=(double)lower*2*M_PI/(double)samplerate;
	u=(double)upper*2*M_PI/(double)samplerate;

	for (i=0;i<tapnum;i++)
	{
		m=i-(tapnum-1)/2;
		if (m==0) 
		{
			pTaps[i]=(u-l)/M_PI;
		} else {
			pTaps[i]=(sin(m*u)-sin(m*l))/(m*M_PI);
		}
	}
	return 1;
}
int generate_bandstop(double *pTaps,int tapnum,int lower,int upper,int samplerate)
{
	int i,m;
	double u,l;

	if (!(tapnum&1)) return 0;
	l=(double)lower*2*M_PI/(double)samplerate;
	u=(double)upper*2*M_PI/(double)samplerate;

	for (i=0;i<tapnum;i++)
	{
		m=i-(tapnum-1)/2;
		if (m==0) 
		{
			pTaps[i]=(M_PI-u+1)/M_PI;
		} else {
			pTaps[i]=(sin(m*M_PI)-sin(m*u)+sin(m*l))/(m*M_PI);
		}
	}
	return 1;
}
int generate_highpass(double *pTaps,int tapnum,int lower,int samplerate)
{
	int i,m;
	double l;

	if (!(tapnum&1)) return 0;
	l=(double)lower*2*M_PI/(double)samplerate;

	for (i=0;i<tapnum;i++)
	{
		m=i-(tapnum-1)/2;
		if (m==0) 
		{
			pTaps[i]=1-1/M_PI;
		} else {
			pTaps[i]=-sin(m*l)/(m*M_PI);
		}
	}
	return 1;
}
int generate_lowpass(double *pTaps,int tapnum,int upper,int samplerate)
{
	int i,m;
	double u;

	if (!(tapnum&1)) return 0;
	u=(double)upper*2*M_PI/(double)samplerate;

	for (i=0;i<tapnum;i++)
	{
		m=i-(tapnum-1)/2;
		if (m==0) 
		{
			pTaps[i]=u/M_PI;
		} else {
			pTaps[i]=sin(m*u)/(m*M_PI);
		}
	}
	return 1;
}
int generate_hilbert(double *pTaps,int tapnum)
{
	int i,m;
	if (!(tapnum&1)) return 0;
	for (i=0;i<tapnum;i++)
	{
		m=i-(tapnum-1)/2;
		if (i&1)
			pTaps[i]=2/(m*M_PI);
		else
			pTaps[i]=0;
	}
	return 1;
}

int apply_blackman(double *pTaps,int tapnum,double *sum)
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
	*sum=s;
	return 1;	
}
int apply_bartlett(double *pTaps,int tapnum,double *sum)
{
	int i,m;
	double s;
	s=0;
	for (i=0;i<tapnum;i++)
	{
		double a;
		m=i-(tapnum-1)/2;
		a=1-2*(abs(m))/(tapnum-1);
		pTaps[i]*=a;
		s+=pTaps[i];
	}
	*sum=s;
	return 1;	
}

int apply_hamming(double *pTaps,int tapnum,double *sum)
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
	*sum=s;
	return 1;	
}
int apply_hanning(double *pTaps,int tapnum,double *sum)
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
	*sum=s;
	return 1;	
}
int apply_kaiser(double *pTaps,int tapnum,double gamma,double *sum)
{
	int i,m;
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
	*sum=s;
	return 1;	
}
int main(int argc,char** argv)
{

	double pTaps[501];
	double sum;
	int i;

	generate_lowpass(pTaps,501,96000,2048000);
	apply_hamming(pTaps,501,&sum);
	printf("t=[");
	for (i=0;i<501;i++) printf("%f ",pTaps[i]);
	printf("];%%%%%%%% %f\n",sum);
	return 0;
}


