bin_PROGRAMS = tutorial2

AM_CXXFLAGS = $(INTI_CFLAGS) -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/benchmark/02_Rabbits_BenchMark

tutorial2_SOURCES = ../../../../tutorials/benchmark/02_Rabbits_BenchMark/CIndieLib_vc2008.cpp ../../../../tutorials/benchmark/02_Rabbits_BenchMark/CRabbit.cpp ../../../../tutorials/benchmark/02_Rabbits_BenchMark/Tutorial02.cpp

tutorial2_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
