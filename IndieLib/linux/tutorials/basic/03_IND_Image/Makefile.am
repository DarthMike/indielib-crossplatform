bin_PROGRAMS = tutorial03

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/03_IND_Image

tutorial03_SOURCES = ../../../../tutorials/basic/03_IND_Image/CIndieLib_vc2008.cpp ../../../../tutorials/basic/03_IND_Image/Tutorial03.cpp

tutorial03_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
