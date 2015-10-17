## Process this file with automake to produce Makefile.in


bin_PROGRAMS = si

si_SOURCES = \
	main.c

si_CFLAGS = -I $(top_srcdir)/src/libs/doctree/include $(DARWIN_CFLAGS)

si_LDADD = \
	$(top_srcdir)/src/libs/smbios/libsmbios.a \
	$(top_srcdir)/src/libs/doctree/src/libdoctree.a
