/*****************************************************************************************
 * Desc: Utility methods for working paths setup
 *****************************************************************************************/

#ifndef _WorkingPath_h
#define _WorkingPath_h

    
class WorkingPathSetup {
	public:
    
    
    static bool setWorkingPathFromExe(const char* relpath);
    
    private:
    static bool readExeDirectory();
};

#endif
