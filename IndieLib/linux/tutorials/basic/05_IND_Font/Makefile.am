bin_PROGRAMS = tutorial05

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/05_IND_Font -I @top_srcdir@/../tutorials

tutorial05_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/basic/05_IND_Font/CIndieLib.cpp ../../../../tutorials/basic/05_IND_Font/Tutorial05.cpp

tutorial05_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
