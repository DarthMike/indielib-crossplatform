/*****************************************************************************************
 * Desc: FunctionalityTests class
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
    
    if (keyboardFocus()) {
        return ;
    }
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

bool FunctionalityTests::keyboardFocus() {
   	CIndieLib *mI = CIndieLib::instance();
	if (mI->_input->touchesWithState(IND_TouchStateUp).size()) {
        mI->_input->isAcceptingKeyboardEvents() ? mI->_input->endKeyboardEvents() : mI->_input->beginKeyboardEvents();
		return true;
	}
    
	return false;
}
