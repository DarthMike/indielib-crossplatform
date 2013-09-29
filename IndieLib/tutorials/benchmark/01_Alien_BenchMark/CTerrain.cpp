/*****************************************************************************************
 * File: CTerrain.h
 * Desc: Tiled terrain
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


// ----- Includes -----

#include "CTerrain.h"

// --------------------------------------------------------------------------------
//		Initialization / Destruction
// --------------------------------------------------------------------------------

/*
==================
Init
==================
*/
CTerrain::CTerrain(int pNumBlocksX, int pNumBlocksY, int pNumHoles, IND_Surface *pTerrainTile, IND_Surface *pHole1, IND_Surface *pHole2)
{
	// Get IndieLib singleton object
	mI = CIndieLib::instance();

	mNumBlocksX = pNumBlocksX;
	mNumBlocksY = pNumBlocksY;
	mNumHoles = pNumHoles;
	// ----- Tiled terrain: Our terrain has pNumBlocksX x pNumBlocksY tiles of 512x512 pixels  each one ----

	// We create a tiled surface using ToogleWrap and SetRegion methods
	mTerrain = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(0, mTerrain);	
	mTerrain->setSurface(pTerrainTile);
	mTerrain->toggleWrap(1);
	mTerrain->setRegion(0, 0, pNumBlocksX * pTerrainTile->getWidth(), pNumBlocksY * pTerrainTile->getHeight());

	// ----- Holes: Some craters of two types with random position and sizes ----

	// Memory allocation in a bidimensional array for the tiles
	mHoles = new IND_Entity2d*[pNumHoles];
	for (int i = 0; i < pNumHoles; i++ ) {
		mHoles[i] = IND_Entity2d::newEntity2d();
	}
	for (int i = 0; i < pNumHoles; i++)
	{
		// Entity adding in layer 0 (terrain)
		mI->_entity2dManager->add(0, mHoles[i]);			

		// 50% chance of creating a hole type 1, 50% type 2
		if (mI->_math->randnum(0, 1))
		{
			mHoles[i]->setSurface(pHole1);	
		}
		else
		{
			mHoles[i]->setSurface(pHole2);

		}

		// Setting the collision areas
		mHoles[i]->setBoundingCircle("area_crater", 250, 250, 270);
		mHoles[i]->setBoundingCircle("area_crater_ufo", 250, 250, 200);

		// Random position for the hole
		mHoles[i]->setPosition((float) mI->_math->randnum(0, pNumBlocksX * pTerrainTile->getWidth()),
								(float) mI->_math->randnum(0, pNumBlocksY * pTerrainTile->getHeight()),
								1);

		// Ramdom size (50% - 100%) for the hole
		int mScale = mI->_math->randnum(50, 100);
		mHoles[i]->setScale((float) mScale / 100.0f, (float) mScale / 100.0f);

		// Check if the hole collides with another hole
		for (int j = 0; j < i; j++)
		{
			// If the new hole collides with a previos hole, delete it
			if (mI->_entity2dManager->isCollision(mHoles[i], "area_crater", mHoles[j], "area_crater"))
			{
				mI->_entity2dManager->remove(mHoles[i]);
				break;
			}
		}
	}
}


/*
==================
End
==================
*/
CTerrain::~CTerrain()
{
	delete [] mHoles;
	mI->_entity2dManager->end(); 
}
