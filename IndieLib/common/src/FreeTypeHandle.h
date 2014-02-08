//
//  FreeTypeHandler.h
//  Indielib
//
//  Created by nope nope on 10/21/13.
//
//

#ifndef Indielib_FreeTypeHandle_h
#define Indielib_FreeTypeHandle_h

#include <ft2build.h>
#include FT_FREETYPE_H


class free_type_ptr_wrapped_impl {
public:
    FT_Library				_FTLib;                 // freetype lib
    
    // ...
public:
    // some functions ...
    friend class IND_TTF_FontManager;
    friend class IND_TTF_Font;
};


#endif
