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

#include "CIndieLib_vc2008.h"
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
    if (iLib->_input->isMouseButtonPressed (IND_MBUTTON_LEFT)){
        cameraAngle += g_rotspeed * dt;
    }
    if (iLib->_input->isMouseButtonPressed (IND_MBUTTON_RIGHT)){
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
