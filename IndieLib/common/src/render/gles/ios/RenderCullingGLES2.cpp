/*****************************************************************************************
 * File: RenderCullingOpenGL.cpp
 * Desc: Culling for 3d objects
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
#include "Defines.h"

#ifdef INDIERENDER_GLES_IOS
// ----- Includes -----

#include "Global.h"
#include "OpenGLES2Render.h"

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
void OpenGLES2Render::reCalculateFrustrumPlanes() {
	// Get combined matrix
	IND_Matrix matComb (_shaderProjectionMatrix);

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
void OpenGLES2Render::transformVerticesToWorld(float pX1, float pY1,
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
#endif //INDIERENDER_GLES_IOS
