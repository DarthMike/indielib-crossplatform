bin_PROGRAMS = tutorial06

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/06_Primitives -I @top_srcdir@/../tutorials

tutorial06_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/basic/06_Primitives/CIndieLib_vc2008.cpp ../../../../tutorials/basic/06_Primitives/Tutorial06.cpp

tutorial06_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
