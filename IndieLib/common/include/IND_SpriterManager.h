/*****************************************************************************************
 * File: IND_SpriterManager.h
 * Desc: Spriter manager
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (javierlopezpro@gmail.com)

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

#ifndef _IND_SPRITERMANAGER_
#define _IND_SPRITERMANAGER_

// ----- Includes -----

#include <list>
#include <vector>

// ----- Forward declarations -----
class IND_SurfaceManager;
class IND_ImageManager;
class IND_Image;
class IND_Timer;
class CollisionParser;
class IND_SpriterEntity;


// --------------------------------------------------------------------------------
//									IND_SpriterManager
// --------------------------------------------------------------------------------

class LIB_EXP IND_SpriterManager {
public:

	// ----- Init/End -----

	IND_SpriterManager(): _ok(false)  { }
	~IND_SpriterManager()              {
		end();
	}

	bool    init();
	void    end();
	bool    isOK() {
		return _ok;
	}

	// ----- Public methods -----

	bool addSpriterFile(list<IND_SpriterEntity*> *pNewSpriterEntityList,const char *pSCMLFileName);	

	bool remove(IND_SpriterEntity *pSen);


private:

	// ----- Private -----

	bool _ok;

	// ----- Enums -----


	// ----- Objects -----

	IND_ImageManager *_imageManager;

	// ----- Containers -----

	list <IND_SpriterEntity *> *_listSpriterEntity;

	// ----- Private methods -----


	void        addToList(IND_SpriterEntity *pNewEntity);
	void        delFromlist(IND_SpriterEntity *pEn);
	IND_Image   *loadImage(char *pName);
	bool        remove(IND_SpriterEntity *pEn, bool pType);

	bool        parseSpriterData(list<IND_SpriterEntity*> *pNewSpriterEntityList,const char *pSCMLFileName);

	void        writeMessage();
	void        initVars();
	void        freeVars();
};

#endif // _IND_SPRITERMANAGER_
