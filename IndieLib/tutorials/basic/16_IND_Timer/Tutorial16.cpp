/*****************************************************************************************
 * Desc: Tutorials a) 16 IND_Timer
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

#include "IND_Font.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Timer.h"
#include "../../WorkingPath.h"

#include <cstring>
#include <sstream>


string convert(int number)
{
   std::ostringstream sin;
   sin << number;
   std::string val = sin.str();
   return val;
}


Indielib_Main			

{
    //Sets the working path at the resources directory. Resources paths are relative to that directory
	if (!WorkingPathSetup::setWorkingPath(WorkingPathSetup::resourcesDirectory())) {
		std::cout<<"\nUnable to Set the working path !";
	}
	
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;			

	// ----- Surface loading -----

	// Font
	IND_Font *mFontBig = IND_Font::newFont();
	if (!mI->_fontManager->addMudFont(mFontBig, "font_big.png", "font_big.xml", IND_ALPHA, IND_32)) return 0;

	// Loading draco
	IND_Surface *mSurfaceDraco = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceDraco, "draco.png", IND_ALPHA, IND_32)) return 0;

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
		mDraco->setAngleXYZ(0.00f, 0.00f, mSecond + 10.00f);
 
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
