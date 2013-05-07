/*****************************************************************************************
 * Desc: Tutorials b) 01 IND_Surface grids
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"
#include "IND_Surface.h"
#include "IND_Font.h"
#include "IND_Entity2d.h"
#include "IND_Timer.h"
#include "../../WorkingPath.h"

#include <cstring>

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

	// ----- Surface loading -----

	// Loading Background
	IND_Surface *mSurfaceBack = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBack, "../../resources/twist.jpg", IND_OPAQUE, IND_32)) return 0;

	// Loading draco
	IND_Surface *mSurfaceDraco = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceDraco, "../../resources/draco.png", IND_ALPHA, IND_32)) return 0;

	// Font
	IND_Font *mFontSmall = IND_Font::newFont();
	if (!mI->_fontManager->add(mFontSmall, "../../resources/font_small.png", "../../resources/font_small.xml", IND_ALPHA, IND_32)) return 0;

	// ----- Font creation -----

	IND_Entity2d *mTextSmallWhite = IND_Entity2d::newEntity2d() ;					
	mI->_entity2dManager->add(mTextSmallWhite);				// Entity adding
	mTextSmallWhite->setFont(mFontSmall);					// Set the font into the entity
	mTextSmallWhite->setLineSpacing(18);
	mTextSmallWhite->setCharSpacing(-8);
	mTextSmallWhite->setPosition(5, 5, 1);
	mTextSmallWhite->setAlign(IND_LEFT);

	// ----- Create a grid for Draco IND_Surface -----

	mSurfaceDraco->setGrid(8, 8);

	// ----- Set the surfaces into 2d entities -----

	// Creating 2d entity for the background
	IND_Entity2d *mBack = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mBack);						// Entity adding
	mBack->setSurface(mSurfaceBack);						// Set the surface into the entity

	// Creating 2d entity for the draco
	IND_Entity2d *mDraco = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mDraco);						// Entity adding
	mDraco->setSurface(mSurfaceDraco);						// Set the surface into the entity

	// ----- Changing the attributes of the 2d entities -----

	// Background
	mBack->setHotSpot(0.5f, 0.5f);
	mBack->setPosition(400, 300, 0);
	mBack->setScale(1.7f, 1.7f);

	// Draco
	mDraco->setPosition(150, 50, 1);

	// ----- Main Loop -----

	int mNumBlocksX = mSurfaceDraco->getBlocksX();
	int mNumBlocksY = mSurfaceDraco->getBlocksY();
	int mWidthBlock = mSurfaceDraco->getWidthBlock();
	int mHeightBlock = mSurfaceDraco->getHeightBlock();
	bool mShowGrid = 0;
	float mAngle = 0;
	IND_Timer *mTimer = new IND_Timer();
	mTimer->start();
	float mT;
	char mText [2048];
	mText [0] = 0;

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Text -----

		strcpy(mText, "Press space to see the grid in action. This is really cool, isn't it?");
		mTextSmallWhite->setText(mText);

		// ----- Input ----

		// Show / Hide the grid pressing "space"
		if (mI->_input->onKeyPress(IND_SPACE))
		{
			if (mShowGrid){
				mShowGrid = 0;
			}else{
				mShowGrid = 1;
			}
		}

		// ----- Updating entities attributes  -----

		mAngle += 0.1f;
		mBack->setAngleXYZ(0, 0, mAngle);

		// Update grid vertices for making a "wave" effect
		mT = mTimer->getTicks() / 1000.0f;
		
		for (int i = 1; i < mNumBlocksX; i++)
			for (int j = 1; j < mNumBlocksY; j++)
				mSurfaceDraco->setVertexPos (j, i, (int) ((j * mHeightBlock + cosf (mT * 10 + (i + j) / 2) * 5)), (int) ((i * mWidthBlock	+ sinf (mT * 10 + (i + j) / 2) * 5)));

		// ----- Render  -----

		mI->_render->beginScene();
		mI->_render->clearViewPort(60, 60, 60);
		mI->_entity2dManager->renderEntities2d();
		if (mShowGrid) mI->_entity2dManager->renderGridAreas(0, 0, 0, 255);
		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}
