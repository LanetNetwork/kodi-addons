noinst_LTLIBRARIES = libjsoncpp.la

libjsoncpp_la_SOURCES = json_reader.cpp \
                        json_value.cpp \
                        json_writer.cpp

AM_CPPFLAGS=-I../../include/

$(LIB): libjsoncpp.la
	cp -f .libs/libjsoncpp.a .
	cp -f .libs/libjsoncpp.la $(LIB)

