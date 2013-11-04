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

#ifndef __INDIELIBIOS_RENDERMACROS_H__
#define __INDIELIBIOS_RENDERMACROS_H__

#ifdef DEBUG
//#define CHECK_GL_ERROR_DEBUG() ({ GLenum __error = glGetError(); if(__error) printf("OpenGL error 0x%04X in %s %d\n", __error, __FUNCTION__, __LINE__); })
//#else
//#define CHECK_GL_ERROR_DEBUG()
//#endif

#define CHECKGLERRORS() GLenum glerror = glGetError(); \
if (glerror) { \
g_debug->header("OpenGL error ", DebugApi::LogHeaderError);\
}
#else
#define CHECKGLERRORS()
#endif

#endif //__INDIELIBIOS_RENDERMACROS_H__
