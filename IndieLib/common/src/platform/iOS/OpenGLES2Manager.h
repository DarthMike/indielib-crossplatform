/*****************************************************************************************
 * File: OpenGLES2Manager.h
 * Desc: OS initializer for OpenGL ES 2. 
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
