/*****************************************************************************************
 * File: Mainline.h
 * Desc: Spriter entity's mainline object
 *****************************************************************************************/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */

#ifndef _MAINLINE_
#define _MAINLINE_

// ----- Includes -----

#include "MainlineKey.h"


// --------------------------------------------------------------------------------
//									 Mainline
// --------------------------------------------------------------------------------


class Mainline {

public:

Mainline();
~Mainline();
        
MainlineKey* addKey(int id, int time);


private:

// ----- Entity attributes -----

std::vector <MainlineKey *>	*_keyList;

};

#endif // _MAINLINE_