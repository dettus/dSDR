#include <stdio.h>
#include <stdlib.h>


#define	INRATE	2048000
#define	OUTRATE  192000
int main(int argc,char** argv)
{
	FILE *f;
	FILE *g;
	int accu;
	double sumre,sumim;
	signed short rpart,ipart;
	int cnt;

	if (argc!=3)
	{
		fprintf(stderr,"Please run with %s INPUT.iq2048 OUTPUT.iq192\n",argv[0]);
		fprintf(stderr,"where INPUT and OUTPUT are IQ-files with\n");
		fprintf(stderr,"2048000 hz/192000hz sampling rate, IQ interleaved,\n");
		fprintf(stderr,"16 bit signed integer. \n");
		return 0;
	}

	if (argv[1][0]=='-' && argv[1][1]==0) f=stdin; else f=fopen(argv[1],"rb");
	if (argv[2][0]=='-' && argv[2][1]==0) g=stdout; else g=fopen(argv[2],"wb");

	accu=0;
	sumre=sumim=0;
	cnt=0;

	fread(&rpart,sizeof(short),1,f);
	while (!feof(f))
	{
		fread(&ipart,sizeof(short),1,f);
		sumre+=rpart;
		sumim+=ipart;
		cnt++;
		accu+=OUTRATE;
		if (accu>=INRATE)
		{
			double c;
			c=cnt;
			c/=2.0f;

			sumre/=c;
			sumim/=c;

			rpart=sumre;
			ipart=sumim;
			fwrite(&rpart,sizeof(short),1,g);
			fwrite(&ipart,sizeof(short),1,g);

			sumre=sumim=0;			
			accu-=INRATE;
			cnt=0;
		}
		fread(&rpart,sizeof(short),1,f);
	}
	fclose(f);
	fclose(g);
	return 1;
	
}
