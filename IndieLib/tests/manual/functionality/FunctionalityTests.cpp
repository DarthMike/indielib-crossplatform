/*****************************************************************************************
 * Desc: FunctionalityTests class
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

#include "CIndieLib_vc2008.h"
#include "FunctionalityTests.h"

static const float g_testingInterval = 500.f;
static const int g_numTestedRes = 3;

struct RESOLUTION {
public:
	int x, y;
	RESOLUTION(int newx, int newy): x(newx), y(newy) {}
};
static RESOLUTION g_possibleRes[g_numTestedRes] = {RESOLUTION(800, 600), RESOLUTION(1024, 768), RESOLUTION(2048, 1024)};

void FunctionalityTests::performTests(float dt) {

	_keyTimer += dt;
	//if (_timer > g_testingInterval) {
		//Only perform 1 test
		_timer = 0.0f;
		if (fullScreenToggle())
			return;

		if (resetParameters())
			return;

		if (changeViewPortColor())
			return;
	//} else {
	//	_timer += dt;
	//}
}

//Default implementation
bool FunctionalityTests::isActive(){
    return (ManualTests::isActive());
}
    
//Default implementation
void FunctionalityTests::setActive(bool active){
    ManualTests::setActive(active);
}

//Tests fullscreen toggle functionality
bool FunctionalityTests::fullScreenToggle() {
	CIndieLib *mI = CIndieLib::instance();
	if (mI->_input->onKeyPress(IND_F)) {
		mI->_render->toggleFullScreen();
		return true;
	}

	return false;
}

//Tests window parameters change
bool FunctionalityTests::resetParameters() {
	static int resind = 0;
	CIndieLib *mI = CIndieLib::instance();
	if (mI->_input->onKeyPress(IND_R)) {
		if (resind >= (g_numTestedRes - 1)) {
			resind = 0;
		} else {
			resind++;
		}
		bool fs(mI->_window->isFullScreen());
		char newTitleText[] = "NEWTITLE";
		IND_WindowProperties props (newTitleText, g_possibleRes[resind].x, g_possibleRes[resind].y, 32, 1, fs);
		mI->_render->reset(props);
		return true;
	}

	return false;
}

//Tests viewport Clearing by color
bool FunctionalityTests::changeViewPortColor() {
	CIndieLib *mI = CIndieLib::instance();
	if (mI->_input->onKeyPress(IND_SPACE)) {
		mI->_render->clearViewPort(static_cast<BYTE>(mI->_math->randnum(0, 255)),
		                           static_cast<BYTE>(mI->_math->randnum(0, 255)),
		                           static_cast<BYTE>(mI->_math->randnum(0, 255)));
		return true;
	}

	return false;
}
