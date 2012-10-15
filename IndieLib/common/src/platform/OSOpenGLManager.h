/*****************************************************************************************
 * File: OSOpenGLManager.h
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
