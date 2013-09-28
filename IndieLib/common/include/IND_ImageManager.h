/*****************************************************************************************
 * File: IND_ImageManager.h
 * Desc: Image Manager
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


#ifndef _IND_IMAGEMANAGER_
#define _IND_IMAGEMANAGER_

// ----- Includes -----
#include <list>


// ----- Forward declarations ----
class IND_Image;
struct FIBITMAP;
// ----- Defines -----

#define MAX_EXT_IMAGE 6


// --------------------------------------------------------------------------------
//									IND_ImageManager
// --------------------------------------------------------------------------------

/**
@defgroup IND_ImageManager IND_ImageManager
@ingroup Managers
Manager of IND_Image objects. Click in ::IND_ImageManager to see all the methods of this class.
*/
/**@{*/

/**
This manager that stores IND_Image objects works as a mediator between the image (bitmap) loaded
in memory and the IND_Surface object which finally appears in the screen. The IND_Image objects
which are stored in this class can not be rendered directly to the screen.
But this class is very useful for applying <b>filters</b> (like in Gimp or Photoshop programs) to
loaded images, and later they can be converted to IND_Surface objects, which can be directly
rendered.

Other possible uses of this class are:
- <b>Hardness maps</b>. Used for making collisions or path finding.
- GetPixel() / PutPixel()
- <b>Colorkey</b>. By changing a certain color into transparent using SetAlpha().
- Generation of new images (procedurally).
- Convert formats. Changing the number of bytes per pixel using Convert()
- Direct accesing to the bitmap. Using IND_Image::getPointer()
*/
class LIB_EXP IND_ImageManager {
public:

	// ----- Init/End -----

	IND_ImageManager(): _ok(false)  { }
	~IND_ImageManager()              {
		end();
	}

	bool    init();
	void    end();
	bool    isOK() const;

	// ----- Public methods -----

	bool add(IND_Image *pNewImage, const char *pName);
	bool add(IND_Image *pNewImage, int pWidth, int pHeight, IND_ColorFormat pColorFormat);
	bool remove(IND_Image *pIm);
	bool save(IND_Image *pIm, const char *pName);
	bool clone(IND_Image *pNewImage, IND_Image *pOldImage);
	FIBITMAP* load(const char *pName);
	bool add(IND_Image *pNewImage, FIBITMAP *pImageToBeCopied);


private:
	/** @cond DOCUMENT_PRIVATEAPI */
	// ----- Private -----

	bool _ok;                               // Manager initialization flag
	char *_supportedExt [MAX_EXT_IMAGE];    // File extensions supported

	// ----- Containers -----

	list <IND_Image *> *_listImages;

	// ----- Private Methods -----

	int defaultBppForColorFormat(IND_ColorFormat format);
	void getExtensionFromName(const char *pName, char* pExtImage);
	bool checkExtImage(const char *pExtImage);

	void addToList(IND_Image *pNewImage);
	void delFromlist(IND_Image *pIm);
	void writeMessage();
	void initVars();
	void freeVars();
    /** @endcond */
};
/**@}*/

#endif // _IND_IMAGEMANAGER_
