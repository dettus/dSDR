#!/bin/sh

CC=gcc
$CC -O3 -g -o shifter.app shifter.c -lm
$CC -O3 -g -o filtertest.app filtertest.c -lm
$CC -O3 -g -o decimation.app decimation.c -lm

