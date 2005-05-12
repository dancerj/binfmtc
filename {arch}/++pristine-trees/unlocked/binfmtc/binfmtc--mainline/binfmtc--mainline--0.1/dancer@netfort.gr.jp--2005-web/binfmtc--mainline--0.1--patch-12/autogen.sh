#!/bin/bash
# autogenerate script

aclocal && autoheader && automake --foreign -a -c && autoconf

