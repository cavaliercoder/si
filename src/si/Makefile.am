## Process this file with automake to produce Makefile.in

#ifeq ($(shell uname -s),Darwin)
DARWIN_CFLAGS = -framework CoreServices -framework IOKit
#endif

bin_PROGRAMS = si

si_SOURCES = \
	main.c

si_CFLAGS = -I $(top_srcdir)/libs/doctree/include $(DARWIN_CFLAGS)

si_LDADD = \
	$(top_srcdir)/src/libs/smbios/libsmbios.a \
	$(top_srcdir)/src/libs/doctree/src/libdoctree.a
