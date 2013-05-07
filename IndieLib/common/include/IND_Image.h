/*****************************************************************************************
 * File: IND_Image.h
 * Desc: Image object
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier L�pez L�pez (info@pixelartgames.com)
MODIFIED BY Miguel Angel Qui�ones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)

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

#ifndef _IND_IMAGE_
#define _IND_IMAGE_

#ifdef linux
#include <string.h>
#endif

#include "IND_Object.h"

// ----- Forward declarations ------
struct FIBITMAP;

// --------------------------------------------------------------------------------
//									 IND_Image
// --------------------------------------------------------------------------------

/**
@defgroup IND_Image IND_Image
@ingroup Objects
IND_Image class managed by IND_ImageManager for loading, saving and applying filters to images, click in IND_Image to see all the methods of this class.
*/
/**@{*/

/**
@b IND_Image is an image object from the class ::IND_ImageManager. Read the explanation in ::IND_ImageManager for more details.
*/
class LIB_EXP IND_Image : public IND_Object {
public:

	// ----- Public methods ------
    static IND_Image* newImage();
    
    virtual void destroy();
    
	bool clear(BYTE pR, BYTE pG, BYTE pB, BYTE pA);
	bool getPixel(int pX, int pY, BYTE *pR, BYTE *pG, BYTE *pB, BYTE *pA);
	bool putPixel(int pX, int pY, BYTE pR, BYTE pG, BYTE pB, BYTE pA);
	bool flipHorizontal();
	bool flipVertical();
	bool convert(IND_ColorFormat pColorFormat, int pBpp);
	bool setAlpha(BYTE pR, BYTE pG, BYTE pB);
	bool pasteImage(IND_Image *pIm, int pX, int pY, int pAlpha);
	bool gaussianBlur(int pIter);
	bool contrast(double pPercentage);
	bool equalize();
	bool setGamma(double pGamma);
	bool negative();
	bool adjustColors(double pBrightness, double pContrast, double pGamma, bool pNegative);
	bool noisify(float pNoise);
	bool pixelize(int pPixSize);
	bool sharpen(float pFactor, int pIter);
	bool scale(int pWidth, int pHeight);
	bool edgeDetect1();
	bool edgeDetect2();
	bool emboss();
	bool rotate(double pAngle);
	bool invertAlpha();
	bool saturation(float pSaturation);

	// ----- Public gets ------

	//! This function returns true if the image is loaded, otherwise false.
	int isImageLoaded()  {
		return _image._handle? 1 : 0;
	}
	//! This function returns the width in pixels of the image.
	int getWidth()  {
		return _image._width;
	}
	//! This function returns the height in pixels of the image
	int getHeight()  {
		return _image._height;
	}

	//! This function returns the BITS per pixel of the image
	int getBpp()  {
		return _image._bpp;
	}

	//! This function returns the BYTES per pixel of the image.
	int getBytespp()  {
		return _image._bytespp;
	}
	//! This function returns the image format in an integer. See ::IND_ColorFormat.
	int getFormatInt()  {
		return _image._format;
	}
	//! This function returns the format from the image in a text string. See ::IND_ColorFormat.
	string getFormatString()  {
		return formatToString(getFormatInt());
	}
	//! This function returns the extension that had the image when it was stored in a file. For example if the image was "loover.jpg" the function will return "jpg".
	char *getExtension()  {
		return _image._ext;
	}
    
	/** 
     This function returns the pointer to the memory bytes which forms the image. It is a function very useful for accessing to the image directy and for modifying it or reading it.
	 Be aware that modifying the data via the pointer is not the recommended way. See Freeimage docs (underlying library used by IndieLib) to get more details. 
     */
	BYTE *getPointer()  {
		return _image._pointer;
	}
    
	//! This function returns the name of the image in a string of chars.
	char *getName()  {
		return _image._name;
	}

    //! Gets the underlying system (FreeImage) handle. Only recommended to use if you really need to make some advanced image operations
	FIBITMAP* getFreeImageHandle() {
		return _image._handle;
	}

private:
	/** @cond DOCUMENT_PRIVATEAPI */
    IND_Image() {}
    virtual ~IND_Image() {}
    
	// ----- Structures ------

	//TYPE
	struct structImage {
		int         _width;             // Image width
		int         _height;            // Image height
		int         _bpp;               // bits per pixel
		int			_bytespp;           // Bytes per pixel
		IND_ColorFormat  _format;       // Image format
		char        *_ext;              // Image file extension
		BYTE        *_pointer;          // Image data pointer
		char        *_name;             // Image name
		FIBITMAP    *_handle;           // FreeImage handle

		structImage() {
			_width = _height = _bpp = _bytespp = _format = 0;
			_ext = new char [10];
			_pointer = NULL;
			_name = new char [MAX_TOKEN];
			_handle = NULL;
		}

		~structImage() {
			DISPOSEARRAY(_ext);
			DISPOSEARRAY(_name);
		}
	};
	typedef struct structImage IMAGE;

	IMAGE _image;

	// ----- Private sets ------

	void setWidth(int pWidth)    {
		_image._width = pWidth;
	}
	void setHeight(int pHeight)   {
		_image._height = pHeight;
	}
	void setBpp(int pBpp)      {
		_image._bpp = pBpp;
	}

	void setBytespp (int pBytespp) {
		_image._bytespp = pBytespp;
	}

	void setFormatInt(int pFormat)   {
		_image._format = pFormat;
	}
	void setExtension(const char *pExt)    {
		strcpy(_image._ext,pExt);
	}
	void setPointer(BYTE *pPtr)    {
		_image._pointer = pPtr;
	}
	void setName(const char *pName)   {
		strcpy(_image._name, pName);
	}
		
	void setFreeImageHandle(FIBITMAP* pHandle) {
		_image._handle = pHandle;
	}

	// ----- Private methods -----

	void setAlphaChannel(BYTE pR, BYTE pG, BYTE pB);
	string formatToString(IND_ColorFormat pColorFormat);

	// ----- Friends -----

	friend class IND_ImageManager;
    /** @endcond */
};
/**@}*/

#endif // _IND_IMAGE_
