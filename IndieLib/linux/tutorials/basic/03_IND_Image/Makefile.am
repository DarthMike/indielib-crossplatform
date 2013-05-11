bin_PROGRAMS = tutorial03

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/03_IND_Image -I @top_srcdir@/../tutorials

tutorial03_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/basic/03_IND_Image/CIndieLib.cpp ../../../../tutorials/basic/03_IND_Image/Tutorial03.cpp

tutorial03_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
