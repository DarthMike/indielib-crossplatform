/*****************************************************************************************
 * File: OSOpenGLManager.cpp
 * Desc: OS initializer for OpenGL. Provides interface and default implementation for OpenGL (desktop)
 *       May need to be subclassed is a new system is supported, to add different initialization
 *       parameters, etc.
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

#if defined (INDIERENDER_OPENGL)
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
	g_debug->header("Initiating OpenGL SDL Context Parameters", 3);
	
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

	g_debug->header("OpenGL Context Parameters Initated", 3);
	g_debug->dataChar("OK", true);
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

	g_debug->header("Created GL context R size: ",3);
	g_debug->dataInt(contextRSize,true);
	g_debug->header("Created GL context G size: ",3);
	g_debug->dataInt(contextGSize,true);
	g_debug->header("Created GL context B size: ",3);
	g_debug->dataInt(contextBSize,true);
	g_debug->header("Created GL context A size: ",3);
	g_debug->dataInt(contextASize,true);
	g_debug->header("Created GL context Depth size: ",3);
	g_debug->dataInt(contextDepthSize,true);
	g_debug->header("Created GL context Stencil size: ",3);
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
	g_debug->header("Creating OpenGL context", 3);
	g_debug->dataChar("OK", true);
  
	_sdlWindow = _window->getSDLWindow();
    _glContext = SDL_GL_CreateContext(_sdlWindow); 

	if (!_glContext) {
        g_debug->header("Error creating OpenGL contect", 2);
		return false;
	}

	g_debug->header("OpenGL context created", 3);
	g_debug->dataChar("OK", true);
	return true;
}

void OSOpenGLManager::endOpenGLContext() {
	if (_glContext) {
		g_debug->header("Destroying OpenGL Context", 3);
		g_debug->dataChar("OK", true);
		SDL_GL_DeleteContext(_glContext);
		g_debug->header("OpenGL Context Destroyed", 3);
		g_debug->dataChar("OK", true);
	}
}

void OSOpenGLManager::presentBuffer() {
	assert(_sdlWindow);
	SDL_GL_SwapWindow(_sdlWindow);
}

#endif //INDIERENDER_OPENGL
