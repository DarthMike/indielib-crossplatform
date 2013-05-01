/*****************************************************************************************
 * Desc: Test c) 02 Rabbits BenchMark
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"
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
int IndieLib ()			
{
    //Sets the working path as the 'exe' directory. All resource paths are relative to this directory
	if (!WorkingPathSetup::setWorkingPathFromExe(NULL)) {
		std::cout<<"\nUnable to Set the working path !";
	}
	
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;		
	
	// ----- Surface loading -----

	// Loading rabbit
	IND_Surface *mSurfaceRabbit = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceRabbit, "../../resources/rabbit.png", IND_ALPHA, IND_32)) return 0;

	// Font
	IND_Font *mFontSmall = IND_Font::newFont();
	if (!mI->_fontManager->add(mFontSmall, "../../resources/font_small.png", "../../resources/font_small.xml", IND_ALPHA, IND_32)) return 0;

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
