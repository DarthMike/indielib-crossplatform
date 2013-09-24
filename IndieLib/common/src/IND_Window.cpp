/*****************************************************************************************
 * File: IND_Window.cpp
 * Desc: Manages the window (uses SDL for creating the window).
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
MODIFIED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)

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

// ----- Includes -----
#include "Defines.h"
#include "Global.h"
#include "IND_Window.h"

#include "dependencies/SDL-2.0/include/SDL.h"
#include "dependencies/SDL-2.0/include/SDL_syswm.h"

// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

/**
 * Changes the title of the actual window.
 * @param pTitle					Title of the window.
 */
void IND_Window::setTitle(const char *pTitle) {
	strcpy(_attributes._title, pTitle);
	SDL_SetWindowTitle(getSDLWindow(), pTitle);
}

/**
 * Shows or hides the cursor.
 * @param pEnable					Show cursor (1 / 0) = (on / off).
 */
void IND_Window::cursor(bool pEnable) {
	if (pEnable)
		SDL_ShowCursor(SDL_ENABLE);
	else
		SDL_ShowCursor(SDL_DISABLE);
}

// --------------------------------------------------------------------------------
//									 Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
==================
Creates a window
==================
*/
bool IND_Window::create(IND_WindowProperties& props) {
	end();
	initVars();

	g_debug->header("Creating SDL window", DebugApi::LogHeaderBegin);

	if(props._bpp <= 0 || props._height <= 0 || props._width <= 0) {
		g_debug->header("Error creating window: Invalid parameters provided", DebugApi::LogHeaderError);
		return 0;
	}
	
	char stringTemp[128];

	_attributes._width      = props._width;
	_attributes._height     = props._height;
	_attributes._bits       = props._bpp;
	_attributes._vsync      = props._vsync;
	_attributes._fullscreen = props._fullscreen;
	_attributes._title      = strcpy(stringTemp, props._title);
	
	Uint32 windowFlags = SDL_WINDOW_SHOWN;
	if (_attributes._fullscreen) {
		windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN;
	}
#if defined (INDIERENDER_OPENGL)
	windowFlags = windowFlags | SDL_WINDOW_OPENGL;
#endif

#ifdef _DEBUG
    printf("\nAVAILABLE WINDOW DISPLAY MODES:");
    int displayModes = SDL_GetNumDisplayModes(0);
    for (int i = 0 ; i < displayModes; ++i) {
        SDL_DisplayMode mode;
        SDL_GetDisplayMode(0, i, &mode);
        printf("\nDISPLAY MODE:");
        printf("\nWith %i Height %i",mode.w,mode.h);
        printf("\nFormat: %04x",mode.format);
        printf("\nRate: %i",mode.refresh_rate);
        
    }
#endif
    
	_attributes._sdlWindow = SDL_CreateWindow(_attributes._title,
										  SDL_WINDOWPOS_CENTERED, 
										  SDL_WINDOWPOS_CENTERED,
										  _attributes._width, 
										 _attributes._height, 
										  windowFlags);
	if (!_attributes._sdlWindow) {
		g_debug->header("Error creating SDL window", DebugApi::LogHeaderError);
		g_debug->header(std::string(SDL_GetError()),DebugApi::LogHeaderError);
		return 0;
	}

	if (!reset(props)) {
		g_debug->header("Error setting SDL window params", DebugApi::LogHeaderError);
		g_debug->header(std::string(SDL_GetError()),DebugApi::LogHeaderError);
		return 0;
	}

	//Get window information (and way to get handle)
	SDL_VERSION(&_attributes._windowInfo->version);
	
	if(SDL_FALSE == SDL_GetWindowWMInfo(getSDLWindow(),_attributes._windowInfo)) {
		
		g_debug->header("Error while getting SDL window manager info: " , DebugApi::LogHeaderError);
		g_debug->header(std::string(SDL_GetError()),DebugApi::LogHeaderError);
		return 0;
	}
	
	// Hide cursor
#ifndef _DEBUG
	SDL_ShowCursor(SDL_DISABLE);
#endif

	g_debug->header("Window created", DebugApi::LogHeaderOk);

	g_debug->header("Mode:", DebugApi::LogHeaderInfo);
	if (!_attributes._fullscreen)
		g_debug->dataChar("Window", 1);
	else
		g_debug->dataChar("Full screen", 1);

	// Puts mouse on the center of the window. Prevents weird behaviours when running in windowed mode
	SDL_WarpMouseInWindow(_attributes._sdlWindow,getWidth()/2,getHeight()/2);


	g_debug->header("Window OK", DebugApi::LogHeaderEnd);

	_ok = true;

	return 1;
}

/*
==================
Frees the manager and all the objects that it contains.
==================
*/
void IND_Window::end() {
	if (_ok) {
		g_debug ->header("Finalizing the window", DebugApi::LogHeaderBegin);
		if (_attributes._sdlWindow) {
			SDL_DestroyWindow(getSDLWindow());
			_attributes._sdlWindow = NULL;
		}
		freeVars();
		g_debug ->header("Window finalized", DebugApi::LogHeaderEnd);
        
		_ok = false;
	}
}

/*
==================
Window destruction
==================
*/
void IND_Window::destroyIndieLibWindow() {
	DISPOSE(_attributes._windowInfo);
}

/*
==================
Resetting
==================
*/
bool IND_Window::reset(IND_WindowProperties& props) {
	SDL_DisplayMode dMode;
	dMode.driverdata = 0;
	dMode.h = props._height;
	dMode.w = props._width;
	dMode.refresh_rate = 0;
	
	if (props._bpp != 32) {
		g_debug->header("Error creating SDL window", DebugApi::LogHeaderError);
		g_debug->header(std::string(SDL_GetError()),DebugApi::LogHeaderError);
		return 0;
	}

    //When in fullscreen, window size can't be changed.
    //Need to switch not to fullscreen, change size and go back to fullscreen
    bool wasFullScreen (isFullScreen());
    if (wasFullScreen) {
        toggleFullScreen();
    }
    
    //The actual size change
    SDL_SetWindowSize(getSDLWindow(),props._width,props._height);
    //Format is hardcoded to rgba, this is also hardcoded on render creation.
	dMode.format = static_cast<Uint32>(SDL_PIXELFORMAT_RGBA4444);

    //Set window display mode as requested... don't know what's the exact use of this
	if (SDL_SetWindowDisplayMode(getSDLWindow(),&dMode)) {
		return false;
	}
    
	_attributes._width = props._width;
	_attributes._height = props._height;
    
    //When in fullscreen, window size can't be changed.
    //Need to switch not to fullscreen, change size and go back to fullscreen
    if (wasFullScreen) {
        toggleFullScreen();
    }
    
    //Change title and leave window in correct position
    SDL_SetWindowTitle(getSDLWindow(),props._title);
	SDL_RaiseWindow(getSDLWindow());
	SDL_SetWindowPosition(getSDLWindow(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);

	return true;
}

/*
==================
Toggle Fullscreen
==================
*/
bool IND_Window::toggleFullScreen () {
	SDL_bool notFullscreen = isFullScreen() ? SDL_FALSE : SDL_TRUE;
	if (SDL_SetWindowFullscreen(getSDLWindow(),notFullscreen)) return false;
	_attributes._fullscreen = !isFullScreen();

	SDL_RaiseWindow(getSDLWindow());
	SDL_SetWindowPosition(getSDLWindow(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);

	return true;
}

/*
==================
Init attributes
==================
*/
void IND_Window::initAttribWindow() {
	_attributes._width      = 0;
	_attributes._height     = 0;
	_attributes._bits       = 0;
	_attributes._fullscreen = false;
	_attributes._title      = (char*)"";
	_attributes._windowInfo = new SDL_SysWMinfo();
	_attributes._sdlWindow = NULL;
}

/*
==================
Init vars
==================
*/
void IND_Window::initVars() {
	initAttribWindow();
}

/*
==================
Free memory
==================
*/
void IND_Window::freeVars() {
	destroyIndieLibWindow();
}

/** @endcond */
