/*****************************************************************************************
 * File: IND_TmxMap.cpp
 * Desc: TmxMap object
 *****************************************************************************************/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */

// ----- Includes -----

#include <cstdio>
#include <stdlib.h>
#include "Global.h"
#include "IND_TmxMap.h"


// ----- Dependencies -----
#include "dependencies/TmxParser/TmxMap.h"

// --------------------------------------------------------------------------------
//										Public methods
// --------------------------------------------------------------------------------

/**
 *  Factory method to build instances of this class.
 *
 *  You should never allocate instances by calling new or allocating objects in the stack.
 *  All memory related to this instance is managed by Indielib internally, and you must add it or remove it from
 *  a manager.
 *
 *  @return A correctly initialized instance
 */
IND_TmxMap* IND_TmxMap::newTmxMap() {
    return new IND_TmxMap();
}

void IND_TmxMap::destroy() {
    delete this;
}