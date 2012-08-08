/*****************************************************************************************
 * File: RenderObject3dOpenGL.cpp
 * Desc: Blitting of 3d objects using OpenGL
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
#include "IND_SurfaceManager.h"
#include "OpenGLRender.h"


// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

/*!
\defgroup Graphical_3d_Objects Bliting 3d Animated 3d Models and setting the transformations directly
\ingroup Advances
*/
/*@{*/

/*!
\b Parameters:

\arg \b p3dMesh                     Pointer to a ::IND_3dMesh object

\b Operation:

This function blits directly to the screen a ::IND_3dMesh object.

In order to change the transformations
and color attributes of the 3d mesh you have to use the OpenGLRender::setTransform3d() and OpenGLRender::SetRainbow()
methods before calling to this function. Remember that you can use IND_Entity3d object for drawing 3d meshes to
the screen without having to use this advanced methods directly. This method is only useful for advanced users
with really concrete purposes.

Using this method is equivalent to using:
- IND_Entity3d::set3dMesh()
*/
void OpenGLRender::blit3dMesh(IND_3dMesh *) {
//TODO
}


/*!
\b Parameters:

\arg \b p3dMesh                     Pointer to a ::IND_3dMesh object
\arg \b pIndex                      Index of the animation

\b Operation:

This function sets the animation sequence that will be rendered. When you use this method, the transition between the
current animation and the following will not be abrubt. IndieLib will fade out the effect of the first animation
and fade in the second. Use IND_3dMesh::setTransitionSpeed() in order set the speed of that transition.
*/
void OpenGLRender::set3dMeshSequence(IND_3dMesh *, unsigned int ) {
//TODO
}
/*@}*/

// --------------------------------------------------------------------------------
//							         Private methods
// --------------------------------------------------------------------------------



#endif //INDIERENDER_OPENGL
