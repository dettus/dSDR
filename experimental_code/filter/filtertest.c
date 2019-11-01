#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int generatefiltercoeffs(double* pTaps,int num,  int samplerate,int lowerfreq,int upperfreq)
{
	int n;
	double omegau;
	double omegal;
	double x;
	for (n=0;n<num;n++) pTaps[n]=0;

	omegau=(double)upperfreq/(double)samplerate;
	omegal=(double)lowerfreq/(double)samplerate;

	if (lowerfreq==0)	// low pass filter
	{
		pTaps[0]=omegau/M_PI;
		for (n=1;n<num;n++)
		{
			x=n*omegau/M_PI;
			pTaps[n]=omegau/M_PI*sin(x)/x;	
		}
	}
	else if (upperfreq==samplerate)	// high pass filter
	{
		pTaps[0]=1-omegal/M_PI;
		for (n=1;n<num;n++)
		{
			x=n*omegal/M_PI;
			pTaps[n]=-omegal/M_PI*sin(x)/x;	
		}
	} 
	else if (upperfreq>lowerfreq)	// band pass filter
	{
		pTaps[0]=(omegau-omegal)/M_PI;
		for (n=1;n<num;n++)
		{
			x=n*omegal/M_PI;
			pTaps[n]=-omegal/M_PI*sin(x)/x;	
			x=n*omegau/M_PI;
			pTaps[n]+=omegau/M_PI*sin(x)/x;	
		}
	} 
	else if (upperfreq<lowerfreq)	// band stop filter
	{
		pTaps[0]=1-(omegau-omegal)/M_PI;

		for (n=1;n<num;n++)
		{
			x=n*omegal/M_PI;
			pTaps[n]=omegal/M_PI*sin(x)/x;	
			x=n*omegau/M_PI;
			pTaps[n]-=omegau/M_PI*sin(x)/x;	
		}
	} else return 0;
	return 1;
}
void hammingwindow(double* pTaps,double c,int num)
{
	int n;
	for (n=0;n<num/2;n++)
	{
		pTaps[num/2+n]=pTaps[n]*c*(0.54+0.46*cos(2*M_PI*n/(num+1)));
	}
	for (n=0;n<num/2;n++)
	{
		pTaps[n]=pTaps[num-1-n];
	}
}
int main(int argc,char** argv)
{

	double pTaps[501];
	int i;
	generatefiltercoeffs(pTaps,250,2048000,0,96000);
	hammingwindow(pTaps,1.0,501);
	for (i=0;i<501;i++) printf("%f ",pTaps[i]);
	printf("\n");
	return 0;
}


