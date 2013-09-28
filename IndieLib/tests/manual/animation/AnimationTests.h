/*****************************************************************************************
 * Desc: AnimationTests class
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
#ifndef _ANIMATIONTESTS_
#define _ANIMATIONTESTS_

#include "manual/ManualTests.h"

class IND_Surface;
class IND_Entity2d;
class IND_Animation;

class AnimationTests : public ManualTests {
public:

    AnimationTests():
        _animations(NULL),
		_entities(NULL){
		init();
	}

	virtual ~AnimationTests() {
		release();
	}
	void prepareTests();
	void performTests(float dt);

    bool isActive();
    void setActive(bool active);

private:
	void init();
	void release();

	//NOTE: UPDATE THIS ACCORDINGLY! (CRASHES, NOT USED NEW TESTS...)
	int _testedEntities;

	IND_Animation** _animations;

	IND_Entity2d** _entities;				
};


#endif // _Camera2dTests_
