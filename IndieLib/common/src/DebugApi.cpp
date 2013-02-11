/*****************************************************************************************
 * File: DebugApi.cpp
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

/** @cond DOCUMENT_PRIVATEAPI */

#include <string.h>
#include "DebugApi.h"
#include "IndieVersion.h"


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 * Init
 */
bool DebugApi::init() {
	end();
	initVars();

	// File
	_count = new ofstream("debug.log", ios::out);

	// Time
	time_t mT;							// TODO: these 
	time(&mT);							//       lines 
	struct tm *mPetm = localtime(&mT);				//       probably needs cleeanup, but im not sure /Michael

	// :D

	*_count << endl;

	*_count << "                         ''~``                          " << endl;
	*_count << "                        ( o o )                         " << endl;
	*_count << "+------------------.oooO--(_)--Oooo.------------------+ " << endl;
	*_count << "|                                                     | " << endl;
	*_count << "|                .-------------------.                | " << endl;
	*_count << "|                | I N D I E  L I B  |                | " << endl;
	*_count << "|                .-------------------.                | " << endl;
	*_count << "|                    .oooO                            | " << endl;
	*_count << "|                    (   )   Oooo.                    | " << endl;
	*_count << "+---------------------\\ (----(   )--------------------+" << endl;
	*_count << "                       \\_)    ) /                      " << endl;
	*_count << "                             (_/                        " << endl;
    *_count << "Indielib version: "<<IND_VERSION.major<<"."<<IND_VERSION.minor<<"."<<IND_VERSION.revision;
	*_count << endl;
	*_count << endl;
	*_count << "[g_debug.log]:" << " (";

	// Date
	string days [7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thrusday", "Friday", "Saturday"};
	string months [12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

	// Date
	*_count << days [mPetm->tm_wday].c_str() << ", " << mPetm->tm_mday << " of " << months [mPetm->tm_mon].c_str() << " " << mPetm->tm_year + 1900 << ")" << endl << endl;

	//Start timer
	_timer.start();

	_ok = true;

	return _ok;
}


/**
 * End
 */
void DebugApi::end() {
	if (_ok) {
		_count->close();
		freeVars();
		_ok = false;
	}
}


// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------


/**
 * Header and message
 *  @param pTextString		TODO describtion  
 *  @param pType			TODO describtion 
 */
void DebugApi::header(string pTextString, int pType) {
	if (!_ok) return;

	switch (pType) {
		// Ok
	case(1): {
		// Line
		*_count << "          ";
		*_count << " [  OK   ] ";
		advance();
        *_count << pTextString.c_str() << endl;

		break;
	}
	// Error
	case(2): {
		// Line
		*_count << "          ";
		*_count << " [ ERROR ] ";
		advance();
        *_count << pTextString.c_str() << endl;

		// If we are inside a BEGIN / END, we go out
		if (_depth > 0) {
			// Going back
			_depth -= ESP;
			// Close bracket
			*_count << "                     ";
			advance();
			*_count << "}" << endl;

			// Line
			writeTime();
			*_count << " [  END  ] ";
			advance();
			*_count << "Error occurred";

			// Measure the time between BEGIN and END
			double elapsedTime = _timer.getTicks() - _tableTime [(_depth + ESP) / ESP];
			if (elapsedTime < 0) elapsedTime = 0; // Medida de seguridad
			*_count << " [Elaped time = " << elapsedTime * 0.001f << " seg]" << endl;

			// Line jump after BEGIN/END
			if (!_depth) {
				*_count << "---------------------------------------------------------------------" << endl;
			}
		}

		break;

	}
	// Info
	// Info dosen't make a line jump in order DataChar and DataInt could write just after that line
	case(3): {
		// Line
		*_count << "          ";
		*_count << " [ INFO  ] ";
		advance();
        *_count << pTextString.c_str();

		break;
	}
	// Warning
	case(4): {
		// Line
		*_count << "          ";
		*_count << " [WARNING] ";
		advance();
        *_count << pTextString.c_str() << endl;

		break;
	}
	// Begin
	case(5): {
		// Line
		writeTime();
		*_count << " [ BEGIN ] ";
		advance();
        *_count << "-- " << pTextString.c_str() << " --" << endl;

		// Open brackets
		*_count << "                     ";
		advance();
		*_count << "{" << endl;

		// Advance
		_depth += ESP;

		// Store the current time in the time table
		_tableTime [_depth / ESP] = _timer.getTicks();

		break;
	}
	// End
	case(6): {
		// Going back
		_depth -= ESP;
		// Close bracket
		*_count << "                     ";
		advance();
		*_count << "}" << endl;

		// Line
		writeTime();
		*_count << " [  END  ] ";
		advance();
        *_count << pTextString.c_str();

		// Measure the time between BEGIN and END
		double elapsedTime = _timer.getTicks() - _tableTime [(_depth + ESP) / ESP];
		if (elapsedTime < 0) elapsedTime = 0; // Security Measure
		*_count << " [Elapsed time = " << elapsedTime * 0.001f << " seg]" << endl;

		// Line jump after BEGIN/END
		if (!_depth) {
			*_count << "---------------------------------------------------------------------" << endl;
		}

		break;
	}
	}
}


/**
 * Writes a string.
 *  @param pTextString		text to add to the debuglog
 *  @param pFlag			true if line should break
 */
void DebugApi::dataChar(string pTextString, bool pFlag) {
	if (!_ok) return;

    *_count << " " << pTextString.c_str();
	// Line jump
	if (pFlag)
		*_count << endl;
}


/**
 * Writes an integer.
 *  @param pDataInt		int to add to the debuglog
 *  @param pFlag			true if line should break
 */
void DebugApi::dataInt(int pDataInt, bool pFlag) {
	if (!_ok) return;

	*_count << " " << pDataInt;
	// Jump line
	if (pFlag)
		*_count << endl;
}


/**
 * Writes a float.
 *  @param pDataFloat		float to add to the debuglog
 *  @param pFlag			true if line should break
 */
void DebugApi::dataFloat(float pDataFloat, bool pFlag) {
	if (!_ok) return;

	*_count << " " << pDataFloat;
	// Line jump
	if (pFlag)
		*_count << endl;
}

/**
 * Writes a signal (for debugging purposes).
 */
void DebugApi::breakPoint() {
	if (!_ok) return;

	*_count << "Abracadabra";
	*_count << endl;
}


/**
 * Duplicates an string.
 *  @param charString 		charString to duplicate
 */
char *DebugApi::duplicateCharString(const char *charString) {
	if (!charString)
		return 0;

	size_t len = strlen(charString) + 1;
	char *newString = new char[len];
	memcpy(newString, charString, len * sizeof(char));

	return newString;
}

// --------------------------------------------------------------------------------
//										 Private methods
// --------------------------------------------------------------------------------

/**
 * Writes the current time.
 */
void DebugApi::writeTime() {
	time_t t;
	time(&t);
	struct tm *petm = localtime(&t);

	// Hour
	*_count << "[";

	// Hours
	if (petm->tm_hour < 10)
		*_count << "0";
	*_count << petm->tm_hour << ":";

	// Minutes
	if (petm->tm_min < 10)
		*_count << "0";
	*_count << petm->tm_min << ":";

	// Seconds
	if (petm->tm_sec < 10)
		*_count << "0";
	*_count << petm->tm_sec << "]";
}


/**
 * Advance as many spaces as Depth.
 */
void DebugApi::advance() {
	for (int i = 0; i < _depth; i++)
		*_count << " ";
}


/**
 * Start measuring the time.
 */
void DebugApi::start() {
	_timer.start();
}


/**
 * Stop measuring the time.
 */
void DebugApi::stop() {
	double elapsedTime = _timer.getTicks();
	_timer.stop();
	if (elapsedTime < 0) elapsedTime = 0;
	elapsedTime = elapsedTime * 0.001;
	*_count << elapsedTime << endl;
}


/**
 * Draw all the characteres, including UNICODE.
 */
void DebugApi::allFont() {
	for (int i = 0; i < 256; i++)
		*_count << (char) i;
	*_count << endl;
}


/**
 * Init variables.
 */
void DebugApi::initVars() {
	_depth = 0;
	_time = 0;

	for (int i = 0; i < 16; i++)
		_tableTime [i] = 0;
}


/**
 * Free variables.
 */
void DebugApi::freeVars() {
	DISPOSE(_count);
}

/*** @endcond */