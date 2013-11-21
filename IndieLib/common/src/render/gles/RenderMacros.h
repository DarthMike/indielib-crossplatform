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

#import <string>
#import <sstream>

#ifdef DEBUG
using namespace std;

string checkGLError(const char* srcFile, int srcLine) {
    GLenum err (glGetError());
    stringstream errors;
    
    while(err!=GL_NO_ERROR) {
        string error;
        switch(err) {
            case GL_INVALID_OPERATION:      error="GL_INVALID_OPERATION";      break;
            case GL_INVALID_ENUM:           error="GL_INVALID_ENUM";           break;
            case GL_INVALID_VALUE:          error="GL_INVALID_VALUE";          break;
            case GL_OUT_OF_MEMORY:          error="GL_OUT_OF_MEMORY";          break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error="GL_INVALID_FRAMEBUFFER_OPERATION";  break;
        }
        
        errors << error.c_str() <<" AT - "<<srcFile<<":"<<srcLine<<endl;
        err=glGetError();
    }
    
    return errors.str();
}

void logDebugGLError(string error) {
    if (error.length()) {
        g_debug->header(error, DebugApi::LogHeaderError);
    }
}

#define CHECKGLERRORS() logDebugGLError(checkGLError(__FILE__,__LINE__));

#else
#define CHECKGLERRORS()
#endif

#endif //__INDIELIBIOS_RENDERMACROS_H__
