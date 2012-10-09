/*****************************************************************************************
 * File: IndieLib.cpp
 * Desc: IndieLib initialization / destruction
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (javierlopezpro@gmail.com)

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

#include "Global.h"

// ----- Library Dependencies -----
#include "dependencies/SDL-2.0/include/SDL.h"
#ifdef PLATFORM_WIN32
#include "platform/win32/guicon.h"
#endif
// ----- Header -----

#include "IndieLib.h"


// --------------------------------------------------------------------------------
//							 Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 * Initializes IndieLib.
 * @param pMode						::IND_InitializationMode. See ::IND_InitializationMode.
 */
void IndieLib::init(IND_InitializationMode pMode) {
#ifdef PLATFORM_WIN32
	#ifdef _DEBUG
	RedirectIOToConsole();
	#endif
#endif

	// g_debug initilization
	if (pMode)  g_debug->init();

	// SDL initialization
	if(0 != SDL_Init(SDL_INIT_VIDEO)) {
		g_debug->header("Unable to initialize SDL!", 2);
	}

	if(0 != SDL_InitSubSystem(SDL_INIT_TIMER) ){
		g_debug->header("Unable to initialize SDL timer subsystem", 2);
	}
}

/**
 * Finalizes IndieLib and frees all the memory of the managers. So, all the resources like textures, 3d meshes, etc. will be freed.
 */
void IndieLib::end() {
	g_debug->end();
	DISPOSE(g_debug);
	SDL_Quit();
}
