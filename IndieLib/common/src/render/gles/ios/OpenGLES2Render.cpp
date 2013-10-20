/*****************************************************************************************
 * File: OpenGLRender.cpp
 * Desc: Initialization / Destruction using OpenGL
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
THIS FILE IS AN ADDITIONAL FILE ADDED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com), BUT HAS THE
SAME LICENSE AS THE WHOLE LIBRARY TO RESPECT ORIGINAL AUTHOR OF LIBRARY

This library is free software; you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
Suite 330, Boston, MA 02111-1307 USA
*/
#include "Defines.h"

#ifdef INDIERENDER_GLES_IOS

// ----- Includes -----

#include "Global.h"
#include "IND_Math.h"
#include "OpenGLES2Render.h"
#include "IND_Window.h"
#include "IND_FontManager.h"
#include "IND_SurfaceManager.h"
#include "IND_AnimationManager.h"
#include "IND_FontManager.h"
#include "IND_Timer.h"
#include "IND_Font.h"
#include "IND_Animation.h"
#include "IND_Camera2d.h"
#include "platform/iOS/OpenGLES2Manager.h"
#include <OpenGLES/ES2/gl.h>

/** @cond DOCUMENT_PRIVATEAPI */
// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

IND_Window* OpenGLES2Render::initRenderAndWindow(IND_WindowProperties& props) {
	if(props._bpp <= 0 || props._height <= 0 || props._width <= 0) {
		g_debug->header("Error initializing window: Invalid parameters provided", DebugApi::LogHeaderError);
		return 0;
	}
	end();
	initVars();

	// Window creation
	_window = new IND_Window();
	if (!_window) {
		freeVars();
		return NULL;
	}
	
	if(!_osOpenGLMgr) {
		_osOpenGLMgr = new OpenGLES2Manager(_window);
	}
	_doubleBuffer = props._doubleBuffer;
    
	//Initialize OpenGL parameters for SDL before creating the window for OpenGL
	_osOpenGLMgr->setOpenGLContextParams(IND_RGBA, //Color format. HARDCODED, AND HAS TO MATCH WINDOW HARDCODED FORMAT!
										 props._bpp/4, //Color depth (Bpp) /  num colors
	                                     24, //Depth Buffer bits
	                                     8,  //Stencil Buffer bits
	                                     props._doubleBuffer //Double buffering
	                                    );

	if(!_window->create(props)) {
		g_debug->header("Error creating window: Not supported params provided", DebugApi::LogHeaderError);
		freeVars();
		return NULL;
	}
    
    if (_window->getWidth() != props._width) {
        g_debug->header("Window width ignored. Set to:", DebugApi::LogHeaderInfo);
        g_debug->dataInt(_window->getWidth(),true);
    }
    
    if (_window->getHeight() != props._height) {
        g_debug->header("Window height ignored. Set to:", DebugApi::LogHeaderInfo);
        g_debug->dataInt(_window->getHeight(),true);
    }
    
    if (_window->isFullScreen()!= props._fullscreen) {
        g_debug->header("Window fullscreen ignored. Set to:", DebugApi::LogHeaderInfo);
        g_debug->dataInt(_window->isFullScreen(),true);
    }

	g_debug->header("Creating OpenGL ES 2 Render", DebugApi::LogHeaderBegin);
	_ok = initializeOpenGLES2Render();
	if (!_ok) {
		g_debug->header("Finalizing OpenGL ES 2", DebugApi::LogHeaderWarning);
		freeVars();
		g_debug->header("OpenGL finalized", DebugApi::LogHeaderEnd);
		return NULL;
	}

	g_debug->header("Checking created OpenGL context pixel format", DebugApi::LogHeaderOk);
	if(!_osOpenGLMgr->checkOpenGLSDLContextProps()) {
		g_debug->header("Different GL context pixel format used", DebugApi::LogHeaderWarning);
	} else {
		g_debug->header("Same GL context pixel format used", DebugApi::LogHeaderOk);
	}

	writeInfo();

	g_debug->header("OpenGL ES 2 Render Created", DebugApi::LogHeaderEnd);
	return _window;
}

bool OpenGLES2Render::reset(IND_WindowProperties& props) {
	if(props._bpp <= 0 || props._height <= 0 || props._width <= 0) {
		g_debug->header("Error resetting window: Invalid parameters provided", DebugApi::LogHeaderError);
		return 0;
	}

    bool viewPortWasFullWindow = (_window->getWidth() == _info._viewPortWidth) && (_window->getHeight() == _info._viewPortHeight);
    
	if (!_window->reset(props)) {
		g_debug->header("Error resetting SDL window", DebugApi::LogHeaderError);
		return 0;
	}
    
    _info._fbWidth = _window->getWidth();
    _info._fbHeight = _window->getHeight();
    
    bool ok = 1;
    if (viewPortWasFullWindow) {
        ok = resetViewport(_window->getWidth(),_window->getHeight());
    }
	
    return ok;
}

bool OpenGLES2Render::toggleFullScreen() {

	g_debug->header("Changing To/From Full Screen", DebugApi::LogHeaderBegin);

	if (!_window->toggleFullScreen()) return false;

	return true;
}

void OpenGLES2Render::beginScene() {

	if (!_ok)
		return;

	//Clear buffers
	glClearColor(0, 0, 0, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLES2Render::endScene() {
	if (!_ok)
		return;

	//Swap memory buffers (OS-dependant)
	_osOpenGLMgr->presentBuffer();

#ifdef _DEBUG
    GLenum glerror = glGetError();
	if (glerror) {
        g_debug->header("OpenGLRenderer::endScene() OpenGL error flag!", DebugApi::LogHeaderError);
		printf("OpenGLRenderer::endScene() Error at end of scene! :%i\n",glerror);
	}
#endif	
}

void OpenGLES2Render::showFpsInWindowTitle(char *pFPSString) {
	if (!_ok)   return;

	_window->setTitle(pFPSString);
}

void OpenGLES2Render::setPointPixelScale (float pNewScale) {
    _info._pointPixelScale = pNewScale;
}

void OpenGLES2Render::getNumrenderedObjectsString(char *pBuffer)      {
	IND_Math::itoa(_numrenderedObjects, pBuffer);
}

void OpenGLES2Render::getNumDiscardedObjectsString(char *pBuffer)      {
	IND_Math::itoa(_numDiscardedObjects, pBuffer);
}

void OpenGLES2Render::end() {
	if (_ok) {
		g_debug->header("Finalizing OpenGL", DebugApi::LogHeaderBegin);
		_osOpenGLMgr->endOpenGLContext();
		freeVars();
		g_debug->header("OpenGL finalized ", DebugApi::LogHeaderEnd);
		_ok = false;
	}
}

// --------------------------------------------------------------------------------
//							        Private methods
// --------------------------------------------------------------------------------

/*
==================
Init vars
==================
*/
void OpenGLES2Render::initVars() {
	_numrenderedObjects = 0;
	_numDiscardedObjects = 0;
	_window = NULL;
	_math.init();
	_osOpenGLMgr = NULL;

}

/*
==================
Init OpenGL
==================
*/
bool OpenGLES2Render::initializeOpenGLES2Render() {

	//Check dependency of window initialization
	if (!_window->isOK()) {
		// Window error
		g_debug->header("This operation can not be done:", DebugApi::LogHeaderInfo);
		g_debug->dataChar("", 1);
		g_debug->header("Invalid Id or IND_Window not correctly initialized.", DebugApi::LogHeaderError);

		return false;
	}

	//OpenGL context creation
	_osOpenGLMgr->createOpenGLSDLContext();

	//Check for GL extensions
	if (!checkGLExtensions())
		return false;

	//Get all graphics device information
	getInfo();
    
    //Window params
    _info._fbWidth = _window->getWidth();
	_info._fbHeight = _window->getHeight();

	// ViewPort initialization
	return resetViewport(_window->getWidth(),_window->getHeight());
}

/*
==================
Check OpenGL extensions
==================
*/
bool OpenGLES2Render::checkGLExtensions() {
	return true;
}

/*
==================
Free memory
==================
*/
void OpenGLES2Render::freeVars() {
	DISPOSE(_osOpenGLMgr);
	DISPOSE(_window);
	_ok = false;
}

/*
==================
Hardware information
==================
*/
void OpenGLES2Render::getInfo() {
    const GLubyte* vendor = glGetString(GL_VENDOR);
    strcpy(_info._vendor,(char*)vendor);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    strcpy(_info._vendor,(char*)renderer);
    const GLubyte* version = glGetString(GL_VERSION);
    strcpy (_info._version,(char*)version);
  
    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&_info._maxTextureSize);
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &_info._maxVertexAttributes);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &_info._maxVertexUniformComponents);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &_info._maxVertexTextureUnits);
    glGetIntegerv(GL_MAX_VARYING_VECTORS, &_info._maxVaryingVectors);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &_info._maxCombinedTextureImageUnits);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_info._maxCombinedTextureImageUnits);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &_info._maxfragmentUniformVectors);
}

/*
==================
Write hardware information to debug log
==================
*/
void OpenGLES2Render::writeInfo() {
	g_debug->header("Hardware information" , DebugApi::LogHeaderBegin);

	// ----- version -----

	g_debug->header("OpenGL version:" , DebugApi::LogHeaderInfo);
	g_debug->dataChar(_info._version, 1);

	// ----- Vendor -----

	g_debug->header("Mark:" , DebugApi::LogHeaderInfo);
	g_debug->dataChar(_info._vendor, 1);

	// ----- Renderer -----

	g_debug->header("Chip:" , DebugApi::LogHeaderInfo);
	g_debug->dataChar(_info._renderer, 1);

	// ----- Antialiasing -----

	g_debug->header("Primitive antialising:", DebugApi::LogHeaderInfo);
	if (_info._antialiasing)
		g_debug->dataChar("Yes", 1);
	else
		g_debug->dataChar("No", 1);

	// ----- Max texture size -----

	g_debug->header("Maximum texture size:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxTextureSize, 0);
	g_debug->dataChar("x", 0);
	g_debug->dataInt(_info._maxTextureSize, 1);
    g_debug->header("Maximum number of vertex attributes:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxVertexAttributes, 0);
    g_debug->header("Maximum number of vertex uniform vectors:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxVertexUniformComponents, 0);
    g_debug->header("Maximum number of vertex texture units:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxVertexTextureUnits, 0);
    g_debug->header("Maximum number of varying vectors:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxVaryingVectors, 0);
    g_debug->header("Maximum number of combined texture image units:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxCombinedTextureImageUnits, 0);
    g_debug->header("Maximum number of texture units:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxTextureImageUnits, 0);
    g_debug->header("Maximum number of fragment uniform vectors:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxfragmentUniformVectors, 0);


	// ----- Vertex Shader version  -----

	/*g_debug->Header ("Vertex Shader:" , DebugApi::LogHeaderInfo);
	g_debug->DataInt (D3DSHADER_VERSION_MAJOR (_info._vertexShaderVersion), 0);
	g_debug->DataChar (".", 0);
	g_debug->DataInt (D3DSHADER_VERSION_MINOR (_info._vertexShaderVersion), 0);

	if (_info._softwareVertexProcessing)
	    g_debug->DataChar ("(Software)", 1);
	else
	    g_debug->DataChar ("", 1);*/

	// ----- Pixel Shader version -----

	/*g_debug->Header ("Pixel Shader:" , DebugApi::LogHeaderInfo);
	g_debug->DataInt (D3DSHADER_VERSION_MAJOR (_info._pixelShaderVersion), 0);
	g_debug->DataChar (".", 0);
	g_debug->DataInt (D3DSHADER_VERSION_MINOR (_info._pixelShaderVersion), 1);*/

	g_debug->header("Hardware Ok" , DebugApi::LogHeaderEnd);
}

/*
==================
Resets the viewport to all window width/height
==================
*/
bool OpenGLES2Render::resetViewport(int pWitdh, int pHeight) {
    if (pHeight == 0 || pWitdh == 0) return false;
    
	if (!setViewPort2d(0, 0, pWitdh, pHeight))
		return false;

	IND_Camera2d mCamera2d(static_cast<float>(_info._viewPortWidth/2),
						   static_cast<float>(_info._viewPortHeight/2));   //Default 2D camera in center of viewport
	setCamera2d(&mCamera2d);
	clearViewPort(0, 0, 0);

	return true;
}
/** @endcond */
#endif //INDIERENDER_GLES_IOS
