AC_INIT([sysinv], [1.0.0], [ryan@cavaliercoder.com])
AM_INIT_AUTOMAKE([-Wall -Werror -Wno-portability foreign])

AC_PROG_CC
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	src/libs/Makefile
	src/libs/smbios/Makefile
	src/si/Makefile
])
AC_CONFIG_SUBDIRS([src/libs/doctree])

AM_CONDITIONAL([DARWIN], [test $(uname -s) -eq "Darwin"])

#m4_pattern_allow([AM_PROG_AR])
AM_PROG_AR
LT_INIT


case $host_os in
	darwin*)
		DARWIN_CFLAGS="-framework CoreServices -framework IOKit"
		AC_SUBST(DARWIN_CFLAGS)
		;;
esac

AC_OUTPUT
