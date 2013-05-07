/*****************************************************************************************
/* Desc: Tutorial a) 04 IND_Animation
/*****************************************************************************************/

#include "CIndieLib_vc2008.h"

#include "IND_Surface.h"
#include "IND_Animation.h"
#include "IND_Entity2d.h"
#include "../../WorkingPath.h"

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
	if (!mI->_surfaceManager->add(mSurfaceBack, "../../resources/blue_background.jpg", IND_OPAQUE, IND_32)) return 0;

	// ----- Animations loading -----

	// Characters animations, we apply a color key of (0, 48, 152)
	IND_Animation *mAnimationCharacter1 = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationCharacter1, "../../resources/animations/character1.xml", IND_ALPHA, IND_32, 0, 48, 152)) return 0;

	// Characters animations, we apply a color key of (0, 48, 152)
	IND_Animation *mAnimationCharacter2 = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationCharacter2, "../../resources/animations/character2.xml", IND_ALPHA, IND_32, 0, 48, 152)) return 0;

	// Dust animation, we apply a color key of (255, 0, 255)
	IND_Animation *mAnimationDust = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationDust, "../../resources/animations/dust.xml", IND_ALPHA, IND_16, 255, 0, 255)) return 0;

	// ----- Set the surface and animations into 2d entities -----

	// Creating 2d entity for the background
	IND_Entity2d *mBack = IND_Entity2d::newEntity2d();			
	mI->_entity2dManager->add(mBack);					// Entity adding
	mBack->setSurface(mSurfaceBack);					// Set the surface into the entity

	// Character 1
	IND_Entity2d *mPlayer1 = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mPlayer1);					// Entity adding
	mPlayer1->setAnimation(mAnimationCharacter1);				// Set the animation into the entity

	// Character 2
	IND_Entity2d *mPlayer2 = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mPlayer2);					// Entity adding
	mPlayer2->setAnimation(mAnimationCharacter2);				// Set the animation into the entity

	// Dust explosion
	IND_Entity2d *mDust = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mDust);					// Entity adding
	mDust->setAnimation(mAnimationDust);					// Set the animation into the entity

	// ----- Changing the attributes of the 2d entities -----

	// Player 1
	mPlayer1->setSequence(0);						// Choose sequence
	mPlayer1->setPosition(140, 200, 0);
	mPlayer1->setMirrorX(1);						// Horizontal mirroring
	
	// Dust explosion
	mDust->setPosition(360, 250, 0);
	
	// Player 2
	mPlayer2->setSequence(0);						// Choose sequence
	mPlayer2->setPosition(550, 200, 0);				
	mPlayer2->setNumReplays(3);						// The animation will be displayed 3 times

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
