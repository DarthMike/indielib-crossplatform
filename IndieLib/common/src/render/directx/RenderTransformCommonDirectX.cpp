/*****************************************************************************************
 * File: RenderTransformCommonDirectX.cpp
 * Desc: Transformations applied before blitting objects usind D3D
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

#ifdef INDIERENDER_DIRECTX

/** @cond DOCUMENT_PRIVATEAPI */

// ----- Includes -----

#include "Global.h"
#include "Defines.h"
#include "DirectXRender.h"

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

void   DirectXRender::clearViewPort(unsigned char pR,
                                    unsigned char pG,
                                    unsigned char pB) {
	_info._device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(pR, pG, pB), 1.0f, 0);
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

/** @endcond */

#endif //INDIERENDER_DIRECTX
