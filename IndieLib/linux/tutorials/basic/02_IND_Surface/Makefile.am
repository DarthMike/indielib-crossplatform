bin_PROGRAMS = tutorial02

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/02_IND_Surface -I @top_srcdir@/../tutorials

tutorial02_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/basic/02_IND_Surface/CIndieLib.cpp ../../../../tutorials/basic/02_IND_Surface/Tutorial02.cpp

tutorial02_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
