# test for assembly
set -e 

case $(uname -m) in
    ppc)
	${srcdir}/tests/asm-ppc.S
	;;
    *)
	echo "Not supported";;
esac

    
