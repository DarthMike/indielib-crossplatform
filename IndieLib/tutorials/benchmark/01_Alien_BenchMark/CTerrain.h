/*****************************************************************************************
 * File: CTerrain.h
 * Desc: Tiled terrain
 *****************************************************************************************/

#ifndef _CTERRAIN_
#define _CTERRAIN_

// ------ Includes -----

#include "CIndieLib_vc2008.h"

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

	IND_Entity2d **mHoles;		// Craters
private:

	// ----- Private ------

	CIndieLib *mI;					// IndieLib object
	IND_Entity2d *mTerrain;				// Wrapped texture

	int mNumHoles;
	int mNumBlocksX, mNumBlocksY;			// Number of horizontal and vertical tiles
};


#endif // _CTERRAIN_
