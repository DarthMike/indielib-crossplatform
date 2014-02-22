/*****************************************************************************************
 * File: IndieLib.h
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


#ifndef _INDIELIB_H_
#define _INDIELIB_H_


#include "Defines.h"

// --------------------------------------------------------------------------------
//									  IndieLib
// --------------------------------------------------------------------------------

/**
@defgroup IndieLib_Init_End Init / End
@ingroup Init_End
IndieLib class for initializing and ending the engine.
*/
/**@{*/

/**
@b IndieLib initialization and destruction.
*/
class LIB_EXP IndieLib {
public:

	// ----- Public methods -----

	static bool init(IND_InitializationMode pMode);
	static void end();

};
/**@}*/

#endif // _INDIELIB_H_
