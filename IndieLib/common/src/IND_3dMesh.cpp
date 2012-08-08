/*****************************************************************************************
 * File: IND_3dMesh.cpp
 * Desc: 3d Mesh object
 *****************************************************************************************/

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
#include "IND_3dMesh.h"

// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

/**
 * If you change the animation sequence of a 3d Mesh when Indielib is rendering(using IND_Entity3d::setSequence())
 * the change will not be abrupt. IndieLib will fade out the effect of the first animation and fade in the second.
 *
 * This function sets the animation speed of this transition animation. The default value is 0.25f. To speed up the
 * transition choose values between 0.25f and 0.0f. The fastest transition possible is 0.0f. In order to slow down the
 * animation speed, choose values higher than 0.25f, the higher the value, the slower the animation.
 * @param pSpeed			Value higher than 0 that sets the animation speed. Default: 0.25f
 */
void IND_3dMesh::setTransitionSpeed(float pSpeed) {
	if (pSpeed < 0.001f) pSpeed = 0.001f;

	_3dMesh._moveTransitionTime = pSpeed;
}


/**
 * This function sets the animation speed when changing from one sequence to another. The default value is 1.0f.
 * To speed up the animation choose values between 1.0f and 0.0f. The fastest animation possible is 0.0f. In order to
 * slow down the animation speed, choose values higher than 1.0f, the higher the value, the slower the animation.
 * @param pSpeed			Value higher than 0 that sets the animation speed. Default: 1.0f
 */
void IND_3dMesh::setAnimationSpeed(float pSpeed) {
	if (pSpeed < 0.001f) pSpeed = 0.001f;

	_3dMesh._speedAdjust = pSpeed;
}
