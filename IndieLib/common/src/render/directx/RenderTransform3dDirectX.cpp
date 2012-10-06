/*****************************************************************************************
 * File: RenderTransform3dDirectX.cpp
 * Desc: Transformations applied before blitting a 3d object usind D3D
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (javierlopezpro@gmail.com)

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

#ifdef INDIERENDER_DIRECTX
// ----- Includes -----

#include "Global.h"
#include "Defines.h"
#include "DirectXRender.h"
#include "IND_Camera3d.h"

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

bool DirectXRender::setViewPort3d(int pX,
                                  int pY,
                                  int pWidth,
                                  int pHeight) {
	// ----- If the region is outside the framebuffer return error -----

	if (pX +  pWidth > _info._fbWidth)  return 0;
	if (pX < 0)                         return 0;
	if (pY + pHeight > _info._fbHeight) return 0;
	if (pY < 0)                         return 0;

	// ----- Viewport characteristics -----

	_info._viewPortX      = pX;
	_info._viewPortY      = pY;
	_info._viewPortWidth  = pWidth;
	_info._viewPortHeight = pHeight;

	// ----- World matrix -----

	D3DXMATRIX mMatWorld;
	_info._device->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&mMatWorld));

	// ----- View region  -----

	D3DVIEWPORT9 mViewData = { pX, pY, pWidth, pHeight, 0.0f, 1.0f };
	_info._device->SetViewport(&mViewData);

	// ----- 3d Sets  -----

	_info._device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	_info._device->SetRenderState(D3DRS_AMBIENT, RGB(0, 0, 0));
	_info._device->SetRenderState(D3DRS_LIGHTING, true);
	_info._device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	_info._device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	_info._device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	_info._device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	return 1;
}

void DirectXRender::setCamera3d(IND_Camera3d *pCamera3d) {
	D3DXMATRIX mTrans, mMatView, mMatProjection;
	D3DXMatrixIdentity(&mMatView);
	D3DXMatrixIdentity(&mMatProjection);

	// ----- View matrix -----

	pCamera3d->_up      = IND_Vector3 (0.0f, 1.0f, 0.0f);
	pCamera3d->_look    = IND_Vector3 (0.0f, 0.0f, 1.0f);
	pCamera3d->_right   = IND_Vector3 (1.0f, 0.0f, 0.0f);
    //Buffer D3DVec3 structs from our camera 3d vectors
    D3DXVECTOR3 d3dpos (pCamera3d->_pos._x,pCamera3d->_pos._y,pCamera3d->_pos._z);
    D3DXVECTOR3 d3dlook (pCamera3d->_look._x,pCamera3d->_look._y, pCamera3d->_look._z);
    D3DXVECTOR3 d3dup (pCamera3d->_up._x,pCamera3d->_up._y, pCamera3d->_up._z);
    D3DXVECTOR3 d3dright (pCamera3d->_right._x,pCamera3d->_right._y, pCamera3d->_right._z);

	// Yaw is rotation around the y axis (_up)
	// Create a matrix that can carry out this rotation
	D3DXMATRIX yawMatrix;
	D3DXMatrixRotationAxis(&yawMatrix, &d3dup, D3DXToRadian(pCamera3d->_yaw));
	// To apply yaw we rotate the _look & _right vectors about the _up vector (using our yaw matrix)
	D3DXVec3TransformCoord(&d3dlook, &d3dlook, &yawMatrix);
	D3DXVec3TransformCoord(&d3dright, &d3dright, &yawMatrix);

	// Pitch is rotation around the x axis (_right)
	// Create a matrix that can carry out this rotation
	D3DXMATRIX pitchMatrix;
	D3DXMatrixRotationAxis(&pitchMatrix, &d3dright, D3DXToRadian(pCamera3d->_pitch));
	// To apply pitch we rotate the _look and _up vectors about the _right vector (using our pitch matrix)
	D3DXVec3TransformCoord(&d3dlook, &d3dlook, &pitchMatrix);
	D3DXVec3TransformCoord(&d3dup, &d3dup, &pitchMatrix);

	// Roll is rotation around the z axis (_look)
	// Create a matrix that can carry out this rotation
	D3DXMATRIX rollMatrix;
	D3DXMatrixRotationAxis(&rollMatrix, &d3dlook, D3DXToRadian(pCamera3d->_roll));
	// To apply roll we rotate up and right about the look vector (using our roll matrix)
	// Note: roll only really applies for things like aircraft unless you are implementing lean
	D3DXVec3TransformCoord(&d3dright, &d3dright, &rollMatrix);
	D3DXVec3TransformCoord(&d3dup, &d3dup, &rollMatrix);

	// Build the view matrix from the transformed camera axis
	mMatView._11 = pCamera3d->_right._x;
	mMatView._12 = pCamera3d->_up._x;
	mMatView._13 = pCamera3d->_look._x;
	mMatView._21 = pCamera3d->_right._y;
	mMatView._22 = pCamera3d->_up._y;
	mMatView._23 = pCamera3d->_look._y;
	mMatView._31 = pCamera3d->_right._z;
	mMatView._32 = pCamera3d->_up._z;
	mMatView._33 = pCamera3d->_look._z;

	mMatView._41 = - D3DXVec3Dot(&d3dright, &d3dright);
	mMatView._42 = - D3DXVec3Dot(&d3dright, &d3dup);
	mMatView._43 = - D3DXVec3Dot(&d3dright, &d3dlook);

	// ---- Zoom ----

	D3DXMatrixScaling(&mTrans, pCamera3d->_zoom, pCamera3d->_zoom, pCamera3d->_zoom);
	D3DXMatrixMultiply(&mMatView, &mTrans, &mMatView);

	_info._device->SetTransform(D3DTS_VIEW, &mMatView);

	// ----- Projection matrix -----

	// Fov projection
	if (!pCamera3d->isOrthoProjection()) {
		D3DXMatrixPerspectiveFovLH(&mMatProjection,                         // output
		                           pCamera3d->_fov,                        // Fov vertical
		                           pCamera3d->_aspect,                     // Relación de aspecto del viewport
		                           pCamera3d->_nearClippingPlane,          // Near clipping plane z
		                           pCamera3d->_farClippingPlane);          // Far clipping  plane z
	}
	// Orthographic projection
	else {
		D3DXMatrixOrthoLH(&mMatProjection, pCamera3d->_orthoWidth, pCamera3d->_orthoHeight, pCamera3d->_nearClippingPlane, pCamera3d->_farClippingPlane);
	}

	_info._device->SetTransform(D3DTS_PROJECTION, &mMatProjection);
}


void DirectXRender::setTransform3d(float pX,
                                   float  pY,
                                   float  pZ,
                                   float pAngleX,
                                   float pAngleY,
                                   float pAngleZ,
                                   float pScaleX,
                                   float pScaleY,
                                   float pScaleZ,
                                   IND_Matrix *pMatrix)


{
	// ----- World matrix initialization -----

	D3DXMATRIX mMatWorld, mMatZ, mMatX, mMatY, mMatTraslation, mMatScale;

	_info._device->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&mMatWorld));

	// ----- Transformation matrix creation -----

	// Scaling
	if (pScaleX != 1.0f || pScaleY != 1.0f || pScaleZ != 1.0f) {
		D3DXMatrixScaling(&mMatScale, pScaleX, pScaleY, pScaleZ);
		D3DXMatrixMultiply(&mMatWorld, &mMatWorld, &mMatScale);
	}

	// Rotations
	if (pAngleX != 0.0f) {
		D3DXMatrixRotationX(&mMatX, D3DXToRadian(pAngleX));
		D3DXMatrixMultiply(&mMatWorld, &mMatWorld, &mMatX);
	}
	if (pAngleY != 0.0f) {
		D3DXMatrixRotationY(&mMatY, D3DXToRadian(pAngleY));
		D3DXMatrixMultiply(&mMatWorld, &mMatWorld, &mMatY);
	}
	if (pAngleZ != 0.0f) {
		D3DXMatrixRotationZ(&mMatZ, D3DXToRadian(pAngleZ));
		D3DXMatrixMultiply(&mMatWorld, &mMatWorld, &mMatZ);
	}

	// Translation
	if (pX != 0 || pY != 0 || pZ != 0) {
		D3DXMatrixTranslation(&mMatTraslation, (float) pX, (float) pY, float(pZ));
		D3DXMatrixMultiply(&mMatWorld, &mMatWorld, &mMatTraslation);
	}

	// ----- Return World Matrix (in IndieLib format) -----

	if (pMatrix) {
		pMatrix->_11 = mMatWorld._11;
		pMatrix->_12 = mMatWorld._12;
		pMatrix->_13 = mMatWorld._13;
		pMatrix->_14 = mMatWorld._14;

		pMatrix->_21 = mMatWorld._21;
		pMatrix->_22 = mMatWorld._22;
		pMatrix->_23 = mMatWorld._23;
		pMatrix->_24 = mMatWorld._24;

		pMatrix->_31 = mMatWorld._31;
		pMatrix->_32 = mMatWorld._32;
		pMatrix->_33 = mMatWorld._33;
		pMatrix->_34 = mMatWorld._34;

		pMatrix->_41 = mMatWorld._41;
		pMatrix->_42 = mMatWorld._42;
		pMatrix->_43 = mMatWorld._43;
		pMatrix->_44 = mMatWorld._44;
	}

	// ----- Applies the transformation -----

	_info._device->SetTransform(D3DTS_WORLD, &mMatWorld);
}


void DirectXRender::setRainbow3d(bool pCull,
                                 bool pFlipNormals,
                                 IND_Filter pFilter,
                                 BYTE pR,
                                 BYTE pG,
                                 BYTE pB,
                                 BYTE pA,
                                 BYTE pFadeR,
                                 BYTE pFadeG,
                                 BYTE pFadeB,
                                 BYTE pFadeA,
                                 IND_BlendingType pSo,
                                 IND_BlendingType pDs) {
	// ----- Filters -----

	_info._device->SetSamplerState(0, D3DSAMP_MIPFILTER, GetD3DFilter(pFilter));
	_info._device->SetSamplerState(0, D3DSAMP_MINFILTER, GetD3DFilter(pFilter));
	_info._device->SetSamplerState(0, D3DSAMP_MAGFILTER, GetD3DFilter(pFilter));

	// ----- Back face culling -----

	if (pCull) {
		if (!pFlipNormals)
			_info._device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		else
			_info._device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	} else {
		_info._device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	// ----- Blending -----

	// Alpha-testing = OFF
	_info._device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Initializes the tinting and alpha values of previous iterations
	_info._device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	_info._device->SetTextureStageState(0, D3DTSS_COLORARG1,  D3DTA_TEXTURE);
	_info._device->SetTextureStageState(0, D3DTSS_COLORARG2,  D3DTA_CURRENT);
	_info._device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	_info._device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_info._device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// Tinting
	if (pR != 255 || pG != 255 || pB != 255) {
		// Tinting color
		_info._device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(pR, pG, pB, pA));

		// Tinting
		_info._device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		_info._device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		_info._device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	}


	// Alpha
	if (pA != 255) {
		// Color alpha
		_info._device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(pR, pG, pB, pA));

		_info._device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		_info._device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		_info._device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	}

	// Fade to color
	if (pFadeA != 255) {
		// Color fade
		_info._device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(pFadeR, pFadeG, pFadeB, pFadeA));

		// Fade
		_info._device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_BLENDFACTORALPHA);
		_info._device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		_info._device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	}

	// Alpha-blending = ON
	_info._device->SetRenderState(D3DRS_ALPHABLENDENABLE,  TRUE);

	// Set source alpha and destination alpha
	if (!pSo || !pDs) {
		_info._device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		_info._device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	} else {
		_info._device->SetRenderState(D3DRS_SRCBLEND,  GetD3DBlendingType(pSo));
		_info._device->SetRenderState(D3DRS_DESTBLEND, GetD3DBlendingType(pDs));
	}
}

#endif //INDIERENDER_DIRECTX
