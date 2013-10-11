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


#ifndef _CRABBIT_
#define _CRABBIT_

// ------ Includes -----

#include "CIndieLib.h"

#include "IND_Entity2d.h"

// --------------------------------------------------------------------------------
//				CRabbit
// --------------------------------------------------------------------------------

class CRabbit
{
public:

	CRabbit();

	void init(IND_Surface * pSurfaceRabbit, int pZ);
	void update();

private:

	enum 
	{
		RIGHT_DOWN = 0,
		RIGHT_UP   = 1,
		LEFT_DOWN  = 2,
		LEFT_UP	   = 3,
	};

	// ----- Private ------

	int mZ;
	IND_Entity2d *mRabbit;              // Rabbit 2d entity
	int mScreenWidth, mScreenHeight;	// ViewPort dimensions
	float mSpeed;                       // Rabbit speed
	int mDir;                           // Direction of the rabbit
	float mSpeedRotation;               // Speed rotation for the rabbit
	CIndieLib *mI;                      // IndieLib object
};


#endif // _CRABBIT_
