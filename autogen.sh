#!/bin/sh
# Run this to generate all the initial makefiles, etc.

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

PKG_NAME="timeout"

(test -f $srcdir/configure.in \
  && test -d $srcdir/src \
   && test -d $srcdir/po \
  && test -d $srcdir/compat) || {
    echo -n "**Error**: Directory "\`$srcdir\'" does not look like the"
    echo " top-level $PKG_NAME directory"

    exit 1
}

. $srcdir/macros/autogen.sh
