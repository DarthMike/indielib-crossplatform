/*****************************************************************************************
 * Desc: Singleton class for parsing collision files
 *****************************************************************************************/

/*
 IndieLib 2d library Copyright (C) 2005 Javier LÛpez LÛpez (javierlopezpro@gmail.com)
 
 This library is free software; you can redistribute it and/or modify it under the
 terms of the GNU Lesser General Public License as published by the Free Software
 Foundation; either version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License along with
 this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
 Suite 330, Boston, MA 02111-1307 USA
 */


// ----- Includes -----

/** @cond DOCUMENT_PRIVATEAPI */

#include "Defines.h"
#include "Global.h"
#include "dependencies/tinyxml/tinyxml.h"
#include "CollisionParser.h"


// --------------------------------------------------------------------------------
//							   Singleton initialization
// --------------------------------------------------------------------------------

CollisionParser *CollisionParser::_pinstance = 0;// initialize pointer

/**
 * Init singleton.
 */
CollisionParser *CollisionParser::instance() {
	if (_pinstance == 0) { // is it the first call?
		_pinstance = new CollisionParser; // create sole instance
	}
	return _pinstance; // address of sole instance
}


// --------------------------------------------------------------------------------
//								 Public methods
// --------------------------------------------------------------------------------

/**
* Bounding triangle.
* @param pBList		TODO describtion
* @param pId		TODO describtion
* @param pAx		TODO describtion
* @param pAy		TODO describtion
* @param pBx		TODO describtion
* @param pBy		TODO describtion
* @param pCx		TODO describtion
* @param pCy		TODO describtion
*/
void CollisionParser::setBoundingTriangle(list <BOUNDING_COLLISION *> *pBList, const char *pId, int pAx, int pAy, int pBx, int pBy, int pCx, int pCy) {
	BOUNDING_COLLISION *_b = new BOUNDING_COLLISION (0,pId);

	_b->_ax     = pAx;
	_b->_ay     = pAy;
	_b->_bx     = pBx;
	_b->_by     = pBy;
	_b->_cx     = pCx;
	_b->_cy     = pCy;

	pBList->push_back(_b);
}


/**
* Bounding circle.
* @param pBList		TODO describtion
* @param pId		TODO describtion
* @param pOffsetX	the x position of the center of the circle	
* @param pOffsetY	the y position of the center of the circle
* @param pRadius	the radius of the circle
*/
void CollisionParser::setBoundingCircle(list <BOUNDING_COLLISION *> *pBList, const char *pId, int pOffsetX, int pOffsetY, int pRadius) {
	BOUNDING_COLLISION *_b = new BOUNDING_COLLISION(1,pId);

	_b->_posX       = pOffsetX;
	_b->_posY       = pOffsetY;
	_b->_radius     = pRadius;

	pBList->push_back(_b);
}


/**
* Bounding rectangle.
* @param pBList		TODO describtion 	
* @param pId		TODO describtion
* @param pOffsetX	the x position of the right upper corner of the rectangle
* @param pOffsetY	the y position of the right upper corner of the rectangle
* @param pWidth		the width of the rectangle
* @param pHeight	the height of the rectangle
*/
void CollisionParser::setBoundingRectangle(list <BOUNDING_COLLISION *> *pBList, const char *pId, int pOffsetX, int pOffsetY, int pWidth, int pHeight) {
	// First triangle
	BOUNDING_COLLISION *mB1 = new BOUNDING_COLLISION(0,pId);

	mB1->_ax    = pOffsetX;
	mB1->_ay    = pOffsetY;
	mB1->_bx    = pOffsetX + pWidth;
	mB1->_by    = pOffsetY;
	mB1->_cx    = pOffsetX;
	mB1->_cy    = pOffsetY + pHeight;

	pBList->push_back(mB1);

	// Second triangle
	BOUNDING_COLLISION *mB2 = new BOUNDING_COLLISION(0,pId);;

	mB2->_ax    = pOffsetX + pWidth;
	mB2->_ay    = pOffsetY;
	mB2->_bx    = pOffsetX + pWidth;
	mB2->_by    = pOffsetY + pHeight;
	mB2->_cx    = pOffsetX;
	mB2->_cy    = pOffsetY + pHeight;

	pBList->push_back(mB2);
}


/**
* Parses a XML collision file.
* Uses Tinyxml
* @param pBList		TODO describtion
* @param pFile		the filepath of the file
*/
bool CollisionParser::parseCollision(list <BOUNDING_COLLISION *> *pBList, const char *pFile) {
	TiXmlDocument   *mXmlDoc = new TiXmlDocument(pFile);

	// Fatal error, cannot load
	if (!mXmlDoc->LoadFile()) {
        DISPOSE(mXmlDoc);
        return 0;
    }

	// Document root
	TiXmlElement *mXBoundingAreas = 0;
	mXBoundingAreas = mXmlDoc->FirstChildElement("bounding_areas");

	if (!mXBoundingAreas) {
		g_debug->header("Invalid name for document root, should be <bounding_areas>", DebugApi::LogHeaderError);
		mXmlDoc->Clear();
		delete mXmlDoc;
		return 0;
	}

	// ----- Triangle -----
	TiXmlElement *mXTriangle = 0;
	mXTriangle = mXBoundingAreas->FirstChildElement("triangle");

	while (mXTriangle) {
		if (mXTriangle->Attribute("id") &&
		        mXTriangle->Attribute("ax") &&
		        mXTriangle->Attribute("ay") &&
		        mXTriangle->Attribute("bx") &&
		        mXTriangle->Attribute("by") &&
		        mXTriangle->Attribute("cx") &&
		        mXTriangle->Attribute("cy")) {
			setBoundingTriangle(pBList,
			                    mXTriangle->Attribute("id"),
			                    atoi(mXTriangle->Attribute("ax")),
			                    atoi(mXTriangle->Attribute("ay")),
			                    atoi(mXTriangle->Attribute("bx")),
			                    atoi(mXTriangle->Attribute("by")),
			                    atoi(mXTriangle->Attribute("cx")),
			                    atoi(mXTriangle->Attribute("cy")));
		} else {
			g_debug->header("The triangle doesn't have all the attributes", DebugApi::LogHeaderError);
			mXmlDoc->Clear();
			delete mXmlDoc;
			return 0;
		}

		// Move to the next element
		mXTriangle = mXTriangle->NextSiblingElement("triangle");
	}

	// ----- Circle -----
	TiXmlElement *mXCircle = 0;
	mXCircle = mXBoundingAreas->FirstChildElement("circle");

	while (mXCircle) {
		if (mXCircle->Attribute("id") &&
		        mXCircle->Attribute("x") &&
		        mXCircle->Attribute("y") &&
		        mXCircle->Attribute("radius")) {
			setBoundingCircle(pBList,
			                  mXCircle->Attribute("id"),
			                  atoi(mXCircle->Attribute("x")),
			                  atoi(mXCircle->Attribute("y")),
			                  atoi(mXCircle->Attribute("radius")));
		} else {
			g_debug->header("The circle doesn't have all the attributes", DebugApi::LogHeaderError);
			mXmlDoc->Clear();
			delete mXmlDoc;
			return 0;
		}

		// Move to the next element
		mXCircle = mXCircle->NextSiblingElement("circle");
	}

	// ----- Rectangle -----
	TiXmlElement *mXRectangle = 0;
	mXRectangle = mXBoundingAreas->FirstChildElement("rectangle");

	while (mXRectangle) {
		if (mXRectangle->Attribute("id") &&
		        mXRectangle->Attribute("x") &&
		        mXRectangle->Attribute("y") &&
		        mXRectangle->Attribute("width") &&
		        mXRectangle->Attribute("height")) {
			setBoundingRectangle(pBList,
			                     mXRectangle->Attribute("id"),
			                     atoi(mXRectangle->Attribute("x")),
			                     atoi(mXRectangle->Attribute("y")),
			                     atoi(mXRectangle->Attribute("width")),
			                     atoi(mXRectangle->Attribute("height")));
		} else {
			g_debug->header("The rectangle doesn't have all the attributes", DebugApi::LogHeaderError);
			mXmlDoc->Clear();
			delete mXmlDoc;
			return 0;
		}

		// Move to the next element
		mXRectangle = mXRectangle->NextSiblingElement("rectangle");
	}

	// Delete our allocated document and return success ;)
	mXmlDoc->Clear();
	delete mXmlDoc;

	return 1;
}


/**
* Delete all the bounding areas with a certain id.
* @param pBList		TODO describtion
* @param pId		id of the the bounding areas, "*" is all areas. 
*/
void CollisionParser::deleteBoundingAreas(list <BOUNDING_COLLISION *> *pBList, const char *pId) {
	bool mExit = 0;

	while (!mExit) {
		mExit = 1;
		list <BOUNDING_COLLISION *>::iterator _listIter;
		for (_listIter  = pBList->begin();
		        _listIter  != pBList->end();
		        _listIter++) {
			// Check the id, if it's equal, delete the area
            const char* const identifier = (*_listIter)->getId();
			if (!strcmp(identifier, pId) || !strcmp(pId, "*")) {
				DISPOSE((*_listIter));
				pBList->remove((*_listIter));
				mExit = 0;
				break;
			}
		}
	}
}

/** @endcond */
