#!/bin/bash
# test real-csh functionality

# basic functionality test to check that most things
# function. Excercise printf and atoi.

echo 'printf("\n%i Hello world\n", atoi("1024"));' | \
    ${srcdir}/realcsh.c | \
    grep "^1024 Hello world"

