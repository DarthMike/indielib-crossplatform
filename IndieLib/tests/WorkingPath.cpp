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
#include <unistd.h>
#include <stdio.h>
#endif //PLATFORM_LINUX

static const int appDirChars = 16084;
static char g_appDir[appDirChars];
static char g_resourcesDir[appDirChars];


const char* WorkingPathSetup::unittestsResourcesDirectory() {
    WorkingPathSetup::initializeResourcesDirectory();
    
    strcat(g_resourcesDir, "/../../assets");
    return g_resourcesDir;
}

const char* WorkingPathSetup::resourcesDirectory() {
    
    WorkingPathSetup::initializeResourcesDirectory();
    
#ifdef PLATFORM_IOS
    // Bundle in iOS apps is flattened, meaning exe is in root, and added dirs start from there
    strcat(g_resourcesDir, "/assets");
#endif
    
#ifdef PLATFORM_OSX
    // Bundle in OSX apps is not flattened, because executable directory can contain plugins
    // There is a Resources directory, where our files will be copied
    strcat(g_resourcesDir, "/../Resources/assets");
#endif
    
#if defined (PLATFORM_WIN32)
    strcat(g_resourcesDir, "/../../assets");
#endif

#if defined (PLATFORM_LINUX)
    strcat(g_resourcesDir, "../../assets");
#endif
    
    return g_resourcesDir;
}

void WorkingPathSetup::initializeResourcesDirectory() {
    if (!WorkingPathSetup::readExeDirectory(g_appDir)) {
        strcpy(g_resourcesDir, "");
    }
    
    strcpy(g_resourcesDir, g_appDir);
}

bool WorkingPathSetup::setWorkingPath(const char* absPath) {
    bool success = false;
    
#ifdef PLATFORM_WIN32
	if (SetCurrentDirectory(absPath)) {
		success = true;
	} else {
		success = false;
	}
#endif
    
    
#ifdef PLATFORM_LINUX
    if ( 0 == chdir(absPath)) {
		success = true;
	} else {
		success = false;
	}
#endif
    
#ifdef __APPLE__
    
    NSString* workingpath = [[NSString alloc] initWithUTF8String:absPath];
    if ([[NSFileManager defaultManager] changeCurrentDirectoryPath:workingpath]) {
        success = true;
    } else {
        success = false;
    }
    
#endif
    
    if (success) {
        printf("Working path changed to: %s\n",absPath);
    }
    
    return success;
}

bool WorkingPathSetup::readExeDirectory(char* exePath) {
	
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
			strcpy(exePath,app_path);
			success = true;
		}
	}
    
	if (!app_dir) {
		strcpy(exePath,"");
	}
    
    
#endif
    
#ifdef PLATFORM_LINUX
    if (getcwd(exePath,sizeof(exePath))) {
        success = true;
    } else {
        strcpy(exePath,"");
        success = false;
    }
    
#endif
    
#ifdef __APPLE__
    @autoreleasepool {
        //Note that in OSX, bundle is in the place of 'exe' for other platforms
        //Exe file will be inside the bundle.
    	NSString *exePath = [[NSBundle mainBundle] executablePath];
        NSString *exeDir = [exePath stringByDeletingLastPathComponent];
        
    	if ([exeDir length]) {
        	strcpy(g_appDir, [exeDir cStringUsingEncoding:NSUTF8StringEncoding]);
        	success = true;
    	}
    }
#endif
    
    size_t length = strlen(g_appDir);
    
    // Replace windows style path characters with unix ones. Windows supports both.
    for (size_t i = 0; i < length; ++i) {
        if ('\\' == g_appDir[i]) {
            g_appDir[i] = '/';
        }
        
        // Remove last path separator in case any API returned a path ending in separator
        if (i == (length - 1)) {
            if ('/' == g_appDir[i]) {
                g_appDir[i] = '\0';
            }
        }
    }
    
    
    return success;
}

