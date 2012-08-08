bin_PROGRAMS = tutorial1

AM_CXXFLAGS = $(INTI_CFLAGS) -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/benchmark/01_Alien_BenchMark

tutorial1_SOURCES = ../../../../tutorials/benchmark/01_Alien_BenchMark/CIndieLib_vc2008.cpp ../../../../tutorials/benchmark/01_Alien_BenchMark/CUfo.cpp ../../../../tutorials/benchmark/01_Alien_BenchMark/CTerrain.cpp ../../../../tutorials/benchmark/01_Alien_BenchMark/Tutorial01.cpp

tutorial1_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL -lGLEW -lGLU -lGL -lILU -lILUT
