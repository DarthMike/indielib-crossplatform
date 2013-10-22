/*****************************************************************************************
 * File: DebugApi.h
 * Desc: Output information manager class
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


#ifndef _DEBUGAPI_H_
#define _DEBUGAPI_H_

#include <time.h>

#define ESP 3

#include <fstream>

class IND_Timer;
using namespace std;

/** @cond DOCUMENT_PRIVATEAPI */

#if defined(DEBUG) || defined(_DEBUG)
#define LOG_REDIRECT_TO_CONSOLE 1
#endif

class IND_Timer;

class DebugApi {
public:
 
	// ----- Init/End -----

	DebugApi(): _ok(false)  {}
	~DebugApi()              {
		end();
	}

	bool    init();
	void    end();
	bool    isOK()           const {
		return _ok;
	}

	// ----- Public methods -----

	void header(string pData, int pType);
	void dataChar(string pDataChar, bool pFlag);
	void dataInt(int  pDataInt, bool pFlag);
	void dataFloat(float pDataFloat, bool pFlag);
	void breakPoint();
	char *duplicateCharString(const char *charString);
    
    static const int LogHeaderOk;
    static const int LogHeaderError;
    static const int LogHeaderInfo;
    static const int LogHeaderWarning;
    static const int LogHeaderBegin;
    static const int LogHeaderEnd;
    
private:

	// ----- Private -----

	bool _ok;

	// Output debug file
#if LOG_REDIRECT_TO_CONSOLE
	ostream *_count;
#else
    ofstream *_count;
#endif //LOG_REDIRECT_TO_CONSOLE

	// Depeth (increases with each  "{" and go down with each "}")
	int _depth;

	// Time table. After each BEGIN we introduce in this table taking in count the depth variable
	// the current time. When we make the END, we substract in order to measure the time that have passed
	// between the BEGIN and the END
	// It is possible to make a total of 16 BEGIN/END
	double _tableTime [16];

	// Used for start/stop
	unsigned long _time;

	void writeTime();
	void advance();
	void start();
	void stop();
	void allFont();

	void initVars();
	void freeVars();

	IND_Timer* _timer;
};

/** @endcond */

#endif // _DEBUGAPI_H_

