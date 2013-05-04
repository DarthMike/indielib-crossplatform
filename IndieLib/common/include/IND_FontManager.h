/*****************************************************************************************
 * File: IND_FontManager.h
 * Desc: Manager de fuentes
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


#ifndef _IND_FONTMANAGER_
#define _IND_FONTMANAGER_

// ----- Includes -----

#include "IND_SurfaceManager.h"
#include <list>

// ----- Forward declarations -----
class IND_Font;

// --------------------------------------------------------------------------------
//									 IND_FontManager
// --------------------------------------------------------------------------------

/**
@defgroup IND_FontManager IND_FontManager
@ingroup Managers
Manager of IND_Font objects. Click in ::IND_FontManager to see all the methods of this class.
*/
/**@{*/

/**
This class manages the fonts objects ::IND_Font. Fonts are used to write texts on the screen.

(::IND_Font) manages the fonts
created with the @b MudFont program. In order to use these fonts, you will need to create a font
using the special modified MudFont editor for @b IndieLib (see tools section in
http://www.indielib.com). Once the font has been created,
it can be added to the manager using IND_FontManager::add().

@b Note: The version of @b MudFont that uses @b IndieLib is a modification of the original
source code. You can find it in the @b tools section in the website. <i>(actually, only the
output has been modified in order that @b IndieLib can read it) </i>

The @b original MudFont program can be found in http://www.midwinter.com/~lch/programming/mudgefont/

@image html mudge.png Generate your own fonts quickly with MudFont and use them directly in IndieLib (IND_Font)
*/
class LIB_EXP IND_FontManager {
public:

	// ----- Init/End -----

	IND_FontManager(): _ok(false)  { }
	~IND_FontManager()              {
		end();
	}

	bool    init(IND_ImageManager *pImageManager, IND_SurfaceManager *pSurfaceManager);
	void    end();
	bool    isOK();

	// ----- Public methods -----

	bool add(IND_Font       *pNewFont,
	         const char    *pName,
	         const char    *pFile,
	         IND_Type        pType,
	         IND_Quality     pQuality);

	bool add(IND_Font       *pNewFont,
	         IND_Image       *pImage,
	         const char    *pFile,
	         IND_Type        pType,
	         IND_Quality     pQuality);

	bool remove(IND_Font       *pFo);

private:
	/** @cond DOCUMENT_PRIVATEAPI */
	// ----- Private -----

	bool _ok;                   // Manager initialization flag

	// ----- Enum -----

	enum {
		CHAR_ID,
		OFFSET_X,
		OFFSET_Y,
		WIDTH,
		HEIGHT,
		KW_UNKNOWN,
	};

	// ----- Objects -----

	IND_ImageManager *_imageManager;
	IND_SurfaceManager *_surfaceManager;

	// ----- Containers -----

	std::list <IND_Font *> *_listFonts;

	// ----- Private methods -----

	bool                parseFont(IND_Font *pNewFont,const char *pFontName);

	void                addToList(IND_Font *pNewFont);
	void                delFromlist(IND_Font *pFo);

	void                writeMessage();
	void                initVars();
	void                freeVars();
    /** @endcond */
};
/**@}*/

#endif // _IND_FONTMANAGER_
