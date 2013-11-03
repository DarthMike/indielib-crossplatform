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

#ifndef __IND_SHADERS_H_
#define __IND_SHADERS_H_

/// Standard uniforms used throughout many programs
extern const char* IND_Uniform_MVMatrix;
extern const char* IND_Uniform_PMatrix;
extern const char* IND_Uniform_Color;

/// Standard vertex attribute names used in many programs
extern const char* IND_VertexAttribute_Position;

/// Sources for shaders, embedded in a string in code for faster loading
extern const char* IND_VertexShader_UniformColor;
extern const char* IND_FragmentShader_UniformColor;

#endif
