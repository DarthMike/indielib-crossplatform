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

class CollisionParser {
public:

	static CollisionParser *instance();

	// ----- Methods -----

	bool parseCollision(list <BOUNDING_COLLISION *> *pBList, char *pFile);
	void setBoundingTriangle(list <BOUNDING_COLLISION *> *pBList, char *pId, int pAx, int pAy, int pBx, int pBy, int pCx, int pCy);
	void setBoundingCircle(list <BOUNDING_COLLISION *> *pBList, char *pId, int pOffsetX, int pOffsetY, int pRadius);
	void setBoundingRectangle(list <BOUNDING_COLLISION *> *pBList, char *pId, int pOffsetX, int pOffsetY, int pWidth, int pHeight);
	void deleteBoundingAreas(list <BOUNDING_COLLISION *> *pBList, char *pId);

protected:

	CollisionParser()  {}
	CollisionParser(const CollisionParser &);
	CollisionParser &operator = (const CollisionParser &);

private:
	static CollisionParser *_pinstance;
};


#endif // _COLLISIONPARSER_
