/*****************************************************************************************
 * File: IND_TmxMap.h
 * Desc: IND_TmxMap object
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


#ifndef _IND_TMXMAP_
#define _IND_TMXMAP_


#ifdef linux
#include <string.h>
#endif

#include "Defines.h"
#include "IND_Object.h"
#include "dependencies/TmxParser/Tmx.h"

// ----- Forward declarations ------

struct FIBITMAP;

// --------------------------------------------------------------------------------
//									 IND_TmxMap
// --------------------------------------------------------------------------------

/*!
\defgroup IND_Image IND_Image
\ingroup Objects
IND_Image class managed by IND_ImageManager for loading, saving and applying filters to images, click in IND_Image to see all the methods of this class.
*/
/**@{*/

/*!
\b IND_Image is an image object from the class ::IND_ImageManager. Read the explanation in ::IND_ImageManager for more details.
*/
class LIB_EXP IND_TmxMap : IND_Object {
public:
    
    static IND_TmxMap* newTmxMap();
    virtual void destroy();

	// ----- Public methods ------

	//bool clear(unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA);
	//bool getPixel(int pX, int pY, unsigned char *pR, unsigned char *pG, unsigned char *pB, unsigned char *pA);

	// ----- Public gets ------
	
	//! This function returns the name of the image in a string of chars.
	char* getName()  {
		return _tmxMap._name;
	}

	Tmx::Map* getTmxMapHandle() {
		return _tmxMap._handle;
	}
    
    FIBITMAP* getImage() {
        return _tmxMap._image;
    }
    
    char* getImagePath()  {
		return _tmxMap._imagePath;
	}

private:

	/** @cond DOCUMENT_PRIVATEAPI */

    IND_TmxMap() {}
    virtual ~IND_TmxMap() {}
    
	// ----- Structures ------

	//TYPE
	struct structTmxMap {
		char        *_name;             // Map name
		Tmx::Map    *_handle;           // Map handle
        FIBITMAP    *_image;            // Map tilesheet image
        char        *_imagePath;        // Map tilesheet imagepath

		structTmxMap() {
			_name = new char [128];
			_handle = NULL;
            _image  = NULL;
            _imagePath = new char [128];
		}

		~structTmxMap() {
			DISPOSEARRAY(_name);
            // TODO: properly dispose the TMX map structure..
            //DISPOSE(_image);
            DISPOSEARRAY(_imagePath);
		}
	};
	typedef struct structTmxMap TmxMap;

	TmxMap _tmxMap;
    
	// ----- Private sets ------

	void setName(const char *pName)   {
		strcpy(_tmxMap._name, pName);
	}
		
	void setTmxMapHandle(Tmx::Map* pHandle) {
		_tmxMap._handle = pHandle;
	}
	
    void setImage(FIBITMAP* pImage) {
		_tmxMap._image = pImage;
	}
    
    void setImagePath(const char *pImagePath)   {
		strcpy(_tmxMap._imagePath, pImagePath);
	}
        
	// ----- Private methods -----

	//void setAlphaChannel(unsigned char pR, unsigned char pG, unsigned char pB);
	//string formatToString(IND_ColorFormat pColorFormat);

	// ----- Friends -----

	friend class IND_TmxMapManager;

    /** @endcond */
};
/**@}*/

#endif //  _IND_TMXMAP_
