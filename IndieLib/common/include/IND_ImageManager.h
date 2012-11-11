/*****************************************************************************************
 * File: IND_ImageManager.h
 * Desc: Image Manager
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
