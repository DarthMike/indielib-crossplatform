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

cmake_minimum_required (VERSION 2.6)
project(IndieLib)

option(USE_OPENGLES "Compile with OpenGL ES support" NO)

set ( SRC	${CMAKE_CURRENT_SOURCE_DIR}/common/src/CollisionParser.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/DebugApi.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/FreeImageHelper.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/Global.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/ImageCutter.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Animation.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_AnimationManager.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Camera2d.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Entity2d.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Entity2dManager.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_FontManager.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IndieLib.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IndieVersion.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Image.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_ImageManager.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Input.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Light.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_LightManager.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Math.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Render.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_SpriterEntity.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_SpriterManager.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Surface.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_SurfaceManager.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Timer.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_TmxMap.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_TmxMapManager.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Window.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/common/src/PrecissionTimer.cpp
	)

# Include headers don't need to be added to the compilation source
#  but some IDEs will only show them if they are, so I will GLOB them
file ( GLOB INC common/include/*.h common/include/*.hpp common/src/*.h common/src/*.hpp )

set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules" )

option (USE_OPENGL "Use OpenGL for rendering." ON)

if (USE_OPENGL)
	find_package(OpenGL REQUIRED)
	find_package(GLEW REQUIRED)

	set ( SRC	${SRC}
				${CMAKE_CURRENT_SOURCE_DIR}/common/src/render/opengl/OpenGLRender.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/common/src/render/opengl/OpenGLTextureBuilder.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/common/src/render/opengl/RenderCullingOpenGL.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/common/src/render/opengl/RenderObject2dOpenGL.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/common/src/render/opengl/RenderObject3dOpenGL.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/common/src/render/opengl/RenderPrimitive2dOpenGL.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/common/src/render/opengl/RenderText2dOpenGL.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/common/src/render/opengl/RenderTransform2dOpenGL.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/common/src/render/opengl/RenderTransform3dOpenGL.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/common/src/render/opengl/RenderTransformCommonOpenGL.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/common/src/platform/OSOpenGLManager.cpp
		)

endif (USE_OPENGL)

set (SDL_SHARED OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/SDL-2.0)
set (SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/SDL-2.0/include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/unittest++)
set ( UnitTest++_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/unittest++/src )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/tinyxml)
set ( TINYXML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/tinyxml)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/FreeImage)
set ( FreeImage_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/FreeImage/Source )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/TmxParser)
set ( TmxParser_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/TmxParser)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/glew-1.9.0)
set ( GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/glew-1.9.0/include)

if (USE_OPENGLES)
	find_package(OpenGLES)

	if(NOT OpenGLES_FOUND)
		set (OpenGLES_INCLUDE_DIR "common/dependencies/OpenGL ES/GLES/v1.1")
	endif(NOT OpenGLES_FOUND)
endif(USE_OPENGLES)

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/common/include"
					 "${CMAKE_CURRENT_SOURCE_DIR}/common/src"
					 "${CMAKE_CURRENT_SOURCE_DIR}/common"
					 SDL2_INCLUDE_DIR
					 FreeImage_INCLUDE_DIR
					 GLEW_INCLUDE_DIR
					 OpenGLES_INCLUDE_DIR
					 TmxParser_INCLUDE_DIR
					 UnitTest++_INCLUDE_DIR
					 TINYXML_INCLUDE_DIR
					)

add_subdirectory(common/dependencies/SpriterParser)

option(IndieLib_BUILD_STATIC "Build IndieLib as a static library. Otherwise it is built as a shared library." OFF)

if (IndieLib_BUILD_STATIC)
	set (IndieLib_BUILD_TYPE STATIC)
else (IndieLib_BUILD_STATIC)
	set (IndieLib_BUILD_TYPE SHARED)
endif (IndieLib_BUILD_STATIC)

add_definitions("-fPIC")

add_library( IndieLib ${IndieLib_BUILD_TYPE} ${SRC} ${INC} )

target_link_libraries( IndieLib SpriterParser TinyXML GLEW SDL2 FreeImage TmxParser UnitTest++  )
