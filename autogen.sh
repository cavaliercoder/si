#!/bin/sh
case `uname -s` in
	"Darwin")
		alias libtoolize=glibtoolize
		;;
esac

libtoolize -c -f || exit 1
#aclocal --force --verbose -I m4 || exit 1
autoreconf -ifv || exit 1
autoheader -fv || exit 1
automake -acfv || exit 1
