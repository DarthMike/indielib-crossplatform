/*****************************************************************************************
 * Desc: Tutorial a) 11 Animated Tile Scrolling
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

	// ----- Animations loading -----

	// Sun animation
	IND_Animation *mAnimationSun = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationSun, "../../resources/animations/sun.xml", IND_OPAQUE, IND_32)) return 0;

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
