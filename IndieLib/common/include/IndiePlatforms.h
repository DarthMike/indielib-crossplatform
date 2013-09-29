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
#define PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX //Min max definitions conflicting with std::min and std::max
#endif


// OSX vs. iOS:
// http://sealiesoftware.com/blog/archive/2010/8/16/TargetConditionalsh.html
#ifdef __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#define PLATFORM_IOS
#else
#define PLATFORM_OSX
#endif
#endif

// Linux
#if defined(linux) || defined(__linux)
#define PLATFORM_LINUX
#endif 

// ----- Renderer settings -----
// Change here to override preprocessor macro definition. Only affects windows, as all other
// platforms compile in one renderer only.
#if !defined (INDIERENDER_DIRECTX) && !defined (INDIERENDER_GLES_IOS) && !defined (INDIERENDER_OPENGL)
//#define INDIERENDER_DIRECTX
#endif
#if !defined (INDIERENDER_OPENGL) && !defined (INDIERENDER_GLES_IOS) && !defined (INDIERENDER_DIRECTX)
//#define INDIERENDER_OPENGL
#endif
#if !defined (INDIERENDER_GLES_IOS) && !defined (INDIERENDER_OPENGL) && !defined (INDIERENDER_DIRECTX)
#define INDIERENDER_GLES_IOS
#endif

// ----- Renderer set checkings -----
//Only one render type per configuration!
//DirectX vs. OPENGLES
#if defined (INDIERENDER_DIRECTX) && defined (INDIERENDER_GLES_IOS)
#error Multiple renderers defined. Check Defines.h
#endif
//DirectX vs. OPENGL
#if defined (INDIERENDER_DIRECTX) && defined (INDIERENDER_OPENGL)
#error Multiple renderers defined. Check Defines.h
#endif
//OPENGL vs. OPENGLES
#if defined (INDIERENDER_GLES_IOS) && defined (INDIERENDER_OPENGL)
#error Multiple renderers defined. Check Defines.h
#endif

//Undefine symbols depending on chosen renderer
#ifdef INDIERENDER_OPENGL
#undef INDIERENDER_DIRECTX
#undef INDIERENDER_GLES_IOS
#undef PLATFORM_IOS			//We need this as iOS is a subset of MAC in apple terms (both defined)
#endif //INDIERENDER_OPENGL

#ifdef INDIERENDER_DIRECTX
#undef INDIERENDER_OPENGL
#undef INDIERENDER_GLES_IOS  
#endif //INDIERENDER_DIRECTX

#ifdef INDIERENDER_GLES_IOS
#undef INDIERENDER_OPENGL
#undef INDIERENDER_DIRECTX
#undef PLATFORM_OSX          //We need this as iOS is a subset of MAC in apple terms (both defined)
#endif //INDIERENDER_GLES_IOS
//****************************************

// ---- Config checkings -----
//Win32 platform vs. render settings
#if defined (PLATFORM_WIN32) && defined (INDIERENDER_GLES_IOS)
#error Render GLES_IOS defined for platform WIN32! Check Defines.h
#endif
//iOS platform vs. render setting
#if defined (PLATFORM_IOS) && !defined (INDIERENDER_GLES_IOS)
#error Render GLES_IOS NOT defined for platform iOS! Check Defines.h
#endif
//OSX platform vs. render setting
#if defined (PLATFORM_OSX) && !defined (INDIERENDER_OPENGL)
#error Render INDIERENDER_OPENGL NOT defined for platform OSX! Check Defines.h
#endif
//Linux platform vs. render setting
#if defined (PLATFORM_LINUX) && !defined (INDIERENDER_OPENGL)
#error Render GL NOT defined for platform Linux! Check Defines.h
#endif
// ----- Lib export -----

#ifdef PLATFORM_WIN32

#ifdef INDIELIB_DLLBUILD
#define LIB_EXP __declspec(dllexport)
#else
#define LIB_EXP __declspec(dllimport)
#endif //INDIELIB_DLLBUILD

#define Indielib_Main() WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif //PLATFORM_WIN32

#ifdef PLATFORM_IOS
typedef unsigned char BYTE;    // HACK: Fixes some code regarding surfaces, fixes same problem as in Linux.
#if defined (__GNUC__) && __GNUC__ >= 4
#define LIB_EXP __attribute__ ((visibility("default")))
#else
#define LIB_EXP
#endif
#define Indielib_Main() main(int argc, char **argv)
#endif //PLATFORM_IOS

#ifdef PLATFORM_OSX
typedef unsigned char BYTE;    // HACK: Fixes some code regarding surfaces, fixes same problem as in Linux.
#if defined (__GNUC__) && __GNUC__ >= 4
#define LIB_EXP __attribute__ ((visibility("default")))
#else
#define LIB_EXP
#endif
#define Indielib_Main() main(int argc, char **argv)
#endif //PLATFORM_OSX

#ifdef PLATFORM_LINUX
#define LIB_EXP
typedef unsigned char BYTE;    // HACK: Fixes some code regarding surfaces, BYTE is not defined in Linux (and remember Linux is case sensitive BYTE != byte )
#define Indielib_Main() main(int argc, char * argv[])
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
