/*****************************************************************************************
 * File: RenderTransform3dOpenGL.cpp
 * Desc: Transformations applied before blitting a 3d object usind OpenGL
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
#include "Defines.h"
#include "OpenGLRender.h"
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
bool OpenGLRender::setViewPort3d(int pX,
                                 int pY,
                                 int pWidth,
                                 int pHeight) {
	//TODO

	return 1;
}


/*!
\b Parameters:

\arg \b pCamera3d               ::IND_Camera3d object that defines a camera.

\b Operation:

This function sets a 3d camera. See the methods of ::IND_Camera3d for information on how you can manipulate the camera.
*/
void OpenGLRender::setCamera3d(IND_Camera3d *pCamera3d) {
	//TODO
}

/**
@brief Pushes camera transformation to recover it later on

Will capture the current matrix transform for the camera set by IND_Render::setCamera3d.

Call this if you manually will call later on IND_Render::setTransform3d and you want to 
preserve the camera transform all along.

If you call this sequentially without ending the transform, the transforms are overwritten, so 
you could consider that the 'stack' has only one position. No overflow occurs.

@see IND_Render::endTransform3d()

@discussion As in OpenGL, it works similar as glPushMatrix(..), but only with the 'camera' transformation
set by IND_Render::setCamera3d
*/
void OpenGLRender::beginTransform3d() {
	//TODO: (should be the same as beginTransform2d() ?)
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
void OpenGLRender::setTransform3d(float pX,
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
	//TODO
}

/**
@brief Pops camera transformation previously pushed

Will restore the matrix transform for the camera set by IND_Render::setCamera3d to the value
previously set. If no value was set before, this has no effect.

Call this if you manually called  IND_Render::setTransform3d and you want to 
restored the camera transform all along.

If you call this sequentially without ending the transform, the transforms are overwritten, so 
you could consider that the 'stack' has only one position. No overflow occurs.

@see IND_Render::beginTransform3d()

@discussion As in OpenGL, it works similar as glPopMatrix(..), but only with the 'camera' transformation
set by IND_Render::setCamera3d
*/
void OpenGLRender::endTransform3d() {
	//TODO: Should it be the same as endTransform2d() ?
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
void OpenGLRender::setRainbow3d(bool pCull,
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
	//TODO
}

#endif //INDIERENDER_OPENGL
