bin_PROGRAMS = tutorial4

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/advanced/04_Several_ViewPorts -I @top_srcdir@/../tutorials

tutorial4_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/advanced/04_Several_ViewPorts/CIndieLib.cpp ../../../../tutorials/advanced/04_Several_ViewPorts/Tutorial04.cpp

tutorial4_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
