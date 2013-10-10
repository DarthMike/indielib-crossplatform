/*****************************************************************************************
 * File: CRabbit.h
 * Desc: Rabbit class
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


// ----- Includes -----

#include "CRabbit.h"


// --------------------------------------------------------------------------------
//								Initialization / Destruction
// --------------------------------------------------------------------------------

/*
==================
Init
==================
*/
CRabbit::CRabbit ()
{
	// Get IndieLib singleton object
	mI = CIndieLib::instance();

	mScreenWidth = mI->_render->getViewPortWidth();
	mScreenHeight = mI->_render->getViewPortHeight();
}


/*
==================
Initializes the attributes of a rabbit
==================
*/
void CRabbit::init(IND_Surface *pSurfaceRabbit, int pZ)
{
	// Depth
	mZ = pZ;

	// Add the entity
	mRabbit = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mRabbit);	
	mRabbit->setSurface(pSurfaceRabbit);

	// Set a random position to the rabbit
	mRabbit->setPosition((float) mI->_math->randnum(0, mScreenWidth), (float) mI->_math->randnum(0, mScreenHeight), mZ);

	// Set a random tint color to some of the rabbits
	if (mI->_math->randnum(0, 5) != 5)
		mRabbit->setTint((unsigned char) mI->_math->randnum(128, 255),
				 (unsigned char) mI->_math->randnum(128, 255),
				 (unsigned char) mI->_math->randnum(128, 255));

	// Hot spot in the middle of the rabbit
	mRabbit->setHotSpot(0.5f, 0.5f);

	// Random scale
	int mScale = mI->_math->randnum(50, 150);
	mRabbit->setScale((float) mScale / 100.0f, (float) mScale / 100.0f);

	// Random direction
	mDir = mI->_math->randnum(0, 3);

	// Random speed rotation
	mSpeedRotation = (float) mI->_math->randnum(-200, 200);

	// Random Speed
	mSpeed = (float) mI->_math->randnum(100, 200);
}


// --------------------------------------------------------------------------------
//										Public Methods
// --------------------------------------------------------------------------------

/*
==================
Update the position of the rabbits on the screen. This method will make the rabbits bounce
in the screen
==================
*/
void CRabbit::update()
{
	float mDirX = 0.0f; 
	float mDirY = 0.0f;

	// Get the elapsed time of the last frame
	float mDelta =  (float) mI->_render->getFrameTime() / 1000.f;
	
	switch (mDir)
	{
		case RIGHT_DOWN:
		{
			mDirX = mDelta * mSpeed;
			mDirY = mDelta * mSpeed;

			if (mRabbit->getPosX() > mScreenWidth)
				mDir = LEFT_DOWN;

			if (mRabbit->getPosY() > mScreenHeight)
				mDir = RIGHT_UP;

			break;
		}

		case LEFT_DOWN:
		{
			mDirX = -mDelta * mSpeed;
			mDirY = mDelta * mSpeed;

			if (mRabbit->getPosX() < 0)
				mDir = RIGHT_DOWN;

			if (mRabbit->getPosY() > mScreenHeight)
				mDir = LEFT_UP;

			break;
		}


		case RIGHT_UP:
		{
			mDirX = mDelta * mSpeed;
			mDirY = -mDelta * mSpeed;

			if (mRabbit->getPosX() > mScreenWidth)
				mDir = LEFT_UP;

			if (mRabbit->getPosY() < 0)
				mDir = RIGHT_DOWN;

			break;
		}


		case LEFT_UP:
		{
			mDirX = -mDelta * mSpeed;
			mDirY = -mDelta * mSpeed;

			if (mRabbit->getPosX() < 0)
				mDir = RIGHT_UP;

			if (mRabbit->getPosY() < 0)
				mDir = LEFT_DOWN;

			break;
		}
	}

	mRabbit->setPosition(mRabbit->getPosX() + mDirX,  mRabbit->getPosY() + mDirY, mZ);
	mRabbit->setAngleXYZ(0, 0, mRabbit->getAngleZ() + mSpeedRotation * mDelta);
}
