#!/bin/bash
# test real-ksh functionality

# requires: sudo, and a working kernel build directory enough to build
# a kernel module, with an interface similar to linux 2.6.14.

echo 'printk("Hello world\n");' | sudo ${srcdir}/realksh.c | grep ^KMSG:
