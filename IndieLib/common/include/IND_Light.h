/*****************************************************************************************
 * File: IND_Light.h
 * Desc: Light object
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


#ifndef _IND_LIGHT_
#define _IND_LIGHT_


// ----- Forward declarations -----

struct LIGHT;

// ----- Includes -----

#include "Defines.h"

#ifdef INDIERENDER_DIRECTX
#include <d3d9.h>
#include <d3dx9.h>
#endif

#ifdef INDIERENDER_GLES_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif

// --------------------------------------------------------------------------------
//									 IND_Light
// --------------------------------------------------------------------------------

/**
@defgroup IND_Light IND_Light
@ingroup Objects
IND_Light class managed by IND_LightManager for managing lights, click in IND_Light to see all the methods of this class.
*/
/**@{*/

/**
@b IND_Image is an image object from the class ::IND_ImageManager. Read the explanation in ::IND_ImageManager for having more details.
*/
class LIB_EXP IND_Light {
public:

	// ----- Init/End -----

	IND_Light();
	~IND_Light();

	// ----- Public sets------

	void setColor(float pR, float pG, float pB, float pA);
	void setPosition(float pX, float pY, float pZ);
	void setDirection(float pD1, float pD2, float pD3);
	void setAttenuation(float pAttenuation);
	void setRange(float pRange);
	void setTheta(float pTheta);
	void setPhi(float pPhi);
	void setFalloff(float pFalloff);

	// ----- Public gets ------

	//! Returns true if the light is enabled, false otherwise.
	bool isEnable();
	//! Returns red color component of the light.
	float getColorR();
	//! Returns green color component of the light.
	float getColorG();
	//! Returns blue color component    of the light.
	float getColorB();
	//! Returns alpha  component    of the light.
	float getColorA();
	//! Returns x position of the light.
	float getPosX();
	//! Returns y position of the light.
	float getPosY();
	//! Returns z position of the light.
	float getPosZ();
	//! Returns the first component of the vector which defines the light direction.
	float getDirP1();
	//! Returns the second component of the vector which defines the light direction.
	float getDirP2();
	//! Returns the third component of the vector which defines the light direction.
	float getDirP3();
	//! Returns the attenuation factor of the light.
	float getAttenuation();
	//! Returns the range factor of the light.
	float getRange();
	//! Returns the range theta of the light.
	float getTheta();
	//! Returns the phi factor of the light.
	float getPhi();
	//! Returns the falloff factor of the light.
	float getFalloff();

private:

	// ----- Structures ------

	LIGHT *_light;

	// ----- Friends -----

	friend class IND_LightManager;

};
/**@}*/

#endif // _IND_LIGHT_
