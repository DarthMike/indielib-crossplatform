bin_PROGRAMS = manual

AM_CXXFLAGS = $(INTI_CFLAGS) -Werror -I @top_srcdir@/../common/include -I @top_srcdir@/../tests 

manual_SOURCES = ../../../tests/CIndieLib.cpp ../../../tests/WorkingPath.cpp ../../../tests/manual/ManualTestsMain.cpp ../../../tests/manual/animation/AnimationTests.cpp ../../../tests/manual/camera2d/Camera2dTests.cpp ../../../tests/manual/collision/CollisionTests.cpp ../../../tests/manual/font/FontTests.cpp ../../../tests/manual/font/FontTests_Pretty.cpp ../../../tests/manual/functionality/FunctionalityTests.cpp ../../../tests/manual/primitives/PrimitiveTests.cpp ../../../tests/manual/surface/SurfaceTests.cpp ../../../tests/manual/surface/SurfaceTests_textBlock.cpp ../../../tests/manual/surface/SurfaceTests_wrap.cpp ../../../tests/manual/image/INDImageTests_Conversions.cpp ../../../tests/manual/image/INDImageTests_Conversions_NonRGB.cpp ../../../tests/manual/image/INDImageTests_nonPOTLoad.cpp ../../../tests/manual/image/INDImageTests_POTLoad.cpp ../../../tests/manual/entities/EntityTests_animateRotations.cpp ../../../tests/manual/entities/EntityTests_animateScale.cpp ../../../tests/manual/entities/EntityTests_interactions.cpp ../../../tests/manual/surface/SurfaceTests_Filters.cpp ../../../tests/manual/surface/SurfaceTests_layout.cpp

manual_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
