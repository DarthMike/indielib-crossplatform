/*****************************************************************************************
 * File: OpenGLES2Manager.h
 * Desc: OS initializer for OpenGL ES 2. 
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


#ifndef _OPENGLES2MANAGER_H_
#define _OPENGLES2MANAGER_H_

// ----- Includes -----

#include "Defines.h"
#include "platform/OSOpenGLManager.h"

// ----- Forward declarations -----
class IND_Window;
struct SDL_Window;

// --------------------------------------------------------------------------------
//									  OpenGLES2Manager
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
This class manages all OpenGL context creation for OS. Generally, as we use SDL we don't need to subclass this
to support different OS.
*/
class OpenGLES2Manager : public OSOpenGLManager {
public:

	// ----- Init/End -----
    OpenGLES2Manager (IND_Window* window) : OSOpenGLManager(window){}

	virtual ~OpenGLES2Manager() {
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


};

/** @endcond */

#endif // _OPENGLES2MANAGER_H_
