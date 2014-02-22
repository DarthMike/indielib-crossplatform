/*****************************************************************************************
 * File: IND_FontManager.h
 * Desc: Manager of Fonts
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

	bool addMudFont(IND_Font    *pNewFont,
                    const char  *pName,
                    const char  *pFile,
                    IND_Type    pType,
                    IND_Quality pQuality);

	bool addMudFont(IND_Font    *pNewFont,
                    IND_Image   *pImage,
                    const char  *pFile,
                    IND_Type    pType,
                    IND_Quality pQuality);
    
    bool addAngelcodeFont(IND_Font      *pNewFont,
                          const char    *pFile,
                          IND_Type      pType,
                          IND_Quality   pQuality);    

	bool remove(IND_Font *pFo);

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

	bool                parseMudFont(IND_Font *pNewFont,const char *pFontName);
    bool                parseAngelCodeFont(IND_Font *pNewFont,const char *pFontName, IND_Type pType, IND_Quality pQuality);

	void                addToList(IND_Font *pNewFont);
	void                delFromlist(IND_Font *pFo);

	void                writeMessage();
	void                initVars();
	void                freeVars();

    /** @endcond */
};
/**@}*/

#endif // _IND_FONTMANAGER_
