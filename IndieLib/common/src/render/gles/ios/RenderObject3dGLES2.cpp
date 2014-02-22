/*****************************************************************************************
 * File: RenderObject3dOpenGL.cpp
 * Desc: Blitting of 3d objects using OpenGL
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

#ifdef INDIERENDER_GLES_IOS

// ----- Includes -----

#include "Global.h"
#include "IND_SurfaceManager.h"
#include "OpenGLES2Render.h"

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------


void OpenGLES2Render::blit3dMesh(IND_3dMesh *) {
//TODO
}

void OpenGLES2Render::set3dMeshSequence(IND_3dMesh *, unsigned int ) {
//TODO
}


// --------------------------------------------------------------------------------
//							         Private methods
// --------------------------------------------------------------------------------

/** @endcond */

#endif //INDIERENDER_GLES_IOS
