bin_PROGRAMS = tutorial04

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/04_IND_Animation  -I @top_srcdir@/../tutorials

tutorial04_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/basic/04_IND_Animation/CIndieLib_vc2008.cpp ../../../../tutorials/basic/04_IND_Animation/Tutorial04.cpp

tutorial04_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
