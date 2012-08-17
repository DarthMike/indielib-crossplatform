bin_PROGRAMS = tutorial01

AM_CXXFLAGS = $(INTI_CFLAGS) -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/01_Installing 

tutorial01_SOURCES = ../../../../tutorials/basic/01_Installing/CIndieLib_vc2008.cpp ../../../../tutorials/basic/01_Installing/Tutorial01.cpp

tutorial01_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
