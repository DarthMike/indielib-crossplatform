/*****************************************************************************************
 * File: IND_Font.h
 * Desc: Font object
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
MODIFIED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)

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
    
	// ----- Public Gets ------

	//! This function returns the number of characters of the font.
	int getNumChars()      {
		return _font._numChars;
	}
	//! This function returns the name of the font configuration file.
	const char* const getFileName()      {
		return _font._name;
	}

private:
	/** @cond DOCUMENT_PRIVATEAPI */
    IND_Font() {}
    virtual ~IND_Font() {}
    
	// ----- Structures ------

	// LETTER
	struct structLetter {
		unsigned char _letter;          // Character
		int _offsetX;                   // Offset x in the bitmap
		int _offsetY;                   // Offset y in the bitmap
		int _widthChar;                 // Character width
		int _heightChar;                // Character height

		// Attributes initialization
		structLetter(): 
			_letter (0),
			_offsetX (0),
			_offsetY (0),
			_widthChar (0),
			_heightChar (0){
		}
	};
	typedef struct structLetter LETTER;

	// FONT
	struct structFont {
		LETTER *_letters;               // Letter array
		int     _numChars;              // Num of chars
		char    *_name;                 // Font name
		IND_Surface *_surface;          // Bitmap (IND_Surface object) with the letters

		structFont() :
		    _letters (NULL),
			_numChars(0),
			_name(NULL),
			_surface(NULL){
                _name = new char[MAX_TOKEN];
                memset(_name, (int)'\0', MAX_TOKEN);
		}
        ~structFont() {
            DISPOSEARRAY(_name);
            DISPOSEARRAY(_letters);
        }
	};
	typedef struct structFont FONT;

	FONT _font;

	// ----- Private sets ------

	void        setLetters(LETTER *pLetters)      {
		_font._letters = pLetters;
	}
	void        setNumChars(int pNumChars)         {
		_font._numChars = pNumChars;
	}
	void        setFileName(const char *pName)           {
		strcpy(_font._name,pName);
	}
	void        setSurface(IND_Surface *pSurface) {
		_font._surface = pSurface;
	}

	// ----- Private gets ------

	LETTER      *getLetters()                      {
		return _font._letters;
	}
	IND_Surface *getSurface()                      {
		return _font._surface;
	}

	// ----- Friends -----

	friend class IND_FontManager;
	friend class DirectXRender;
	friend class OpenGLRender;
    /** @endcond */
};
/**@}*/

#endif // _IND_FONT_
