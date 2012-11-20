bin_PROGRAMS = tutorial2

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/advanced/02_Blitting_2d_Directly -I @top_srcdir@/../tutorials

tutorial2_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/advanced/02_Blitting_2d_Directly/CIndieLib_vc2008.cpp ../../../../tutorials/advanced/02_Blitting_2d_Directly/Tutorial02.cpp

tutorial2_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
