/*****************************************************************************************
 * Desc: Tutorials b) 06 Spriter
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"
#include "IND_Entity2D.h"
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
    
    // ----- Font -----
	
    IND_Font *mFontSmall = new IND_Font();
	if (!mI->_fontManager->add(mFontSmall, "../../resources/font_small.png", "../../resources/font_small.xml", IND_ALPHA, IND_32)) return 0;
    
	// ----- Font creation -----
    
	IND_Entity2d *mTextSmallWhite = new IND_Entity2d() ;
	mI->_entity2dManager->add(mTextSmallWhite);				// Entity adding
	mTextSmallWhite->setFont(mFontSmall);					// Set the font into the entity
	mTextSmallWhite->setLineSpacing(18);
	mTextSmallWhite->setCharSpacing(-8);
	mTextSmallWhite->setPosition(5, 5, 1);
	mTextSmallWhite->setAlign(IND_LEFT);
    

	// ----- Spriter animation loading -----

	if (!mI->_spriterManager->addSpriterFile(NULL,"Example.SCML")){
        
        //return 0;

    }
    
    
    char mText [2048];
	mText [0] = 0;

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Text -----

		strcpy(mText, "This is the unfinished Spriter support tutorial, - currently there is nothing to see, so move along please =)");
		mTextSmallWhite->setText(mText);

		// ----- Input ----

		// for future use ...
		if (mI->_input->onKeyPress(IND_SPACE))
		{

		}

		// ----- Updating entities attributes  -----


		// ----- Render  -----

		mI->_render->beginScene();
		mI->_render->clearViewPort(0, 0, 0);
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}
