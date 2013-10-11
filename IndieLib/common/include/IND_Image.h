/*****************************************************************************************
 * File: IND_Image.h
 * Desc: Image object
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


#ifndef _IND_IMAGE_
#define _IND_IMAGE_

#ifdef linux
#include <string.h>
#endif

#include "Defines.h"
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
    
	bool clear(unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA);
	bool getPixel(int pX, int pY, unsigned char *pR, unsigned char *pG, unsigned char *pB, unsigned char *pA);
	bool putPixel(int pX, int pY, unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA);
	bool flipHorizontal();
	bool flipVertical();
	bool convert(IND_ColorFormat pColorFormat, int pBpp);
	bool setAlpha(unsigned char pR, unsigned char pG, unsigned char pB);
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
	unsigned char *getPointer()  {
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
		unsigned char        *_pointer;          // Image data pointer
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
	void setPointer(unsigned char *pPtr)    {
		_image._pointer = pPtr;
	}
	void setName(const char *pName)   {
		strcpy(_image._name, pName);
	}
		
	void setFreeImageHandle(FIBITMAP* pHandle) {
		_image._handle = pHandle;
	}

	// ----- Private methods -----

	void setAlphaChannel(unsigned char pR, unsigned char pG, unsigned char pB);
	string formatToString(IND_ColorFormat pColorFormat);

	// ----- Friends -----

	friend class IND_ImageManager;
    /** @endcond */
};
/**@}*/

#endif // _IND_IMAGE_
