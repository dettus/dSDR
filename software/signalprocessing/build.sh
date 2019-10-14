#!/bin/sh

g++ -O3 -c -o SimpleFft.o SimpleFft.cpp
g++ -O3 -c -o testsuite.o testsuite.cpp
g++ -o testsuite.app testsuite.o SimpleFft.o
