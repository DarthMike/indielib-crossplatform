/*****************************************************************************************
 * Desc: PrimitiveTests class
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


#include "../../CIndieLib.h"
#include "PrimitiveTests.h"
#include "IND_Entity2d.h"

static IND_Point g_vertPoly2[] = { IND_Point(440, 200),  IND_Point(480, 100),  IND_Point(450, 10),  IND_Point(470, 220) };		// Poly points

void PrimitiveTests::prepareTests() {
}


void PrimitiveTests::performTests(float dt) {
    
    //IF - Check if test is active
    if(!_active)
        return;
    
    IND_Point trianglePoints[3];
	trianglePoints[0].x = 300;
	trianglePoints[0].y = 500;
	trianglePoints[1].x = 200;
	trianglePoints[1].y = 200;
	trianglePoints[2].x = 400;
	trianglePoints[2].y = 200;

	CIndieLib *iLib = CIndieLib::instance();
	// Direct bliting of primitives
	for (int i = 0; i < 400; i += 5) {
		iLib->_render->blitLine(70, 150, i * 2, 500, static_cast<unsigned char>(i), static_cast<unsigned char>(255 - i), static_cast<unsigned char>(255), static_cast<unsigned char>(255));
		iLib->_render->blitRegularPoly(600, 600, i, 70, static_cast<unsigned char>(0), static_cast<unsigned char>(255 - i), static_cast<unsigned char>(i), static_cast<unsigned char>(i * 4), static_cast<unsigned char>(255));
	}

	iLib->_render->blitTriangleList(trianglePoints, 3, static_cast<unsigned char>(255), static_cast<unsigned char>(0), static_cast<unsigned char>(0), static_cast<unsigned char>(255));
    iLib->_render->blitColoredTriangle(500,500,400,200,600,200,static_cast<unsigned char>(255),static_cast<unsigned char>(0),static_cast<unsigned char>(0),static_cast<unsigned char>(0),static_cast<unsigned char>(255),static_cast<unsigned char>(0),static_cast<unsigned char>(0),static_cast<unsigned char>(0),static_cast<unsigned char>(255),static_cast<unsigned char>(255));
}

//Default implementation
bool PrimitiveTests::isActive(){
    return (ManualTests::isActive());
}
    
//Default implementation
void PrimitiveTests::setActive(bool active){
    ManualTests::setActive(active);

    CIndieLib *iLib = CIndieLib::instance();
    //IF - Active
    if(active){
	    // Pixel
	    iLib->_entity2dManager->add (_pixel);
	    _pixel->setPrimitive2d (IND_PIXEL);
	    _pixel->setPosition (200, 75, 0);
	    _pixel->setTint (255, 255, 255);

	    // Regular poly

	    iLib->_entity2dManager->add(_regularPoly);
	    _regularPoly->setPrimitive2d(IND_REGULAR_POLY);
	    _regularPoly->setPosition(200, 75, 0);
	    _regularPoly->setNumSides(5);
	    _regularPoly->setRadius(50);
	    _regularPoly->setTint(255, 0, 0);

	    // Line

	    iLib->_entity2dManager->add(_line);
	    _line->setPrimitive2d(IND_LINE);
	    _line->setLine(100, 100, 50, 50);
	    _line->setTint(0, 255, 0);

	    // Rectangle
    	
	    iLib->_entity2dManager->add (_rectangle);
	    _rectangle->setPrimitive2d (IND_RECTANGLE);
	    _rectangle->setRectangle (350, 50, 400, 100);
	    _rectangle->setTint (0, 0, 255);

	    // Filled rectangle

	    iLib->_entity2dManager->add (_fillRectangle);
	    _fillRectangle->setPrimitive2d (IND_FILL_RECTANGLE);
	    _fillRectangle->setRectangle (550, 40, 650, 110);
	    _fillRectangle->setTint (255, 0, 0);
	    _fillRectangle->setTransparency (100);

	    // 2d Poly

	    iLib->_entity2dManager->add (_poly2d);			
	    _poly2d->setPrimitive2d (IND_POLY2D);
	    _poly2d->setPolyPoints (g_vertPoly2);													
	    _poly2d->setNumLines	(3);	 														// Number of edges - 1
	    _poly2d->setTint (255, 128, 255);
    } else { //Inactive
        iLib->_entity2dManager->remove(_pixel);
        iLib->_entity2dManager->remove(_regularPoly);
        iLib->_entity2dManager->remove(_line);
        iLib->_entity2dManager->remove(_rectangle);
        iLib->_entity2dManager->remove(_fillRectangle);
        iLib->_entity2dManager->remove(_poly2d);	
    }
}

//-----------------------------------PRIVATE METHODS----------------------------

void PrimitiveTests::init() {
	_pixel = IND_Entity2d::newEntity2d();
	_regularPoly = IND_Entity2d::newEntity2d();
	_line = IND_Entity2d::newEntity2d();
	_rectangle = IND_Entity2d::newEntity2d();
	_fillRectangle = IND_Entity2d::newEntity2d();
	_poly2d = IND_Entity2d::newEntity2d();

}

void PrimitiveTests::release() {
    CIndieLib *iLib = CIndieLib::instance();
    //Release all entities from manager before deleting
    iLib->_entity2dManager->remove(_pixel);
    iLib->_entity2dManager->remove(_regularPoly);
    iLib->_entity2dManager->remove(_line);
    iLib->_entity2dManager->remove(_rectangle);
    iLib->_entity2dManager->remove(_fillRectangle);
    iLib->_entity2dManager->remove(_poly2d);	
}
