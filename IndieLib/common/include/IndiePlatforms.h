/*****************************************************************************************
 * File: IndiePlatforms.h
 * Desc: Platform Definitions
 *****************************************************************************************/

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


#ifndef _INDIEPLATFORMS_
#define _INDIEPLATFORMS_


//Platform definition checkings

//Win32
#if defined (WIN32) && defined (TARGET_IPHONE_SIMULATOR) && defined (TARGET_OS_IPHONE)
#error Defined WIN32 and TARGET_IPHONE....
#endif

#if defined (WIN32) || defined (_WIN32) || defined (_WIN32_)
#include <windows.h>
#define PLATFORM_WIN32 1
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX //Min max definitions conflicting with std::min and std::max
#endif

// OSX vs. iOS:
// http://sealiesoftware.com/blog/archive/2010/8/16/TargetConditionalsh.html
#ifdef __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#define PLATFORM_IOS 1
#else
#define PLATFORM_OSX 1
#endif
#endif

// Linux
#if defined(linux) || defined(__linux)
#define PLATFORM_LINUX 1
#endif 

// ----- Platform definition checks -----
// A platform must be defined
#if !defined (PLATFORM_WIN32) && !defined (PLATFORM_OSX) && !defined(PLATFORM_LINUX) && !defined (PLATFORM_IOS)
#error No Platform defined
#endif
// Win32 vs. any
#if defined (PLATFORM_WIN32) && (defined (PLATFORM_OSX) || defined(PLATFORM_LINUX) || defined (PLATFORM_IOS))
#error Win32 defined when other platforms are defined
#endif
// OSX vs. any
#if defined (PLATFORM_OSX) && (defined (PLATFORM_WIN32) || defined(PLATFORM_LINUX) || defined (PLATFORM_IOS))
#error OSX defined when other platforms are defined
#endif
// Linux vs. any
#if defined (PLATFORM_LINUX) && (defined (PLATFORM_WIN32) || defined(PLATFORM_OSX) || defined (PLATFORM_IOS))
#error OSX defined when other platforms are defined
#endif
// iOS vs. any
#if defined (PLATFORM_IOS) && (defined (PLATFORM_WIN32) || defined(PLATFORM_OSX) || defined (PLATFORM_LINUX))
#error iOS defined when other platforms are defined
#endif

// ----- Renderer settings -----
//YOU SHOULD CHANGE HERE THE DEFINITION OF WHICH RENDERER TO COMPILE WITH, INCLUDING LIBRARY USER
// Change here to override preprocessor macro definition. Only affects windows, as all other
// platforms compile in one renderer only.
#if !defined (INDIERENDER_DIRECTX) && !defined (INDIERENDER_GLES_IOS) && !defined (INDIERENDER_OPENGL)
//#define INDIERENDER_DIRECTX 1
#endif
#if !defined (INDIERENDER_OPENGL) && !defined (INDIERENDER_GLES_IOS) && !defined (INDIERENDER_DIRECTX)
#define INDIERENDER_OPENGL 1
#endif
#if !defined (INDIERENDER_GLES_IOS) && !defined (INDIERENDER_OPENGL) && !defined (INDIERENDER_DIRECTX)
//#define INDIERENDER_GLES_IOS 1
#endif

// ----- Renderer set safety -----
// A renderer must be defined
#if !defined (INDIERENDER_DIRECTX) && !defined (INDIERENDER_GLES_IOS) && !defined (INDIERENDER_OPENGL)
#error A renderer must be defined. Check IndiePlatforms.h
#endif
//Only one render type per configuration!
//DirectX vs. OPENGLES
#if defined (INDIERENDER_DIRECTX) && defined (INDIERENDER_GLES_IOS)
#error Multiple renderers defined. Check IndiePlatforms.h
#endif
//DirectX vs. OPENGL
#if defined (INDIERENDER_DIRECTX) && defined (INDIERENDER_OPENGL)
#error Multiple renderers defined. Check IndiePlatforms.h
#endif
//OPENGL vs. OPENGLES
#if defined (INDIERENDER_GLES_IOS) && defined (INDIERENDER_OPENGL)
#error Multiple renderers defined. Check IndiePlatforms.h
#endif

//****************************************

// ---- Config checkings -----
//Win32 platform vs. render settings
#if defined (PLATFORM_WIN32) && !defined (INDIERENDER_OPENGL) && !defined (INDIERENDER_DIRECTX)
#error Render GLES_IOS defined for platform WIN32! Check IndiePlatforms.h
#endif
//iOS platform vs. render setting
#if defined (PLATFORM_IOS) && !defined (INDIERENDER_GLES_IOS)
#error Render GLES_IOS NOT defined for platform iOS! Check IndiePlatforms.h
#endif
//OSX platform vs. render setting
#if defined (PLATFORM_OSX) && !defined (INDIERENDER_OPENGL)
#error Render INDIERENDER_OPENGL NOT defined for platform OSX! Check IndiePlatforms.h
#endif
//Linux platform vs. render setting
#if defined (PLATFORM_LINUX) && !defined (INDIERENDER_OPENGL)
#error Render GL NOT defined for platform Linux! Check IndiePlatforms.h
#endif

// ----- Lib export -----

#if PLATFORM_WIN32
#ifdef INDIELIB_DLLBUILD
#define LIB_EXP __declspec(dllexport)
#else
#define LIB_EXP __declspec(dllimport)
#endif //INDIELIB_DLLBUILD
#endif //PLATFORM_WIN32

#if defined (__GNUC__) && __GNUC__ >= 4
#define LIB_EXP __attribute__ ((visibility("default")))
#endif //__GNUC__

#if PLATFORM_LINUX
#ifndef LIB_EXP
#define LIB_EXP
#endif //LIB_EXP
#endif //PLATFORM_LINUX

// ------ Indielib main -----
// On desktop platforms, this definition is provided as a shorthand for C-based main().
// On iOS it is strictly needed, as using SDL we need to implement a redefined SDL_Main. :(

#ifdef PLATFORM_WIN32
#define Indielib_Main int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif //PLATFORM_WIN32

#ifdef PLATFORM_IOS
#define Indielib_Main extern "C" int SDL_main(int argc, char *argv[])
#endif //PLATFORM_IOS

#ifdef PLATFORM_OSX
#define Indielib_Main int main(int argc, char **argv)
#endif //PLATFORM_OSX

#ifdef PLATFORM_LINUX
#define Indielib_Main int main(int argc, char * argv[])
#endif //PLATFORM_LINUX

// --------------------------------------------------------------------------------
//							       Warnings OFF
// --------------------------------------------------------------------------------

//Visual C++ compilation warnings
#ifdef _MSC_VER
// ' ' needs to have dll-interface to be used by clients of class ' '
#pragma warning (disable : 4251)
// Identifier was truncated to '255' characters in the debug information
#pragma warning (disable : 4786)
// Defaultlib "MSVCRT" conflicts with use of other libs
#pragma warning (disable : 4098)
// all references to "GDI32.dll" discarded by /OPT:REF
#pragma warning (disable : 4089)
// 'sprintf': This function or variable may be unsafe.
#pragma warning (disable : 4996)
// Disables 'unreferenced formal parameter'
#pragma warning( disable : 4100 )  
// Disables 'deprecated function'
#pragma warning( disable : 4996 )  
#endif

#endif
