## Process this file with automake to produce Makefile.in

@SET_MAKE@

bin_PROGRAMS = timeout

timeout_COMMON_SRC = timeout.c \
		timeout.h \
		signames.h \
		signals.c \
		signals.h \
		invokes.c \
		invokes.h \
		defaults.h \
		variables.c \
		variables.h \
		localestuff.h \
		initlocale.h

timeout_SIGNALH = /usr/include/signal.h

timeout_SIGBLOCK = ../compat/sigblock.c ../compat/sigblock.h

if HAVE_SIGBLOCK
timeout_SIGBLOCK_OBJ =
else 
timeout_SIGBLOCK_OBJ = ./compat/sigblock.o
endif 

timeout_SOURCES = $(timeout_COMMON_SRC)

timeout_LDADD = $(timeout_SIGBLOCK_OBJ) $(INTLLIBS)

timeout_DEPENDENCIES = $(timeout_SIGBLOCK_OBJ)\
		     $(INTLDEPS)


if HAVE_SIGBLOCK
else 
./compat/sigblock.o: $(timeout_SIGBLOCK)
endif 

signames.h: $(timeout_SIGNALH)
	/lib/cpp -dM </usr/include/signal.h |	\
	tr -s '\t ' '  ' | sort -n +2 | sed		\
	's:#define SIG\([A-Z]\+[0-9]*\) \([0-9]\+\) *\(\|/\*.*\)$$:{\
	\2,"\1" },:p;d'  > signames.h


EXTRA_DIST = $(timeout_SIGBLOCK) $(timeout_SIGNALH)

clean-local:
	rm -f signames.h
