/*****************************************************************************************
 * Desc: Tutorial a) 06 Primitives
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"

#include "IND_Entity2d.h"
#include "../../WorkingPath.h"

/*
==================
Main
==================
*/
int IndieLib()			
{
    //Sets the working path as the 'exe' directory. All resource paths are relative to this directory
	if (!WorkingPathSetup::setWorkingPathFromExe(NULL)) {
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
	IND_Point mVertPoly2 [] = { {440, 200},  {480, 100},  {450, 10},  {470, 220} };		// Poly points
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
			mI->_render->blitLine(70, 150, i * 2, 500, i, 255 -i, 255, 255);
			mI->_render->blitRegularPoly(600, 600, i, 70, 0, 255 - i, i, i*4, 255);
		}

		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}
