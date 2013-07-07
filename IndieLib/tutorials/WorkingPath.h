/*****************************************************************************************
 * Desc: Utility methods for working paths setup
 *****************************************************************************************/

#ifndef _WorkingPath_h
#define _WorkingPath_h


class WorkingPathSetup {
public:
    
    static const char* resourcesDirectory();
    
    static bool setWorkingPath(const char* absPath);
    
private:
    static bool readExeDirectory(char* exePath);
};

#endif
