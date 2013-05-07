/*****************************************************************************************
 * Desc: Tutorials a) 16 IND_Timer
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"

#include "IND_Font.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Timer.h"
#include "../../WorkingPath.h"

#include <cstring>
#include <sstream>

/*
==================
Main
==================
*/



string convert(int number)
{
   std::ostringstream sin;
   sin << number;
   std::string val = sin.str();
   return val;
}


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

	// Font
	IND_Font *mFontBig = IND_Font::newFont();
	if (!mI->_fontManager->add(mFontBig, "../../resources/font_big.png", "../../resources/font_big.xml", IND_ALPHA, IND_32)) return 0;

	// Loading draco
	IND_Surface *mSurfaceDraco = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceDraco, "../../resources/draco.png", IND_ALPHA, IND_32)) return 0;

	// ----- Entities -----

	// Title text
	IND_Entity2d *mTextTime = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mTextTime);						// Entity adding
	mTextTime->setFont(mFontBig);							// Set the font into the entity

	// Creating 2d entity for the draco
	IND_Entity2d *mDraco = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mDraco);						// Entity adding
	mDraco->setSurface(mSurfaceDraco);						// Set the surface into the entity
	mDraco->setHotSpot(0.5f, 0.5f);
	mDraco->setPosition(400, 330, 1);

	// ----- Changing the attributes of the 2d entities -----

	// Text showing the time
	mTextTime->setLineSpacing(18);
	mTextTime->setCharSpacing(-8);
	mTextTime->setPosition(280, 20, 1);
	mTextTime->setAlign(IND_LEFT);

	// ----- Main Loop -----

	char mTimeString[128];
	mTimeString[0] = 0;
	char mTempTime[1024];
		
	// Create and start the timer;
	IND_Timer *mTimer = new IND_Timer();
	mTimer->start();

	int mX = 0;
	int mSecond;

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Input ----

		// Pause / Restart time when pressing space
		if (mI->_input->onKeyPress(IND_SPACE))
		{
			if (mTimer->isPaused()){
				mTimer->unpause();
			}
			else{
				mTimer->pause();
			}
		}

		// ----- Updating entities attributes  -----
 
		mSecond = (int) (mTimer->getTicks() / 1000.0f);

		// Show the time passing in seconds
		mI->_math->itoa(mSecond,mTempTime);
           						   
		strcpy (mTimeString, "Seconds: ");
		strcat (mTimeString, mTempTime);
		mTextTime->setText(mTimeString);

		// Update Draco position each second
		mDraco->setAngleXYZ(0, 0, mSecond + 10);
 
		// ----- Render  -----

		mI->_render->beginScene();
		mI->_render->clearViewPort(60, 60, 60);
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}
