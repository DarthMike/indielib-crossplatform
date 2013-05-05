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