#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc,char** argv)
{
	FILE *f;
	FILE *g;

	signed short ipart,qpart;
	double angle_old;
	double angle_new;
	double delta;
	signed short pcm;

	angle_old=angle_new=0;
	if (argc!=3)
	{
		fprintf(stderr,"please run with %s INPUT.iq192 OUTPUT.pcm\n",argv[0]);
		return 0;
	}

	f=fopen(argv[1],"rb");
	g=fopen(argv[2],"wb");

	fread(&ipart,sizeof(short),1,f);
	while (!feof(f))
	{
		fread(&qpart,sizeof(short),1,f);
		angle_old=angle_new;
		angle_new=atan2(qpart,ipart);
		delta=angle_new-angle_old;
		if (delta<-M_PI) delta+=2*M_PI;
		if (delta>M_PI) delta-=2*M_PI;
		pcm=(signed short)(delta*8192.0/M_PI);
		fwrite(&pcm,sizeof(short),1,g);

		fread(&ipart,sizeof(short),1,f);
	}

	fclose(f);
	fclose(g);
}
