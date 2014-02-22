/*****************************************************************************************
 * File: IND_Font.h
 * Desc: Font object
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


#ifndef _IND_FONT_
#define _IND_FONT_

#define MAX_CHARS 4096

#include "IND_Object.h"

class IND_Surface;
// --------------------------------------------------------------------------------
//									 IND_Font
// --------------------------------------------------------------------------------

/**
@defgroup IND_Font IND_Font
@ingroup Objects
Font class managed by IND_FontManager for drawing texts into the screen, click in IND_Font to see all the methods of this class.
*/
/**@{*/

/**
@b IND_Font is a bitmap font object from the class ::IND_FontManager. Read the explanation in ::IND_FontManager for having more details.
*/
class LIB_EXP IND_Font : public IND_Object {
public:
    static IND_Font* newFont() {
        return new IND_Font();
    }
    
    virtual void destroy() {
        delete this;
    }
    
    // ----- Enums ------------

    //! The different types of fonts
    enum FONTTYPE {
        FONTTYPE_MudFont,
        FONTTYPE_AngelCode,
    };
    
    
	// ----- Public Gets ------

	//! This function returns the number of characters of the font.
	int getNumChars() {
		return _font._numChars;
	}
    
    //! This function returns the number of kernings of the font (angelcode specific).
	int getNumKernings() {
		return _font._numKernings;
	}
    
	//! This function returns the name of the font configuration file.
	const char* const getFileName() {
		return _font._name;
	}


private:

	/** @cond DOCUMENT_PRIVATEAPI */

    IND_Font() {}
    virtual ~IND_Font() {}
    
	// ----- Structures ------
    
	// LETTER
	struct structLetter {
		unsigned char _letter;  // Character
		int _x;                 // The left position of the character image in the texture
		int _y;                 // The top position of the character image in the texture
		int _width;             // The width of the character image in the texture
		int _height;            // The height of the character image in the texture

        // angelcode specific
        int _xOffset;            // How much the current position should be offset when copying the image from the texture to the screen
        int _yOffset;            // How much the current position should be offset when copying the image from the texture to the screen
        int _xAdvance;           // How much the current position should be advanced after drawing the character
        int _page;               // The texture page where the character image is found
        int _chnl;               // The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels)
        
		// Attributes initialization
		structLetter(): 
			_letter (0),
			_x (0),
			_y (0),
			_width (0),
			_height (0),
        
            // angelcode specific
            _xOffset(0),
            _yOffset(0),
            _xAdvance(0),
            _page(0),
            _chnl(0){
		}
	};
	typedef struct structLetter LETTER;
    
    
    // KERNING (angelcode specific)
    // The kerning information is used to adjust the distance between certain characters, e.g. some characters should be placed closer to each other than others.
    struct structKerning {
        int _first;             // The first character id.
        int _second;            // The second character id.
        int _amount;            // How much the x position should be adjusted when drawing the second character immediately following the first.
        structKerning():
            _first(0),
            _second(0),
            _amount(0){
		}
	};
	typedef struct structKerning KERNING;

	// FONT
	struct structFont {
		LETTER *_letters;       // Letter array
		int     _numChars;      // Num of chars
		char    *_name;         // Font name
		IND_Surface *_surface;  // Bitmap (IND_Surface object) with the letters // TODO: This needs to be a list of surfaces, since angelcode can make use of more than one image....

        FONTTYPE _type;         // Font type that is being used. ( MudFont / AngelCode )
        int     _numKernings;   // Num of kernings (angelcode specific).
        KERNING *_kernings;     // Kerning array (angelcode specific).
        
                                // TODO: add the toplevel info variables here if needed ( see angelcode documentation )
        

		structFont() :
		    _letters (NULL),
			_numChars(0),
			_name(NULL),
			_surface(NULL),
            _type(FONTTYPE_MudFont),
            _numKernings(0),
            _kernings(NULL){
                _name = new char[MAX_TOKEN];
                memset(_name, (int)'\0', MAX_TOKEN);
		}
        ~structFont() {
            DISPOSEARRAY(_name);
            DISPOSEARRAY(_letters);
            DISPOSEARRAY(_kernings);
        }
	};
	typedef struct structFont FONT;

	FONT _font;

	// ----- Private sets ------

	void setLetters(LETTER *pLetters) {
		_font._letters = pLetters;
	}
	void setNumChars(int pNumChars) {
		_font._numChars = pNumChars;
	}
    void setKernings(KERNING *pKernings) {
		_font._kernings = pKernings;
	}
	void setNumKernings(int pNumKernings) {
		_font._numKernings = pNumKernings;
	}
	void setFileName(const char *pName) {
		strcpy(_font._name,pName);
	}
	void setSurface(IND_Surface *pSurface) {
		_font._surface = pSurface;
	}
    
    void setFontType(FONTTYPE type) {
		_font._type = type;
	}

	// ----- Private gets ------

	LETTER *getLetters() {
		return _font._letters;
	}
    KERNING *getKernings() {
		return _font._kernings;
	}
	IND_Surface *getSurface() { // TODO: needs to take an index (angelcode font support)
		return _font._surface;
	}
    
    FONTTYPE getFontType() {
		return _font._type;
	}

	// ----- Friends -----

	friend class IND_FontManager;
	friend class DirectXRender;
	friend class OpenGLRender;
    friend class OpenGLES2Render;

    /** @endcond */
};
/**@}*/

#endif // _IND_FONT_
