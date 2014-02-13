/*****************************************************************************************
 * File: OpenGLRender.cpp
 * Desc: Initialization / Destruction using OpenGL
 *****************************************************************************************/

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
#include "IND_ShaderProgram.h"
#include "IND_Shaders.h"
#include "IND_ShaderManager.h"

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

    initializeBuffers();
    
    _ok = initializeDefaultPrograms();
    if (!_ok) {
        g_debug->header("Error compiling default shaders", DebugApi::LogHeaderError);
        return NULL;
    }
    
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
    _shaderManager = new IND_ShaderManager();

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

void OpenGLES2Render::initializeBuffers() {
    glGenBuffers(1, &_pointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _pointBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_POS)*MAX_PIXELS, NULL, GL_DYNAMIC_DRAW);
    glGenBuffers(1, &_pointWithColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _pointWithColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_POSANDCOLOR)*MAX_PIXELS, NULL, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool OpenGLES2Render::initializeDefaultPrograms() {
    bool success = _shaderManager->init();
    IND_ShaderProgram* uniformColorNoTexture = IND_ShaderProgram::newShaderProgram();
    success &= uniformColorNoTexture->compile(IND_VertexShader_UniformRGBAColor, IND_FragmentShader_Color);
    success &= uniformColorNoTexture->link();
    success &= _shaderManager->add(uniformColorNoTexture, IND_Program_UniformRGBAColor);
    
    IND_ShaderProgram* pervertexColorNoTexture = IND_ShaderProgram::newShaderProgram();
    success &= pervertexColorNoTexture->compile(IND_VertexShader_PerVertexRGBAColor, IND_FragmentShader_Color);
    success &= pervertexColorNoTexture->link();
    success &= _shaderManager->add(pervertexColorNoTexture, IND_Program_PerVertexRGBAColor);
    
    IND_ShaderProgram* simple2dTexture = IND_ShaderProgram::newShaderProgram();
    success &= simple2dTexture->compile(IND_VertexShader_Simple2DTexture, IND_FragmentShader_Simple2DTexture_BGRA);
    success &= simple2dTexture->link();
    success &= _shaderManager->add(simple2dTexture, IND_Program_Simple2DTexture);

    IND_ShaderProgram* texture2dWithTinting = IND_ShaderProgram::newShaderProgram();
    success &= texture2dWithTinting->compile(IND_VertexShader_Simple2DTexture, IND_FragmentShader_2DTexture_RGBATint);
    success &= texture2dWithTinting->link();
    success &= _shaderManager->add(texture2dWithTinting, IND_Program_2DTexture_RGBATint);
    
    IND_ShaderProgram* texture2dWithFadeToColor = IND_ShaderProgram::newShaderProgram();
    success &= texture2dWithFadeToColor->compile(IND_VertexShader_Simple2DTexture, IND_FragmentShader_2DTexture_RGBAFade);
    success &= texture2dWithFadeToColor->link();
    success &= _shaderManager->add(texture2dWithFadeToColor, IND_Program_2DTexture_RGBAFade);
    
    return success;
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
    DISPOSE(_shaderManager);
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
    
    const GLubyte* shaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    strcpy(_info._glslVersion, (char*)shaderVersion);
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
	g_debug->dataInt(_info._maxVertexAttributes, 1);
    g_debug->header("Maximum number of vertex uniform vectors:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxVertexUniformComponents, 1);
    g_debug->header("Maximum number of vertex texture units:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxVertexTextureUnits, 1);
    g_debug->header("Maximum number of varying vectors:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxVaryingVectors, 1);
    g_debug->header("Maximum number of combined texture image units:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxCombinedTextureImageUnits, 1);
    g_debug->header("Maximum number of texture units:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxTextureImageUnits, 1);
    g_debug->header("Maximum number of fragment uniform vectors:" , DebugApi::LogHeaderInfo);
	g_debug->dataInt(_info._maxfragmentUniformVectors, 1);


	// ----- Shading language version  -----
	g_debug->header ("Shading language version:" , DebugApi::LogHeaderInfo);
	g_debug->dataChar(_info._glslVersion, 1);
    
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
