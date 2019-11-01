#!/bin/sh

clang -O3 -g -o filtertest.app filtertest.c -lm
clang -O3 -g -o decimation.app decimation.c -lm

