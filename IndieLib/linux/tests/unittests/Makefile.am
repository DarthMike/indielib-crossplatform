bin_PROGRAMS = unittest

AM_CXXFLAGS = $(INTI_CFLAGS) -I @top_srcdir@/../common -I @top_srcdir@/../common/include -I @top_srcdir@/../tests 

unittest_SOURCES = ../../../tests/CIndieLib_vc2008.cpp  ../../../tests/WorkingPath.cpp ../../../common/dependencies/unittest++/src/TestRunner.cpp ../../../common/dependencies/unittest++/src/Test.cpp ../../../common/dependencies/unittest++/src/TestResults.cpp ../../../common/dependencies/unittest++/src/TestDetails.cpp ../../../common/dependencies/unittest++/src/CurrentTest.cpp ../../../common/dependencies/unittest++/src/TestList.cpp ../../../common/dependencies/unittest++/src/TestReporter.cpp ../../../common/dependencies/unittest++/src/TestReporterStdout.cpp ../../../common/dependencies/unittest++/src/Posix/SignalTranslator.cpp ../../../common/dependencies/unittest++/src/Posix/TimeHelpers.cpp ../../../common/dependencies/unittest++/src/AssertException.cpp ../../../common/dependencies/unittest++/src/MemoryOutStream.cpp ../../../tests/unittests/Collisions.cpp ../../../tests/unittests/Image.cpp ../../../tests/unittests/ImageManager.cpp ../../../tests/unittests/Math.cpp ../../../tests/unittests/UnitTests.cpp ../../../tests/unittests/Vector2.cpp

unittest_LDADD = -L@top_srcdir@/.libs $(INTI_LIBS) -lIndieLib -lSDL2 -lGLEW -lGLU -lGL
