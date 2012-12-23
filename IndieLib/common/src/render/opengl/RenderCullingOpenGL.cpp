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
==================
*/
void OpenGLRender::reCalculateFrustrumPlanes() {
	//TODO
	//D3DXMATRIX mMatView, mMatProj;
	//_info._device->GetTransform(D3DTS_VIEW, &mMatView);
	//_info._device->GetTransform(D3DTS_PROJECTION, &mMatProj);

	//// Get combined matrix
	//D3DXMATRIXA16 matComb;
	//D3DXMatrixMultiply(&matComb, &mMatView, &mMatProj);

	//// Left clipping plane
	//mFrustrumPlanes.planes[0]._normal.x     = -(matComb._14 + matComb._11);
	//mFrustrumPlanes.planes[0]._normal.y     = -(matComb._24 + matComb._21);
	//mFrustrumPlanes.planes[0]._normal.z     = -(matComb._34 + matComb._31);
	//mFrustrumPlanes.planes[0]._distance     = -(matComb._44 + matComb._41);

	//// Right clipping plane
	//mFrustrumPlanes.planes[1]._normal.x     = -(matComb._14 - matComb._11);
	//mFrustrumPlanes.planes[1]._normal.y     = -(matComb._24 - matComb._21);
	//mFrustrumPlanes.planes[1]._normal.z     = -(matComb._34 - matComb._31);
	//mFrustrumPlanes.planes[1]._distance     = -(matComb._44 - matComb._41);

	//// Top clipping plane
	//mFrustrumPlanes.planes[2]._normal.x     = -(matComb._14 - matComb._12);
	//mFrustrumPlanes.planes[2]._normal.y     = -(matComb._24 - matComb._22);
	//mFrustrumPlanes.planes[2]._normal.z     = -(matComb._34 - matComb._32);
	//mFrustrumPlanes.planes[2]._distance     = -(matComb._44 - matComb._42);

	//// Bottom clipping plane
	//mFrustrumPlanes.planes[3]._normal.x     = -(matComb._14 + matComb._12);
	//mFrustrumPlanes.planes[3]._normal.y     = -(matComb._24 + matComb._22);
	//mFrustrumPlanes.planes[3]._normal.z     = -(matComb._34 + matComb._32);
	//mFrustrumPlanes.planes[3]._distance     = -(matComb._44 + matComb._42);

	//// Near clipping plane
	//mFrustrumPlanes.planes[4]._normal.x     = -(matComb._14 + matComb._13);
	//mFrustrumPlanes.planes[4]._normal.y     = -(matComb._24 + matComb._23);
	//mFrustrumPlanes.planes[4]._normal.z     = -(matComb._34 + matComb._33);
	//mFrustrumPlanes.planes[4]._distance     = -(matComb._44 + matComb._43);

	//// Far clipping plane
	//mFrustrumPlanes.planes[5]._normal.x     = -(matComb._14 - matComb._13);
	//mFrustrumPlanes.planes[5]._normal.y     = -(matComb._24 - matComb._23);
	//mFrustrumPlanes.planes[5]._normal.z     = -(matComb._34 - matComb._33);
	//mFrustrumPlanes.planes[5]._distance     = -(matComb._44 - matComb._43);

	//for (int i = 0; i < 6; i++)
	//mFrustrumPlanes [i].Normalise();
}

/** @endcond */
#endif //INDIERENDER_OPENGL
