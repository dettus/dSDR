#!/bin/sh

g++ -g -c -o SimpleFft.o SimpleFft.cpp
g++ -g -c -o testsuite.o testsuite.cpp
g++ -o testsuite.app testsuite.o SimpleFft.o
