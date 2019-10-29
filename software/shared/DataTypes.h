// the purpose of this file is to provide the typedefs,
// that are meant to be shared between the classes.
//
// $Id$

#ifndef	DATATYPES_H
#define	DATATYPES_H

// in case a new type of input tuner is being implemented, it should be
// registered here.
enum eTunerType
{
	TUNER_UNDEF=-1,TUNER_QUIT=0,	// special tuner types

	TUNER_DUMMY=1,			// the example tuner
	TUNER_RTLTCP=2			// the realtek tuner
};
typedef struct _tSComplex
{
	signed short real;
	signed short imag;
} tSComplex;
typedef struct _tIQSamplesBlock
{
	tSComplex* pData;
	int sampleNum;
	int sampleRate;
	int centerFreq;
} tIQSamplesBlock;

#endif
