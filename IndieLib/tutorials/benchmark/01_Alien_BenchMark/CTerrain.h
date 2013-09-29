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


#ifndef _CTERRAIN_
#define _CTERRAIN_

// ------ Includes -----

#include "CIndieLib.h"

#include "IND_Entity2d.h"
#include "IND_Surface.h"


// --------------------------------------------------------------------------------
//									 CTerrain
// --------------------------------------------------------------------------------

class CTerrain
{
public:

	CTerrain(int pNumBlocksX, int pNumBlocksY, int pNumHoles, 
						IND_Surface *pTerrainTile, IND_Surface *pHole1, IND_Surface *pHole2);

	~CTerrain();

	IND_Entity2d **mHoles;          // Craters
private:

	// ----- Private ------

	CIndieLib *mI;					// IndieLib object
	IND_Entity2d *mTerrain;			// Wrapped texture

	int mNumHoles;
	int mNumBlocksX, mNumBlocksY;	// Number of horizontal and vertical tiles
};


#endif // _CTERRAIN_
