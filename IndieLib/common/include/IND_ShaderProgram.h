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


#ifndef __IND_SHADERPROGRAM_H_
#define __IND_SHADERPROGRAM_H_

#include "IND_Object.h"
#include "Defines.h"
#include "IND_GLShaderUniform.h"

#include <string>
#include <map>

using namespace std;
struct IND_ShaderProgramImpl;

class LIB_EXP IND_ShaderProgram : public IND_Object{
public:
    static IND_ShaderProgram* newShaderProgram() {
        return new IND_ShaderProgram();
    }
    
    virtual void destroy() {
        delete this;
    }
    
    bool compile(const char* vertexShader, const char* fragmentShader);
    bool link();
    void use();
    
    IND_GLSLShaderUniform* getUniform(const char* uniformName);
    
    void setSingleUniformValue(const void* value, const char* uniformName);
    int getPositionForVertexAttribute(const char* vertextAttribureName);
    
    string errorLog();

    typedef map<string,IND_GLSLShaderUniform*> UniformsMap;
    
private:
    IND_ShaderProgram() : _impl(NULL) { init(); }
    virtual ~IND_ShaderProgram() { end(); }
    
    void init();
    void end();
    
    void readProgramUniforms();

    UniformsMap _uniformsMap;
    IND_ShaderProgramImpl* _impl;
};

#endif //__IND_SHADER_H_
