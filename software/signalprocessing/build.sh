#!/bin/sh

CXX="g++"

$CXX -g -c -o testbench.o testbench.cpp -I../shared/
$CXX -g -c -o SimpleShifter.o SimpleShifter.cpp -I../shared/
$CXX -g -c -o Filter.o Filter.cpp -I../shared/
$CXX -g -c -o Downsampler.o Downsampler.cpp -I../shared/

$CXX -g -o testbench.app SimpleShifter.o Filter.o Downsampler.o testbench.o -lm

