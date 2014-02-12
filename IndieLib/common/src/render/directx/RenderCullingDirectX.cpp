/*****************************************************************************************
 * File: RenderCullingDirectX.cpp
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

/** @cond DOCUMENT_PRIVATEAPI */

#ifdef INDIERENDER_DIRECTX
// ----- Includes -----

#include "Global.h"
#include "DirectXRender.h"
#include "IND_Math.h"

// --------------------------------------------------------------------------------
//							         Private methods
// --------------------------------------------------------------------------------

/*
==================
Calculates 6 planes defining the frustum
==================
*/
void DirectXRender::reCalculateFrustrumPlanes() {
	D3DXMATRIX mMatView, mMatProj;
	_info._device->GetTransform(D3DTS_VIEW, &mMatView);
	_info._device->GetTransform(D3DTS_PROJECTION, &mMatProj);

	// Get combined matrix
	D3DXMATRIXA16 matComb;
	D3DXMatrixMultiply(&matComb, &mMatView, &mMatProj);

	// Left clipping plane
	_frustrumPlanes._planes[0]._normal._x     = (matComb._14 + matComb._11);
	_frustrumPlanes._planes[0]._normal._y     = (matComb._24 + matComb._21);
	_frustrumPlanes._planes[0]._normal._z     = (matComb._34 + matComb._31);
	_frustrumPlanes._planes[0]._distance     = (matComb._44 + matComb._41);

	// Right clipping plane
	_frustrumPlanes._planes[1]._normal._x     = (matComb._14 - matComb._11);
	_frustrumPlanes._planes[1]._normal._y     = (matComb._24 - matComb._21);
	_frustrumPlanes._planes[1]._normal._z     = (matComb._34 - matComb._31);
	_frustrumPlanes._planes[1]._distance     = (matComb._44 - matComb._41);

	// Top clipping plane
	_frustrumPlanes._planes[2]._normal._x     = (matComb._14 - matComb._12);
	_frustrumPlanes._planes[2]._normal._y     = (matComb._24 - matComb._22);
	_frustrumPlanes._planes[2]._normal._z     = (matComb._34 - matComb._32);
	_frustrumPlanes._planes[2]._distance     = (matComb._44 - matComb._42);

	// Bottom clipping plane
	_frustrumPlanes._planes[3]._normal._x     = (matComb._14 + matComb._12);
	_frustrumPlanes._planes[3]._normal._y     = (matComb._24 + matComb._22);
	_frustrumPlanes._planes[3]._normal._z     = (matComb._34 + matComb._32);
	_frustrumPlanes._planes[3]._distance     = (matComb._44 + matComb._42);

	// Near clipping plane
	_frustrumPlanes._planes[4]._normal._x     = matComb._13;
	_frustrumPlanes._planes[4]._normal._y     = matComb._23;
	_frustrumPlanes._planes[4]._normal._z     = matComb._33;
	_frustrumPlanes._planes[4]._distance     = matComb._43;

	// Far clipping plane
	_frustrumPlanes._planes[5]._normal._x     = (matComb._14 - matComb._13);
	_frustrumPlanes._planes[5]._normal._y     = (matComb._24 - matComb._23);
	_frustrumPlanes._planes[5]._normal._z     = (matComb._34 - matComb._33);
	_frustrumPlanes._planes[5]._distance     = (matComb._44 - matComb._43);

	for (int i = 0;i<6;++i) {
		_frustrumPlanes._planes[i].normalise();
	}
}

/*
==================
Creates a bounding rectangle surronding the block for discarding it using frustum culling
==================
*/
void DirectXRender::Transform4Vertices(float pX1, float pY1,
                                       float pX2, float pY2,
                                       float pX3, float pY3,
                                       float pX4, float pY4,
                                       D3DXVECTOR4 *mP1Res,
                                       D3DXVECTOR4 *mP2Res,
                                       D3DXVECTOR4 *mP3Res,
                                       D3DXVECTOR4 *mP4Res) {
	// ----- Transform 4 vertices of the quad into world space coordinates -----

	D3DXMATRIX mMatWorld;
	_info._device->GetTransform(D3DTS_WORLD, &mMatWorld);

	D3DXVECTOR2 mP1(pX1, pY1);
	D3DXVECTOR2 mP2(pX2, pY2);
	D3DXVECTOR2 mP3(pX3, pY3);
	D3DXVECTOR2 mP4(pX4, pY4);

	D3DXVECTOR4 mP1_f, mP2_f, mP3_f, mP4_f;
	D3DXVec2Transform(mP1Res, &mP1, &mMatWorld);
	D3DXVec2Transform(mP2Res, &mP2, &mMatWorld);
	D3DXVec2Transform(mP3Res, &mP3, &mMatWorld);
	D3DXVec2Transform(mP4Res, &mP4, &mMatWorld);
}

/** @endcond */

#endif //INDIERENDER_DIRECTX
