#!/bin/bash
# script to initialize binfmt_misc handling, run it as root.
set -e
trap "echo Error!" ERR 
[ $(id -u) = 0 ];
echo -1 > /proc/sys/fs/binfmt_misc/ccompile
echo -1 > /proc/sys/fs/binfmt_misc/cxxcompile
echo -1 > /proc/sys/fs/binfmt_misc/asmassembly
echo -1 > /proc/sys/fs/binfmt_misc/fcompile
echo -1 > /proc/sys/fs/binfmt_misc/f95compile
echo -1 > /proc/sys/fs/binfmt_misc/gcjcompile
echo -1 > /proc/sys/fs/binfmt_misc/pcompile
echo SUCCESS

