/*****************************************************************************************
 * File: OSOpenGLManager.cpp
 * Desc: OS initializer for OpenGL. Provides interface and default implementation for OpenGL (desktop)
 *       May need to be subclassed is a new system is supported, to add different initialization
 *       parameters, etc.
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

#if defined (INDIERENDER_OPENGL) || defined (INDIERENDER_GLES_IOS)

// ----- Libs -----

#include "dependencies/SDL-2.0/include/SDL.h"
#include "dependencies/SDL-2.0/include/SDL_video.h"

// ----- Includes -----

#include "Global.h"
#include "OSOpenGLManager.h"
#include "IND_Window.h"

// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

void OSOpenGLManager::setOpenGLContextParams(IND_ColorFormat colorFormat, int numBitsPerColor, int numDepthBits, int numStencilBits, bool doublebuffer) {
	g_debug->header("Initiating OpenGL SDL Context Parameters", DebugApi::LogHeaderOk);
	
	if (IND_RGBA == colorFormat) {
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            numBitsPerColor);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          numBitsPerColor);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           numBitsPerColor);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          numBitsPerColor);
	} else {
		//This is a very strange setting. but we support it.
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            numBitsPerColor);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          numBitsPerColor);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           numBitsPerColor);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          0);
	}

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          numDepthBits);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,        numStencilBits);

	if (doublebuffer) {
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ) ;
	} else {
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 0 ) ;
	}

	_numBitsPerColor = numBitsPerColor;
	_numDepthBits = numDepthBits;
	_numStencilBits = numStencilBits;
	_colorBufferFormat = colorFormat;

	g_debug->header("OpenGL Context Parameters Initated", DebugApi::LogHeaderOk);
}

bool OSOpenGLManager::checkOpenGLSDLContextProps() {
	int contextRSize;
	int contextGSize;
	int contextBSize;
	int contextASize;
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	SDL_GL_GetAttribute(SDL_GL_RED_SIZE,&contextRSize);
	SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE,&contextGSize);
	SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE,&contextBSize);
	SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE,&contextASize);

	int contextDepthSize;
	int contextSencilSize;
	SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE,&contextDepthSize);
    SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE,&contextSencilSize);

	g_debug->header("Created GL context R size: ",DebugApi::LogHeaderInfo);
	g_debug->dataInt(contextRSize,true);
	g_debug->header("Created GL context G size: ",DebugApi::LogHeaderInfo);
	g_debug->dataInt(contextGSize,true);
	g_debug->header("Created GL context B size: ",DebugApi::LogHeaderInfo);
	g_debug->dataInt(contextBSize,true);
	g_debug->header("Created GL context A size: ",DebugApi::LogHeaderInfo);
	g_debug->dataInt(contextASize,true);
	g_debug->header("Created GL context Depth size: ",DebugApi::LogHeaderInfo);
	g_debug->dataInt(contextDepthSize,true);
	g_debug->header("Created GL context Stencil size: ",DebugApi::LogHeaderInfo);
	g_debug->dataInt(contextSencilSize,true);

	//RGB color buffer size check
	if (_numBitsPerColor != contextRSize
			||
			_numBitsPerColor != contextGSize
			||
			_numBitsPerColor != contextBSize) {
			return false;
		}

	//Alpha channel size color buffer check
	if (IND_RGBA == _colorBufferFormat) {
		if (_numBitsPerColor != contextASize) {
			return false;
		}
	} else {
		if (0 != contextASize) {
			return false;
		}
	}

	if (_numDepthBits != contextDepthSize) {
		return false;
	}

	if (_numStencilBits != contextSencilSize) {
		return false;
	}

	return true;
}

bool OSOpenGLManager::createOpenGLSDLContext() {
	g_debug->header("Creating OpenGL context", DebugApi::LogHeaderOk);
  
	_sdlWindow = _window->getSDLWindow();
    _glContext = SDL_GL_CreateContext(_sdlWindow); 

	if (!_glContext) {
        g_debug->header("Error creating OpenGL contect", DebugApi::LogHeaderError);
		return false;
	}

	g_debug->header("OpenGL context created", DebugApi::LogHeaderOk);
	return true;
}

void OSOpenGLManager::endOpenGLContext() {
	if (_glContext) {
		g_debug->header("Destroying OpenGL Context", DebugApi::LogHeaderOk);
		SDL_GL_DeleteContext(_glContext);
		g_debug->header("OpenGL Context Destroyed", DebugApi::LogHeaderOk);
	}
}

void OSOpenGLManager::presentBuffer() {
	assert(_sdlWindow);
	SDL_GL_SwapWindow(_sdlWindow);
}

#endif //defined (INDIERENDER_OPENGL) || defined (INDIERENDER_GLES_IOS)
