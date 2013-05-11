bin_PROGRAMS = tutorial01

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/01_Installing -I @top_srcdir@/../tutorials

tutorial01_SOURCES = ../../../../tutorials/WorkingPath.cpp ../../../../tutorials/basic/01_Installing/CIndieLib.cpp ../../../../tutorials/basic/01_Installing/Tutorial01.cpp

tutorial01_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
