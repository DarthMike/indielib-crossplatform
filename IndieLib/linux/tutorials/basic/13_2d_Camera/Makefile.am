bin_PROGRAMS = tutorial13

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/13_2d_Camera -I @top_srcdir@/../tutorials

tutorial13_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/basic/13_2d_Camera/CIndieLib_vc2008.cpp ../../../../tutorials/basic/13_2d_Camera/Tutorial13.cpp

tutorial13_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
