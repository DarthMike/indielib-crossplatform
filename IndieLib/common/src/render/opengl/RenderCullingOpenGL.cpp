/*****************************************************************************************
 * File: RenderCullingOpenGL.cpp
 * Desc: Culling for 3d objects
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
#include "Defines.h"

#ifdef INDIERENDER_OPENGL
// ----- Includes -----

#include "Global.h"
#include "OpenGLRender.h"

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Private methods
// --------------------------------------------------------------------------------

/*
==================
Calculates 6 planes defining the frustum
Plane extraction works as per this:
http://www8.cs.umu.se/kurser/5DV051/HT12/lab/plane_extraction.pdf
We calculate planes in model space, that is, before being transformed by camera.
To perform culling calculations, the comparisons need to be in same space. In our case, model space.
==================
*/
void OpenGLRender::reCalculateFrustrumPlanes() {
	IND_Matrix  mMatProj;
	float temp [16];
	glGetFloatv(GL_PROJECTION_MATRIX,temp);
	mMatProj.readFromArray(temp);

	// Get combined matrix
	IND_Matrix matComb;
	_math.matrix4DMultiply(mMatProj,_cameraMatrix,matComb);

	// Left clipping plane
	_frustrumPlanes._planes[0]._normal._x     = (matComb._41 + matComb._11);
	_frustrumPlanes._planes[0]._normal._y     = (matComb._42 + matComb._12);
	_frustrumPlanes._planes[0]._normal._z     = (matComb._43 + matComb._13);
	_frustrumPlanes._planes[0]._distance     = (matComb._44 + matComb._14);

	// Right clipping plane
	_frustrumPlanes._planes[1]._normal._x     = (matComb._41 - matComb._11);
	_frustrumPlanes._planes[1]._normal._y     = (matComb._42 - matComb._12);
	_frustrumPlanes._planes[1]._normal._z     = (matComb._43 - matComb._13);
	_frustrumPlanes._planes[1]._distance     = (matComb._44 - matComb._14);

	// Top clipping plane
	_frustrumPlanes._planes[2]._normal._x     = (matComb._41 - matComb._21);
	_frustrumPlanes._planes[2]._normal._y     = (matComb._42 - matComb._22);
	_frustrumPlanes._planes[2]._normal._z     = (matComb._43 - matComb._23);
	_frustrumPlanes._planes[2]._distance     = (matComb._44 - matComb._24);

	// Bottom clipping plane
	_frustrumPlanes._planes[3]._normal._x     = (matComb._41 + matComb._21);
	_frustrumPlanes._planes[3]._normal._y     = (matComb._42 + matComb._22);
	_frustrumPlanes._planes[3]._normal._z     = (matComb._43 + matComb._23);
	_frustrumPlanes._planes[3]._distance     = (matComb._44 + matComb._24);

	// Near clipping plane
	_frustrumPlanes._planes[4]._normal._x     = (matComb._41 + matComb._31);
	_frustrumPlanes._planes[4]._normal._y     = (matComb._42 + matComb._32);
	_frustrumPlanes._planes[4]._normal._z     = (matComb._43 + matComb._33);
	_frustrumPlanes._planes[4]._distance     = (matComb._44 + matComb._34);

	// Far clipping plane
	_frustrumPlanes._planes[5]._normal._x     = (matComb._41 - matComb._31);
	_frustrumPlanes._planes[5]._normal._y     = (matComb._42 - matComb._32);
	_frustrumPlanes._planes[5]._normal._z     = (matComb._43 - matComb._33);
	_frustrumPlanes._planes[5]._distance     = (matComb._44 - matComb._34);

	for (int i = 0;i<6;++i) {
		_frustrumPlanes._planes[i].normalise();
	}
}

/*
==================
Transforms vertices (supposedly from a quad) to world coordinates using the cached
model-to-world transform, already loaded in GL state
==================
*/
void OpenGLRender::transformVerticesToWorld(float pX1, float pY1,
											float pX2, float pY2,
											float pX3, float pY3,
											float pX4, float pY4,
											IND_Vector3 *mP1Res,
											IND_Vector3 *mP2Res,
											IND_Vector3 *mP3Res,
											IND_Vector3 *mP4Res) {
	if (!mP1Res || !mP2Res || !mP3Res || !mP4Res) {
		return;
	}

	IND_Vector3 mP1(pX1, pY1,0.0f);
	IND_Vector3 mP2(pX2, pY2,0.0f);
	IND_Vector3 mP3(pX3, pY3,0.0f);
	IND_Vector3 mP4(pX4, pY4,0.0f);

	_math.transformVector3DbyMatrix4D(mP1,_modelToWorld);
	_math.transformVector3DbyMatrix4D(mP2,_modelToWorld);
	_math.transformVector3DbyMatrix4D(mP3,_modelToWorld);
	_math.transformVector3DbyMatrix4D(mP4,_modelToWorld);

	//What we want to do here is copy members, not pointers. We rely on operator overloading
	*mP1Res = mP1;
	*mP2Res = mP2;
	*mP3Res = mP3;
	*mP4Res = mP4;
}

/** @endcond */
#endif //INDIERENDER_OPENGL
