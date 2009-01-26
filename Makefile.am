
ALL_LINGUAS =
always_built_SUBDIRS = src

sometimes_built_SUBDIRS = 

SUBDIRS = doc po intl $(always_built_SUBDIRS)

EXTRA_DIST = TODO INSTALL KOPIOWANIE COPYING AUTHORS


## to automatically rebuild aclocal.m4 if any of the macros in `macros/' change
@MAINT@include macros/macros.dep
@MAINT@macros/macros.dep: macros/Makefile.am
@MAINT@	cd macros && $(MAKE) macros.dep

## make rpms
rpm: Makefile
	$(MAKE) dist
	rpm -ta --clean $(PACKAGE)-$(VERSION).tar.gz
		
