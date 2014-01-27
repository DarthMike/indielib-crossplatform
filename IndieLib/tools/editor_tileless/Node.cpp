/*****************************************************************************************
 * File: Node.cpp
 * Desc: Node class. Each node is a backdrop of the map that contains an IndieLib entity
 *       that holds all its attributes
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

// ------ Includes -----

#include "Node.h"

/* 
======================================									
Init
====================================== 
*/
Node::Node (int pX, int pY, int pZ, int pId, int pLayer, IND_Surface *pSurface)
{
	// Get IndieLib instante
	mI = CIndieLib::instance();

	// Entity 
	mEntity = IND_Entity2d::newEntity2d();

	// Surface id
	mId = pId;

	// First, we have to add the entity to the manager
	mI->_entity2dManager->add (pLayer, mEntity);

	mEntity->setSurface (pSurface); 						// Set the surface into the entity
	mEntity->setHotSpot (0.5f, 0.5f); 					// Set the hotspot (pivot point) centered
	mEntity->setPosition ((float) pX, (float) pY, pZ);	// Set the position
	mLayer = pLayer;									// Layer where it is created
		
	// Set a collision bounding rectangle. This is used for checking collisions between the mouse pointer
	// and the entity, in order to interact with it
	mEntity->setBoundingRectangle ("editor", 0, 0, pSurface->getWidth(), pSurface->getHeight());
	mEntity->showGridAreas (false);
}


/* 
======================================									
End
====================================== 
*/
Node::~Node() 
{
	mI->_entity2dManager->remove (mEntity);
}
