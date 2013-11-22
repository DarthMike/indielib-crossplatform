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

#include "IND_GLShaderUniform.h"

IND_GLSLShaderUniform::UniformType IND_GLSLShaderUniform::getType() {
    
    UniformType type;
    switch (_glType) {
        case GL_INT:
            type = UniformTypeInteger;
            break;
        case GL_INT_VEC2:
        case GL_INT_VEC3:
        case GL_INT_VEC4:
            type = UniformTypeIntVector;
            break;
        case GL_FLOAT:
            type = UniformTypeFloat;
            break;
        case GL_FLOAT_MAT2:
        case GL_FLOAT_MAT3:
        case GL_FLOAT_MAT4:
            type = UniformTypeFloatMatrix;
            break;
        case GL_FLOAT_VEC2:
        case GL_FLOAT_VEC3:
        case GL_FLOAT_VEC4:
            type = UniformTypeFloatVector;
            break;
        case GL_SAMPLER_2D:
            type = UniformTypeSampler2D;
            break;
        //TODO :GL_SAMPLER_CUBE, GL_BOOL types
        default:
            type = UniformTypeUnknown;
            break;
    }
    
    return type;
}

int IND_GLSLShaderUniform::arrayLength() {
    int length;
    switch (_glType) {
        case GL_INT_VEC2:
        case GL_FLOAT_VEC2:
            length = 2;
            break;
        case GL_INT_VEC3:
        case GL_FLOAT_VEC3:
            length = 3;
            break;
        case GL_INT_VEC4:
        case GL_FLOAT_VEC4:
            length = 4;
            break;
        default:
            length = 0;
            break;
    }
    
    return length;
}


int IND_GLSLShaderUniform::matrixSize() {
    int length;
    switch (_glType) {
        case GL_FLOAT_MAT2:
            length = 2;
            break;
        case GL_FLOAT_MAT3:
            length = 3;
            break;
        case GL_FLOAT_MAT4:
            length = 4;
            break;
        default:
            length = 0;
            break;
    }
    
    return length;
}

#endif //INDIERENDER_GLES_IOS
