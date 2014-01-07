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

set ( SRC	TmxImage.cpp
			TmxLayer.cpp
			TmxMap.cpp
			TmxObject.cpp
			TmxObjectGroup.cpp
			TmxPolygon.cpp
			TmxPolyline.cpp
			TmxPropertySet.cpp
			TmxTile.cpp
			TmxTileset.cpp
			TmxUtil.cpp
			base64/base64.cpp
	)

file ( GLOB INC *.h *.hpp base64/*.h base64/*.hpp )

include_directories(${CMAKE_SOURCE_DIR}/common)

add_definitions("-fPIC")

add_library(TmxParser STATIC ${SRC} ${INC})
