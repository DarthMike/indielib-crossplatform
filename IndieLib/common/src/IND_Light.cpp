/*****************************************************************************************
/* File: IND_Light.cpp
/* Desc: Light object
/*****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
MODIFIED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)

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

// ----- Includes -----

#include "Global.h"
#include "IND_Light.h"

#ifdef INDIERENDER_DIRECTX
#include <d3d9.h>
#include <d3dx9.h>
#endif

/** @cond DOCUMENT_PRIVATEAPI */
// Internal structure definition
// Attributes
struct structAttributes {
	float _x, _y, _z;               // Position
	float _r, _g, _b, _a;           // Color
	float _lastR, _lastG, _lastB;   // Previous color of the ambient light (for being able to disabe/enable;
	float _d1, _d2, _d3;            // Direction
	float _attenuation;             // Attenuation (poing lights)
	float _range;                   // Range (point lights)
	float _theta;                   // Theta (spot lights)
	float _phi;                     // Phi (spot lights)
	float _falloff;                 // Falloff (spot lights)
	bool _isEnabled;                // Light ON / OFF

	structAttributes() {
		_x = _y = _z = 0;
		_r = _g = _b = _a = 1.0f;
		_lastR = _lastG = _lastB = 0.0f;
		_d1 = _d2 = _d3 = _attenuation = _range = _theta = _phi = _falloff = 0;
		_isEnabled = 0;
	}
};
typedef struct structAttributes ATTRIBUTES;


struct LIGHT {
	bool            _setLightFlag;              // Flag for knowing if we have to set the light because their attributes has been modified
	int             _lightIndex;                // Index that we have assigned to the light
	IND_LightType   _lightType;                 // Light type
	ATTRIBUTES      _attributes;                // Light attributes

#ifdef INDIERENDER_DIRECTX
	D3DLIGHT9       _d3dLight;                  // Direct3D light
	LIGHT() {
		ZeroMemory(&_d3dLight, sizeof(D3DLIGHT9));
	}
#endif
};

/** @endcond */

// --------------------------------------------------------------------------------
//										Public methods
// --------------------------------------------------------------------------------

/**
 * Initialize light.
 */
IND_Light::IND_Light() {
	_light = new LIGHT();
}

/**
 * End light.
 */
IND_Light::~IND_Light() {
	DISPOSE(_light);
}

/**
 * Returns true if the light is enabled, false otherwise.
 */
bool IND_Light::isEnable() {
	return _light->_attributes._isEnabled;
}

/**
 * Returns red color component of the light.
 */
float IND_Light::getColorR() {
	return _light->_attributes._r;
}

/**
 * Returns green color component of the light.
 */
float IND_Light::getColorG() {
	return _light->_attributes._g;
}

/**
 * Returns blue color component of the light.
 */
float IND_Light::getColorB() {
	return _light->_attributes._b;
}

/**
 * Returns alpha component of the light.
 */
float IND_Light::getColorA() {
	return _light->_attributes._a;
}

/**
 * Returns x position of the light.
 */
float IND_Light::getPosX() {
	return _light->_attributes._x;
}

/** 
 * Returns y position of the light.
 */
float IND_Light::getPosY() {
	return _light->_attributes._y;
}

/**
 * Returns z position of the light.
 */
float IND_Light::getPosZ() {
	return _light->_attributes._z;
}

/** 
 * Returns the first component of the vector which defines the light direction.
 */
float IND_Light::getDirP1() {
	return _light->_attributes._d1;
}

/**
 * Returns the second component of the vector which defines the light direction.
 */
float IND_Light::getDirP2() {
	return _light->_attributes._d2;
}

/**
 * Returns the third component of the vector which defines the light direction.
 */
float IND_Light::getDirP3() {
	return _light->_attributes._d3;
}

/**
 * Returns the attenuation factor of the light.
 */
float IND_Light::getAttenuation() {
	return _light->_attributes._attenuation;
}

/**
 * Returns the range factor of the light.
 */
float IND_Light::getRange() {
	return _light->_attributes._range;
}

/**
 * Returns the range theta of the light.
 */
float IND_Light::getTheta() {
	return _light->_attributes._theta;
}

/**
 * Returns the phi factor of the light.
 */
float IND_Light::getPhi() {
	return _light->_attributes._phi;
}

/**
 * Returns the falloff factor of the light.
 */
float IND_Light::getFalloff() {
	return _light->_attributes._falloff;
}

/**
 * Sets the light color. The color is specified using three floats (red, green and blue values). Usually you will use the
 * range between 0.0f and 1.0f. The higer the value the nearer to the color. For example, red color will be:
 * (1.0f, 0.0f, 0.0f).
 * Using values higher that 1.0f will increase the light intensity.
 * Default: (1.0f, 1.0f, 1.0f).
 * @param pR						Float R (Red).
 * @param pG						Float G (Green).
 * @param pB						Float B (Blue).
 * @param pA 						TODO: Describtion.
 */
void IND_Light::setColor(float pR, float pG, float pB, float pA) {
	_light->_attributes._r = pR;
	_light->_attributes._g = pG;
	_light->_attributes._b = pB;
	_light->_attributes._a = pA;
#ifdef INDIERENDER_DIRECTX
	_light->_d3dLight.Diffuse.r = pR;
	_light->_d3dLight.Diffuse.g = pG;
	_light->_d3dLight.Diffuse.b = pB;
	_light->_d3dLight.Diffuse.a = pA;
#endif
	_light->_setLightFlag = 1;
}

/**
 * Sets the light position.
 * Default: (0, 0, 0).
 * @param pX						X coordinate in the space.
 * @param pY						Y coordinate in the space.
 * @param pZ						Z coordinate in the space.
 */
void IND_Light::setPosition(float pX, float pY, float pZ) {
	_light->_attributes._x = pX;
	_light->_attributes._y = pY;
	_light->_attributes._z = pZ;
#ifdef INDIERENDER_DIRECTX
	_light->_d3dLight.Position.x = pX;
	_light->_d3dLight.Position.y = pY;
	_light->_d3dLight.Position.z = pZ;
#endif
	_light->_setLightFlag = 1;
}

/**
 * Sets the light direction. This value is used for ::IND_DIRECTIONAL_LIGHT and ::IND_SPOT_LIGHT lights.
 * For example, for a Sun light, you can choose a direction: (0.0f, -0.3f, 0.5) with a ::IND_DIRECTIONAL_LIGHT.
 * Default: (0, 0, 0).
 * @param pD1, pD2, pD3             The three coordinates define a vector with the direction of the light.
 */
void IND_Light::setDirection(float pD1, float pD2, float pD3) {
	_light->_attributes._d1 = pD1;
	_light->_attributes._d2 = pD2;
	_light->_attributes._d3 = pD3;
#ifdef INDIERENDER_DIRECTX
	// Create a direction for our light, it must be normalized
	D3DXVECTOR3 mVecDir;
	mVecDir = D3DXVECTOR3(pD1, pD2, pD3);
	D3DXVec3Normalize((D3DXVECTOR3 *) &_light->_d3dLight.Direction, &mVecDir);
#endif
	_light->_setLightFlag = 1;
}

/**
 * Sets the light attenuation. This value is used for ::IND_POINT_LIGHT and ::IND_SPOT_LIGHT lights.
 * Default: (0).
 * @param pAttenuation					Attenuation factor.
 */
void IND_Light::setAttenuation(float pAttenuation) {
	_light->_attributes._attenuation = pAttenuation;
#ifdef INDIERENDER_DIRECTX
	_light->_d3dLight.Attenuation0 = pAttenuation;
#endif
	_light->_setLightFlag = 1;
}

/**
 * Sets the light range. This value is used for ::IND_DIRECTIONAL_LIGHT, ::IND_POINT_LIGHT and ::IND_SPOT_LIGHT lights.
 * Default: (0).
 * @param pRange					Range factor.
 */
void IND_Light::setRange(float pRange) {
	_light->_attributes._range = pRange;
#ifdef INDIERENDER_DIRECTX
	_light->_d3dLight.Range = pRange;
#endif
	_light->_setLightFlag = 1;
}

/**
 * Sets the light theta factor. This value is used for ::IND_SPOT_LIGHT lights.
 * Default: (0).
 * @param pTheta					Theta factor.
 */
void IND_Light::setTheta(float pTheta) {
	_light->_attributes._theta = pTheta;
#ifdef INDIERENDER_DIRECTX
	_light->_d3dLight.Theta = pTheta;
#endif
	_light->_setLightFlag = 1;
}

/**
 * Sets the light phi factor. This value is used for ::IND_SPOT_LIGHT lights.
 * Default: (0).
 * @param pPhi						Phi factor.
 */
void IND_Light::setPhi(float pPhi) {
	_light->_attributes._phi = pPhi;
#ifdef INDIERENDER_DIRECTX
	_light->_d3dLight.Phi = pPhi;
#endif
	_light->_setLightFlag = 1;
}

/**
 * Sets the light falloff factor. This value is used for ::IND_SPOT_LIGHT lights.
 * Default: (0).
 * @param pFalloff					Falloff factor.
 */
void IND_Light::setFalloff(float pFalloff) {
	_light->_attributes._falloff = pFalloff;
#ifdef INDIERENDER_DIRECTX
	_light->_d3dLight.Falloff = pFalloff;
#endif
	_light->_setLightFlag = 1;
}
