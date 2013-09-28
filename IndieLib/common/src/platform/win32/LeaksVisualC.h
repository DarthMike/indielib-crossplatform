/*****************************************************************************************
 * File: LeaksVisualC.h
 * Desc: Static detections of elements even in singleton objects. BUT, not static
 *       variables defined as global constants (this object gets created later and
 *       deleted before them...).
 *       I.e. Static memory leaks detection using a singleton class.
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


//Memory leaks detection
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

/** @cond DOCUMENT_PRIVATEAPI */

//Static variables are shown as leaked (dont worry with this)
#ifdef _DEBUG 
//Memory leak detection
#define FINDMEMLEAK(val) _DetectMemoryLeaks::DetectMemoryLeaks(val) 
class _DetectMemoryLeaks 
{ 
public: 
	static _DetectMemoryLeaks& DetectMemoryLeaks(int val) 
	{ 
		static _DetectMemoryLeaks inst; 
		_crtBreakAlloc=val; 
		return inst; 
	} 
	~_DetectMemoryLeaks() 
	{ 
		//Dumps memory leaks when destructed (end of program)
		_CrtDumpMemoryLeaks(); 
	} 
};

#else 
#define FINDMEMLEAK(val) 
#endif

/** @endcond */