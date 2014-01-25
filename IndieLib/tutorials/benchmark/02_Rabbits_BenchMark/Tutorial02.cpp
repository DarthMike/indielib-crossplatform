/*****************************************************************************************
 * Desc: Test c) 02 Rabbits BenchMark
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
#include "CRabbit.h"

#include "IND_Surface.h"
#include "IND_Font.h"
#include "../../WorkingPath.h"

#include <cstring>

#define MAX_OBJECTS 1000

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
	
	// ----- Surface loading -----

	// Loading rabbit
	IND_Surface *mSurfaceRabbit = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceRabbit, "rabbit.png", IND_ALPHA, IND_32)) return 0;

	// Font
	IND_Font *mFontSmall = IND_Font::newFont();
	if (!mI->_fontManager->addMudFont(mFontSmall, "font_small.png", "font_small.xml", IND_ALPHA, IND_32)) return 0;

	// ----- Create rabbits -----
	
	CRabbit *mRabbits = new CRabbit[MAX_OBJECTS];
	
	int i;
	for (i = 0; i < MAX_OBJECTS; i++) mRabbits[i].init(mSurfaceRabbit, i);

	// ----- Font creation -----

	IND_Entity2d *mTextSmallWhite = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mTextSmallWhite);			// Entity adding
	mTextSmallWhite->setFont(mFontSmall);				// Set the font into the entity
	mTextSmallWhite->setLineSpacing(18);
	mTextSmallWhite->setCharSpacing(-8);
	mTextSmallWhite->setPosition(5, 5, i);
	mTextSmallWhite->setAlign(IND_LEFT);
	
	// ----- Main Loop -----

	char mFpsString[128];
	char mFpsValueString[15];
	mFpsString [0] = 0;

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Fps -----

		strcpy(mFpsString, "Fps: ");
		mI->_render->getFpsString(mFpsValueString);
		strcat(mFpsString, mFpsValueString);
		strcat(mFpsString, "\nPress space to toggle full screen");
		mTextSmallWhite->setText(mFpsString);	

		// ----- Game logic ----

		// Toogle full screen when pressing "space"
		if (mI->_input->onKeyPress(IND_SPACE)) mI->_render->toggleFullScreen();

		// Update rabbits position
		for (int i = 0; i < MAX_OBJECTS; i++) mRabbits[i].update();	

		// ----- Render -----

		mI->_render->beginScene();
		mI->_render->clearViewPort(60, 60, 60);
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();
	}

	// ----- Free -----

	mI->end();
	delete [] mRabbits;

	return 0;
}
