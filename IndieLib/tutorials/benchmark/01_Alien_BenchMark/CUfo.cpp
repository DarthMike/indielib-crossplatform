/*****************************************************************************************
 * File: CUfo.h
 * Desc: Ufo object
 *****************************************************************************************/

// ----- Includes -----

#include "CUfo.h"

// --------------------------------------------------------------------------------
//								Initialization / Destruction
// --------------------------------------------------------------------------------

/*
==================
Init
==================
*/
CUfo::CUfo ()
{
	// Get IndieLib singleton object
	mI = CIndieLib::instance();
	mUfo = IND_Entity2d::newEntity2d();
}


/*
==================
Init
==================
*/
void CUfo::Init (float pX, float pY, IND_Animation *pUfoAnimation)
{
	mI->_entity2dManager->add(0, mUfo);			// Entity adding in layer 0 (terrain)
	mUfo->setAnimation(pUfoAnimation);
	mUfo->setPosition(pX, pY, 2);
}
