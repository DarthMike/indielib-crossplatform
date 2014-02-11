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
extern const char* IND_Uniform_RGBAColor;
extern const char* IND_Uniform_SpriteTexture;

/// Standard vertex attribute names used in many programs
extern const char* IND_VertexAttribute_Position;
extern const char* IND_VertexAttribute_RGBAColor;
extern const char* IND_VertexAttribute_TexCoord;

/// Sources for vertex shaders
extern const char* IND_VertexShader_UniformRGBAColor;
extern const char* IND_FragmentShader_Color;
extern const char* IND_VertexShader_PerVertexRGBAColor;
extern const char* IND_VertexShader_Simple2DTexture;

/// Sources for fragment shaders
extern const char* IND_FragmentShader_Simple2DTexture_RGBA;
extern const char* IND_FragmentShader_Simple2DTexture_BGRA;
extern const char* IND_FragmentShader_2DTexture_RGBATint;
extern const char* IND_FragmentShader_2DTexture_RGBAFade;

/// Default engine existing shader programs. Compiled and linked, added to internal manager.
extern const char* IND_Program_UniformRGBAColor;
extern const char* IND_Program_PerVertexRGBAColor;
extern const char* IND_Program_Simple2DTexture;
extern const char* IND_Program_2DTexture_RGBATint;
extern const char* IND_Program_2DTexture_RGBAFade;

#endif
