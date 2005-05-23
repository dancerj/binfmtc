bin_PROGRAMS=binfmtc-interpreter binfmtcxx-interpreter binfmtasm-interpreter binfmtgcj-interpreter
binfmtcxx_interpreter_SOURCES=binfmtc-interpreter.c binfmtc-lang-cxx.c binfmtc.h
binfmtc_interpreter_SOURCES=binfmtc-interpreter.c binfmtc-lang-c.c binfmtc.h
binfmtasm_interpreter_SOURCES=binfmtc-interpreter.c binfmtc-lang-asm.c binfmtc.h
binfmtgcj_interpreter_SOURCES=binfmtc-interpreter.c binfmtc-lang-gcj.c binfmtc.h
dist_bin_SCRIPTS= realcsh.c

TESTS= tests/prerequisite.sh \
	tests/print.sh \
	tests/wrongmagic.sh \
	tests/multisource.sh \
	tests/wrongcode.sh \
	tests/cxxprint.sh \
	tests/java.sh \
	tests/space.sh \
	tests/asm.sh \
	tests/news-okay.sh

dist_man_MANS= binfmtc-interpreter.1 binfmtcxx-interpreter.1 realcsh.1 binfmtasm-interpreter.1 \
	binfmtgcj-interpreter.1

TESTS_ENVIRONMENT=bindir=$(bindir) \
	LC_ALL=C \
	LANG=C \
	VERSION=@VERSION@ 

EXTRA_DIST= autogen.sh init.sh unreg.sh \
	$(TESTS) \
	tests/multisource.c \
	tests/multisource1.c \
	tests/print.c \
	tests/wrongcode.c \
	tests/wrongmagic.c \
	tests/asm-ppc.S \
	tests/cxxprint.cxx  \
	tests/javatest.java  \
	upaccho2/upaccho2.c \
	upaccho2/upaccho2-webservice.c \
	upaccho2/upaccho2-webservice.h

# Make debian pristine-source
../@PACKAGE@_@VERSION@.orig.tar.gz: 
	if [ ! -f $@ ]; then $(MAKE) distcheck && dch -v "@VERSION@-1" "New upstream version" && mv @PACKAGE@-@VERSION@.tar.gz ../@PACKAGE@_@VERSION@.orig.tar.gz; \
	else \
	rm -f @PACKAGE@-@VERSION@.tar.gz; \
	fi

debiandist: ../@PACKAGE@_@VERSION@.orig.tar.gz

viper: distcheck
	scp @PACKAGE@-@VERSION@.tar.gz viper2.netfort.gr.jp:public_html/software/downloads
	rm -f @PACKAGE@-@VERSION@.tar.gz

upload-dist-all: distcheck
	scp @PACKAGE@-@VERSION@.tar.gz viper2.netfort.gr.jp:public_html/software/downloads
	$(MAKE) ../@PACKAGE@_@VERSION@.orig.tar.gz

# tla check.
tree-check:
	-chmod u+w -R /tmp/tlatest/
	-rm -rf /tmp/tlatest
	-mkdir /tmp/tlatest 
	cd /tmp/tlatest && tla get $(shell tla tree-version) && cd binfmtc--mainline* && ./autogen.sh && ./configure && make distcheck
