bin_PROGRAMS = tutorial2

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/benchmark/02_Rabbits_BenchMark -I @top_srcdir@/../tutorials

tutorial2_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/benchmark/02_Rabbits_BenchMark/CIndieLib.cpp ../../../../tutorials/benchmark/02_Rabbits_BenchMark/CRabbit.cpp ../../../../tutorials/benchmark/02_Rabbits_BenchMark/Tutorial02.cpp

tutorial2_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
