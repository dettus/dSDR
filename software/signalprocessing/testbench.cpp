#include <stdio.h>
#include <stdlib.h>
#include "DataTypes.h"

#include "Downsampler.h"
#include "Filter.h"
#include "SimpleFft.h"
#include "SimpleShifter.h"
tSComplex inSamples[2048000];
tSComplex tmpSamples[2048000];
tSComplex outSamples[2048000];

int main(int argc,char** argv)
{
	FILE *f;
	FILE *g;
	int n;

	if (argc!=6)
	{
		fprintf(stderr,"please run with %s INPUTFILE.iq2048 OUTPUTFILE.iq192 INSAMPLERATE FREQHZ OUTSAMPLERATE\n",argv[0]);
		return 0;
	}

	f=fopen(argv[1],"rb");
	g=fopen(argv[2],"wb");

	int insamplerate=atoi(argv[3]);
	int freqhz=atoi(argv[4]);
	int outsamplerate=atoi(argv[5]);

	tIQSamplesBlock	inBlock,outBlock;	

	SimpleShifter *shifter=new SimpleShifter(insamplerate,freqhz);
	Downsampler *downSampler=new Downsampler(insamplerate,outsamplerate,outsamplerate/4);

	

	while (!feof(f))
	{
		n=fread(inSamples,sizeof(tSComplex),2048000,f);
		shifter->process(inSamples,tmpSamples,n);
		inBlock.pData=tmpSamples;
		inBlock.sampleNum=n;
		inBlock.sampleRate=insamplerate;

		outBlock.pData=outSamples;

		downSampler->process(&inBlock,&outBlock);
		fwrite(outSamples,sizeof(tSComplex),outBlock.sampleNum,g);

	}

	fclose(f);	
	fclose(g);
	return 1;
}
