# RBDL - Rigid Body Dynamics Library
# Copyright (c) 2011-2012 Martin Felis <martin.felis@iwr.uni-heidelberg.de>
#
# (zlib license)
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
#    1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
#
#    2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
#
#    3. This notice may not be removed or altered from any source
#   distribution.

# File copied from https://raw.github.com/erwincoumans/rbdl/master/CMake/FindUnitTest++.cmake

# - Try to find UnitTest++
#
#

SET (UNITTEST++_FOUND FALSE)

FIND_PATH (UNITTEST++_INCLUDE_DIR UnitTest++.h
	/usr/include/unittest++ 
	/usr/local/include/unittest++ 
	/opt/local/include/unittest++
	$ENV{UNITTESTXX_PATH}/src 
	$ENV{UNITTESTXX_INCLUDE_PATH}
	)

FIND_LIBRARY (UNITTEST++_LIBRARY NAMES UnitTest++ PATHS 
	/usr/lib 
	/usr/local/lib 
	/opt/local/lib 
	$ENV{UNITTESTXX_PATH} 
	ENV{UNITTESTXX_LIBRARY_PATH}
	)

IF (UNITTEST++_INCLUDE_DIR AND UNITTEST++_LIBRARY)
	SET (UNITTEST++_FOUND TRUE)
ENDIF (UNITTEST++_INCLUDE_DIR AND UNITTEST++_LIBRARY)

IF (UNITTEST++_FOUND)
   IF (NOT UnitTest++_FIND_QUIETLY)
      MESSAGE(STATUS "Found UnitTest++: ${UNITTEST++_LIBRARY}")
   ENDIF (NOT UnitTest++_FIND_QUIETLY)
ELSE (UNITTEST++_FOUND)
   IF (UnitTest++_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find UnitTest++")
   ENDIF (UnitTest++_FIND_REQUIRED)
ENDIF (UNITTEST++_FOUND)

MARK_AS_ADVANCED (
	UNITTEST++_INCLUDE_DIR
	UNITTEST++_LIBRARY
	)
