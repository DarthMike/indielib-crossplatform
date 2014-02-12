/*****************************************************************************************
 * Desc: ManualTests virtual base class
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
#ifndef _MANUALTESTS_
#define _MANUALTESTS_

class ManualTests {
public:

	ManualTests():
		_timer(0.0f),
		_keyTimer(0.0f),
        _active(false)
	{}

	virtual ~ManualTests()
	{}

    //----- Interface to implement -----
	virtual void prepareTests() = 0;
	virtual void performTests(float dt) = 0;
    
    //----- Base variable accessors -----
    virtual bool isActive(){
        return _active;
    }
    virtual void setActive(bool active){
        _active = active;
    }

protected:
	float _timer;
	float _keyTimer;
    bool _active;
};


#endif // _MANUALTESTS_
