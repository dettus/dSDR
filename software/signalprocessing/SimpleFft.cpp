#include "SimpleFft.h"
#include "math.h"

SimpleFft::SimpleFft(int fftsize)
{

	int i;
	mTwiddle_r=new double[fftsize];
	mTwiddle_i=new double[fftsize];
	mTmp_r=new double[fftsize];
	mTmp_i=new double[fftsize];
	mPermute=new int[fftsize];
	

	for (i=0;i<fftsize;i++)
	{
		mTwiddle_r[i]= cos(2*M_PI*(double)i/(double)fftsize);
		mTwiddle_i[i]=-sin(2*M_PI*(double)i/(double)fftsize);
	}	
	mFftSize=fftsize;
	

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

	mSigmas=0;
	mOmegaMask=1;
	mNotMask=2;

	switch(mFftSize)
	{
		case 256:
			mSigmas=8;
			mOmegaMask=0x7f;
			mNotMask=0x1ff;
			for (i=0;i<fftsize;i++) mPermute[i]=PERMUTE8(i);
			break;
		case 512:
			mSigmas=9;
			mOmegaMask=0xff;
			mNotMask=0x3ff;
			for (i=0;i<fftsize;i++) mPermute[i]=PERMUTE9(i);
			break;
		case 1024:
			mSigmas=10;
			mOmegaMask=0x1ff;
			mNotMask=0x7ff;
			for (i=0;i<fftsize;i++) mPermute[i]=PERMUTE10(i);
			break;
		case 2048:
			mSigmas=11;
			mOmegaMask=0x3ff;
			mNotMask=0xfff;
			for (i=0;i<fftsize;i++) mPermute[i]=PERMUTE11(i);
			break;
		case 4096:
			mSigmas=12;
			mOmegaMask=0x7ff;
			mNotMask=0x1fff;
			for (i=0;i<fftsize;i++) mPermute[i]=PERMUTE12(i);
			break;
		case 8192:
			mSigmas=13;
			mOmegaMask=0xfff;
			mNotMask=0x3fff;
			for (i=0;i<fftsize;i++) mPermute[i]=PERMUTE13(i);
			break;
		case 16384:
			mSigmas=14;
			mOmegaMask=0x1fff;
			mNotMask=0x7fff;
			for (i=0;i<fftsize;i++) mPermute[i]=PERMUTE14(i);
			break;
		case 32768:
			mSigmas=15;
			mOmegaMask=0x3fff;
			mNotMask=0xffff;
			for (i=0;i<fftsize;i++) mPermute[i]=PERMUTE15(i);
			break;
		case 65536:
			mSigmas=16;
			mOmegaMask=0x7fff;
			mNotMask=0x1ffff;
			for (i=0;i<fftsize;i++) mPermute[i]=PERMUTE16(i);
			break;

	}
}
void SimpleFft::process(signed short* input)
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
	int i;
	double xr,xi,yr,yi;
	
	sigmas=mSigmas;
	omegamask=mOmegaMask;
	notmask=mNotMask;	
	for (i=0;i<mFftSize;i++)
	{
		mTmp_r[i]=input[2*mPermute[i]+0];
		mTmp_i[i]=input[2*mPermute[i]+1];

	}
	betainc=1;
	omegainc=mFftSize/2;
	mask=0;

        for (i=0;i<mFftSize/2;i++)
        {
                alpha=(i<<1);
                beta=alpha | 0x01;

                xr=mTmp_r[alpha];xi=mTmp_i[alpha];
                yr=mTmp_r[beta]; yi=mTmp_i[beta];

                mTmp_r[alpha]=yr+xr;mTmp_i[alpha]=yi+xi;
                mTmp_r[beta] =xr-yr;mTmp_i[beta] =xi-yi;

        }

        for (sigma=1;sigma<sigmas;sigma++)
        {
                double omegar;
                double omegai;
                double mulr;
                double muli;
		int omega;
                omega=0;
                betainc<<=1;
                omegainc>>=1;
                mask=(mask<<1)|0x01;
                notmask=notmask<<1;
                for (i=0;i<mFftSize/2;i++)
                {
                        alpha=((i&notmask)<<1) | (i&mask);
                        beta=alpha|betainc;
                        omegar=mTwiddle_r[omega];
                        omegai=mTwiddle_i[omega];
                        xr=mTmp_r[alpha];xi=mTmp_i[alpha];
                        yr=mTmp_r[beta]; yi=mTmp_i[beta];

                        mulr=omegar*yr-omegai*yi;
                        muli=omegar*yi+omegai*yr;

                        mTmp_r[alpha]=(xr+mulr);mTmp_i[alpha]=(xi+muli);
                        mTmp_r[beta] =(xr-mulr);mTmp_i[beta] =(xi-muli);
                        omega += omegainc;
                        omega &= omegamask;
                }
        }
}
void SimpleFft::getResult(signed short *output,bool flipSpectrum)
{
	int i;
	if (flipSpectrum)
	{
		for (i=0;i<mFftSize/2;i++)
		{
			output[(i+mFftSize/2)*2+0]=(signed short)(mTmp_r[i]/(double)mSigmas);
			output[(i+mFftSize/2)*2+1]=(signed short)(mTmp_i[i]/(double)mSigmas);
			output[i*2+0]=(signed short)(mTmp_r[i+mFftSize/2]/(double)mSigmas);
			output[i*2+1]=(signed short)(mTmp_i[i+mFftSize/2]/(double)mSigmas);
		}
	} else {
		for (i=0;i<mFftSize;i++)
		{
			output[i*2+0]=(signed short)(mTmp_r[i]/(double)mSigmas);
			output[i*2+1]=(signed short)(mTmp_i[i]/(double)mSigmas);
		}
	}
}
void SimpleFft::getResult(double *output,bool flipSpectrum)
{
	int i;
	if (flipSpectrum)
	{
		for (i=0;i<mFftSize/2;i++)
		{
			output[(i+mFftSize/2)*2+0]=(double)(mTmp_r[i]/(double)mSigmas);
			output[(i+mFftSize/2)*2+1]=(double)(mTmp_i[i]/(double)mSigmas);
			output[i*2+0]=(double)(mTmp_r[i+mFftSize/2]/(double)mSigmas);
			output[i*2+1]=(double)(mTmp_i[i+mFftSize/2]/(double)mSigmas);
		}
	} else {
		for (i=0;i<mFftSize;i++)
		{
			output[i*2+0]=(double)(mTmp_r[i]/(double)mSigmas);
			output[i*2+1]=(double)(mTmp_i[i]/(double)mSigmas);
		}
	}

}
void SimpleFft::getSpectrum(double *output,bool flipSpectrum)
{
	int i;
	if (flipSpectrum)
	{
		for (i=0;i<mFftSize/2;i++)
		{
			output[i+mFftSize/2] =(double)(mTmp_r[i]*mTmp_r[i]);
			output[i+mFftSize/2]+=(double)(mTmp_i[i]*mTmp_i[i]);
			output[i] =(double)(mTmp_r[i+mFftSize/2]*mTmp_r[i+mFftSize/2]);
			output[i]+=(double)(mTmp_i[i+mFftSize/2]*mTmp_i[i+mFftSize/2]);
		}
	} else {
		for (i=0;i<mFftSize;i++)
		{
			output[i] =(double)(mTmp_r[i]*mTmp_r[i]);
			output[i]+=(double)(mTmp_i[i]*mTmp_i[i]);
		}
	}
}
void SimpleFft::addSpectrum(double *output,bool flipSpectrum)
{
	int i;
	if (flipSpectrum)
	{
		for (i=0;i<mFftSize/2;i++)
		{
			output[i+mFftSize/2]+=(double)(mTmp_r[i]*mTmp_r[i]);
			output[i+mFftSize/2]+=(double)(mTmp_i[i]*mTmp_i[i]);
			output[i]+=(double)(mTmp_r[i+mFftSize/2]*mTmp_r[i+mFftSize/2]);
			output[i]+=(double)(mTmp_i[i+mFftSize/2]*mTmp_i[i+mFftSize/2]);
		}
	} else {
		for (i=0;i<mFftSize;i++)
		{
			output[i]+=(double)(mTmp_r[i]*mTmp_r[i]);
			output[i]+=(double)(mTmp_i[i]*mTmp_i[i]);
		}
	}
}
int SimpleFft::getSize()
{
	return mFftSize;	
}
