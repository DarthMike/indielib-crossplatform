/*****************************************************************************************
 * File: IND_Entity2dManager.h
 * Desc: Manager of 2D entities. Contains the graphic objects and manages their attributes.
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


#ifndef _IND_ENTITY2DMANAGER_
#define _IND_ENTITY2DMANAGER_

// ----- Includes -----

#include "Defines.h"
#include <algorithm>
#include <vector>
#include <list>

// ----- Forward declarations

class IND_SurfaceManager;
class IND_AnimationManager;
class IND_Render;
class CollisionParser;
class IND_Entity2d;
class IND_Math;

// ----- Defines -----

#define NUM_LAYERS 64

// --------------------------------------------------------------------------------
//									IND_Entity2dManager
// --------------------------------------------------------------------------------

/**
@defgroup IND_Entity2dManager IND_Entity2dManager
@ingroup EntityManagers
Manager of ::IND_Entity2d objects. Click in ::IND_Entity2dManager to see all the methods of this class.
*/
/**@{*/

/**
Manager of ::IND_Entity2d objects. Used for storing these type of objects.

Other uses of this class:
- Check collisions between the entities stored
- Function for drawing all the entities stored
*/
class LIB_EXP IND_Entity2dManager {
public:

	// ----- Init/End -----

	IND_Entity2dManager(): _ok(false),_render(NULL),_math(NULL)  { }
	~IND_Entity2dManager()              {
		end();
	}

	bool    init(IND_Render *pRender);
	void    end();
	bool    isOK() const;

	// ----- Methods -----

	bool            add(IND_Entity2d *pNewEntity2d);
	bool            add(int pLayer, IND_Entity2d *pNewEntity2d);
	bool            remove(IND_Entity2d *pEn);

	/**
	@b Operation:
	
	This function renders (draws on the screen) all the entities of the manager.
	
	Using this method is equivalent to rendering the entities of layer number 0.
	*/
	inline void	renderEntities2d() {
		renderEntities2d(0);
	};
	void     renderEntities2d(int pLayer);
	void     renderCollisionAreas(unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA);
	void     renderCollisionAreas(int pLayer, unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA);
	/**
	@b Operation:
	
	This function renders (blits on the screen) all the grid areas of the entities.
	
	This method is equivalent to rendering the grid areas of layer number 0.
	*/	
	inline void     renderGridAreas(unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA) {
		renderGridAreas(0, pR, pG, pB, pA);
	};
	void     renderGridAreas(int pLayer, unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA);

	bool     isCollision(IND_Entity2d *pEn1, const char *pId1, IND_Entity2d *pEn2, const char *pId2);

private:
	
    /** @cond DOCUMENT_PRIVATEAPI */

	// ----- Private -----

	bool _ok;

    static unsigned int _idTrack;
    
	IND_Render *_render;
	IND_Math *_math;

	// ----- Containers -----

	vector <IND_Entity2d *> *_listEntities2d  [NUM_LAYERS];

	// ----- Private methods -----

	bool isCollision(list <BOUNDING_COLLISION *> *pBoundingList1, list <BOUNDING_COLLISION *> *pBoundingList2,
	                 const char *pId1, const char *pId2,
	                 IND_Matrix pMat1, IND_Matrix pMat2,
	                 float pScale1, float pScale2);

	bool isNullMatrix(IND_Matrix pMat);

	void addToList(int pLayer, IND_Entity2d *pNewEntity2d);

	void writeMessage();
	void initVars();
	void freeVars();

    /** @endcond */
};
/**@}*/

#endif // _IND_ENTITY2DMANAGER_
