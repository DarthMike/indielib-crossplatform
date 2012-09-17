/*****************************************************************************************
 * File: RenderTransform2dDirectX.cpp
 * Desc: Transformations applied before blitting a 2d object usind D3D
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
#include "IND_Camera2d.h"


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

This function returns 1 (true) if a 2d ViewPort is created in the specified area.

The ViewPort is the drawing region. By default, IndieLib is initialized with a Viewport
that has the same area as the window. Using this method you can define different viewports before
calling to IND_Entity2dManager::renderEntities2d().

After using this method, all the bliting methods will render the graphical objects inside the Viewport.
Objects or blocks drawn outside the viewport will be discarded.

This method returns 0 (false) if the user tries to create a Viewport outside the window area.
*/
bool DirectXRender::setViewPort2d(int pX,
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

	D3DVIEWPORT9 mViewData = { pX, pY, pWidth, pHeight, -2048.0f, 2048.0f };
	_info._device->SetViewport(&mViewData);

	// ----- 2d Sets -----

	_info._device->SetRenderState(D3DRS_NORMALIZENORMALS, false);
	_info._device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	_info._device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	_info._device->SetRenderState(D3DRS_LIGHTING, false);

	// ----- Texture clamp on -----

	_info._device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	_info._device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	return 1;
}


/*!
\b Parameters:

\arg \b pCamera2d               ::IND_Camera2d object that defines a camera.

\b Operation:

This function sets a 2d camera. See the methods of ::IND_Camera2d for information on how you can manipulate the camera.
*/
void DirectXRender::setCamera2d(IND_Camera2d *pCamera2d) {
    
	D3DXMATRIX mScale, mRotate, mMatLookAt, mMatView, mMatProjection;
	D3DXMatrixIdentity (&mScale);
	D3DXMatrixIdentity (&mRotate);
	D3DXMatrixIdentity (&mMatLookAt);
	D3DXMatrixIdentity (&mMatView);
	D3DXMatrixIdentity (&mMatProjection);

    //Buffer D3DVec3 structs from our camera 3d vectors
    D3DXVECTOR3 d3dpos (pCamera2d->_pos._x,pCamera2d->_pos._y,pCamera2d->_pos._z);
    D3DXVECTOR3 d3dlook (pCamera2d->_look._x,pCamera2d->_look._y, pCamera2d->_look._z);
    D3DXVECTOR3 d3dup (pCamera2d->_up._x,pCamera2d->_up._y, pCamera2d->_up._z);
    D3DXVECTOR3 d3dright (pCamera2d->_right._x,pCamera2d->_right._y, pCamera2d->_right._z);

	// ----- Rotation -----
	// Roll is rotation around the z axis (_look)
	D3DXMatrixRotationAxis (&mRotate, &d3dlook, D3DXToRadian(pCamera2d->_angle - pCamera2d->_prevAngle));
	//To apply roll we rotate up and right about the look vector (using our roll matrix)
	D3DXVec3TransformCoord (&d3dright, &d3dright, &mRotate); 
	D3DXVec3TransformCoord (&d3dup, &d3dup, &mRotate);
	//Put the values back again to camera.
	pCamera2d->_right._x = d3dright.x;
	pCamera2d->_right._y = d3dright.y;
	pCamera2d->_right._z = d3dright.z;
	pCamera2d->_up._x = d3dup.x;
	pCamera2d->_up._y = d3dup.y;
	pCamera2d->_up._z = d3dup.z;
	//Make prev and actual angle in camera equal
	pCamera2d->setAngle(pCamera2d->_angle);

	// Build the view matrix from the camera axis
	//Utility DirectX function has use of 'at' vector instead of 'look' vector. Compute it:
	D3DXVECTOR3 d3dAt (d3dpos.x + d3dlook.x,d3dpos.y + d3dlook.y,d3dpos.z + d3dlook.z);
	D3DXMatrixLookAtLH(&mMatLookAt,&d3dpos,&d3dAt,&d3dup);
	D3DXMatrixMultiply(&mMatView, &mMatView, &mMatLookAt);

	// ---- Zoom ----

	if (pCamera2d->_zoom != 1.0f)
	{
		D3DXMatrixScaling (&mScale, pCamera2d->_zoom, pCamera2d->_zoom, pCamera2d->_zoom);
		D3DXMatrixMultiply (&mMatView, &mMatView, &mScale);
	}

	// ----- Set transformation -----
	_info._device->SetTransform(D3DTS_VIEW, &mMatView);

	// ----- Projection matrix -----

	D3DXMatrixOrthoLH(&mMatProjection, static_cast<float>( _info._viewPortWidth), static_cast<float>( _info._viewPortHeight), -2048.0f, 2048.0f);

	_info._device->SetTransform(D3DTS_PROJECTION, &mMatProjection);
}


/*!
\defgroup Graphical_Objects Bliting Surfaces, Animations, Fonts and setting transformations directly
\ingroup Advances
*/
/*@{*/

/*!
\b Parameters:

\arg \b pX                          Translation in the X coordinate. The (0, 0) position is the
                                    upper-left corner of the Viewport
\arg \b pY                          Translation in the Y coordinate. The (0, 0) position is the
                                    upper-left corner of the Viewport
\arg \b pAngleX                     Rotation in the angle x in degrees
\arg \b pAngleY                     Rotation in the angle y in degrees
\arg \b pAngleZ                     Rotation in the angle z in degrees
\arg \b pScaleX                     Scaling in the x coordinate. 1 for maintaining the original size
\arg \b pScaleY                     Scaling in the y coordinate. 1 for maintaining the original size
\arg \b pAxisCalX, \b pAxisCalY     Parameters that indicates the displacement that the graphical
                                    object undergoes due to the HotSpot. If the HotSpot is not specified,
                                    the value should be 0 for both of them. All the transformation
                                    will be aplied from the upper-left corner of the object.
\arg \b pMirrorX                    Horizontal mirroring. (true / false) = (activated / deactivated).
\arg \b pMirrorY                    Vertical mirroring. (true / false) = (activated / deactivated).
\arg \b pWidth                      Width of the graphical object that we are going to blit just after
                                    applying the transformation. You shoud use the getWidth() method
                                    of the object.
\arg \b pHeight                     Height of the graphical object that we are going to blit just after
                                    applying the transformation. You shoud use the getHeight() method
                                    on the object.
\arg \b pMatrix                     Pointer to a ::IND_Matrix matrix. In this parameter will be
                                    returned by reference the world matrix transformation that
                                    will be aplied to the graphical object. This matrix can be useful
                                    for advanced programmers that need the algebraic description
                                    of the transformation. It is possible to use the value 0
                                    if it not necessary to have this matrix information.
\b Operation:

This function sets the 2d transformation (translation, rotation, scaling, mirroring and hotspot)
of the following 2d graphical objects that will be rendered
by the engine. You should use this method before calling to any of the Bliting methods.

Remember that you can use IND_Entity2d object for applying 2d transformations to the graphical
objects without having to use this advanced method directly. This method is only useful for advanced
users with really concrete purposes.

Using this method is equivalent to using a combination of these methods:
- IND_Entity2d::setAnimation()
- IND_Entity2d::setSurface()
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setFont()
- IND_Entity2d::setPosition()
- IND_Entity2d::setAngleXYZ()
- IND_Entity2d::setScale()
- IND_Entity2d::setBackCull()
- IND_Entity2d::setMirrorX()
- IND_Entity2d::setMirrorY()
- IND_Entity2d::setFilter()
- IND_Entity2d::setHotSpot()
- IND_Entity2d::setRegion()
- IND_Entity2d::toggleWrap()
*/
void DirectXRender::setTransform2d(int pX,
                                   int pY,
                                   float pAngleX,
                                   float pAngleY,
                                   float pAngleZ,
                                   float pScaleX,
                                   float pScaleY,
                                   int pAxisCalX,
                                   int pAxisCalY,
                                   bool pMirrorX,
                                   bool pMirrorY,
                                   int pWidth,
                                   int pHeight,
                                   IND_Matrix *pMatrix) {
	// ----- World matrix initialization -----

	D3DXMATRIX mMatWorld, mMatZ, mMatX, mMatY, mMatTraslation, mMatScale;
	//Initializes every object transform with pixel to point scale transform
	_info._device->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&mMatWorld));

	// ----- Transformation matrix creation -----

	// Mirroring (180º rotations)
	if (pMirrorX || pMirrorY) {
		//A mirror is a rotation in desired axis (the actual mirror) and a repositioning because rotation
		//also moves 'out of place' the entity translation-wise
		if (pMirrorX) {
			D3DXMatrixTranslation(&mMatTraslation, 
								  static_cast<float>(- pWidth+pAxisCalX),//pWidth is the neeeded amount for normal mirroring, pAxisCalX is a correction for hotspot
								  static_cast<float>(-pAxisCalY), //Corrects the next translation when hotspot is on in Y
								  0);
			D3DXMatrixMultiply(&mMatWorld, &mMatWorld, &mMatTraslation);
			D3DXMatrixRotationY(&mMatY, D3DXToRadian(180));
			D3DXMatrixMultiply(&mMatWorld, &mMatWorld, &mMatY);
		}

		//A mirror is a rotation in desired axis (the actual mirror) and a repositioning because rotation
		//also moves 'out of place' the entity translation-wise
		if (pMirrorY) {
			D3DXMatrixTranslation(&mMatTraslation, 
								  static_cast<float>(-pAxisCalX),  //Corrects the next translation when hotspot is on in X
								  static_cast<float>( - pHeight+pAxisCalY), //pHeight is the neeeded amount for normal mirroring, pAxisCalY is a correction for hotspot
								  0);
			D3DXMatrixMultiply(&mMatWorld, &mMatWorld, &mMatTraslation);
			D3DXMatrixRotationX(&mMatX, D3DXToRadian(180));
			D3DXMatrixMultiply(&mMatWorld, &mMatWorld, &mMatX);
		}
	}

	// Hotspot
	if (pAxisCalX != 0 || pAxisCalY != 0) {
		D3DXMatrixTranslation(&mMatTraslation, static_cast<float>( pAxisCalX), static_cast<float>( pAxisCalY), 0);
		D3DXMatrixMultiply(&mMatWorld, &mMatWorld, &mMatTraslation);
	}

	// Scaling
	if (pScaleX != 1.0f || pScaleY != 1.0f) {
		D3DXMatrixScaling(&mMatScale, pScaleX, pScaleY, 1.0f);
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

	// Translations
	if (pX != 0 || pY != 0) {
		D3DXMatrixTranslation(&mMatTraslation, static_cast<float>(pX), static_cast<float>(pY), 0);
		D3DXMatrixMultiply(&mMatWorld, &mMatWorld, &mMatTraslation);
	}

	D3DXMATRIX mMatPointPixel;
	D3DXMatrixScaling(&mMatPointPixel,_info._pointPixelScale,_info._pointPixelScale,1.0f);
	D3DXMatrixMultiply(&mMatWorld,&mMatWorld,&mMatPointPixel);

	// ----- Return World Matrix (in IndieLib format) -----
	if (pMatrix) {
		pMatrix->readFromArray(&mMatWorld.m[0][0]);
	}

	// ----- Applies the transformation -----
	_info._device->SetTransform(D3DTS_WORLD, &mMatWorld);
}

void DirectXRender::setTransform2d(IND_Matrix &pMatrix) {
	// ----- Return World Matrix (in DirectX format) -----
	float matArray[16];
	pMatrix.arrayRepresentation(matArray);
	D3DXMATRIX mMatWorld (matArray);

	//Apply pixel to point scale to each object transform
	D3DXMATRIX mMatPointPixel;
	D3DXMatrixScaling(&mMatPointPixel,_info._pointPixelScale,_info._pointPixelScale,1.0f);
	D3DXMatrixMultiply(&mMatWorld,&mMatWorld,&mMatPointPixel);

	// ----- Applies the transformation -----
	_info._device->SetTransform(D3DTS_WORLD, &mMatWorld);
}

void DirectXRender::setIdentityTransform2d ()  {
	// ----- Applies the transformation -----
	D3DXMATRIX mMatWorld;
	//Initializes every object transform with pixel to point scale transform
	_info._device->SetTransform(D3DTS_WORLD, D3DXMatrixScaling(&mMatWorld,_info._pointPixelScale,_info._pointPixelScale,1.0f));
}


/*!
\b Parameters:

\arg \b pType                                       ::IND_Type type of the following graphical object.
\arg \b pCull                                       Backface culling. (true / false) = (on / off). This feature can
                                                    be deactivated when we are going to rotate the graphical object
                                                    in the x or y coordinates. This way it is possible to draw for
                                                    example the leaf of a tree that falls down rotating and shows
                                                    both faces. In cases that you don't want to draw back faces
                                                    deactivate this for faster render times.
\arg \b pMirrorX                                    Horizontal mirroring. (true / false) = (activated / deactivated).
\arg \b pMirrorY                                    Vertical mirroring. (true / false) = (activated / deactivated).
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

This functions sets the color and blending attributes of a graphical object. It should
be used before any calling to the Blit methods.

Important: It is not possible to use tinting and fading at the same time, the fading will overide the tinting.

Remember that you can use IND_Entity2d objects for applying color
transformations to the graphical objects without having to use this advanced method directly.
This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using a combination of these methods:
- IND_Entity2d::setTransparency()
- IND_Entity2d::setFade()
- IND_Entity2d::setTint()
- IND_Entity2d::setBlendSource()
- IND_Entity2d::setBlendDest()
*/
void DirectXRender::setRainbow2d(IND_Type pType,
                                 bool pCull,
                                 bool pMirrorX,
                                 bool pMirrorY,
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

	if (pCull)
		_info._device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else
		_info._device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Mirroring (180º rotations)
	//Only when rotated in one axis, x or y
	if ((pMirrorX && !pMirrorY)
		||
		(!pMirrorX && pMirrorY)) {
	
			// Back face culling
			if (pCull)
				_info._device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			else
				_info._device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
		}

	// ----- Blending -----

	switch (pType) {
	case IND_OPAQUE: {
		// Alphablending and alpha test = OFF
		_info._device->SetRenderState(D3DRS_ALPHABLENDENABLE,  FALSE);
		_info._device->SetRenderState(D3DRS_ALPHATESTENABLE,   FALSE);

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
			// Alpha color
			_info._device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(pR, pG, pB, pA));

			// Alpha-blending = ON
			_info._device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			// Set source alpha and destination alpha
			_info._device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
			_info._device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// Alpha
			_info._device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			_info._device->SetTextureStageState(0, D3DTSS_ALPHAARG1,  D3DTA_TFACTOR);
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

		if (pSo && pDs) {
			// Alpha-blending = ON
			_info._device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			_info._device->SetRenderState(D3DRS_SRCBLEND,  GetD3DBlendingType(pSo));
			_info._device->SetRenderState(D3DRS_DESTBLEND, GetD3DBlendingType(pDs));
		}

		break;
	}

	case IND_ALPHA: {
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

	default: {
	}
	}
}

#endif //INDIERENDER_DIRECTX
