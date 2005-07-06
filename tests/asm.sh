# test for assembly
set -e 

case $(uname -m) in
    ppc)
	${srcdir}/tests/asm-ppc.S
	;;
    i?86)
	${srcdir}/tests/asm-x86.S
	;;
    *)
	echo "Not supported";;
esac

    
