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

/*!
\b Parameters:

\arg \b pX              Upper left X coordinate of the Viewport
\arg \b pY              Upper left Y coordinate of the Viewport
\arg \b pWidth          Viewport width
\arg \b pHeight         Viewport height

\b Operation:

This function returns 1 (true) if a 3d ViewPort is created in the specified area.

The ViewPort is the drawing region. You have to call to this method before
calling to IND_Entity3dManager::renderEntities3d().

After using this method, all the 3d bliting methods will render the graphical objects inside this Viewport.

This method returns 0 (false) if the user tries to create a Viewport outside the window area.
*/
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
	_info.mDevice->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&mMatWorld));

	// ----- View region  -----

	D3DVIEWPORT9 mViewData = { pX, pY, pWidth, pHeight, 0.0f, 1.0f };
	_info.mDevice->SetViewport(&mViewData);

	// ----- 3d Sets  -----

	_info.mDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	_info.mDevice->SetRenderState(D3DRS_AMBIENT, RGB(0, 0, 0));
	_info.mDevice->SetRenderState(D3DRS_LIGHTING, true);
	_info.mDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	_info.mDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	_info.mDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	_info.mDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	return 1;
}


/*!
\b Parameters:

\arg \b pCamera3d               ::IND_Camera3d object that defines a camera.

\b Operation:

This function sets a 3d camera. See the methods of ::IND_Camera3d for information on how you can manipulate the camera.
*/
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

	_info.mDevice->SetTransform(D3DTS_VIEW, &mMatView);

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

	_info.mDevice->SetTransform(D3DTS_PROJECTION, &mMatProjection);
}

void DirectXRender::beginTransform3d() {
	//Does nothing
}


/*!
\b Parameters:

\arg \b pX                          Translation in the X coordinate.
\arg \b pY                          Translation in the Y coordinate.
\arg \b pZ                          Translation in the Z coordinate.
\arg \b pAngleX                     Rotation in the angle x in degrees
\arg \b pAngleY                     Rotation in the angle y in degrees
\arg \b pAngleZ                     Rotation in the angle z in degrees
\arg \b pScaleX                     Scaling in the x coordinate. 1 to maintain the original size
\arg \b pScaleY                     Scaling in the y coordinate. 1 to maintain the original size
\arg \b pScaleZ                     Scaling in the z coordinate. 1 to maintain the original size
\arg \b pMatrix                     Pointer to a ::IND_Matrix matrix. In this parameter will be
                                    returned by reference the world matrix transformation that
                                    will be aplied to the graphical object. This matrix can be useful
                                    for advanced programmers that need the algebraic description
                                    of the transformation done. It is possible to use the value 0
                                    if it not necessary to have this matrix information.
\b Operation:

This function sets the 3d transformation (translation, rotation, scaling and mirroring)
of the following 3d graphical objects that will be rendered
by the engine. You should use this method before calling to any of the Bliting methods.

Remember that you can use IND_Entity3d object for applying 3d transformations to the graphical
objects without having to use this advanced method directly. This method is only useful for advanced
users for really concrete purposes.

This method is equivalent to use a combination of this methods:
- IND_Entity3d::set3dMesh()
- IND_Entity3d::setPosition()
- IND_Entity3d::setAngleXYZ()
- IND_Entity3d::setScale()
- IND_Entity2d::setBackCull()
- IND_Entity2d::setFilter()
*/
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

	_info.mDevice->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&mMatWorld));

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

	_info.mDevice->SetTransform(D3DTS_WORLD, &mMatWorld);
}


void DirectXRender::endTransform3d() {
	//Does nothing
}

/*!
\b Parameters:

\arg \b pCull                                       Backface culling. (true / false) = (on / off). This parameter can
                                                    be deactivated when we are going to go inside a 3dMesh and we want
                                                    to render both faces of the polygons.In any
                                                    other case this parameter must be activated for faster rendering
                                                    times preventing the hardware from drawing back faces.
\arg \b pFilter                                     Type of filter ::IND_Filter. There are two types of filters that
                                                    can be applied to the graphical object when it suffers a rotation
                                                    or scaling:
                                                    - ::IND_FILTER_POINT (Nearest point filter). Less quality, but
                                                    bigger performance. It is possible to use this one without
                                                    quality loss in graphical objects that don't rotate or are
                                                    affected by scaling.
                                                    - ::IND_FILTER_LINEAR (Bilinear filter). More quality, but less
                                                    performance. Used in graphical objects that be rotated or
                                                    affected by scaling.
\arg \b pR, pG, pB                                  Color bytes FadeR, FadeG, FadeB
                                                    Used for fading the image to a certain color.
                                                    For example, if we use RGB = (255, 0, 0),
                                                    the image will progressively become red.
\arg \b pA                                          The A byte indicates the level of transparency.
                                                    If a value of 0 is used, the following graphical
                                                    object will be completely transparent, as opposed
                                                    to the value 255 that will cause the object
                                                    to be drawn completely opaque. It is possible to use all the
                                                    intermediate values for different levels of
                                                    transparency.
\arg \b pFadeR, \b pFadeG, \b pFadeB, \b pFadeA     Bytes FadeR, FadeG, FadeB
                                                    Used for fading the image to a certain color.
                                                    For example, if we use RGB = (255, 0, 0),
                                                    the image will progressively become red.
\arg \b pSo                                         Indicates the blending source,
                                                    see (::IND_BlendingType).
\arg \b pDs                                         Indicates the blending destiny,
                                                    see (::IND_BlendingType).

Operation:

This functions sets the color and blending attributes of the following graphical object. It should
be used before any calling to the Blit methods.

Important: It is not possible to use tinting and fading at the same time, the fading will overide the tinting.

Remember that you can use IND_Entity2d objects for applying color
transformations to the graphical objects without having to use this advanced method directly.
This method is only useful for advanced users for really concrete purposes.

Using this method is equivalent to using a combination of these methods:
- IND_Entity2d::setTransparency()
- IND_Entity2d::setFade()
- IND_Entity2d::setTint()
- IND_Entity2d::setBlendSource()
- IND_Entity2d::setBlendDest()
*/
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

	_info.mDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, GetD3DFilter(pFilter));
	_info.mDevice->SetSamplerState(0, D3DSAMP_MINFILTER, GetD3DFilter(pFilter));
	_info.mDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, GetD3DFilter(pFilter));

	// ----- Back face culling -----

	if (pCull) {
		if (!pFlipNormals)
			_info.mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		else
			_info.mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	} else {
		_info.mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	// ----- Blending -----

	// Alpha-testing = OFF
	_info.mDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Initializes the tinting and alpha values of previous iterations
	_info.mDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	_info.mDevice->SetTextureStageState(0, D3DTSS_COLORARG1,  D3DTA_TEXTURE);
	_info.mDevice->SetTextureStageState(0, D3DTSS_COLORARG2,  D3DTA_CURRENT);
	_info.mDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	_info.mDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_info.mDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// Tinting
	if (pR != 255 || pG != 255 || pB != 255) {
		// Tinting color
		_info.mDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(pR, pG, pB, pA));

		// Tinting
		_info.mDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		_info.mDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		_info.mDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	}


	// Alpha
	if (pA != 255) {
		// Color alpha
		_info.mDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(pR, pG, pB, pA));

		_info.mDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		_info.mDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		_info.mDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	}

	// Fade to color
	if (pFadeA != 255) {
		// Color fade
		_info.mDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(pFadeR, pFadeG, pFadeB, pFadeA));

		// Fade
		_info.mDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_BLENDFACTORALPHA);
		_info.mDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		_info.mDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	}

	// Alpha-blending = ON
	_info.mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,  TRUE);

	// Set source alpha and destination alpha
	if (!pSo || !pDs) {
		_info.mDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		_info.mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	} else {
		_info.mDevice->SetRenderState(D3DRS_SRCBLEND,  GetD3DBlendingType(pSo));
		_info.mDevice->SetRenderState(D3DRS_DESTBLEND, GetD3DBlendingType(pDs));
	}
}

#endif //INDIERENDER_DIRECTX
