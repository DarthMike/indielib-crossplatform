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

cmake_minimum_required (VERSION 2.6)
project(IndieLib)

option(USE_OPENGLES "Compile with OpenGL ES support" NO)

option(IndieLib_BUILD_STATIC "Build IndieLib as a static library. Otherwise it is built as a shared library." ON)

option (USE_OPENGL "Use OpenGL for rendering." ON)


# this is only for testing the linux output dir (run cmake ../ -G "Unix Makefiles" in the "bin" dir)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/linux)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/linux)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/linux)
# end testing linux output dir


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
			#${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_Light.cpp
			#${CMAKE_CURRENT_SOURCE_DIR}/common/src/IND_LightManager.cpp
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
					 freetype_INCLUDE_DIR
					)

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

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/freetype-2.9)
set ( freetype_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/dependencies/freetype-2.9/include)

add_subdirectory(common/dependencies/SpriterParser)



if (USE_OPENGLES)
	find_package(OpenGLES)

	if(NOT OpenGLES_FOUND)
		set (OpenGLES_INCLUDE_DIR "common/dependencies/OpenGL ES/GLES/v1.1")
	endif(NOT OpenGLES_FOUND)
endif(USE_OPENGLES)


if (IndieLib_BUILD_STATIC)
	set (IndieLib_BUILD_TYPE STATIC)
else ()
	set (IndieLib_BUILD_TYPE SHARED)
endif (IndieLib_BUILD_STATIC)


add_definitions("-fPIC")

add_library( IndieLib ${IndieLib_BUILD_TYPE} ${SRC} ${INC} )

target_link_libraries( IndieLib SpriterParser TinyXML GLEW SDL2 freeimage TmxParser freetype UnitTest++  )

set ( EXETUTORIALSRC01 ${EXETUTORIALSRC01}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/01_Installing/Tutorial01.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/01_Installing/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/01_Installing/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial01 ${EXETUTORIALSRC01} ${INC} )
target_link_libraries( Tutorial01 IndieLib )

set ( EXETUTORIALSRC02 ${EXETUTORIALSRC02}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/02_IND_Surface/Tutorial02.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/02_IND_Surface/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/02_IND_Surface/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial02 ${EXETUTORIALSRC02} ${INC} )
target_link_libraries( Tutorial02 IndieLib )

set ( EXETUTORIALSRC03 ${EXETUTORIALSRC03}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/03_IND_Image/Tutorial03.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/03_IND_Image/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/03_IND_Image/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial03 ${EXETUTORIALSRC03} ${INC} )
target_link_libraries( Tutorial03 IndieLib )

set ( EXETUTORIALSRC04 ${EXETUTORIALSRC04}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/04_IND_Animation/Tutorial04.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/04_IND_Animation/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/04_IND_Animation/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial04 ${EXETUTORIALSRC04} ${INC} )
target_link_libraries( Tutorial04 IndieLib )

set ( EXETUTORIALSRC05 ${EXETUTORIALSRC05}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/05_IND_Font/Tutorial05.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/05_IND_Font/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/05_IND_Font/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial05 ${EXETUTORIALSRC05} ${INC} )
target_link_libraries( Tutorial05 IndieLib )

set ( EXETUTORIALSRC06 ${EXETUTORIALSRC06}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/06_Primitives/Tutorial06.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/06_Primitives/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/06_Primitives/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial06 ${EXETUTORIALSRC06} ${INC} )
target_link_libraries( Tutorial06 IndieLib )

set ( EXETUTORIALSRC07 ${EXETUTORIALSRC07}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/07_IND_Input/Tutorial07.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/07_IND_Input/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/07_IND_Input/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial07 ${EXETUTORIALSRC07} ${INC} )
target_link_libraries( Tutorial07 IndieLib )

set ( EXETUTORIALSRC08 ${EXETUTORIALSRC08}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/08_Collisions/Tutorial08.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/08_Collisions/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/08_Collisions/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial08 ${EXETUTORIALSRC08} ${INC} )
target_link_libraries( Tutorial08 IndieLib )

#set ( EXETUTORIALSRC09 ${EXETUTORIALSRC09}
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/09_IND_3dMesh/Tutorial09.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/09_IND_3dMesh/CIndieLib.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/09_IND_3dMesh/CIndieLib.h
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
#)
#add_executable( Tutorial09 ${EXETUTORIALSRC09} ${INC} )
#target_link_libraries( Tutorial09 IndieLib )

#set ( EXETUTORIALSRC10 ${EXETUTORIALSRC10}
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/10_IND_Light/Tutorial10.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/10_IND_Light/CIndieLib.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/10_IND_Light/CIndieLib.h
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
#)
#add_executable( Tutorial10 ${EXETUTORIALSRC10} ${INC} )
#target_link_libraries( Tutorial10 IndieLib )

set ( EXETUTORIALSRC11 ${EXETUTORIALSRC11}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/11_Animated_Tile_Scrolling/Tutorial11.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/11_Animated_Tile_Scrolling/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/11_Animated_Tile_Scrolling/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial11 ${EXETUTORIALSRC11} ${INC} )
target_link_libraries( Tutorial11 IndieLib )

set ( EXETUTORIALSRC13 ${EXETUTORIALSRC13}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/13_2d_Camera/Tutorial13.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/13_2d_Camera/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/13_2d_Camera/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial13 ${EXETUTORIALSRC13} ${INC} )
target_link_libraries( Tutorial13 IndieLib )

#set ( EXETUTORIALSRC14 ${EXETUTORIALSRC14}
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/14_3d_Camera/Tutorial14.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/14_3d_Camera/CIndieLib.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/14_3d_Camera/CIndieLib.h
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
#)
#add_executable( Tutorial14 ${EXETUTORIALSRC14} ${INC} )
#target_link_libraries( Tutorial14 IndieLib )

set ( EXETUTORIALSRC15 ${EXETUTORIALSRC15}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/15_Parallax_Scrolling/Tutorial15.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/15_Parallax_Scrolling/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/15_Parallax_Scrolling/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial15 ${EXETUTORIALSRC15} ${INC} )
target_link_libraries( Tutorial15 IndieLib )

set ( EXETUTORIALSRC16 ${EXETUTORIALSRC16}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/16_IND_Timer/Tutorial16.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/16_IND_Timer/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/16_IND_Timer/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial16 ${EXETUTORIALSRC16} ${INC} )
target_link_libraries( Tutorial16 IndieLib )

#set ( EXETUTORIALSRC17 ${EXETUTORIALSRC17}  //FIXME
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/17_IND_TTF_Font/Tutorial17.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/17_IND_TTF_Font/CIndieLib.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/basic/17_IND_TTF_Font/CIndieLib.h
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
#)
#add_executable( Tutorial17 ${EXETUTORIALSRC17} ${INC} )
#target_link_libraries( Tutorial17 IndieLib )

set ( EXE_TUTORIAL_ADVANCED_SRC_01 ${EXE_TUTORIAL_ADVANCED_SRC_01}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/01_IND_Surface_Grids/Tutorial01.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/01_IND_Surface_Grids/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/01_IND_Surface_Grids/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial_Advanced_01 ${EXE_TUTORIAL_ADVANCED_SRC_01} ${INC} )
target_link_libraries( Tutorial_Advanced_01 IndieLib )

set ( EXE_TUTORIAL_ADVANCED_SRC_02 ${EXE_TUTORIAL_ADVANCED_SRC_02}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/02_Blitting_2d_Directly/Tutorial02.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/02_Blitting_2d_Directly/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/02_Blitting_2d_Directly/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial_Advanced_02 ${EXE_TUTORIAL_ADVANCED_SRC_02} ${INC} )
target_link_libraries( Tutorial_Advanced_02 IndieLib )

#set ( EXE_TUTORIAL_ADVANCED_SRC_03 ${EXE_TUTORIAL_ADVANCED_SRC_03}
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/03_Blitting_3d_Directly/Tutorial03.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/03_Blitting_3d_Directly/CIndieLib.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/03_Blitting_3d_Directly/CIndieLib.h
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
#)
#add_executable( Tutorial_Advanced_03 ${EXE_TUTORIAL_ADVANCED_SRC_03} ${INC} )
#target_link_libraries( Tutorial_Advanced_03 IndieLib )

set ( EXE_TUTORIAL_ADVANCED_SRC_04 ${EXE_TUTORIAL_ADVANCED_SRC_04}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/04_Several_ViewPorts/Tutorial04.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/04_Several_ViewPorts/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/04_Several_ViewPorts/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial_Advanced_04 ${EXE_TUTORIAL_ADVANCED_SRC_04} ${INC} )
target_link_libraries( Tutorial_Advanced_04 IndieLib )

set ( EXE_TUTORIAL_ADVANCED_SRC_05 ${EXE_TUTORIAL_ADVANCED_SRC_05}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/05_IND_TmxMap/Tutorial05.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/05_IND_TmxMap/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/05_IND_TmxMap/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial_Advanced_05 ${EXE_TUTORIAL_ADVANCED_SRC_05} ${INC} )
target_link_libraries( Tutorial_Advanced_05 IndieLib )

set ( EXE_TUTORIAL_ADVANCED_SRC_06 ${EXE_TUTORIAL_ADVANCED_SRC_06}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/06_Spriter/Tutorial06.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/06_Spriter/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/advanced/06_Spriter/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial_Advanced_06 ${EXE_TUTORIAL_ADVANCED_SRC_06} ${INC} )
target_link_libraries( Tutorial_Advanced_06 IndieLib )

set ( EXE_TUTORIAL_BENCHMARK_SRC_01 ${EXE_TUTORIAL_BENCHMARK_SRC_01}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/benchmark/01_Alien_BenchMark/Tutorial01.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/benchmark/01_Alien_BenchMark/CTerrain.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/benchmark/01_Alien_BenchMark/CTerrain.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/benchmark/01_Alien_BenchMark/CUfo.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/benchmark/01_Alien_BenchMark/CUfo.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/benchmark/01_Alien_BenchMark/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/benchmark/01_Alien_BenchMark/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial_Benchmark_01 ${EXE_TUTORIAL_BENCHMARK_SRC_01} ${INC} )
target_link_libraries( Tutorial_Benchmark_01 IndieLib )

set ( EXE_TUTORIAL_BENCHMARK_SRC_02 ${EXE_TUTORIAL_BENCHMARK_SRC_02}
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/benchmark/02_Rabbits_BenchMark/Tutorial02.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/benchmark/02_Rabbits_BenchMark/CRabbit.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/benchmark/02_Rabbits_BenchMark/CRabbit.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/benchmark/02_Rabbits_BenchMark/CIndieLib.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/benchmark/02_Rabbits_BenchMark/CIndieLib.h
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
)
add_executable( Tutorial_Benchmark_02 ${EXE_TUTORIAL_BENCHMARK_SRC_02} ${INC} )
target_link_libraries( Tutorial_Benchmark_02 IndieLib )

# TODO: WIN specific use of [MAX_PATH] ... DarthMike fixed this in workinpath class for about 4 years ago .... must take a closer look when im not so tired =(
#set ( EXE_TOOL_TILELESS_EDITOR_SRC ${EXE_TOOL_TILELESS_EDITOR_SRC}
#                ${CMAKE_CURRENT_SOURCE_DIR}/tools/editor_tileless/Main.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tools/editor_tileless/EditorMap.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tools/editor_tileless/EditorMap.h
#				${CMAKE_CURRENT_SOURCE_DIR}/tools/editor_tileless/Listener.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tools/editor_tileless/Listener.h
#				${CMAKE_CURRENT_SOURCE_DIR}/tools/editor_tileless/Node.cpp
#                ${CMAKE_CURRENT_SOURCE_DIR}/tools/editor_tileless/Node.h
#                ${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tutorials/WorkingPath.h
#                ${CMAKE_CURRENT_SOURCE_DIR}/tools/editor_tileless/Resources.cpp
#				${CMAKE_CURRENT_SOURCE_DIR}/tools/editor_tileless/Resources.h
#)
#add_executable( Tool_Tileless_Editor ${EXE_TOOL_TILELESS_EDITOR_SRC} ${INC} )
#target_link_libraries( Tool_Tileless_Editor IndieLib )

