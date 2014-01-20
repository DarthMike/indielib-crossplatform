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

/** @endcond */
#endif //INDIERENDER_GLES_IOS
