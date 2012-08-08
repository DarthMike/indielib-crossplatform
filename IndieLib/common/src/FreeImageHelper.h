/*
    Filename: FreeImageHelper.h
    Copyright: Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)
    Description: Helper class containing methods to calculate several parameters from/to freeImage library
    Comments: 
    Attribution:
    License: You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
             Nevertheless it would be nice if you tell me you are using something I made, just for curiosity
*/

#ifndef _FREEIMAGEHELPER
#define _FREEIMAGEHELPER

// ----- Dependencies -----
#include "dependencies/FreeImage/Dist/FreeImage.h"

#ifdef linux
#include <string.h>
#endif

// ----- Includes -----
#include "Defines.h"

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

#endif


