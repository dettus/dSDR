#include <stdio.h>
#include "SimpleFft.h"

int main(int argc,char** argv)
{
	int fftsize=2048;
	signed short input[2*2048];
	double output[2*2048];
	double spectrum[2048];
	int i;
	SimpleFft* simpleFft=new SimpleFft(fftsize);
	
	for (i=0;i<fftsize;i++)
	{
		input[i]=i;
	}
	simpleFft->process(input);
	simpleFft->getResult(output);
	simpleFft->getSpectrum(spectrum);

	printf("tmp=[");
	for (i=0;i<fftsize;i++)
	{
		printf("%.f ",spectrum[i]);
	}
	printf("];\n");
	printf("plot(1:length(tmp),tmp);\n");
}
