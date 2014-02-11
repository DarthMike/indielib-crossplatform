//
//  RenderMacros.c
//  indielibios
//
//  Created by Miquel Angel Quinones Garcia on 15/12/13.
//  Copyright (c) 2013 indielib. All rights reserved.
//

#include "RenderMacros.h"
#include "Global.h"

#include <OpenGLES/ES2/gl.h>
#include <sstream>

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