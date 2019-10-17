#!/bin/sh

g++ -g -c -o SimpleFft.o SimpleFft.cpp -I../central
g++ -g -c -o SimpleShifter.o SimpleShifter.cpp -I../central
g++ -g -c -o testsuite.o testsuite.cpp -I../central
g++ -o testsuite.app testsuite.o SimpleFft.o SimpleShifter.o
