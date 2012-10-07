/*****************************************************************************************
 * Desc: Utility methods for working paths setup
 *****************************************************************************************/

#include "Defines.h"
#include "WorkingPath.h"
#include <string.h>

#if defined (PLATFORM_OSX) || defined (PLATFORM_IOS)
#ifndef __OBJC__
#error No way to compile this OSX code without ObjC compiler
#endif// __OBJC__
#import <Foundation/Foundation.h>

#endif // (PLATFORM_OSX) || defined (PLATFORM_IOS)


#ifdef PLATFORM_WIN32
#include "TCHAR.h"
#endif //PLATFORM_WIN32

#ifdef PLATFORM_LINUX
#include <stdio.h>
#endif //PLATFORM_LINUX

static const int appDirChars = 8042;
static char g_appDir[appDirChars];

bool WorkingPathSetup::setWorkingPathFromExe(const char* relpathFromExe) {
    
    //Fetch in global variable the exe directory
    if (!readExeDirectory()) return false;
    
    bool success = false;
    
    //TODO: Check correctness in path formatting for 'relpath'
    //1. Should start with // or .
    //2. Should concatenate correctly with g_appDir 
    char totalPath [8042];
    strcpy(totalPath, g_appDir);
    if (relpathFromExe) {
        strcat(totalPath, relpathFromExe);
    }
#ifdef PLATFORM_WIN32
	if (SetCurrentDirectory(g_appDir)) {
		success = true;
	} else {
		success = false;
	}
#endif
    
    
#ifdef PLATFORM_LINUX
    if ( 0 == chdir(totalPath)) {
		success = true;
	} else {
		success = false;
	}
#endif
    
#ifdef PLATFORM_OSX
    
    NSString* workingpath = [[NSString alloc] initWithUTF8String:totalPath];
    if ([[NSFileManager defaultManager] changeCurrentDirectoryPath:workingpath]) {
        success = true;
    } else {
        success = false;
    }
    
    [workingpath release];
#endif
    
    if (success) {
        printf("Working path changed to: %s\n",totalPath);
    }
    
    return success;
}


bool WorkingPathSetup::readExeDirectory() {
    bool success = false;
	

#ifdef PLATFORM_WIN32
	TCHAR app_path[MAX_PATH] = _T("");
	DWORD size_in_tchars = sizeof(app_path)/sizeof(TCHAR);
	// get full app path with the exe filename
	GetModuleFileName( 0, app_path, size_in_tchars-1 );
	// making app_path to end on dir char ('\\')
	// _tcsrchr - search for char from the string's end
	TCHAR* app_dir = _tcsrchr(app_path, _T('\\'));
	if (app_dir) {
		app_dir += 1;
		if (app_dir) { 
			*app_dir = '\0'; 
			strcpy(g_appDir,app_path);
			success = true;
		}
	}

	if (!app_dir) {
		strcpy(g_appDir,"");
	}


#endif
    
#ifdef PLATFORM_LINUX
    if (getcwd(g_appDir,sizeof(g_appDir))) {
        success = true;
    } else {
        strcpy(g_appDir,"");
        success = false;
    }
    
#endif
    
#ifdef PLATFORM_OSX
    @autoreleasepool {
        //Note that in OSX, bundle is in the place of 'exe' for other platforms
        //Exe file will be inside the bundle.
    	NSString *bundlePath = [[NSBundle mainBundle] bundlePath];
        NSString *exeDir = bundlePath;
        
        //Handle case when app is not bundled. In this case bundle path is the same as executable directory
        if (NSNotFound != [bundlePath rangeOfString:@".app"].location) {
            exeDir = [bundlePath stringByDeletingLastPathComponent];
        }
        
    	if (exeDir && [exeDir length]) {
        	strcpy(g_appDir, [exeDir cStringUsingEncoding:NSUTF8StringEncoding]);
        	success = true;
    	}
    }
#endif
    return success;
}

