/*****************************************************************************************
 * File: Node.h
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

#ifndef _NODE_
#define _NODE_

// ------ Includes -----

#include "CIndieLib.h"
#include "IND_Entity2d.h"
#include "IND_Surface.h"
#include <vector>


// --------------------------------------------------------------------------------
//										Node
// --------------------------------------------------------------------------------

class Node
{
public:

	Node			(int pX, int pY, int pZ, int pId, int pLayer, IND_Surface *pSurface);
	~Node			();

	IND_Entity2d	*GetEntity		()				{ return mEntity;	}
	int				GetLayer		()				{ return mLayer;	}
	int				GetSurfaceId	()				{ return mId;		}

private:

	IND_Entity2d* mEntity;
	int mId;			// Surface id
	CIndieLib *mI;		// Pointer to IndieLib class
	int mLayer;			// Layer where the node is created
};

#endif // _NODE_