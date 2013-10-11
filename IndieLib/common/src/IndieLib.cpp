/*****************************************************************************************
 * File: IndieLib.cpp
 * Desc: IndieLib initialization / destruction
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
bool IndieLib::init(IND_InitializationMode pMode) {
#ifdef PLATFORM_WIN32
	#ifdef _DEBUG
	RedirectIOToConsole();
	#endif
#endif

	// g_debug initilization
	if (pMode) {
        if (!g_debug) {
            g_debug = new DebugApi();
            g_debug->init();
        }
    }

	// SDL initialization
	if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        g_debug->header(SDL_GetError(), DebugApi::LogHeaderError);
        return false;
	}

	if(0 != SDL_InitSubSystem(SDL_INIT_TIMER) ){
        g_debug->header(SDL_GetError(), DebugApi::LogHeaderError);
        return false;
	}
    
    return true;
}

/**
 * Finalizes IndieLib and frees all the memory of the managers. So, all the resources like textures, 3d meshes, etc. will be freed.
 */
void IndieLib::end() {
	g_debug->end();
	DISPOSE(g_debug);
	SDL_Quit();
}
