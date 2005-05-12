# make sure that wrong code will fail
if ${srcdir}/tests/wrongcode.c; then
    exit 1;
else
    exit 0;
fi
