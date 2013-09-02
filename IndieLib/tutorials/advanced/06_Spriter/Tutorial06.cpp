/*****************************************************************************************
 * Desc: Tutorials b) 06 Spriter
 *****************************************************************************************/

#include "CIndieLib.h"
#include "IND_Entity2D.h"
#include "IND_Spriterentity.h"
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
    

	// ----- Spriter animation loading -----
    
    
	if (!mI->_spriterManager->addSpriterFile("../../resources/Spriter/monster/Example.SCML")){
        
        return 0;

    }
    
    
    char mText [2048];
	mText [0] = 0;
    
    //char mTimeString[128];
	//mTimeString[0] = 0;
	//char mTempTime[1024];
    

    //IND_Timer *mTimer = new IND_Timer();
	//mTimer->start();
    //int deltaTime = 0;
    //int lastTime = 0;
    //int mSecond = 0;
    
    
    IND_SpriterEntity *sEnt = mI->_spriterManager->getEntities()->at(0);
    sEnt->playAnimation(0);
    
	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();


		// ----- Input ----

		// Pause / Restart time when pressing space
		//if (mI->_input->onKeyPress(IND_SPACE))
		//{
		//	if (mTimer->isPaused()){
		//		mTimer->unpause();
		//	}
		//	else{
		//		mTimer->pause();
		//	}
		//}

		// ----- Updating entities attributes  -----
        
        //mSecond = (int) (mTimer->getTicks() / 1000.0f);
        
        //deltaTime = mSecond - lastTime;
        
        //lastTime = mSecond;
        
        
        //if ( deltaTime > 0 ) { // TODO: MFK, move timer functionality into spriter manager, and let it have full control.
        
            // ----- Text -----
            
        
            strcpy(mText, "This is the unfinished Spriter support tutorial, - currently there is nothing to see, so move along please =)  \nCroshair coords (x,y) = 400,500 \nTime ");
            
            // Show the time passing in seconds
            //mI->_math->itoa(mSecond,mTempTime);

            //strcat (mText, mTempTime);
            
            
            mTextSmallWhite->setText(mText);


            // ----- Render  -----

            mI->_render->beginScene();
            mI->_render->clearViewPort(127, 127, 127);
            mI->_render->blitLine(400,50, 400, 550, 255, 255, 255, 255);
            mI->_render->blitLine(50,500, 750, 500, 255, 255, 255, 255);
            mI->_render->blitPixel(400, 500, 0, 0, 0, 255); // crosshair center point
            mI->_entity2dManager->renderEntities2d();
            mI->_spriterManager->renderEntities();
            mI->_render->endScene();
        
        //}
            
	}

	// ----- Free -----

	mI->end();

	return 0;
}
