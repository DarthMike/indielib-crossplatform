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


#ifndef __IND_GLSHADERUNIFORM_H_
#define __IND_GLSHADERUNIFORM_H_

#include "IND_Object.h"
#include "Defines.h"

#include <string>
#include <OpenGLES/ES2/gl.h>

using namespace std;

class LIB_EXP IND_GLSLShaderUniform : public IND_Object{
public:
    static IND_GLSLShaderUniform* newUniform() {
        return new IND_GLSLShaderUniform();
    }
    
    virtual void destroy() {
        delete this;
    }
    
    typedef enum UniformType {
        UniformTypeUnknown,
        UniformTypeInteger,
        UniformTypeFloat,
        UniformTypeIntVector,
        UniformTypeFloatVector,
        UniformTypeFloatMatrix,
        UniformTypeSampler2D
    } UniformType;
    
    string _nameInShader;
    GLint _glLocation;
    GLenum _glType;
    
    UniformType getType();
    int arrayLength();
    int matrixSize();
};

#endif //__IND_GLSHADERUNIFORM_H_
