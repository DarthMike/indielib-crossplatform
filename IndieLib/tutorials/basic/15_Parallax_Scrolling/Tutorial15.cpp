/*****************************************************************************************
 * Desc: Tutorials a) 15 Parallax Scrolling
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"

#include "IND_Surface.h"
#include "IND_Font.h"
#include "IND_Entity2d.h"
#include "IND_Camera2d.h"
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

	// Loading cave
	IND_Surface *mSurfaceCave = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceCave, "../../resources/cave.png", IND_ALPHA, IND_32)) return 0;

	// Loading cave (first plane)
	IND_Surface *mSurfaceCaveFirstPlane = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceCaveFirstPlane, "../../resources/cave_near.png", IND_ALPHA, IND_32)) return 0;

	// Loading sky
	IND_Surface *mSurfaceSky = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceSky, "../../resources/sky.jpg", IND_OPAQUE, IND_32)) return 0;

	// Font
	IND_Font *mFontSmall = IND_Font::newFont();
	if (!mI->_fontManager->add(mFontSmall, "../../resources/font_small.png", "../../resources/font_small.xml", IND_ALPHA, IND_32)) return 0;

	// ----- Font creation -----

	IND_Entity2d *mTextSmallWhite = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(3, mTextSmallWhite);				// Entity adding (Layer 3)
	mTextSmallWhite->setFont(mFontSmall);					// Set the font into the entity
	mTextSmallWhite->setLineSpacing(18);
	mTextSmallWhite->setCharSpacing	(-8);
	mTextSmallWhite->setPosition(5, 5, 1);
	mTextSmallWhite->setAlign(IND_LEFT);

	// ----- Entities -----

	// Creating 2d entity for the sky
	IND_Entity2d *mSky = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(0, mSky);					// Entity adding (Layer 0)
	mSky->setSurface(mSurfaceSky);
	mSky->setPosition(600, 0, 0);

	// Creating 2d entity for the cave
	IND_Entity2d *mCave = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(1, mCave);					// Entity adding (Layer 1)
	mCave->setSurface(mSurfaceCave);

	// Creating 2d entity for the cave (first plane)
	IND_Entity2d *mCaveFirstPlane = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(2, mCaveFirstPlane);				// Entity adding (Layer 2)
	mCaveFirstPlane->setSurface(mSurfaceCaveFirstPlane);

	// ----- Cameras -----

	// --- Cameras for the parallax layers --- 

	int mMiddleScreenX = mI->_window->getWidth() / 2;
	int mMiddleScreenY = mI->_window->getHeight() / 2;

	float mPosXCamera0 = (float) mMiddleScreenX;
	float mPosXCamera1 = (float) mMiddleScreenX;
	float mPosXCamera2 = (float) mMiddleScreenX;

	IND_Camera2d *mCamera0 = new IND_Camera2d((int) mPosXCamera0, mMiddleScreenY);
	IND_Camera2d *mCamera1 = new IND_Camera2d((int) mPosXCamera1, mMiddleScreenY);
	IND_Camera2d *mCamera2 = new IND_Camera2d((int) mPosXCamera2, mMiddleScreenY);

	int mSpeedLayer0 = 50;
	int mSpeedLayer1 = 162;
	int mSpeedLayer2 = 250;

	// --- Camera for showing the text that explain the input controls --- 

	IND_Camera2d *mCameraText = new IND_Camera2d((int) mI->_window->getWidth() / 2, mI->_window->getHeight() / 2);

	// --- Some variables ---

	char mText [2048];
	mText [0] = 0;
	float mDelta;

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Text -----

		strcpy (mText, "Use right and left arrow keys for moving the camera\n");
		mTextSmallWhite->setText(mText);

		// ----- Input ----

		mDelta = mI->_render->getFrameTime() / 1000.0f;

		// Move cameras when pressing right key
		if (mI->_input->isKeyPressed(IND_KEYRIGHT))	
		{
			// Check limits
			if (mPosXCamera2 < mSurfaceCaveFirstPlane->getWidth() - mMiddleScreenX)
			{
				mPosXCamera0 += mSpeedLayer0 * mDelta;
				mPosXCamera1 += mSpeedLayer1 * mDelta;
				mPosXCamera2 += mSpeedLayer2 * mDelta;
			}
		}

		// Move cameras when pressing left key
		if (mI->_input->isKeyPressed(IND_KEYLEFT))	
		{
			// Check limits
			if (mPosXCamera2 > mMiddleScreenX)
			{
				mPosXCamera0 -= mSpeedLayer0 * mDelta;
				mPosXCamera1 -= mSpeedLayer1 * mDelta;
				mPosXCamera2 -= mSpeedLayer2 * mDelta;
			}
		}

		// ----- Updating cameras-----

		mCamera0->setPosition((int) mPosXCamera0, mMiddleScreenY);
		mCamera1->setPosition((int) mPosXCamera1, mMiddleScreenY);
		mCamera2->setPosition((int) mPosXCamera2, mMiddleScreenY);

		// ----- Render  -----

		mI->_render->clearViewPort(60, 60, 60);
		mI->_render->beginScene();

		// Render sky (Layer 0)
		mI->_render->setCamera2d(mCamera0);
		mI->_entity2dManager->renderEntities2d(0);

		// Render cave (Layer 1)
		mI->_render->setCamera2d(mCamera1);
		mI->_entity2dManager->renderEntities2d(1);

		// Render the first plane cave (Layer 2)
		mI->_render->setCamera2d(mCamera2);
		mI->_entity2dManager->renderEntities2d(2);

		// Render the text explaining the controls plane cave (Layer 3)
		mI->_render->setCamera2d(mCameraText);
		mI->_entity2dManager->renderEntities2d(3);

		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}
