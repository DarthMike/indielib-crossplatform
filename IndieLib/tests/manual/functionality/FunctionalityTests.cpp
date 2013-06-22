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

#include "CIndieLib.h"
#include "FunctionalityTests.h"

static const float g_testingInterval = 500.f;
static const float g_maxPointPixelRatio = 3.0f;

struct RESOLUTION {
public:
	int x, y;
	RESOLUTION(int newx, int newy): x(newx), y(newy) {}
};

//Desktop/laptop platforms
#if defined (PLATFORM_WIN32) || defined (PLATFORM_LINUX) || defined (PLATFORM_OSX)
static const int g_numTestedRes = 8;
static RESOLUTION g_possibleRes[g_numTestedRes] = {RESOLUTION(480, 320),
												   RESOLUTION(640, 480), 
												   RESOLUTION(800, 600), 
												   RESOLUTION(960, 640), 
												   RESOLUTION(1024, 768), 
												   RESOLUTION(1440, 900),
												   RESOLUTION(1280,960),
												   RESOLUTION(1920, 1200)};
#endif //defined (PLATFORM_WIN32) || defined (PLATFORM_LINUX) || defined (PLATFORM_OSX)

//Phone platforms
#if defined (PLATFORM_IOS)
static const int g_numTestedRes = 1;
static RESOLUTION g_possibleRes[g_numTestedRes] = {
    RESOLUTION(1024, 768)
};
#endif //defined (PLATFORM_IOS)

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
    
        if (pixelPointScaleChange()) {
            return;
        }
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

bool FunctionalityTests::pixelPointScaleChange() {
    static float pointPixelRatio = 1.0f;
    CIndieLib *mI = CIndieLib::instance();
	if (mI->_input->onKeyPress(IND_P)) {
        pointPixelRatio += 0.25f;
        
        if (g_maxPointPixelRatio < pointPixelRatio) {
            pointPixelRatio = 0.25f;
        }
        
		mI->_render->setPointPixelScale(pointPixelRatio);
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
		IND_WindowProperties props (newTitleText, g_possibleRes[resind].x, g_possibleRes[resind].y, 32, 1, fs, 1);
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
