/*****************************************************************************************
 * Desc: Node class. Each node is a backdrop of the map that contains and IndieLib entity
 * that holds all its attributes
 *
 * gametuto.com - Javier López López (javilop.com)
 *
 *****************************************************************************************
 *
 * Creative Commons - Attribution 3.0 Unported
 * You are free:
 *	to Share — to copy, distribute and transmit the work
 *	to Remix — to adapt the work
 *
 * Under the following conditions:
 * Attribution. You must attribute the work in the manner specified by the author or licensor 
 * (but not in any way that suggests that they endorse you or your use of the work).
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
