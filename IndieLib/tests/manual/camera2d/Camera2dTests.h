/*****************************************************************************************
 * Desc: Camera2dTests class
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
#ifndef _CAMERA2DTESTS_
#define _CAMERA2DTESTS_

#include "manual/ManualTests.h"

class IND_Camera2d;

class Camera2dTests : public ManualTests {
public:

    Camera2dTests():_layer0Camera(NULL){
		init();
	}

	virtual ~Camera2dTests() {
		release();
	}
	void prepareTests();
	void performTests(float dt);

private:
	IND_Camera2d* _layer0Camera;
	void init();
	void release();
};


#endif // _Camera2dTests_
