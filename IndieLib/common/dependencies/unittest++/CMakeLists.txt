 ################################### The zlib License #####################################
 #
 # Copyright (c) 2013 Indielib-crossplatform Development Team
 #
 # This software is provided 'as-is', without any express or implied
 # warranty. In no event will the authors be held liable for any damages
 # arising from the use of this software.
 #
 # Permission is granted to anyone to use this software for any purpose,
 # including commercial applications, and to alter it and redistribute it
 # freely, subject to the following restrictions:
 #
 # 1. The origin of this software must not be misrepresented; you must not
 # claim that you wrote the original software. If you use this software
 # in a product, an acknowledgment in the product documentation would be
 # appreciated but is not required.
 #
 # 2. Altered source versions must be plainly marked as such, and must not be
 # misrepresented as being the original software.
 #
 # 3. This notice may not be removed or altered from any source
 # distribution.
 #
 ##########################################################################################

set ( SRC	src/AssertException.cpp
			src/Checks.cpp
			src/CurrentTest.cpp
			src/DeferredTestReporter.cpp
			src/DeferredTestResult.cpp
			src/MemoryOutStream.cpp
			src/ReportAssert.cpp
			src/Test.cpp
			src/TestDetails.cpp
			src/TestList.cpp
			src/TestReporter.cpp
			src/TestResults.cpp
			src/TestRunner.cpp
			src/TimeConstraint.cpp
			src/XmlTestReporter.cpp
	)

file ( GLOB INC src/*.h src/*.hpp )

if ( WIN32 )
	set ( SRC ${SRC} src/win32/TimeHelpers.cpp )
	set ( INC ${INC} src/win32/TimeHelpers.h )
else ( WIN32 )
	set ( SRC ${SRC} src/Posix/TimeHelpers.cpp src/Posix/SignalTranslator.cpp )
	set ( INC ${INC} src/Posix/TimeHelpers.h src/Posix/SignalTranslator.cpp )
endif ( WIN32 )

option( UnitTest++_BUILD_STATIC "Build UnitTest++ as a static library. If not, then it is built as a shared library" YES )

if (UnitTest++_BUILD_STATIC)
	set ( UnitTest++_BUILD_TYPE STATIC )
else (UnitTest++_BUILD_STATIC)
	set ( UnitTest++_BUILD_TYPE SHARED )
endif (UnitTest++_BUILD_STATIC)

add_library(UnitTest++ ${UnitTest++_BUILD_TYPE} ${SRC} ${INC})
