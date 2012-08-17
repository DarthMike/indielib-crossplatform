bin_PROGRAMS = tutorial02

AM_CXXFLAGS = $(INTI_CFLAGS) -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/02_IND_Surface 

tutorial02_SOURCES = ../../../../tutorials/basic/02_IND_Surface/CIndieLib_vc2008.cpp ../../../../tutorials/basic/02_IND_Surface/Tutorial02.cpp

tutorial02_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
