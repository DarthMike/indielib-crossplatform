/*****************************************************************************************
 * File: IND_Frame.h
 * Desc: Frame object
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


#ifndef _IND_FRAME_
#define _IND_FRAME_

#include <list>
#include "IND_Image.h"
#include "IND_Surface.h"
#include "Defines.h"

// --------------------------------------------------------------------------------
//									 IND_Frame
// --------------------------------------------------------------------------------
/**
 Contains data of a frame in an IND_Animation entity. Mainly is for private use in IND_Animation objects.
 */
class IND_Frame {
private:
	/** @cond DOCUMENT_PRIVATEAPI */
	// ----- Structures ------

	// Frame
	struct structFrame {
		char *_name;
		IND_Image *_image;
		IND_Surface *_surface;
		int _offsetX;
		int _offsetY;
		list <BOUNDING_COLLISION *> *_listBoundingCollision;
		structFrame() : _name(NULL), _image(NULL), _surface(NULL), _offsetX(0), _offsetY(0),_listBoundingCollision(NULL){
			_name       = new char [MAX_TOKEN];
			_image      = 0;
			_surface    = 0;
			_offsetX = _offsetY = 0;
            _listBoundingCollision = new list<BOUNDING_COLLISION*>;
		}
        ~structFrame() {
            DISPOSEARRAY(_name);
            list<BOUNDING_COLLISION*>::iterator itr;
            for (itr = _listBoundingCollision->begin(); itr != _listBoundingCollision->end();++itr) {
                delete  (*itr);
            }
            DISPOSE(_listBoundingCollision);
        }
	};
	typedef struct structFrame A_FRAME;

	A_FRAME _frame;

	// ----- Private sets ------

	void setName(const char *pName)       {
		strcpy(_frame._name,pName);
	}
	void setImage(IND_Image *pIm)    {
		_frame._image = pIm;
	}
	void setSurface(IND_Surface *pSu)  {
		_frame._surface = pSu;
	}
	void SetOffsetX(int pOffsetX)      {
		_frame._offsetX = pOffsetX;
	}
	void SetOffsetY(int pOffsetY)      {
		_frame._offsetY = pOffsetY;
	}

	// ----- Private gets ------

	char        *getName()  {
		return _frame._name;
	}
	IND_Image   *getImage()  {
		return _frame._image;
	}
	IND_Surface *getSurface()  {
		return _frame._surface;
	}
	int         GetOffsetX()  {
		return _frame._offsetX;
	}
	int         GetOffsetY()  {
		return _frame._offsetY;
	}
	list <BOUNDING_COLLISION *> *GetListBoundingCollision() {
		return _frame._listBoundingCollision;
	}

	// ----- Friends -----

	friend class IND_Animation;
	friend class IND_AnimationManager;
	friend class IND_Entity2dManager;
    /** @endcond */
};

#endif // _IND_SEQUENCE_
