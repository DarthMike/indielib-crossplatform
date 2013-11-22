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


/** @cond DOCUMENT_PRIVATEAPI */

#include "IND_ShaderManager.h"
#include "IND_ShaderProgram.h"
#include "Global.h"

bool IND_ShaderManager::init() {
    initVars();
    
    _ok = true;
    
    return _ok;
}

void IND_ShaderManager::end() {
    freeVars();
    _ok = false;
}

void IND_ShaderManager::initVars() {
}

void IND_ShaderManager::freeVars() {
    ProgramsMapIterator it;
    for (it = programs.begin(); it != programs.end(); ++it) {
        DISPOSEMANAGED(it->second);
    }
    programs.clear();
}

bool IND_ShaderManager::add(IND_ShaderProgram* program, const char* programName) {
    std::string name (programName);
    
    if (programs.find(name) == programs.end()) {
        programs[name] = program;
        return true;
    }
    return false;
}

bool IND_ShaderManager::remove(const char* programName) {
    std::string name (programName);
    ProgramsMapIterator it = programs.find(name);
    if (it != programs.end()) {
        DISPOSEMANAGED(it->second);
        programs.erase(it);
    }
    
    return true;
}

IND_ShaderProgram* IND_ShaderManager::getProgram (const char* programName) {
    std::string name (programName);
    ProgramsMapIterator it = programs.find(name);
    if (it != programs.end()) {
        return it->second;
    }
    
    return  NULL;
}

/** @endcond */
