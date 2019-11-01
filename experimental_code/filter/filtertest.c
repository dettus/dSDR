#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// sinc(x)=\frac{sin(x)}{x}
//
// lowpass:    h[n]= \frac{\Omega_u}{\pi}\sinc \left(\frac{n\Omega_u}{pi}
// highpass:   h[n]=-\frac{\Omega_l}{\pi}\sinc \left(\frac{n\Omega_l}{pi} 
// bandpass:   h[n]= \frac{\Omega_u}{\pi}\sinc \left(\frac{n\Omega_u}{pi}-\frac{\Omega_l}{\pi}\sinc \left(\frac{n\Omega_l}{pi} 
// bandstop:   h[n]= \frac{\Omega_l}{\pi}\sinc \left(\frac{n\Omega_l}{pi}-\frac{\Omega_u}{\pi}\sinc \left(\frac{n\Omega_u}{pi} 

// h_lowpass[0]&=&\frac{\Omega_u}{\pi}
// h_highpass[0]&=&1-\frac{\Omega_u}{\pi}
// h_bandpass[0]&=&\frac{\Omega_u-\Omega_l}{\pi}
// h_bandstop[0]&=&1-\frac{\Omega_u-\Omega_l}{\pi}


int generatefiltercoeffs(double* pTaps,int num,  int samplerate,int lowerfreq,int upperfreq)
{
	int n;
	double omegau;
	double omegal;
	double x;
	for (n=0;i<num;n++) pTaps[n]=0;
	pTaps[num/2]=1;

	omegau=(double)upper/(double)samplerate;
	omegal=(double)lower/(double)samplerate;

	if (lowerfreq==0)	// low pass filter
	{
		pTaps[0]=omegau/M_PI;
		for (n=1;n<num/2;n++)
		{
			x=n*omegau/M_PI;
			pTaps[n]=omegau/M_PI*sin(x)/x;	
		}
	}
	else if (upperfreq==samplerate)	// high pass filter
	{
		pTaps[0]=1-omegal/M_PI;
		for (n=1;n<num/2;n++)
		{
			x=n*omegal/M_PI;
			pTaps[n]=-omegal/M_PI*sin(x)/x;	
		}
	} 
	else if (upperfreq>lowerfreq)	// band pass filter
	{
		pTaps[0]=(omegau-omegal)/M_PI;
		for (n=1;n<num/2;n++)
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

		for (n=1;n<num/2;n++)
		{
			x=n*omegal/M_PI;
			pTaps[n]=omegal/M_PI*sin(x)/x;	
			x=n*omegau/M_PI;
			pTaps[n]-=omegau/M_PI*sin(x)/x;	
		}
	}
	// mirroring the filter taps along the middle
	for (i=num/2+1;i<num;i++)
	{
		pTaps[i]=pTaps[i-num/2];	
	}
	for (i=0;i<num/2+1;i++)
	{
		pTaps[i]=pTaps[num-i];	
	}
}

int main(int argc,char** argv)
{

}


