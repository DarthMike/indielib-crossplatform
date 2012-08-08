/*******************************************************************************************************************
 *	File: LeaksVisualC.h
 *      Desc: Static detections of elements even in singleton objects. BUT, not static variables defined as
 *	      global constants (this object gets created later and deleted before them...).
 *            I.e. Static memory leaks detection using a singleton class.
 ********************************************************************************************************************/

/* 
	Copyright:   Miguel Angel Quinones (mikeskywalker007@gmail.com)
	Attribution: FROM: http://cgempire.com/forum/tutorials-101/easy-memory-leak-detection-c-c-546.html
  	License:     You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
  	             Nevertheless it would be nice if you tell me you are using something I made, just for curiosity 
*/

//Memory leaks detection
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

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
