/*****************************************************************************************
 * Desc: ManualTests virtual base class
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
THIS FILE IS AN ADDITIONAL FILE ADDED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com), BUT HAS THE
SAME LICENSE AS THE WHOLE LIBRARY TO RESPECT ORIGINAL AUTHOR OF LIBRARY

This library is free software; you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
Suite 330, Boston, MA 02111-1307 USA
*/
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


#endif // _FUNCTIONALITYTESTS_
