# test for assembly
set -e 

case $(uname -m) in
    ppc)
	${srcdir}/tests/asm-ppc.S
	;;
    i?86)
	${srcdir}/tests/asm-x86.S
	;;
    x86_64)
	${srcdir}/tests/asm-x86_64.S
	;;
    *)
	echo "Not supported";;
esac

    
