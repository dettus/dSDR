#include "SimpleFft.h"
#include "math.h"

SimpleFft::SimpleFft(int fftsize)
{

	int i;
	twiddle_r=new double[fftsize];
	twiddle_i=new double[fftsize];
	tmp_r=new double[fftsize];
	tmp_i=new double[fftsize];

	for (i=0;i<fftsize;i++)
	{
		twiddle_r= cos(2*M_PI*(double)i/(double)fftsize);
		twiddle_i=-sin(2*M_PI*(double)i/(double)fftsize);
	}	
	mFftSize=fftsize;
	mOmegas=1;
}
void SimpeFft::process(signed short* input)
{
	int sigmas;
	int omegamask;
	int notmask;
	int betainc;
	int omegainc;
	int mask;
	int sigma;
	int alpha;
	int beta;

#define PERMUTE8(i)  (((i & 0x001) << 7) | ((i & 0x002) << 5) | ((i & 0x004) << 3) \
		| ((i & 0x008) << 1) | ((i & 0x010) >> 1) | ((i & 0x020) >> 3) \
		| ((i & 0x040) >> 5) | ((i & 0x080) >> 7))

#define PERMUTE9(i)  (((i & 0x001) << 8) | ((i & 0x002) << 6) | ((i & 0x004) << 4) \
		| ((i & 0x008) << 2) | ((i & 0x010) << 0) | ((i & 0x020) >> 2) \
		| ((i & 0x040) >> 4) | ((i & 0x080) >> 6) | ((i & 0x100) >> 8))

#define PERMUTE10(i) (((i & 0x001) << 9) | ((i & 0x002) << 7) | ((i & 0x004) << 5) \
		| ((i & 0x008) << 3) | ((i & 0x010) << 1) | ((i & 0x020) >> 1) \
		| ((i & 0x040) >> 3) | ((i & 0x080) >> 5) | ((i & 0x100) >> 7) \
		| ((i & 0x200) >> 9))

#define PERMUTE11(i) (((i & 0x001) << 10) | ((i & 0x002) << 8) | ((i & 0x004) << 6) \
		| ((i & 0x008) << 4) | ((i & 0x010) << 2) | ((i & 0x020) >> 0) \
		| ((i & 0x040) >> 2) | ((i & 0x080) >> 4) | ((i & 0x100) >> 6) \
		| ((i & 0x200) >> 8) | ((i & 0x400) >> 10))

#define PERMUTE12(i) (((i & 0x001) <<11) | ((i & 0x002) << 9) | ((i & 0x004) << 7) \
		| ((i & 0x008) << 5) | ((i & 0x010) << 3) | ((i & 0x020) << 1) \
		| ((i & 0x040) >> 1) | ((i & 0x080) >> 3) | ((i & 0x100) >> 5) \
		| ((i & 0x200) >> 7) | ((i & 0x400) >> 9) | ((i & 0x800) >>11))

#define PERMUTE13(i) (((i & 0x001) << 12) | ((i & 0x002) <<10) | ((i & 0x004) << 8) \
		| ((i & 0x008) << 6) | ((i & 0x010) << 4) | ((i & 0x020) << 2) | ((i& 0x040) <<0 ) \
		| ((i & 0x080) >> 2) | ((i & 0x100) >> 4) | ((i & 0x200) >> 6) \
		| ((i & 0x400) >> 8) | ((i & 0x800) >> 10)| ((i & 0x1000) >>12))

#define PERMUTE14(i) (((i & 0x001) <<13) | ((i & 0x002) <<11) | ((i & 0x004) << 9) \
		| ((i & 0x008) << 7) | ((i & 0x010) << 5) | ((i & 0x020) << 3) \
		| ((i & 0x040) << 1) | ((i & 0x080) >> 1) | ((i & 0x100) >> 3) \
		| ((i & 0x200) >> 5) | ((i & 0x400) >> 7) | ((i & 0x800) >> 9) \
		| ((i & 0x1000) >> 11) | ((i&0x2000) >> 13))

#define PERMUTE15(i) (((i & 0x001) << 14) | ((i & 0x002) <<12) | ((i & 0x004) <<10) \
		| ((i & 0x008) << 8) | ((i & 0x010) << 6) | ((i & 0x020) << 4) | ((i& 0x040) <<2 ) | ((i&0x080) << 0)\
		| ((i & 0x100) >> 2) | ((i & 0x200) >> 4) | ((i & 0x400) >> 6) \
		| ((i & 0x800) >> 8) | ((i & 0x1000) >> 10)| ((i & 0x2000) >>12) | ((i&0x4000)>>14))

#define PERMUTE16(i) (((i & 0x001) <<15) | ((i & 0x002) <<13) | ((i & 0x004) <<11) \
		| ((i & 0x008) << 9) | ((i & 0x010) << 7) | ((i & 0x020) << 5) \
		| ((i & 0x040) << 3) | ((i & 0x080) << 1) | ((i & 0x100) >> 1) \
		| ((i & 0x200) >> 3) | ((i & 0x400) >> 5) | ((i & 0x800) >> 7) \
		| ((i & 0x1000) >> 9) | ((i&0x2000) >> 11)| ((i &0x4000) >>13) | ((i&0x8000) >>15))

	switch(mFftsize)
	{
		case 256:
			sigmas=8;
			omegamask=0x7f;
			notmask=0x1ff;
			break;
		case 512:
			sigmas=9;
			omegamask=0xff;
			notmask=0x3ff;
			break;
		case 1024:
			sigmas=10;
			omegamask=0x1ff;
			notmask=0x7ff;
			break;
		case 2048:
			sigmas=11;
			omegamask=0x3ff;
			notmask=0xfff;
			break;
		case 4096:
			sigmas=12;
			omegamask=0x7ff;
			notmask=0x1fff;
			break;
		case 8192:
			sigmas=13;
			omegamask=0xfff;
			notmask=0x3fff;
			break;
		case 16384:
			sigmas=14;
			omegamask=0x1fff;
			notmask=0x7fff;
			break;
		case 32768:
			sigmas=15;
			omegamask=0x3fff;
			notmask=0xffff;
			break;
		case 65536:
			sigmas=16;
			omegamask=0x7fff;
			notmask=0x1ffff;
			break;
	}
	mSigmas=sigmas;
	for (i=0;i<mFftSize;i++)
	{
		switch(mFftsize)
		{
			case 256:
				tmp_r[i]=input[2*PERMUTE8(i)+0];
				tmp_i[i]=input[2*PERMUTE8(i)+1];
				break;
			case 512:
				tmp_r[i]=input[2*PERMUTE9(i)+0];
				tmp_i[i]=input[2*PERMUTE9(i)+1];
				break;
			case 1024:
				tmp_r[i]=input[2*PERMUTE10(i)+0];
				tmp_i[i]=input[2*PERMUTE10(i)+1];
				break;
			case 2048:
				tmp_r[i]=input[2*PERMUTE11(i)+0];
				tmp_i[i]=input[2*PERMUTE11(i)+1];
				break;
			case 4096:
				tmp_r[i]=input[2*PERMUTE12(i)+0];
				tmp_i[i]=input[2*PERMUTE12(i)+1];
				break;
			case 8192:
				tmp_r[i]=input[2*PERMUTE13(i)+0];
				tmp_i[i]=input[2*PERMUTE13(i)+1];
				break;
			case 16384:
				tmp_r[i]=input[2*PERMUTE14(i)+0];
				tmp_i[i]=input[2*PERMUTE14(i)+1];
				break;
			case 32768:
				tmp_r[i]=input[2*PERMUTE15(i)+0];
				tmp_i[i]=input[2*PERMUTE15(i)+1];
				break;
			case 65536:
				tmp_r[i]=input[2*PERMUTE16(i)+0];
				tmp_i[i]=input[2*PERMUTE16(i)+1];
				break;

		}
	}
	betainc=1;
	omegainc=mFftSize/2;
	mask=0;

        for (i=0;i<mFftSize/2;i++)
        {
                alpha=(i<<1);
                beta=alpha | 0x01;

                xr=tmp_r[alpha];xi=tmp_i[alpha];
                yr=tmp_r[beta]; yi=tmp_i[beta];

                tmp_r[alpha]=yr+xr;tmp_i[alpha]=yi+xi;
                tmp_r[beta] =xr-yr;tmp_i[beta] =xi-yi;

        }

        for (sigma=1;sigma<sigmas;sigma++)
        {
                double omegar;
                double omegai;
                double mulr;
                double muli;
                omega=0;
                betainc<<=1;
                omegainc>>=1;
                mask=(mask<<1)|0x01;
                notmask=notmask<<1;
                for (i=0;i<mFftSize/2;i++)
                {
                        alpha=((i&notmask)<<1) | (i&mask);
                        beta=alpha|betainc;
                        omegar=omega_r[omega];
                        omegai=omega_i[omega];
                        xr=tmp_r[alpha];xi=tmp_i[alpha];
                        yr=tmp_r[beta]; yi=tmp_i[beta];

                        mulr=omegar*yr-omegai*yi;
                        muli=omegar*yi+omegai*yr;

                        tmp_r[alpha]=(xr+mulr);tmp_i[alpha]=(xi+muli);
                        tmp_r[beta] =(xr-mulr);tmp_i[beta] =(xi-muli);
                        omega += omegainc;
                        omega &= omegamask;
                }
        }
}
void SimpleFft::getResult(signed short &output,bool flipSpectrum)
{
	int i;
	if (flipspectrum)
	{
		for (i=0;i<mFftSize/2;i++)
		{
			output[(i+mFftSize/2)*2+0]=(signed short)(tmp_r[i]/(double)mSigmas);
			output[(i+mFftSize/2)*2+1]=(signed short)(tmp_i[i]/(double)mSigmas);
			output[i*2+0]=(signed short)(tmp_r[i+mFftSize/2]/(double)mSigmas);
			output[i*2+1]=(signed short)(tmp_i[i+mFftSize/2]/(double)mSigmas);
		}
	} else {
		for (i=0;i<mFftSize;i++)
		{
			output[i*2+0]=(signed short)(tmp_r[i]/(double)mSigmas);
			output[i*2+1]=(signed short)(tmp_i[i]/(double)mSigmas);
		}
	}
}
void SimpleFft::getResult(double &output,bool flipSpectrum)
{
	int i;
	if (flipspectrum)
	{
		for (i=0;i<mFftSize/2;i++)
		{
			output[(i+mFftSize/2)*2+0]=(double)(tmp_r[i]/(double)mSigmas);
			output[(i+mFftSize/2)*2+1]=(double)(tmp_i[i]/(double)mSigmas);
			output[i*2+0]=(double)(tmp_r[i+mFftSize/2]/(double)mSigmas);
			output[i*2+1]=(double)(tmp_i[i+mFftSize/2]/(double)mSigmas);
		}
	} else {
		for (i=0;i<mFftSize;i++)
		{
			output[i*2+0]=(double)(tmp_r[i]/(double)mSigmas);
			output[i*2+1]=(double)(tmp_i[i]/(double)mSigmas);
		}
	}

}
void SimpleFft::getSpectrum(double &output,bool flipSpectrum)
{
	int i;
	if (flipspectrum)
	{
		for (i=0;i<mFftSize/2;i++)
		{
			output[i+mFftSize/2] =(double)(tmp_r[i]*tmp_r[i]);
			output[i+mFftSize/2]+=(double)(tmp_i[i]*tmp_i[i]);
			output[i] =(double)(tmp_r[i+mFftSize/2]*tmp_r[i+mFftSize/2]);
			output[i]+=(double)(tmp_i[i+mFftSize/2]*tmp_i[i+mFftSize/2]);
		}
	} else {
		for (i=0;i<mFftSize;i++)
		{
			output[i] =(double)(tmp_r[i]*tmp_r[i]);
			output[i]+=(double)(tmp_i[i]*tmp_i[i]);
		}
	}
}
int SimpleFft::getSize()
{
	return mFftSize;	
}
