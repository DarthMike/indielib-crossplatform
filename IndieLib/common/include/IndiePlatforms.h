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

#endif
