bin_PROGRAMS=binfmtc-interpreter \
	binfmtf-interpreter \
	binfmtf95-interpreter \
	binfmtcxx-interpreter \
	binfmtasm-interpreter \
	binfmtgcj-interpreter

binfmtcxx_interpreter_SOURCES=binfmtc-interpreter.c binfmtc-lang-cxx.c binfmtc.h
binfmtc_interpreter_SOURCES=binfmtc-interpreter.c binfmtc-lang-c.c binfmtc.h
binfmtf_interpreter_SOURCES=binfmtc-interpreter.c binfmtc-lang-f.c binfmtc.h
binfmtf95_interpreter_SOURCES=binfmtc-interpreter.c binfmtc-lang-f95.c binfmtc.h
binfmtasm_interpreter_SOURCES=binfmtc-interpreter.c binfmtc-lang-asm.c binfmtc.h
binfmtgcj_interpreter_SOURCES=binfmtc-interpreter.c binfmtc-lang-gcj.c binfmtc.h
dist_bin_SCRIPTS= realcsh.c realcxxsh.cc
dist_sbin_SCRIPTS= realksh.c 

TESTS= tests/prerequisite.sh \
	tests/print.sh \
	tests/wrongmagic.sh \
	tests/multisource.sh \
	tests/wrongcode.sh \
	tests/cxxprint.sh \
	tests/java.sh \
	tests/fortran.sh \
	tests/space.sh \
	tests/asm.sh \
	tests/realksh.sh \
	tests/realcsh.sh \
	tests/news-okay.sh

dist_man_MANS= binfmtc-interpreter.1 binfmtcxx-interpreter.1 realcsh.c.1 binfmtasm-interpreter.1 \
	binfmtf-interpreter.1 \
	binfmtf95-interpreter.1 \
	binfmtgcj-interpreter.1 \
	realksh.c.8 \
	realcxxsh.cc.1

TESTS_ENVIRONMENT=bindir=$(bindir) \
	LC_ALL=C \
	LANG=C \
	VERSION=@VERSION@ 

EXTRA_DIST= autogen.sh binfmtc-init.sh binfmtc-unreg.sh \
	$(TESTS) \
	tests/multisource.c \
	tests/multisource1.c \
	tests/print.c \
	tests/wrongcode.c \
	tests/wrongmagic.c \
	tests/asm-ppc.S \
	tests/asm-x86.S \
	tests/asm-x86_64.S \
	tests/cxxprint.cxx  \
	tests/javatest.java  \
	tests/fortran.f  \
	upaccho2/upaccho2.c \
	upaccho2/upaccho2-webservice.c \
	upaccho2/upaccho2-webservice.h

install-exec-local:
	-mkdir -p ${DESTDIR}${sbindir}
	sed -e "s,[@]bindir[@],@bindir@,g" < ${srcdir}/binfmtc-unreg.sh > ${DESTDIR}${sbindir}/binfmtc-unreg.sh
	sed -e "s,[@]bindir[@],@bindir@,g" < ${srcdir}/binfmtc-init.sh > ${DESTDIR}${sbindir}/binfmtc-init.sh
	chmod 755 ${DESTDIR}${sbindir}/binfmtc-unreg.sh
	chmod 755 ${DESTDIR}${sbindir}/binfmtc-init.sh

uninstall-local:
	rm -f ${DESTDIR}${sbindir}/binfmtc-unreg.sh ${DESTDIR}${sbindir}/binfmtc-init.sh

# Make debian pristine-source
../@PACKAGE@_@VERSION@.orig.tar.gz: 
	if [ ! -f $@ ]; then $(MAKE) distcheck && dch -v "@VERSION@-1" "New upstream version" && mv @PACKAGE@-@VERSION@.tar.gz ../@PACKAGE@_@VERSION@.orig.tar.gz; \
	else \
	rm -f @PACKAGE@-@VERSION@.tar.gz; \
	fi

debiandist: ../@PACKAGE@_@VERSION@.orig.tar.gz

viper: distcheck
	scp @PACKAGE@-@VERSION@.tar.gz aegis.netfort.gr.jp:public_html/software/downloads
	rm -f @PACKAGE@-@VERSION@.tar.gz

upload-dist-all: distcheck
	scp @PACKAGE@-@VERSION@.tar.gz aegis.netfort.gr.jp:public_html/software/downloads
	$(MAKE) ../@PACKAGE@_@VERSION@.orig.tar.gz

check-syntax:
	$(CC) -c -O2 -Wall $(CHK_SOURCES)  -o/dev/null -I/usr/include/readline
