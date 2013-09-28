/*****************************************************************************************
 * File: IndiePlatforms.h
 * Desc: Platform Definitions
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
THIS FILE IS AN ADDITIONAL FILE ADDED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com), BUT HAS THE
SAME LICENSE AS THE WHOLE LIBRARY TO RESPECT ORIGINAL AUTHOR OF LIBRARY

This library is free software; you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
Suite 330, Boston, MA 02111-1307 USA
*/


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

// IOS
#if defined (__APPLE__)
#include "TargetConditionals.h"
#if defined (TARGET_OS_IPHONE) || defined (TARGET_IPHONE_SIMULATOR)
#define PLATFORM_IOS
#endif
#endif

// OSX
#if defined (__APPLE__) && defined (__MACH__)
#include "AvailabilityMacros.h"
#include "TargetConditionals.h"
#define PLATFORM_OSX
#endif

// Linux
#if defined(linux) || defined(__linux)
#define PLATFORM_LINUX
#endif 

// ----- Renderer settings -----
//****Renderer choosing in static time****
//YOU SHOULD CHANGE HERE THE DEFINITION OF WHICH RENDERER TO COMPILE WITH
//#define INDIERENDER_DIRECTX
#define INDIERENDER_OPENGL
//#define INDIERENDER_GLES_IOS

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

#define IndieLib() WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif //PLATFORM_WIN32

#ifdef PLATFORM_IOS
typedef unsigned char BYTE;    // HACK: Fixes some code regarding surfaces, fixes same problem as in Linux.
#if defined (__GNUC__) && __GNUC__ >= 4
#define LIB_EXP __attribute__ ((visibility("default")))
#else
#define LIB_EXP
#endif
#define IndieLib() main(int argc, char **argv)
#endif //PLATFORM_IOS

#ifdef PLATFORM_OSX
typedef unsigned char BYTE;    // HACK: Fixes some code regarding surfaces, fixes same problem as in Linux.
#if defined (__GNUC__) && __GNUC__ >= 4
#define LIB_EXP __attribute__ ((visibility("default")))
#else
#define LIB_EXP
#endif
#define IndieLib() main(int argc, char **argv)
#endif //PLATFORM_OSX

#ifdef PLATFORM_LINUX
#define LIB_EXP
typedef unsigned char BYTE;    // HACK: Fixes some code regarding surfaces, BYTE is not defined in Linux (and remember Linux is case sensitive BYTE != byte )
#define IndieLib() main(int argc, char * argv[])
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
