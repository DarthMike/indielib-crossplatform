/*****************************************************************************************
 * File: IndieLib.h
 * Desc: IndieLib initialization / destruction
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

#ifndef _INDIELIB_H_
#define _INDIELIB_H_


// --------------------------------------------------------------------------------
//									  IndieLib
// --------------------------------------------------------------------------------

/**
@defgroup IndieLib_Init_End Init / End
@ingroup Init_End
IndieLib class for initializing and ending the engine.
*/
/**@{*/

/**
@b IndieLib initialization and destruction.
*/
class LIB_EXP IndieLib {
public:

	// ----- Public methods -----

	static void init(IND_InitializationMode pMode);
	static void end();

};
/**@}*/

#endif // _INDIELIB_H_
