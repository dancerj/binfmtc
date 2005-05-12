#!/bin/bash
# script to initialize binfmt_misc handling, run it as root.
set -e
trap "echo Error!" ERR 
[ $(id -u) = 0 ];
test -e ./binfmtc-interpreter
test -e ./binfmtcxx-interpreter
test -e ./binfmtasm-interpreter
mount binfmt_misc -t binfmt_misc /proc/sys/fs/binfmt_misc || true
echo ":ccompile:M::/*BINFMTC\\x3a::$(pwd)/binfmtc-interpreter:" > /proc/sys/fs/binfmt_misc/register
echo ":cxxcompile:M::/*BINFMTCXX\\x3a::$(pwd)/binfmtcxx-interpreter:" > /proc/sys/fs/binfmt_misc/register
echo ":asmassembly:M::/*BINFMTASMCPP\\x3a::$(pwd)/binfmtasm-interpreter:" > /proc/sys/fs/binfmt_misc/register
echo SUCCESS
