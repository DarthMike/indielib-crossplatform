bin_PROGRAMS = tutorial07

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/07_IND_Input -I @top_srcdir@/../tutorials

tutorial07_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/basic/07_IND_Input/CIndieLib_vc2008.cpp ../../../../tutorials/basic/07_IND_Input/Tutorial07.cpp

tutorial07_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
