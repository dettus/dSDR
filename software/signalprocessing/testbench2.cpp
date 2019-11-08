#include <stdio.h>
#include <stdlib.h>
#include "DataTypes.h"

#include "Upsampler.h"
#include "Downsampler.h"
#include "Filter.h"
#include "SimpleFft.h"
tSComplex inSamples[2048000];
tSComplex outSamples[2048000];

int main(int argc,char** argv)
{
	FILE *f;
	FILE *g;
	int n;

	if (argc!=5)
	{
		fprintf(stderr,"please run with %s INPUTFILE.iq12 OUTPUTFILE.iq48 INSAMPLERATE OUTSAMPLERATE\n",argv[0]);
		return 0;
	}

	f=fopen(argv[1],"rb");
	g=fopen(argv[2],"wb");

	int insamplerate=atoi(argv[3]);
	int outsamplerate=atoi(argv[4]);

	tIQSamplesBlock	inBlock,outBlock;	

	Downsampler *downSampler=nullptr;
	Upsampler *upSampler=nullptr;

	if (insamplerate>outsamplerate)
	{
		downSampler=new Downsampler(insamplerate,outsamplerate,outsamplerate/4);
	} else {
		upSampler=new Upsampler(insamplerate,outsamplerate);
	}

	
	inBlock.pData=inSamples;
	while (!feof(f))
	{
		n=fread(inSamples,sizeof(tSComplex),insamplerate,f);
		inBlock.sampleNum=n;
		inBlock.sampleRate=insamplerate;

		outBlock.pData=outSamples;

		if (upSampler!=nullptr) upSampler->process(&inBlock,&outBlock);
		if (downSampler!=nullptr) downSampler->process(&inBlock,&outBlock);
		fwrite(outSamples,sizeof(tSComplex),outBlock.sampleNum,g);

	}

	fclose(f);	
	fclose(g);
	return 1;
}
