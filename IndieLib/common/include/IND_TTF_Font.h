/*****************************************************************************************
 * File: IND_TTF_Font.h
 * Desc: TrueType Fontobject
 *****************************************************************************************/

/*
 Created by Joel Gao a.k.a venomJ (joel_gao@yahoo.com), Feb 17, 2009 and with his
 blessing added to:
 
 IndieLib 2d library Copyright (C) 2005 Javier LÛpez LÛpez (info@pixelartgames.com)
 
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


#ifndef IND_TTF_FONT_H
#define IND_TTF_FONT_H

#include "Indie.h"
#include "IND_Image.h"
#include "IND_Surface.h"

//#include <ft2build.h>
//#include FT_FREETYPE_H

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
struct free_type_ptr_wrapped_impl;   // forward-declare the freetype wrapped pointer delivered by the manger in the init method

// --------------------------------------------------------------------------------
//									 IND_TTF_Font
// --------------------------------------------------------------------------------


class LIB_EXP IND_TTF_Font {

public:
	typedef unsigned char byte;             // TODO : should be moved to the defines setup
	typedef unsigned int uint32_t;          // TODO : should be moved to the defines setup

	// ----- Init/End -----
    
	IND_TTF_Font(	free_type_ptr_wrapped_impl *freetype_wrapped, IND_Render *pIndieRender, IND_ImageManager *pIndieImageManager,
					IND_SurfaceManager *pIndieSurfaceManager);
	~IND_TTF_Font();

    // ----- Public methods -----
        
	// load a TTF font from disk file
	bool loadTTFFontFromDisk(	const std::string& strname, const std::string& strpath,
								int iSize, bool bBold, bool bItalic);

	// unload the TTF font and free all variables
	void unloadFont();

	// cache chars
	bool buildStringCache(const std::wstring& str);

	// is the character cached?
	bool isCharCached(wchar_t charCode);

	// clear all the cache entries
	void clearAllCache();

	// draw a tring
	bool drawText(	const std::wstring& s, float x, float y, uint32_t clrFont,bool bFlipX, bool bFlipY,
					float fZRotate, byte btTrans, bool bKerning, bool bUnderl);

	// advanced draw text function
	int drawTextEx(	const std::wstring& sText, float fLeft, float fTop, float fRight, float fBottom,
					uint32_t nFormat, uint32_t clrFont, uint32_t clrBorder, uint32_t clrBack,byte btBorderTrans, 
					byte btBackTrans,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
					bool bKerning, bool bUnderl);

	// get the font name 
	const std::string getFontName(){return _strName;}

	// set auto cache status
	void setAutoCache(bool bautocache) {_bAutoCache = bautocache;}

	// is auto cache enabled
	bool isAutoCache() {return _bAutoCache;}

	// does font face has kerning
	bool isKerningSupportedByFace() {return _bHasKerning;}

	// set x scale when bliting
	void setXScale(float Scale) {_fXScale = Scale;}

	// set y scale when bliting
	void setYScale(float Scale) {_fYScale = Scale;}

	// set x/y scale when bliting
	void setScale(float Scale) {_fXScale = _fYScale = Scale;}

	// set x hotspot when bliting
	void setXHotspot(float spot) {_fXHotSpot = spot;}

	// set y hotspot when bliting
	void setYHotspot(float spot) {_fYHotSpot = spot;}

	// set x/y hotspot when bliting
	void setHotspot(float spot) {_fXHotSpot = _fYHotSpot = spot;}

private:
    
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

   
    free_type_impl          *_impl;         // free type library wrapper
    
	//FT_Library				_FTLib;                 // freetype lib
	//FT_Face					_Face;                  // THIS font face
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
	//FT_Matrix				_matItalic;             // transformation matrix for italic

	float					_fXScale;               // x scale for bliting
	float					_fYScale;               // y scale for bliting

	float					_fXHotSpot;             // x hotspot for bliting
	float					_fYHotSpot;             // y hotspot for bliting

	CharCacheMap			_FontCharCache;         // character cache map

private:
    
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
};
#endif