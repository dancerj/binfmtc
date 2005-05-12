[ "$( ${srcdir}/tests/print.c 1 2 3)" = "\
argc: 4
argv[0]: ${srcdir}/tests/print.c
argv[1]: 1
argv[2]: 2
argv[3]: 3" ];
