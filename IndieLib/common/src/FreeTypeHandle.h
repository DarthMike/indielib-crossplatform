/*****************************************************************************************
 * File: Timeline.h
 * Desc: Spriter entity's timeline object
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

#ifndef Indielib_FreeTypeHandle_h
#define Indielib_FreeTypeHandle_h

#include <ft2build.h>
#include FT_FREETYPE_H

//! FreeType library wrapper class
class free_type_ptr_wrapped_impl {
public:
    FT_Library				_FTLib;                 // freetype lib handle
    
public:
    // some functions ...
    friend class IND_TTF_FontManager;
    friend class IND_TTF_Font;
};


#endif
