bin_PROGRAMS = tutorial08

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/08_Collisions -I @top_srcdir@/../tutorials

tutorial08_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/basic/08_Collisions/CIndieLib_vc2008.cpp ../../../../tutorials/basic/08_Collisions/Tutorial08.cpp

tutorial08_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
