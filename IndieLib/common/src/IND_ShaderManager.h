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


#ifndef _IND_SHADERMANAGER_
#define _IND_SHADERMANAGER_

#include <string>
#include <map>

class IND_ShaderProgram;

class  IND_ShaderManager {
public:
    
	// ----- Init/End -----
    
	IND_ShaderManager(): _ok(false)  { }
	~IND_ShaderManager() {
		end();
	}
    
	bool    init();
	void    end();
	bool    isOK() const {
        return _ok;
    }
    
	// ----- Public methods -----
    
	bool add(IND_ShaderProgram* program, const char* programName);
    bool add(IND_ShaderProgram* program, const char* vertexSourcePath, const char* fragmentSourcePath, const char* programName);
	bool remove(const char* programName);
    
private:
	// ----- Private -----
    
	bool _ok;
    
	// ----- Containers -----
    typedef std::map<std::string, IND_ShaderProgram*> shadersMap;
    
	// ----- Private Methods -----
    bool createDefaultShaders();
	void initVars();
	void freeVars();
};

#endif
