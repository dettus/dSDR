#ifndef	DATATYPES_H
#define	DATATYPES_H

typedef struct _tDComplex
{
	double real;
	double imag;
} tDComplex;

typedef struct _tSComplex
{
	signed short real;
	signed short imag;
} tSComplex;

typedef	struct _tSComplexBuf
{
	tSComplex* samples;
	int	size;
	int	fillcur;
	int	fillnext;
	int	used;
} tSComplexBuf;
#endif
