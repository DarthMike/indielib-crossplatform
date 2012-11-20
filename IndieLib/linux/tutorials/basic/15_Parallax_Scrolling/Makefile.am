bin_PROGRAMS = tutorial15

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/15_Parallax_Scrolling -I @top_srcdir@/../tutorials

tutorial15_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/basic/15_Parallax_Scrolling/CIndieLib_vc2008.cpp ../../../../tutorials/basic/15_Parallax_Scrolling/Tutorial15.cpp

tutorial15_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
