/*****************************************************************************************
 * File: DebugApi.h
 * Desc: Output information manager class
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (javierlopezpro@gmail.com)

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

#ifndef _DEBUGAPI_H_
#define _DEBUGAPI_H_

#include "IndiePlatforms.h"
#include <time.h>

#define ESP 3

#include <fstream>
using namespace std;

/** @cond DOCUMENT_PRIVATEAPI */

class IND_Timer;

class DebugApi {
public:
 
	// ----- Init/End -----

	DebugApi(): _ok(false)  { }
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
	ofstream *_count;

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

