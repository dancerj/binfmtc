#!/bin/bash
# script to initialize binfmt_misc handling, run it as root.
set -e
trap "echo Error!" ERR 
[ $(id -u) = 0 ];
test -e @bindir@/binfmtc-interpreter
test -e @bindir@/binfmtcxx-interpreter
test -e @bindir@/binfmtasm-interpreter
test -e @bindir@/binfmtf-interpreter
test -e @bindir@/binfmtp-interpreter
test -e @bindir@/binfmtgcj-interpreter
mount binfmt_misc -t binfmt_misc /proc/sys/fs/binfmt_misc || true
echo ":ccompile:M::/*BINFMTC\\x3a::@bindir@/binfmtc-interpreter:" > /proc/sys/fs/binfmt_misc/register
echo ":cxxcompile:M::/*BINFMTCXX\\x3a::@bindir@/binfmtcxx-interpreter:" > /proc/sys/fs/binfmt_misc/register
echo ":asmassembly:M::/*BINFMTASMCPP\\x3a::@bindir@/binfmtasm-interpreter:" > /proc/sys/fs/binfmt_misc/register
echo ":gcjcompile:M:://BINFMTGCJ\\x3a::@bindir@/binfmtgcj-interpreter:" > /proc/sys/fs/binfmt_misc/register
echo ":fcompile:M::c\\x20BINFMTF\\x3a::@bindir@/binfmtf-interpreter:" > /proc/sys/fs/binfmt_misc/register
echo ":pcompile:M::{BINFMTP\\x3a::@bindir@/binfmtp-interpreter:" > /proc/sys/fs/binfmt_misc/register
echo SUCCESS
