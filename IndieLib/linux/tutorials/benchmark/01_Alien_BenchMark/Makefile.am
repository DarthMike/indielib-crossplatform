bin_PROGRAMS = tutorial1

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/benchmark/01_Alien_BenchMark -I @top_srcdir@/../tutorials

tutorial1_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/benchmark/01_Alien_BenchMark/CIndieLib_vc2008.cpp ../../../../tutorials/benchmark/01_Alien_BenchMark/CUfo.cpp ../../../../tutorials/benchmark/01_Alien_BenchMark/CTerrain.cpp ../../../../tutorials/benchmark/01_Alien_BenchMark/Tutorial01.cpp

tutorial1_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
