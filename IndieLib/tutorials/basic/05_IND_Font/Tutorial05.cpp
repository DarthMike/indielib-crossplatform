/*****************************************************************************************
 * Desc: Tutorial a) 05 IND_Font
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

#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Font.h"
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

        
	// ----- Surface and font loading -----

	// Loading Background
	IND_Surface *mSurfaceBack = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBack, "blue_background.jpg", IND_OPAQUE, IND_32)) return 0;

	// Loading tip page
	IND_Surface *mSurfaceTip = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceTip, "tip.png", IND_ALPHA, IND_32)) return 0;

	// Font 1
	IND_Font *mFontSmall = IND_Font::newFont();
	if (!mI->_fontManager->addMudFont(mFontSmall, "font/bitmap/MudFont/font_small.png", "font/bitmap/MudFont/font_small.xml", IND_ALPHA, IND_32)) return 0;

	// Font 2
	IND_Font *mFontBig = IND_Font::newFont();
	if (!mI->_fontManager->addMudFont(mFontBig, "font/bitmap/MudFont/font_big.png", "font/bitmap/MudFont/font_big.xml", IND_ALPHA, IND_32)) return 0;
        
    // Font 3
    IND_Font *mFontCooper = IND_Font::newFont();
    if (!mI->_fontManager->addAngelcodeFont(mFontCooper, "font/bitmap/AngelCode/cooper.xml", IND_ALPHA, IND_32)) return 0;


	// ----- Set the surfaces and fonts into 2d entities -----

	// Creating 2d entity for the background
	IND_Entity2d *mBack = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mBack);                           // Entity adding
	mBack->setSurface(mSurfaceBack);                            // Set the surface into the entity

	// Creating 2d entity for the tips page
	IND_Entity2d *mTip = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mTip);                            // Entity adding
	mTip->setSurface(mSurfaceTip);                              // Set the surface into the entity

	// Title text
	IND_Entity2d *mTextTitle = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mTextTitle);                      // Entity adding
	mTextTitle->setFont(mFontBig);                              // Set the font into the entity

	// Text small black
	IND_Entity2d *mTextSmallBlack = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mTextSmallBlack);                 // Entity adding
	mTextSmallBlack->setFont(mFontSmall);                       // Set the font into the entity

	// Text small white
	IND_Entity2d *mTextSmallWhite = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mTextSmallWhite);                 // Entity adding
	mTextSmallWhite->setFont(mFontSmall);                       // Set the font into the entity

    // Text small cooper
    IND_Entity2d *mTextSmallCooper = IND_Entity2d::newEntity2d();
    mI->_entity2dManager->add(mTextSmallCooper);                // Entity adding
    mTextSmallCooper->setFont(mFontCooper);                      // Set the font into the entity
        
        
	// ----- Changing the attributes of the 2d entities -----

	mTip->setHotSpot(0.5f, 0.5f);
	mTip->setPosition(250, 300, 0);

	mTextTitle->setText("IndieLib Fonts");	
	mTextTitle->setLineSpacing(18);
	mTextTitle->setCharSpacing(-8);
	mTextTitle->setPosition(400, 40, 1);
	mTextTitle->setAlign(IND_CENTER);

	mTextSmallBlack->setText("This paragraph is\ncentered using.\nIND_CENTER\n\nThis font has\nblack color using\nSetTint() method.\n\nRocks!");	
	mTextSmallBlack->setLineSpacing	(18);
	mTextSmallBlack->setCharSpacing	(-8);
	mTextSmallBlack->setPosition(242, 230, 1);
	mTextSmallBlack->setAlign(IND_CENTER);
	mTextSmallBlack->setTint(0, 0, 0);

	mTextSmallWhite->setText("This is a another paragraph. This time\nusing the font without changing the color\nand with a IND_LEFT alignment.\n\nThis is a really easy way of writing\ntexts, isn't it?");	
	mTextSmallWhite->setLineSpacing(18);
	mTextSmallWhite->setCharSpacing(-7);
	mTextSmallWhite->setPosition(400, 230, 1);
	mTextSmallWhite->setAlign(IND_LEFT);
        
    mTextSmallCooper->setText("This time we're fooling around with\nthe AngelCode font format");
    mTextSmallCooper->setLineSpacing(25);
    mTextSmallCooper->setCharSpacing(1);
    mTextSmallCooper->setPosition(150, 475, 1);
    mTextSmallCooper->setAlign(IND_LEFT);

        
	// ----- Main Loop -----

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		mI->_input->update();
		mI->_render->beginScene();
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();	
	}

        
	// ----- Free -----

	mI->end();

	return 0;
}
