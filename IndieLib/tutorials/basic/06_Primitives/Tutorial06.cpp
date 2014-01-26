/*****************************************************************************************
 * Desc: Tutorial a) 06 Primitives
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


#include "CIndieLib.h"

#include "IND_Entity2d.h"
#include "../../WorkingPath.h"

/*
==================
Main
==================
*/
Indielib_Main			
{
    //Sets the working path at the resources directory. Resources paths are relative to that directory
	if (!WorkingPathSetup::setWorkingPath(WorkingPathSetup::resourcesDirectory())) {
		std::cout<<"\nUnable to Set the working path !";
	}
	
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;									

	// ----- Set the primitives into 2d entities -----

	// Pixel
	IND_Entity2d *mPixel = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mPixel);			
	mPixel->setPrimitive2d(IND_PIXEL);
	mPixel->setPosition(200, 75, 0);
	mPixel->setTint(255, 255, 255);
	
	// Regular poly
	IND_Entity2d *mRegularPoly = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mRegularPoly);			
	mRegularPoly->setPrimitive2d(IND_REGULAR_POLY);		
	mRegularPoly->setPosition(200, 75, 0);									
	mRegularPoly->setNumSides(5);
	mRegularPoly->setRadius(50);
	mRegularPoly->setTint(255, 0, 0);

	// Line
	IND_Entity2d *mLine  = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mLine);			
	mLine->setPrimitive2d (IND_LINE);
	mLine->setLine(100, 100, 50, 50);
	mLine->setTint(0, 255, 0);

	// Rectangle
	IND_Entity2d *mRectangle = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mRectangle);			
	mRectangle->setPrimitive2d(IND_RECTANGLE);
	mRectangle->setRectangle(350, 50, 400, 100);
	mRectangle->setTint(0, 0, 255);

	// Filled rectangle
	IND_Entity2d *mFillRectangle = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mFillRectangle);			
	mFillRectangle->setPrimitive2d(IND_FILL_RECTANGLE);
	mFillRectangle->setRectangle(550, 40, 650, 110);
	mFillRectangle->setTint(255, 0, 0);
	mFillRectangle->setTransparency(50);

	// 2d Poly
	IND_Entity2d *mPoly2d  = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mPoly2d);			
	mPoly2d->setPrimitive2d(IND_POLY2D);
	IND_Point mVertPoly2 [] = { IND_Point(440, 200),  IND_Point(480, 100),  IND_Point(450, 10),  IND_Point(470, 220) };		// Poly points
	mPoly2d->setPolyPoints(mVertPoly2);													
	mPoly2d->setNumLines(3);	 							// Number of edges - 1
	mPoly2d->setTint(255, 128, 255);

	// ----- Main Loop -----

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update -----

		mI->_input->update();

		// ----- Render -----

		mI->_render->clearViewPort (0, 0, 0);
		mI->_render->beginScene();

		// Direct bliting of primitives
		for (int i = 0; i < 400; i += 5)
		{	
			mI->_render->blitLine(70, 150, i * 2, 500,  (unsigned char)i,  (unsigned char)(255 - i),  (unsigned char)255,  (unsigned char)255);
			mI->_render->blitRegularPoly(600, 600, i, 70, 0.0f, (unsigned char)(255 - i), (unsigned char)i, (unsigned char)(i*4), (unsigned char)255);
		}

		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}
