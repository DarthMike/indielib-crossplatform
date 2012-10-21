/*****************************************************************************************
 * File: IND_Entity2dManager.h
 * Desc: Manager de entidades. Una entidad contiene el un objeto gráfico y gestiona
 * sus atributos. Además, facilitan el uso de animaciones.
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


#ifndef _IND_ENTITY2DMANAGER_
#define _IND_ENTITY2DMANAGER_

// ----- Includes -----

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
	void     renderCollisionAreas(BYTE pR, BYTE pG, BYTE pB, BYTE pA);
	void     renderCollisionAreas(int pLayer, BYTE pR, BYTE pG, BYTE pB, BYTE pA);
	/**
	@b Operation:
	
	This function renders (blits on the screen) all the grid areas of the entities.
	
	This method is equivalent to rendering the grid areas of layer number 0.
	*/	
	inline void     renderGridAreas(BYTE pR, BYTE pG, BYTE pB, BYTE pA) {
		renderGridAreas(0, pR, pG, pB, pA);
	};
	void     renderGridAreas(int pLayer, BYTE pR, BYTE pG, BYTE pB, BYTE pA);

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
