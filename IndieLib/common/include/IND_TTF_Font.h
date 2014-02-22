/*****************************************************************************************
 * File: IND_TTF_Font.h
 * Desc: TrueType Fontobject
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


#ifndef IND_TTF_FONT_H
#define IND_TTF_FONT_H

// ----- Includes -----

#include "Indie.h"
#include "IND_Image.h"
#include "IND_Surface.h"

#include <map>


// NOTE: This class uses STL, the perfermance will be a lot better in Release version.

#define RGBCOLOR(r,g,b)          ((unsigned long)(((unsigned char)(r)|((unsigned short)((unsigned char)(g))<<8))|(((unsigned short)(unsigned char)(b))<<16)))


#define DT_EX_LEFT                     0x00000001
#define DT_EX_CENTER                   0x00000002
#define DT_EX_RIGHT                    0x00000004
#define DT_EX_TOP                      0x00000008
#define DT_EX_VCENTER                  0x00000010
#define DT_EX_BOTTOM                   0x00000020
#define DT_EX_VERTICAL                 0x00000040
#define DT_EX_RTOLREADING              0x00000080
#define DT_EX_LINEWRAP	               0x00000100
#define DT_EX_BORDER	               0x00000200
#define DT_EX_BACKCOLOR	               0x00000400

class free_type_impl;               // forward-declare private "implementation" class.
class free_type_ptr_wrapped_impl;   // forward-declare the freetype wrapped pointer delivered by the manger in the init method

// --------------------------------------------------------------------------------
//									 IND_TTF_Font
// --------------------------------------------------------------------------------

/**
@defgroup IND_TTF_Font IND_TTF_Font
@ingroup Objects
TTF Font class managed by IND_TTF_FontManager for drawing texts into the screen, click in IND_TTF_Font to see all the methods of this class.
*/
/**@{*/

/**
@b IND_Font is a True Typr Font object from the class ::IND_TTF_FontManager. Read the explanation in ::IND_TTF_FontManager for having more details.
*/
class LIB_EXP IND_TTF_Font {

public:

    //! Typedef byte - TODO : should be moved to the defines setup
	typedef unsigned char byte;
	
    //! Typedef uint32_t - TODO : should be moved to the defines setup
    typedef unsigned int uint32_t;

	// ----- Init/End -----
    
	IND_TTF_Font(	free_type_ptr_wrapped_impl *freetype_wrapped, IND_Render *pIndieRender, IND_ImageManager *pIndieImageManager,
					IND_SurfaceManager *pIndieSurfaceManager);
	~IND_TTF_Font();

    // ----- Public methods -----
        
	//! Load a TTF font from disk file
	bool loadTTFFontFromDisk(	const std::string& strname, const std::string& strpath,
								int iSize, bool bBold, bool bItalic);

	//! Unload the TTF font and free all variables
	void unloadFont();

	//! Cache chars
	bool buildStringCache(const std::wstring& str);

	//! Is the character cached?
	bool isCharCached(wchar_t charCode);

	//! Clear all the cache entries
	void clearAllCache();

	//! Draw a tring
	bool drawText(	const std::wstring& s, float x, float y, uint32_t clrFont,bool bFlipX, bool bFlipY,
					float fZRotate, byte btTrans, bool bKerning, bool bUnderl);

	//! Advanced draw text function
	int drawTextEx(	const std::wstring& sText, float fLeft, float fTop, float fRight, float fBottom,
					uint32_t nFormat, uint32_t clrFont, uint32_t clrBorder, uint32_t clrBack,byte btBorderTrans, 
					byte btBackTrans,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
					bool bKerning, bool bUnderl);

	//! Get the font name 
	const std::string getFontName(){return _strName;}

	//! Set auto cache status
	void setAutoCache(bool bautocache) {_bAutoCache = bautocache;}

	//! Is auto cache enabled
	bool isAutoCache() {return _bAutoCache;}

	//! Does font face has kerning
	bool isKerningSupportedByFace() {return _bHasKerning;}

	//! Set x scale when bliting
	void setXScale(float Scale) {_fXScale = Scale;}

	//! Set y scale when bliting
	void setYScale(float Scale) {_fYScale = Scale;}

	//! Set x/y scale when bliting
	void setScale(float Scale) {_fXScale = _fYScale = Scale;}

	//! Set x hotspot when bliting
	void setXHotspot(float spot) {_fXHotSpot = spot;}

	//! Set y hotspot when bliting
	void setYHotspot(float spot) {_fYHotSpot = spot;}

	//! Set x/y hotspot when bliting
	void setHotspot(float spot) {_fXHotSpot = _fYHotSpot = spot;}

private:

    /** @cond DOCUMENT_PRIVATEAPI */

    // ----- Structures ------
    
	// Struct for every cached character
	// Finally character is cached in an IND_Surface for bliting
	struct CharCacheNode
	{
		wchar_t		charCode;						// unicode char value
		uint32_t	charGlyphIndex;					// glyph index in the font face
		uint32_t	charAdvance;					// advance value

		int			charLeftBearing;				// left bearing of the glyph in the image
		int			charTopBearing;					// top bearing of the glyph in the image

		IND_Surface *pSurface;						// where the glyph texture stored
	};
    typedef std::map<wchar_t, CharCacheNode*> CharCacheMap;
    typedef CharCacheMap::iterator CharCacheMapIterator;

	// ----- Objects -----
    
    //Number of spaces in a tab
	static const unsigned int nTabSize = 4;
   
    free_type_impl          *_impl;                 // free type library wrapper
    
	float					_fFaceAscender;

	IND_Render				*_pIndieRender;
	IND_ImageManager		*_pIndieImageManager;
	IND_SurfaceManager		*_pIndieSurfaceManager;
        
	std::string				_strName;               // font name
	std::string				_strFilePath;           // TTF file path

	int						_CharWidth;             // current font width
	int						_CharHeight;            // current font height

	bool					_bAutoCache;            // auto cache
	bool					_bHasKerning;           // font face has kerning
	
	bool					_bBold;                 // bold
	bool					_bItalic;               // italic

	float					_fXScale;               // x scale for bliting
	float					_fYScale;               // y scale for bliting

	float					_fXHotSpot;             // x hotspot for bliting
	float					_fYHotSpot;             // y hotspot for bliting

	CharCacheMap			_FontCharCache;         // character cache map
    
    // ----- Private methods -----
    
	// cache a single char
	bool buildCharCache(wchar_t charCode);

	// render a single char
	bool renderChar(	wchar_t charCode, float x, float y ,uint32_t clrFont, bool bFlipX, bool bFlipY, float fZRotate,
						byte btTrans, bool bKerning, bool bUnderl);

	// get char cache entry
	CharCacheNode* getCharCacheNode(wchar_t charCode);

	// render glyph to image
	bool renderGlyph(free_type_impl* impl, IND_Image *pImage);

	// advance with space 
	uint32_t getSpaceAdvance();

	// prepare for DrawTextEx
	std::wstring textFormat(	const std::wstring& sText, float &fLineWidth, float &fTotalWidth, float &fTotalHeight,
								int &iTotalLineNum, bool bLineWrap,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
								bool bKerning, bool bUnderl);

	// get single line width
	uint32_t getLineWidth( const std::wstring& sText, bool bFlipX, bool bFlipY, float fZRotate, bool bKerning);

	// draw a single text line
	void drawTextLineEx(	const std::wstring& sText, float penX, float penY, float fL, float fT,
							float fR, float fB, uint32_t clrFont, bool bVertical, bool bR2L,bool bFlipX, 
							bool bFlipY, float fZRotate, byte btTrans, bool bKerning, bool bUnderl);

	void doDrawBorder(float fX_s, float fX_e, float fY, uint32_t clr, byte btTrans);

     /** @endcond */
};
/**@}*/

#endif