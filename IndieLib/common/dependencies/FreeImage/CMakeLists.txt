/*********************************** The zlib License ************************************
 *
 * Copyright (c) 2013 Indielib-crossplatform Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 *****************************************************************************************/

file( GLOB SRC	Source/DeprecationManager/*.cpp
				Source/FreeImage/*.cpp
				Source/FreeImageLib/*.cpp
				Source/FreeImageToolkit/*.cpp
				Source/LibOpenJPEG/*.cpp
				Source/LibPNG/*.cpp
				Source/LibRawLite/*.cpp
				Source/Metadata/*.cpp
				Source/OpenEXR/*.cpp
				Source/ZLib/*.cpp
				Source/DeprecationManager/*.c
				Source/FreeImage/*.c
				Source/FreeImageLib/*.c
				Source/FreeImageToolkit/*.c
				Source/LibOpenJPEG/*.c
				Source/LibPNG/*.c
				Source/LibRawLite/*.c
				Source/Metadata/*.c
				Source/OpenEXR/*.c
				Source/ZLib/*.c
	)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Source
					 ${CMAKE_CURRENT_SOURCE_DIR}/Source/OpenEXR/IlmImf
					 ${CMAKE_CURRENT_SOURCE_DIR}/Source/OpenEXR/Imath
					 ${CMAKE_CURRENT_SOURCE_DIR}/Source/OpenEXR/IlmThread
					 ${CMAKE_CURRENT_SOURCE_DIR}/Source/OpenEXR/Iex
					 ${CMAKE_CURRENT_SOURCE_DIR}/Source/OpenEXR/Half
					)

add_definitions("-fPIC")

add_library(FreeImage STATIC ${SRC})
