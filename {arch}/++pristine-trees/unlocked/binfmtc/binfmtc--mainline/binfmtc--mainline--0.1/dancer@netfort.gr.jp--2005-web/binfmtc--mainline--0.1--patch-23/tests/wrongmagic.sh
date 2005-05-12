# make sure that wrong magic number will fail.
if ./binfmtc-interpreter ${srcdir}/tests/wrongmagic.c; then
    exit 1;
else
    exit 0;
fi
