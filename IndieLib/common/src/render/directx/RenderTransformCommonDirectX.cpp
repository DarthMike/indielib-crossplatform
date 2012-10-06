/*****************************************************************************************
 * File: RenderTransformCommonDirectX.cpp
 * Desc: Transformations applied before blitting objects usind D3D
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

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

void   DirectXRender::clearViewPort(BYTE pR,
                                    BYTE pG,
                                    BYTE pB) {
	_info._device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(pR, pG, pB), 1.0f, 0);
}


void DirectXRender::lookAt(float pEyeX, float pEyeY, float pEyeZ,
                           float pLookAtX, float pLookAtY, float pLookAtZ,
                           float pUpX, float pUpY, float pUpZ) {
	// ----- View matrix -----

	D3DXMATRIX mMatView;
	D3DXVECTOR3 mEyePt(pEyeX, pEyeY, pEyeZ);
	D3DXVECTOR3 mLookatPt(pLookAtX, pLookAtY, pLookAtZ);
	D3DXVECTOR3 mUpVec(pUpX, pUpY, pUpZ);
	D3DXMatrixLookAtLH(&mMatView, &mEyePt, &mLookatPt, &mUpVec);

	_info._device->SetTransform(D3DTS_VIEW, &mMatView);
}

void DirectXRender::perspectiveFov(float pFov, float pAspect, float pNearClippingPlane, float pFarClippingPlane) {
	// ----- Projection matrix -----

	D3DXMATRIX mMatProj;

	D3DXMatrixPerspectiveFovLH(&mMatProj,               // output
	                           pAspect,                 // Fov vertical
	                           pAspect,                 // Viewport aspect ratio
	                           pNearClippingPlane,      // Near clipping plane z
	                           pFarClippingPlane);      // Far clipping  plane z


	_info._device->SetTransform(D3DTS_PROJECTION, &mMatProj);
}

void DirectXRender::perspectiveOrtho(float pWidth, float pHeight, float pNearClippingPlane, float pFarClippingPlane) {
	D3DXMATRIX mMatProj;
	D3DXMatrixOrthoLH(&mMatProj, pWidth, pHeight, pNearClippingPlane, pFarClippingPlane);
	_info._device->SetTransform(D3DTS_PROJECTION, &mMatProj);
}


// --------------------------------------------------------------------------------
//							         Private methods
// --------------------------------------------------------------------------------

/*
==================
IND_BlendingType to D3DBLEND conversion
==================
*/
D3DBLEND DirectXRender::GetD3DBlendingType(IND_BlendingType pBlendingType) {
	switch (pBlendingType) {
	case IND_ZERO:
		return D3DBLEND_ZERO;
	case IND_ONE:
		return D3DBLEND_ONE;
	case IND_SRCCOLOR:
		return D3DBLEND_SRCCOLOR;
	case IND_INVSRCCOLOR:
		return D3DBLEND_INVSRCCOLOR;
	case IND_SRCALPHA:
		return D3DBLEND_SRCALPHA;
	case IND_INVSRCALPHA:
		return D3DBLEND_INVSRCALPHA;
	case IND_DESTALPHA:
		return D3DBLEND_DESTALPHA;
	case IND_INVDESTALPHA:
		return D3DBLEND_INVDESTALPHA;
	case IND_DESTCOLOR:
		return D3DBLEND_DESTCOLOR;
	case IND_INVDESTCOLOR:
		return D3DBLEND_INVDESTCOLOR;
	case IND_SRCALPHASAT:
		return D3DBLEND_SRCALPHASAT;
	case IND_BOTHSRCALPHA:
		return D3DBLEND_BOTHSRCALPHA;
	case IND_BOTHINVSRCALPHA:
		return D3DBLEND_BOTHINVSRCALPHA;
	case IND_BLENDFACTOR:
		return D3DBLEND_BLENDFACTOR;
	case IND_INVBLENDFACTOR:
		return D3DBLEND_INVBLENDFACTOR;

	default:
		return D3DBLEND_SRCALPHA;
	}
}


/*
==================
IND_BlendingType to D3DBLEND conversion
==================
*/
D3DTEXTUREFILTERTYPE DirectXRender::GetD3DFilter(IND_Filter pFilter) {
	switch (pFilter) {
	case IND_FILTER_NONE:
		return D3DTEXF_NONE;
	case IND_FILTER_POINT:
		return D3DTEXF_POINT;
	case IND_FILTER_LINEAR:
		return D3DTEXF_LINEAR;

	default:
		return D3DTEXF_LINEAR;
	}
}


#endif //INDIERENDER_DIRECTX
