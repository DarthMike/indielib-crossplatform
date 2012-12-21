/*****************************************************************************************
 * File: RenderCullingDirectX.cpp
 * Desc: Culling for 3d objects
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
void DirectXRender::calculateFrustumPlanes() {
	D3DXMATRIX mMatView, mMatProj;
	_info._device->GetTransform(D3DTS_VIEW, &mMatView);
	_info._device->GetTransform(D3DTS_PROJECTION, &mMatProj);

	// Get combined matrix
	D3DXMATRIXA16 matComb;
	D3DXMatrixMultiply(&matComb, &mMatView, &mMatProj);

	// Left clipping plane
	mFrustumPlanes[0].mNormal._x     = -(matComb._14 + matComb._11);
	mFrustumPlanes[0].mNormal._y     = -(matComb._24 + matComb._21);
	mFrustumPlanes[0].mNormal._z     = -(matComb._34 + matComb._31);
	mFrustumPlanes[0].mDistance     = -(matComb._44 + matComb._41);

	// Right clipping plane
	mFrustumPlanes[1].mNormal._x     = -(matComb._14 - matComb._11);
	mFrustumPlanes[1].mNormal._y     = -(matComb._24 - matComb._21);
	mFrustumPlanes[1].mNormal._z     = -(matComb._34 - matComb._31);
	mFrustumPlanes[1].mDistance     = -(matComb._44 - matComb._41);

	// Top clipping plane
	mFrustumPlanes[2].mNormal._x     = -(matComb._14 - matComb._12);
	mFrustumPlanes[2].mNormal._y     = -(matComb._24 - matComb._22);
	mFrustumPlanes[2].mNormal._z     = -(matComb._34 - matComb._32);
	mFrustumPlanes[2].mDistance     = -(matComb._44 - matComb._42);

	// Bottom clipping plane
	mFrustumPlanes[3].mNormal._x     = -(matComb._14 + matComb._12);
	mFrustumPlanes[3].mNormal._y     = -(matComb._24 + matComb._22);
	mFrustumPlanes[3].mNormal._z     = -(matComb._34 + matComb._32);
	mFrustumPlanes[3].mDistance     = -(matComb._44 + matComb._42);

	// Near clipping plane
	mFrustumPlanes[4].mNormal._x     = -(matComb._14 + matComb._13);
	mFrustumPlanes[4].mNormal._y     = -(matComb._24 + matComb._23);
	mFrustumPlanes[4].mNormal._z     = -(matComb._34 + matComb._33);
	mFrustumPlanes[4].mDistance     = -(matComb._44 + matComb._43);

	// Far clipping plane
	mFrustumPlanes[5].mNormal._x     = -(matComb._14 - matComb._13);
	mFrustumPlanes[5].mNormal._y     = -(matComb._24 - matComb._23);
	mFrustumPlanes[5].mNormal._z     = -(matComb._34 - matComb._33);
	mFrustumPlanes[5].mDistance     = -(matComb._44 - matComb._43);

	//for (int i = 0; i < 6; i++)
	//mFrustumPlanes [i].Normalise();
}


/*
==================
Taking an AABB min and max in world space, work out its interaction with the view frustum
0 is outside
1 is partially in
2 is completely within
Note: the viewing frustum must be calculated first
==================
*/
WORD DirectXRender::CullFrustumBox(const IND_Vector3 &pAABBMin, const IND_Vector3 &pAABBMax) {
	bool mIntersect = 0;
	WORD mResult = 0;
	IND_Vector3 mMinExtreme, mMaxExtreme;

	for (WORD i = 0; i < 6; i++) {
		if (mFrustumPlanes[i].mNormal._x >= 0) {
			mMinExtreme._x = pAABBMin._x;
			mMaxExtreme._x = pAABBMax._x;
		} else {
			mMinExtreme._x = pAABBMax._x;
			mMaxExtreme._x = pAABBMin._x;
		}

		if (mFrustumPlanes[i].mNormal._y >= 0) {
			mMinExtreme._y = pAABBMin._y;
			mMaxExtreme._y = pAABBMax._y;
		} else {
			mMinExtreme._y = pAABBMax._y;
			mMaxExtreme._y = pAABBMin._y;
		}

		if (mFrustumPlanes[i].mNormal._z >= 0) {
			mMinExtreme._z = pAABBMin._z;
			mMaxExtreme._z = pAABBMax._z;
		} else {
			mMinExtreme._z = pAABBMax._z;
			mMaxExtreme._z = pAABBMin._z;
		}

		if (mFrustumPlanes[i].DistanceToPoint(mMinExtreme) > 0) {
			mResult  = 0;
			return mResult;
		}

		if (mFrustumPlanes[i].DistanceToPoint(mMaxExtreme) >= 0)
			mIntersect = 1;
	}

	if (mIntersect)
		mResult = 1;
	else
		mResult = 2;

	return mResult;

}


/*
==================
Creates a bounding rectangle surronding the block for discarding it using frustum culling
==================
*/
void DirectXRender::CalculateBoundingRectangle(IND_Vector3 *mP1, IND_Vector3 *mP2, IND_Vector3 *mP3, IND_Vector3 *mP4) {
	int mMinX, mMaxX, mMinY, mMaxY;

	_math->minAndMax4((int) mP1->_x, (int) mP2->_x, (int) mP3->_x, (int) mP4->_x, &mMaxX, &mMinX);
	_math->minAndMax4((int) mP1->_y, (int) mP2->_y, (int) mP3->_y, (int) mP4->_y, &mMaxY, &mMinY);

	mP1->_x = (float) mMinX;
	mP1->_y = (float) mMinY;
	mP2->_x = (float) mMaxX;
	mP2->_y = (float) mMaxY;
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
