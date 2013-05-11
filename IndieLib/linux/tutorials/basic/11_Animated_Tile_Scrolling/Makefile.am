bin_PROGRAMS = tutorial11

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tutorials/basic/11_Animated_Tile_Scrolling -I @top_srcdir@/../tutorials

tutorial11_SOURCES = ../../../../tutorials/WorkingPath.cpp  ../../../../tutorials/basic/11_Animated_Tile_Scrolling/CIndieLib.cpp ../../../../tutorials/basic/11_Animated_Tile_Scrolling/Tutorial11.cpp

tutorial11_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
