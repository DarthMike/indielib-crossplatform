/*****************************************************************************************
 * Desc: Parse collision files
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
