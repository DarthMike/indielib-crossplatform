/*****************************************************************************************
 * File: IND_Entity3d.cpp
 * Desc: Entity 3d object
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
#include "IND_Entity3d.h"
#include "IND_SurfaceManager.h"
#include "IND_3dMesh.h"

// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

/**
 * Assigns a 3d mesh IND_3dMesh to the 3d entity
 * @param pNewMesh					Pointer to a mesh.
 */
void IND_Entity3d::set3dMesh(IND_3dMesh *pNewMesh) {
	initAttrib();

	_me = pNewMesh;
}

/**
 * Showes / hides the entity. It dosen't delete it.
 * Default: 0.
 * @param pShow						True = show the entity / False = hide entity.
 */
void IND_Entity3d::setShow(bool pShow) {
	_show = pShow;
}

/**
 * Sets the animation sequence that will be rendered. When you use this method, the transition between the
 * current animation and the following will not be abrubt. IndieLib will fade out the effect of the first animation
 * and fade in the second. Use ::setTransitionSpeed() in order set the speed of that transition.
 * Default: 0 (first sequence).
 * @param pS						Number of sequence to draw.
 */
bool IND_Entity3d::setSequence(int pS) {
	if (!_me) return 0;

	if (pS < 0 || pS > _me->getNumSequences()) return 0;

	_sequence = pS;
	_changeSequenceFlag = 1;

	return 1;
}

/**
 * Sets the animation speed when changing from one sequence to another. The default value is 1.0f.
 * To speed up the animation choose values between 1.0f and 0.0f. The fastest animation possible is 0.0f. In order to
 * slow down the animation speed, choose values higher than 1.0f, the higher the value, the slower the animation.
 * Default: 1.0f
 * @param pSpeed					Value higher than 0 that sets the animation speed.
 */
void IND_Entity3d::setAnimationSpeed(float pSpeed) {
	if (!_me) return;

	_me->setAnimationSpeed(pSpeed);
}

/**
 * When IndieLib is rendering an animation sequence of a 3d Mesh, if you change the animation sequence (using
 * IND_Entity3d::setSequence()) the change will not be abrupt. IndieLib will fade out the effect of the first animation
 * and fade in the second.
 *
 * This function sets the animation speed of this transition animation. The default value is 0.25f. To speed up the
 * transition choose values between 0.25f and 0.0f. The faster transition possible is 0.0f. In order to slow down the
 * animation speed, choose values higher than 0.25f, the higher the value, the slower the animation.
 * Default: 0.25f
 * @param pSpeed                    Value higher than 0 that sets the animation speed.
 */
void IND_Entity3d::setTransitionSpeed(float pSpeed) {
	if (!_me) return;

	_me->setTransitionSpeed(pSpeed);
}

/**
 * Sets the position where the entity will be drawn.
 * Default (0, 0, 0).
 * @param pX, pY, pZ					Translation int the x,y and z axis.
 */
void IND_Entity3d::setPosition(float pX, float pY, float pZ) {
	_x = pX;
	_y = pY;
	_z = pZ;
}

/**
 * Sets the rotation of the entity in each of the coordinate axis.
 * Default (0, 0, 0).
 * @param pAnX						Angle of rotation in the x axis in degrees.
 * @param pAnY						Angle of rotation in the y axis in degrees.
 * @param pAnZ						Angle of rotation in the z axis in degrees.
 */
void IND_Entity3d::setAngleXYZ(float pAnX, float pAnY, float pAnZ) {
	_angleX = pAnX;
	_angleY = pAnY;
	_angleZ = pAnZ;
}

/**
 * Sets the scaling of the entity in the x, y and z axis.
 * Default (1, 1, 1).
 * @param pSx, pSy, pSz					Level of scaling in the x, y and z axis. (1,1,1) to keep the normal size
 */
void IND_Entity3d::setScale(float pSx, float pSy, float pSz) {
	_scaleX = pSx;
	_scaleY = pSy;
	_scaleZ = pSz;
}

/**
 * Activates or deactivates the backface culling. (1 / 0) = (activated / deactivated).
 * This parameter is interesting to deactivate when we rotate the graphical object in the x or y axis.
 * Default: Activated (1).
 * @param pCull						Back faces culling activated / deactivated.
 */
void IND_Entity3d::setBackCull(bool pCull) {
	_cull = pCull;
}

/**
 * This function sets the filter that will affect the graphical object when drawing.
 * Default: ::IND_FILTER_LINEAR
 * @param pF						Type of filter ::IND_Filter. There are two types of filters that can be applied to the graphical object
							when it undergoes a rotation or scaling:
                                			- ::IND_FILTER_POINT (Nearest point filter). Less quality, but bigger performance. It is possible to use this without 								  quality loss in graphical objects that don't rotate or are affected by scaling.
							- ::IND_FILTER_LINEAR (Bilinear filter). More quality, but less performance. Used in graphical objects that be rotated
							  or affected by scaling.
 */
void IND_Entity3d::setFilter(IND_Filter pF) {
	_filter = pF;
}

/**
 * Toggles the back face culling of the model. You can use this method if you see
 * if your model is being displayed incorrectly.
 * Default: 0
 * @param pFlipNormals					Flip normals (1 / 0) = on / off.
 */
void IND_Entity3d::flipNormals(bool pFlipNormals) {
	_flipNormals = pFlipNormals;
}

/**
 * This function sets the bytes pR, pG, pB for "tinting" the 3d Mesh to a certain color. For example,
 * if we use RGB = (255, 0, 0), the mesh will be tinted  red. Note: It is not the same as tinting.
 * The tint produces a colored transparent effect (similar to looking through celophane),
 * whereas the fade completely changes the image to the color selected.
 *
 * It is not possible to use IND_Entity3d::setTint() and IND_Entity3d::setFade() at the same time,
 * the fade will prevail upon the tinting.
 * Default: RGBA = (0, 0, 0) (Without tint)
 * @param pR, pG, pB					Bytes R, G, B.
 */
void IND_Entity3d::setTint(BYTE pR, BYTE pG, BYTE pB, BYTE pA) {
	_r = pR;
	_g = pG;
	_b = pB;
	_a = pA;
}

/**
 * Sets the level of transparency of the entity.
 * The A byte indicates the level of transparency. If a value 0 is used, the following graphical
 * object will be completely transparent, as opposed to the value 255 that will be drawn completely
 * opaque.
 *
 * It is possible to use all the intermediate values for different levels of transparency.
 * Default: A = (255) (Without transparency)
 * @param pA						Byte A.
 */
void IND_Entity3d::setTransparency(BYTE pA) {
	_a = pA;
}

/**
 * Sets the bytes FadeR, FadeG, FadeB for making a fade of the 3d mesh to a certain
 * color. When we speak of fade we refer to the progressive change of the mesh to certain color.
 * For example, if we use RGB = (255, 0, 0), the mesh will be faded red. Note: It is not
 * the same as tinting. Tint produces an colored transparent effect(like looking through cellophane,
 * whereas the fade changes the mesh to the color selected completely.
 *
 * The byte A indicates the level of fade. If the value 0 is used, the following graphical object
 * will be filled solid with the selected color, as opposed to the value 255 that would cause
 * the object to be drawn completely normally. It is possible to use all the intermediate values for different
 * levels of fade.
 *
 * It is not possible to use IND_Entity3d::setTint() and IND_Entity3d::setFade() at the same time,
 * the fade will override the tinting.
 * Default: RGBA = (0, 0, 0, 255) (Without fade)
 * @param pR, pG, pB, pA				Bytes R, G, B, A.
 */
void IND_Entity3d::setFade(BYTE pR, BYTE pG, BYTE pB, BYTE pA) {
	_fadeR = pR;
	_fadeG = pG;
	_fadeB = pB;
	_fadeA = pA;
}

/**
 * Sets the blending source.
 * Default: 0. (Blending source is not in use).
 * @param pSo						Indicates the blending source, see ( ::IND_BlendingType).
 */
void IND_Entity3d::setBlendSource(IND_BlendingType pSo) {
	_so = pSo;
}

/**
 * Sets the blending destiny.
 * Default: 0. (Blending destiny is not in use).
 * @param pDs						Indicates blending destiny, see ( ::IND_BlendingType).
 */
void IND_Entity3d::setBlendDest(IND_BlendingType pDs) {
	_ds = pDs;
}

// --------------------------------------------------------------------------------
//									 Private methods
// --------------------------------------------------------------------------------

/*
==================
Attributes initialization
==================
*/
void IND_Entity3d::initAttrib() {
	// Assigned object
	_me = 0;

	// Showing
	_show = 1;

	// Space transformation attributes
	_x = 0;
	_y = 0;
	_z = 0;
	_angleX = 0;
	_angleY = 0;
	_angleZ = 0;
	_scaleX = 1;
	_scaleY = 1;
	_scaleZ = 1;
	_filter = IND_FILTER_LINEAR;

	// Color, transperency, fading and back face culling attributes
	_cull = 1;
	_flipNormals = 0;
	_r = 255;
	_g = 255;
	_b = 255;
	_a = 255;
	_fadeR = 0;
	_fadeG = 0;
	_fadeB = 0;
	_fadeA = 255;
	_so = 0;
	_ds = 0;

	// Animations attributes
	_changeSequenceFlag = 0;
	_sequence = 0;
}
