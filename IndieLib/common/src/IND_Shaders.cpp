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

#include "IND_Shaders.h"

const char* IND_Program_UniformRGBAColor = "IND_Program_UniformRGBAColor";
const char* IND_Program_PerVertexRGBAColor = "IND_Program_PerVertexRGBAColor";
const char* IND_Program_Simple2DTexture = "IND_Program_Simple2DTexture";

const char* IND_Uniform_MVMatrix = "uMVmatrix";
const char* IND_Uniform_PMatrix = "uPMatrix";
const char* IND_Uniform_RGBAColor = "uColor";
const char* IND_Uniform_SpriteTexture = "uTexture";
const char* IND_VertexAttribute_Position = "aPosition";
const char* IND_VertexAttribute_RGBAColor = "aRGBAColor";
const char* IND_VertexAttribute_TexCoord = "aTexCoord";

const char* IND_VertexShader_UniformRGBAColor =
"                                                   \n\
#version 100                                        \n\
attribute vec3 aPosition;							\n\
#ifdef GL_ES										\n\
varying lowp vec4 varFragmentColor;					\n\
#else												\n\
varying vec4 varFragmentColor;						\n\
#endif												\n\
uniform	vec4 uColor;								\n\
uniform mat4 uMVmatrix;                             \n\
uniform mat4 uPMatrix;                              \n\
                                                    \n\
void main()											\n\
{                                                   \n\
    vec4 pos4 = vec4(aPosition, 1.0);               \n\
    gl_Position = uPMatrix * uMVmatrix  * pos4;     \n\
    varFragmentColor = uColor;                      \n\
}													\n\
";


const char* IND_FragmentShader_Color =
"                                       \n\
#version 100 							\n\
#ifdef GL_ES							\n\
precision lowp float;					\n\
#endif									\n\
                                        \n\
varying vec4 varFragmentColor;			\n\
                                        \n\
void main()								\n\
{										\n\
    gl_FragColor = varFragmentColor;		\n\
}										\n\
";

const char* IND_VertexShader_PerVertexRGBAColor =
"                                                   \n\
#version 100                                        \n\
attribute vec3 aPosition;                           \n\
attribute vec4 aRGBAColor;                          \n\
#ifdef GL_ES										\n\
varying lowp vec4 varFragmentColor;					\n\
#else												\n\
varying vec4 varFragmentColor;						\n\
#endif												\n\
uniform mat4 uMVmatrix;                             \n\
uniform mat4 uPMatrix;                              \n\
\n\
void main()											\n\
{                                                   \n\
vec4 pos4 = vec4(aPosition, 1.0);                   \n\
gl_Position = uPMatrix * uMVmatrix  * pos4;         \n\
varFragmentColor = aRGBAColor;                      \n\
}													\n\
";

const char* IND_VertexShader_Simple2DTexture =
"                                                   \n\
#version 100                                        \n\
attribute vec3 aPosition;							\n\
attribute vec2 aTexCoord;							\n\
#ifdef GL_ES                                        \n\
varying lowp vec2 varTexCoord;                      \n\
#else                                               \n\
varying vec2 varTexCoord;                           \n\
#endif                                              \n\
uniform mat4 uMVmatrix;                             \n\
uniform mat4 uPMatrix;                              \n\
\n\
void main()											\n\
{                                                   \n\
vec4 pos4 = vec4(aPosition, 1.0);                   \n\
gl_Position = uPMatrix * uMVmatrix  * pos4;         \n\
varTexCoord = aTexCoord;                            \n\
}													\n\
";

const char* IND_FragmentShader_Simple2DTexture_BGRA =
"                                                   \n\
#version 100                                        \n\
#ifdef GL_ES                                        \n\
precision lowp float;                              \n\
#endif                                              \n\
varying vec2 varTexCoord;                           \n\
uniform sampler2D uTexture;                         \n\
\n\
void main()											\n\
{                                                   \n\
    vec4 rgbColor = texture2D(uTexture,varTexCoord);\n\
    gl_FragColor = rgbColor.bgra;                   \n\
}													\n\
";

const char* IND_FragmentShader_Simple2DTexture_RGBA =
"                                                   \n\
#version 100                                        \n\
#ifdef GL_ES                                        \n\
precision lowp float;                               \n\
#endif                                              \n\
varying vec2 varTexCoord;                           \n\
uniform sampler2D uTexture;                         \n\
\n\
void main()											\n\
{                                                   \n\
gl_FragColor = rtexture2D(uTexture,varTexCoord);    \n\
}													\n\
";




