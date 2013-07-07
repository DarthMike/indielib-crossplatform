/*****************************************************************************************
 * Desc: Tutorial a) 11 Animated Tile Scrolling
 *****************************************************************************************/

#include "CIndieLib.h"
#include "IND_Animation.h"
#include "IND_Entity2d.h"
#include "../../WorkingPath.h"

/*
==================
Main
==================
*/
IndieLib_Main()			
{
    //Sets the working path at the resources directory. Resources paths are relative to that directory
	if (!WorkingPathSetup::setWorkingPath(WorkingPathSetup::resourcesDirectory())) {
		std::cout<<"\nUnable to Set the working path !";
	}
	
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;								

	// ----- Animations loading -----

	// Sun animation
	IND_Animation *mAnimationSun = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationSun, "animations/sun.xml", IND_OPAQUE, IND_32)) return 0;

	// ----- Set the surface and animations into 2d entities -----

	// Sun
	IND_Entity2d *mSun = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mSun);			// Entity adding
	mSun->setAnimation(mAnimationSun);			// Set the animation into the entity

	// ----- Changing the attributes of the 2d entities -----

	mSun->toggleWrap(1);
	mSun->setRegion(0, 0, 800, 600);

	// ----- Main Loop -----

	float mDisp = 0.0f;
	float mSpeed = 0.5f;
	float mDelta;

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Updating entities attributes  -----

		mDelta = mI->_render->getFrameTime() / 1000.0f;

		mDisp += mSpeed * mDelta;
		mSun->setWrapDisplacement(mDisp, mDisp);	

		// ----- Render  -----

		mI->_render->beginScene();
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}
