#!/bin/bash
# test real-ksh functionality

# requires: sudo, and a working kernel build directory enough to build
# a kernel module, with an interface similar to linux 2.6.14-

if [ -d /lib/modules/$(uname -r)/build ]; then
    echo 'printk("Hello world\n");' | sudo ${srcdir}/realksh.c | grep ^KMSG:
else
    # ignore this test if it's within a chroot, or without access to kernel tree.
    exit 77
fi
