LIBS   = ../lib_json/libjsoncpp.la
LIBDIR = ../../addons/pvr.lanettv
LIB    = ../../addons/pvr.lanettv/$(ADDONNAME)

lib_LTLIBRARIES = liblanettvaddon.la

liblanettvaddon_la_SOURCES = \
	client.cpp \
	client.h \
	PVRLanetTVData.cpp \
	PVRLanetTVData.h
	
liblanettvaddon_la_LDFLAGS = \
	-avoid-version

liblanettvaddon_la_LIBADD = \
	$(ZLIB_LIBS)

AM_CPPFLAGS = \
	$(ARCH_DEFINES) \
	-I. \
	-I$(top_srcdir)/include -I$(top_srcdir)/include/kodi -I$(top_srcdir)/src \
	-DUSE_DEMUX

EXTRA_DIST = \
	include

$(ADDONNAME): liblanettvaddon.la
	cp -f .libs/liblanettvaddon.$(SOEXT) $(ADDONNAME)

$(LIB): $(ADDONNAME)
	cp -f $(ADDONNAME) $(LIB)

all: $(LIB)

clean-local:
	-rm -f $(LIB) $(ADDONNAME)

install:
