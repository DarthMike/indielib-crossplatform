/*****************************************************************************************
 * File: CollisionParser.h
 * Desc: Parse collision files
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


#ifndef _COLLISIONPARSER_
#define _COLLISIONPARSER_

// ----- Includes -----

#include <list>


// --------------------------------------------------------------------------------
//									CollisionParser
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */
class CollisionParser {
public:

	static CollisionParser *instance();

	// ----- Methods -----

	bool parseCollision(list <BOUNDING_COLLISION *> *pBList, const char *pFile);
	void setBoundingTriangle(list <BOUNDING_COLLISION *> *pBList, const char *pId, int pAx, int pAy, int pBx, int pBy, int pCx, int pCy);
	void setBoundingCircle(list <BOUNDING_COLLISION *> *pBList, const char *pId, int pOffsetX, int pOffsetY, int pRadius);
	void setBoundingRectangle(list <BOUNDING_COLLISION *> *pBList, const char *pId, int pOffsetX, int pOffsetY, int pWidth, int pHeight);
	void deleteBoundingAreas(list <BOUNDING_COLLISION *> *pBList, const char *pId);

protected:

	CollisionParser()  {}
	CollisionParser(const CollisionParser &);
	CollisionParser &operator = (const CollisionParser &);

private:
	static CollisionParser *_pinstance;
};

/** @endcond */

#endif // _COLLISIONPARSER_
