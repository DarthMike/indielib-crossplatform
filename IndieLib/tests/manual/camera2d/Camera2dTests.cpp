/*****************************************************************************************
 * Desc: Camera2dTests class
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
#include "Camera2dTests.h"
#include "IND_Entity2d.h"

static float g_movespeed = 0.001f;
static float g_rotspeed = 0.0001f;
static float g_scrollspeed = 0.001f;

void Camera2dTests::prepareTests() {

}


void Camera2dTests::performTests(float dt) {
   
    //Indielib
    CIndieLib* iLib = CIndieLib::instance();

    //Get camera params
    float cameraAngle (_layer0Camera->getAngle());
    float cameraZoom (_layer0Camera->getZoom());
        
	// Camera translation
    if (iLib->_input->isKeyPressed (IND_W)){
		_layer0Camera->moveFly(g_movespeed * dt);
    }
    if (iLib->_input->isKeyPressed (IND_S)){
        _layer0Camera->moveFly(-g_movespeed * dt);
    }
    if (iLib->_input->isKeyPressed (IND_A)){
        _layer0Camera->moveStrafe(-g_movespeed * dt);
    }
    if (iLib->_input->isKeyPressed (IND_D)){
        _layer0Camera->moveStrafe(g_movespeed * dt);
    }

	// Camera Zoom in / out
	if (iLib->_input->isMouseScroll()) {
		cameraZoom += iLib->_input->getMouseScrollY() * g_scrollspeed;
	}

	// Camera angle
    if (iLib->_input->isKeyPressed(IND_Q)){
        cameraAngle += g_rotspeed * dt;
    }
    if (iLib->_input->isKeyPressed(IND_E)){
        cameraAngle -= g_rotspeed * dt;
    }

    //Set modified parameters to camera
    _layer0Camera->setAngle(cameraAngle);
    _layer0Camera->setZoom(cameraZoom);

    iLib->_render->setCamera2d(_layer0Camera);
}

//-----------------------------------PRIVATE METHODS----------------------------

void Camera2dTests::init() {
    CIndieLib* iLib = CIndieLib::instance();
    _layer0Camera = new IND_Camera2d(static_cast<float>(iLib->_window->getWidth() / 2),
									 static_cast<float>(iLib->_window->getHeight() / 2));
    
}

void Camera2dTests::release() {
	DISPOSE(_layer0Camera);
}
