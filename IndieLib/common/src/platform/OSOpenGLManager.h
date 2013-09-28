/*****************************************************************************************
 * File: OSOpenGLManager.h
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


#ifndef _OSOPENGLMANAGER_H_
#define _OSOPENGLMANAGER_H_

// ----- Includes -----

#include "Defines.h"

// ----- Forward declarations -----
class IND_Window;
struct SDL_Window;

// --------------------------------------------------------------------------------
//									  OSOpenGLManager
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
This class manages all OpenGL context creation for OS. Generally, as we use SDL we don't need to subclass this
to support different OS.
*/
class OSOpenGLManager{
public:

	// ----- Init/End -----

	OSOpenGLManager(IND_Window* window):
	    _window(window),
		_sdlWindow(NULL),
		_glContext(NULL),
		_doubleBuffering(false),
		_numBitsPerColor(0),
		_numDepthBits(0),
		_colorBufferFormat(IND_RGBA)
		{
	}
	virtual ~OSOpenGLManager() {
	}

	// ----- Public methods -----
	//Inits OpenGL context (SDL Properties)
	virtual void setOpenGLContextParams(IND_ColorFormat colorFormat, int numBitsPerColor, int numDepthBits, int numStencilBits, bool doublebuffer);

	//Verifies properties set (call this after window is created)
	virtual bool checkOpenGLSDLContextProps();

	//Creates an SDL openGL context
	virtual bool createOpenGLSDLContext();

	//Ends OpenGL context
	virtual void endOpenGLContext();

	//Performs buffer swapping or buffer drawing
	virtual void presentBuffer();
protected:

	// ----- Private methods -----
	void _init();
	void _end();
	// ----- Objects -----
	IND_Window* _window;
	SDL_Window* _sdlWindow;
	void* _glContext;
	// ----- Vars -----

	bool _doubleBuffering;  //Whether use double buffering or not

	int _numBitsPerColor;	//Number of bits per color requested (color buffer)

	int _numDepthBits;		//Depth buffer requested bit depth 

	int _numStencilBits;	//Stencil buffer requested bit depth 

	IND_ColorFormat _colorBufferFormat;  //Requested color buffer format

};

/** @endcond */

#endif // _OSOPENGLMANAGER_H_
