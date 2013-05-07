/*****************************************************************************************
 * Desc: Tutorial a) 07 IND_Input
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"

#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Font.h"
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

	// Loading Beettleship
	IND_Surface *mSurfaceBeetleship = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBeetleship, "../../resources/beetleship.png", IND_ALPHA, IND_32)) return 0;

	// Loading Octopus
	IND_Surface *mSurfaceOctopus = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceOctopus, "../../resources/octopus.png", IND_ALPHA, IND_32)) return 0;

	// Loading Planet
	IND_Surface *mSurfacePlanet = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfacePlanet, "../../resources/planet.png", IND_ALPHA, IND_32)) return 0;

	// Font
	IND_Font *mFontSmall = IND_Font::newFont() ;
	if (!mI->_fontManager->add(mFontSmall, "../../resources/font_small.png", "../../resources/font_small.xml", IND_ALPHA, IND_32)) return 0;

	// ----- Font creation -----

	IND_Entity2d *mTextSmallWhite = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mTextSmallWhite);			// Entity adding
	mTextSmallWhite->setFont(mFontSmall);				// Set the font into the entity
	mTextSmallWhite->setLineSpacing(18);
	mTextSmallWhite->setCharSpacing(-8);
	mTextSmallWhite->setPosition(5, 5, 1);
	mTextSmallWhite->setAlign(IND_LEFT);

	// ----- Set the surfaces into 2d entities -----

	// Creating 2d entity for the background
	IND_Entity2d *mBack = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mBack);				// Entity adding
	mBack->setSurface(mSurfaceBack);				// Set the surface into the entity

	// Creating 2d entity for the bettleship
	IND_Entity2d *mBeetleship = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mBeetleship);				// Entity adding
	mBeetleship->setSurface(mSurfaceBeetleship);			// Set the surface into the entity

	// Creating 2d entity for the octopus
	IND_Entity2d *mOctopus = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mOctopus);				// Entity adding
	mOctopus->setSurface(mSurfaceOctopus);				// Set the surface into the entity

	// Creating 2d entity for the planet
	IND_Entity2d *mPlanet = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mPlanet);				// Entity adding
	mPlanet->setSurface(mSurfacePlanet);				// Set the surface into the entity

	// ----- Changing the attributes of the 2d entities -----

	mBack->setHotSpot(0.5f, 0.5f);
	mBack->setPosition(400, 300, 0);
	mBack->setScale(1.7f, 1.7f);

	mBeetleship->setHotSpot(0.5f, 0.5f); 

	mOctopus->setMirrorX(true);
	mOctopus->setHotSpot(0.5f, 0.5f);

	mPlanet->setHotSpot(0.5f, 0.5f);

	// ----- Main Loop -----

	float mAngle = 0;
	float mPos = 400;
	int mSpeed = 200;
	float mDelta;
	char mText [2048];
	mText [0] = 0;

	while (!mI->_input->onKeyPress (IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update -----

		mI->_input->update();

		// ----- Text -----

		strcpy (mText, "Use the mouse for moving the planet\n");
		strcat (mText, "Use left and right arrow keys for moving the ships\n");
		strcat (mText, "Press CTRL + X or ESC key to quit");
		mTextSmallWhite->setText(mText);	

		// ----- Input -----

		mDelta = mI->_render->getFrameTime() / 1000.0f;

		// Move entities when pressing right
		if (mI->_input->isKeyPressed(IND_KEYRIGHT)){
			mPos += mSpeed * mDelta;
		}

		// Move entities when pressing left
		if (mI->_input->isKeyPressed(IND_KEYLEFT)){
			mPos -= mSpeed * mDelta;
		}

		// If CTRL + X pressed then exit
		if (mI->_input->isKeyPressed(IND_LCTRL) && mI->_input->isKeyPressed(IND_X)){
			mI->_render->endScene();
			mI->end();
			exit(0);
		}

		mAngle += (mSpeed / 4) * mDelta;

		// ----- Updating entities attributes  -----

		// Back
		mBack->setAngleXYZ(0, 0, mAngle);		

		// Beetle
		mBeetleship->setPosition((float) mPos, 140, 1);
		mBeetleship->setAngleXYZ(0, 0, (float) mPos);

		// Planet
		mPlanet->setPosition((float) mI->_input->getMouseX(), 300, 2);
		
		// Octopus
		mOctopus->setPosition(800 - (float) mPos, 480, 3);

		// ----- Render -----

		mI->_render->beginScene();
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}
