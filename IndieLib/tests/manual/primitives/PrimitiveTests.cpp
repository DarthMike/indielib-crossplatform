/*****************************************************************************************
 * Desc: PrimitiveTests class
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
THIS FILE IS AN ADDITIONAL FILE ADDED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com), BUT HAS THE
SAME LICENSE AS THE WHOLE LIBRARY TO RESPECT ORIGINAL AUTHOR OF LIBRARY

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

#include "CIndieLib_vc2008.h"
#include "PrimitiveTests.h"
#include "IND_Entity2d.h"

static IND_Point g_vertPoly2[] = { {440, 200},  {480, 100},  {450, 10},  {470, 220} };		// Poly points

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
		iLib->_render->blitLine(70, 150, i * 2, 500, i, 255 - i, 255, 255);
		iLib->_render->blitRegularPoly(600, 600, i, 70, 0, 255 - i, i, i * 4, 255);
	}

	iLib->_render->blitTriangleList(trianglePoints, 3, 255, 0, 0, 255);
    iLib->_render->blitColoredTriangle(500,500,400,200,600,200,255,0,0,0,255,0,0,0,255,255);
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
	_pixel = new IND_Entity2d();
	_regularPoly = new IND_Entity2d();
	_line = new IND_Entity2d();
	_rectangle = new IND_Entity2d();
	_fillRectangle = new IND_Entity2d();
	_poly2d = new IND_Entity2d();

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
	DISPOSE(_pixel);
	DISPOSE(_regularPoly);
	DISPOSE(_line);
	DISPOSE(_rectangle);
	DISPOSE(_fillRectangle);
	DISPOSE(_poly2d);
}
