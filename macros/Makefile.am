## Please update this variable if any new macros are created
MACROS=	aclocal-include.m4 \
	codeset.m4 \
	compiler-flags.m4 \
	curses.m4 \
	gettext.m4 \
	glibc21.m4 \
	iconv.m4 \
	isc-posix.m4 \
	lcmessage.m4 \
	progtest.m4 \
	readline.m4

EXTRA_DIST=$(MACROS) autogen.sh
MAINTAINERCLEANFILES=macros.dep

@MAINT@macros.dep: Makefile.am
@MAINT@	@echo '$$(top_srcdir)/aclocal.m4: $(MACROS:%=macros/%)' > $@
