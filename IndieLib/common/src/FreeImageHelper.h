/*****************************************************************************************
 * File: FreeImageHelper.h
 * Description: Helper class containing methods to calculate several parameters from/to
 *              the freeImage library
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


#ifndef _FREEIMAGEHELPER
#define _FREEIMAGEHELPER

// ----- Dependencies -----
#include "dependencies/FreeImage/Dist/FreeImage.h"

#ifdef linux
#include <string.h>
#endif

// ----- Includes -----
#include "Defines.h"

/** @cond DOCUMENT_PRIVATEAPI */

class FreeImageHelper {
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	FreeImageHelper() {
	}
	~FreeImageHelper() {
	}
	//----- GET/SET FUNCTIONS -----

	//----- OTHER FUNCTIONS -----
	static int calculateINDFormat(FIBITMAP* pHandle);
	static FREE_IMAGE_COLOR_TYPE calculateFIColorType(int pIndFormat);
	static bool isLittleIndian();
	static bool isImageFormatSupported(FREE_IMAGE_FORMAT format);
	static void getImageFormatName(FREE_IMAGE_FORMAT format, char* pExtImage);
	static FIBITMAP* convertBpp(FIBITMAP* pHandle, int pNewBpp);
	static FIBITMAP* convertColorFormat(FIBITMAP* pHandle, int pNewFormat);
	//----- PUBLIC VARIABLES ------

private:
	//----- INTERNAL VARIABLES -----


	//----- INTERNAL FUNCTIONS -----
};

/** @endcond */

#endif


