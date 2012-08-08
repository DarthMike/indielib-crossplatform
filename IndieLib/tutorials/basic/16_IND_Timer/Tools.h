/*****************************************************************************************
 * File: Tools.h
 * Desc: IndieLib Helper class
 *****************************************************************************************/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */

#ifndef _TOOLS_
#define _TOOLS_

class Tools {
public:
	//----- CONSTRUCTORS/DESTRUCTORS -----
	Tools() {
	}
	~Tools() {
	}
	//----- GET/SET FUNCTIONS -----

	//----- OTHER FUNCTIONS -----

	static int itoa(int val, char* buf);
		//----- PUBLIC VARIABLES ------

private:
	//----- INTERNAL VARIABLES -----


	//----- INTERNAL FUNCTIONS -----
};

#endif // _TOOLS_