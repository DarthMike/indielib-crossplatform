/*****************************************************************************************
 * File: CRabbit.h
 * Desc: Rabbit class
 *****************************************************************************************/

#ifndef _CRABBIT_
#define _CRABBIT_

// ------ Includes -----

#include "CIndieLib_vc2008.h"

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
	IND_Entity2d *mRabbit;			// Rabbit 2d entity
	int mScreenWidth, mScreenHeight;	// ViewPort dimensions
	float mSpeed;				// Rabbit speed
	int mDir;				// Direction of the rabbit
	float mSpeedRotation;			// Speed rotation for the rabbit	
	CIndieLib *mI;				// IndieLib object
};


#endif // _CRABBIT_
