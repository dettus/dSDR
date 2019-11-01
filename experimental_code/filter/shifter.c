#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define	SAMPLERATE	2048000

#define	MULR(ar,ai, br,bi)   ((ar)*(br)-(ai)*(bi))
#define	MULI(ar,ai, br,bi)   ((ar)*(bi)+(ai)*(br))
double costab[SAMPLERATE];
double sintab[SAMPLERATE];
int main(int argc,char** argv)
{
	FILE *f;
	FILE *g;
	signed short rpart,ipart;
	int shiftfreq;
	int idx;
	int i;

	if (argc!=4)
	{
		fprintf(stderr,"Please run with %s INPUT.iq2048 OUTPUT.iq2048 SHIFT\n",argv[0]);
		fprintf(stderr,"where INPUT and OUTPUT are IQ-files with\n");
		fprintf(stderr,"2048000 hz sampling rate, IQ interleaved,\n");
		fprintf(stderr,"16 bit signed integer. \n");
		fprintf(stderr,"And SHIFT is in Hz\n");
		return 0;
	}
	shiftfreq=atoi(argv[3]);
	for (i=0;i<SAMPLERATE;i++)
	{
		costab[i]=cos(2*M_PI*(double)i*(double)shiftfreq/SAMPLERATE);
		sintab[i]=sin(2*M_PI*(double)i*(double)shiftfreq/SAMPLERATE);
	}

	if (argv[1][0]=='-' && argv[1][1]==0) f=stdin; else f=fopen(argv[1],"rb");
	if (argv[2][0]=='-' && argv[2][1]==0) g=stdout; else g=fopen(argv[2],"wb");
	fread(&rpart,sizeof(short),1,f);
	idx=0;
	while (!feof(f))
	{
		double treal,timag;
		fread(&ipart,sizeof(short),1,f);

		treal=MULR(rpart,ipart,  costab[idx],sintab[idx]);
		timag=MULI(rpart,ipart,  costab[idx],sintab[idx]);
		idx=(idx+1)%SAMPLERATE;

		rpart=treal;
		ipart=timag;
		fwrite(&rpart,sizeof(short),1,g);
		fwrite(&ipart,sizeof(short),1,g);
		fread(&rpart,sizeof(short),1,f);

	}
	fclose(f);
	fclose(g);

}
