AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4 --install

lib_LTLIBRARIES = libIndieLib.la

libIndieLib_la_SOURCES = ../common/src/CollisionParser.cpp ../common/src/DebugApi.cpp ../common/src/Global.cpp ../common/src/ImageCutter.cpp ../common/src/IND_Animation.cpp ../common/src/IND_AnimationManager.cpp ../common/src/IND_Camera2d.cpp ../common/src/IND_Entity2d.cpp ../common/src/IND_Entity2dManager.cpp ../common/src/IND_FontManager.cpp ../common/src/IndieLib.cpp ../common/src/IND_Image.cpp ../common/src/IND_ImageManager.cpp ../common/src/IND_Input.cpp ../common/src/IND_Math.cpp ../common/src/IND_Render.cpp ../common/src/IND_Surface.cpp ../common/src/IND_SurfaceManager.cpp ../common/src/IND_Timer.cpp ../common/src/IND_Window.cpp ../common/src/PrecissionTimer.cpp  ../common/src/FreeImageHelper.cpp ../common/dependencies/tinyxml/tinyxml.cpp ../common/dependencies/tinyxml/tinystr.cpp ../common/dependencies/tinyxml/tinyxmlerror.cpp ../common/dependencies/tinyxml/tinyxmlparser.cpp ../common/src/render/opengl/OpenGLRender.cpp ../common/src/platform/OSOpenGLManager.cpp ../common/src/render/opengl/OpenGLTextureBuilder.cpp ../common/src/render/opengl/RenderCullingOpenGL.cpp ../common/src/render/opengl/RenderObject2dOpenGL.cpp ../common/src/render/opengl/RenderObject3dOpenGL.cpp ../common/src/render/opengl/RenderPrimitive2dOpenGL.cpp ../common/src/render/opengl/RenderText2dOpenGL.cpp ../common/src/render/opengl/RenderTransform2dOpenGL.cpp ../common/src/render/opengl/RenderTransform3dOpenGL.cpp ../common/src/render/opengl/RenderTransformCommonOpenGL.cpp ../common/src/IND_TmxMap.cpp ../common/src/IND_TmxMapManager.cpp ../common/dependencies/TmxParser/TmxMap.cpp ../common/dependencies/TmxParser/TmxPropertySet.cpp ../common/dependencies/TmxParser/TmxObjectGroup.cpp ../common/dependencies/TmxParser/TmxLayer.cpp ../common/dependencies/TmxParser/TmxTileset.cpp ../common/dependencies/TmxParser/TmxObject.cpp ../common/dependencies/TmxParser/TmxUtil.cpp ../common/dependencies/TmxParser/TmxImage.cpp ../common/dependencies/TmxParser/TmxTile.cpp ../common/dependencies/TmxParser/TmxPolygon.cpp ../common/dependencies/TmxParser/TmxPolyline.cpp ../common/dependencies/TmxParser/base64/base64.cpp ../common/dependencies/FreeImage/Source/ZLib/inflate.c ../common/dependencies/FreeImage/Source/ZLib/crc32.c ../common/dependencies/FreeImage/Source/ZLib/adler32.c ../common/dependencies/FreeImage/Source/ZLib/uncompr.c ../common/dependencies/FreeImage/Source/ZLib/inftrees.c ../common/dependencies/FreeImage/Source/ZLib/inffast.c ../common/dependencies/FreeImage/Source/ZLib/zutil.c ../common/dependencies/FreeImage/Source/ZLib/gzwrite.c ../common/dependencies/FreeImage/Source/ZLib/deflate.c ../common/dependencies/FreeImage/Source/ZLib/trees.c ../common/dependencies/FreeImage/Source/ZLib/gzlib.c

libIndieLib_la_LDFLAGS =-static -version-info 1:0:0 -lfreeimage

libIndieLib_la_CPPFLAGS =-fPIC -Wall -Wno-implicit-function-declaration -Wno-error=unused-result -Werror -DNDEBUG -O2 -s -I../common/ -I../common/src/ -I../common/include/
# libIndieLib_la_CPPFLAGS =-fPIC -Wall -DNDEBUG -O2 -s -I../common/ -I../common/src/ -I../common/include/ -I../common/dependencies/DevIL-SDK-x86-1.7.8/include/
dist_noinst_SCRIPTS = autogen.sh

# CollisionParser.cpp       // ok
# DebugApi.cpp              // ok
# Global.cpp                // ok
# ImageCutter.cpp           // ok
# IND_3dMesh.cpp            // not ported yet
# IND_3dMeshManager.cpp     // not ported yet
# IND_Animation.cpp         // ok
# IND_AnimationManager.cpp  // ok
# IND_Camera2d.cpp          // ok
# IND_Camera3d.cpp          // not ported yet
# IND_Entity2d.cpp          // ok
# IND_Entity2dManager.cpp   // ok
# IND_Entity3d.cpp          // not ported yet
# IND_Entity3dManager.cpp   // not ported yet
# IND_FontManager.cpp       // ok
# IndieLib.cpp              // ok
# IND_Image.cpp             // ok
# IND_ImageManager.cpp      // ok
# IND_Input.cpp             // ok
# IND_Light.cpp             // not ported yet
# IND_LightManager.cpp      // not ported yet
# IND_Math.cpp              // ok
# IND_Render.cpp            // ok
# IND_Surface.cpp           // ok
# IND_SurfaceManager.cpp    // ok
# IND_Timer.cpp             // ok
# IND_Window.cpp            // ok
# PrecissionTimer.cpp       // ok

#libIndieLib_la_CPPFLAGS =-fPIC -Wall -Winline -DNDEBUG -O3 -pipe -fomit-frame-pointer -s -I../common/dependencies/DevIL-SDK-x86-1.7.8/include/ -I../common/src/ -I../common/include/ -I../common/dependencies/ -I../common/dependencies/SDL\ 1.3/include/


# read this example for more info http://www.codealias.info/technotes/autotools_for_libraries
