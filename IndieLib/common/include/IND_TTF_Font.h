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

#include <ft2build.h>
#include FT_FREETYPE_H

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


class IND_TTF_Font
{
public:
	typedef unsigned char byte;
	typedef unsigned int uint32_t;

	// constructor
	IND_TTF_Font(	FT_Library ftlib, IND_Render *pIndieRender, IND_ImageManager *pIndieImageManager, 
					IND_SurfaceManager *pIndieSurfaceManager);
	~IND_TTF_Font(void);

	// load a TTF font from disk file
	bool LoadTTFFontFromDisk(	const std::string& strname, const std::string& strpath, 
								int iSize, bool bBold, bool bItalic);

	// unload the TTF font and free all variables
	void UnloadFont(void);

	// cache chars
	bool BuildStringCache(const std::wstring& str);

	// is the character cached?
	bool IsCharCached(wchar_t charCode);

	// clear all the cache entries
	void ClearAllCache(void);

	// draw a tring
	bool DrawText(	const std::wstring& s, float x, float y, uint32_t clrFont,bool bFlipX, bool bFlipY, 
					float fZRotate, byte btTrans, bool bKerning, bool bUnderl);

	// advanced draw text function
	int DrawTextEx(	const std::wstring& sText, float fLeft, float fTop, float fRight, float fBottom, 
					uint32_t nFormat, uint32_t clrFont, uint32_t clrBorder, uint32_t clrBack,byte btBorderTrans, 
					byte btBackTrans,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
					bool bKerning, bool bUnderl);

	// get the font name 
	const std::string GetFontName(void){return m_strName;}

	// set auto cache status
	void SetAutoCache(bool bautocache) {m_bAutoCache = bautocache;}

	// is auto cache enabled
	bool IsAutoCache() {return m_bAutoCache;}

	// does font face has kerning
	bool IsKerningSupportedByFace() {return m_bHasKerning;}

	// set x scale when bliting
	void SetXScale(float Scale) {m_fXScale = Scale;}

	// set y scale when bliting
	void SetYScale(float Scale) {m_fYScale = Scale;}

	// set x/y scale when bliting
	void SetScale(float Scale) {m_fXScale = m_fYScale = Scale;}

	// set x hotspot when bliting
	void SetXHotspot(float spot) {m_fXHotSpot = spot;}

	// set y hotspot when bliting
	void SetYHotspot(float spot) {m_fYHotSpot = spot;}

	// set x/y hotspot when bliting
	void SetHotspot(float spot) {m_fXHotSpot = m_fYHotSpot = spot;}

private:
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

	//Number of spaces in a tab
	static const unsigned int nTabSize = 4;

	FT_Library				m_FTLib;			// freetype lib
	FT_Face					m_Face;				// THIS font face
	float					m_fFaceAscender;	

	IND_Render				*m_pIndieRender;
	IND_ImageManager		*m_pIndieImageManager;
	IND_SurfaceManager		*m_pIndieSurfaceManager;
	
	std::string				m_strName;			// font name
	std::string				m_strFilePath;		// TTF file path

	int						m_CharWidth;		// current font width
	int						m_CharHeight;		// current font height

	bool					m_bAutoCache;		// auto cache
	bool					m_bHasKerning;		// font face has kerning
	
	bool					m_bBold;			// bold
	bool					m_bItalic;			// italic
	FT_Matrix				m_matItalic;        // transformation matrix for italic

	float					m_fXScale;			// x scale for bliting
	float					m_fYScale;			// y scale for bliting

	float					m_fXHotSpot;		// x hotspot for bliting
	float					m_fYHotSpot;		// y hotspot for bliting

	CharCacheMap			m_FontCharCache;	// character cache map

private:
	// cache a single char
	bool _BuildCharCache(wchar_t charCode);

	// render a single char
	bool _RenderChar(	wchar_t charCode, float x, float y ,uint32_t clrFont, bool bFlipX, bool bFlipY, float fZRotate, 
						byte btTrans, bool bKerning, bool bUnderl);

	// get char cache entry
	CharCacheNode* _GetCharCacheNode(wchar_t charCode);

	// render glyph to image
	bool _RenderGlyph(FT_Bitmap* Glyph, IND_Image *pImage);

	// advance with space 
	uint32_t _GetSpaceAdvance();

	// prepare for DrawTextEx
	std::wstring _TextFormat(	const std::wstring& sText, float &fLineWidth, float &fTotalWidth, float &fTotalHeight,
								int &iTotalLineNum, bool bLineWrap,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
								bool bKerning, bool bUnderl);

	// get single line width
	uint32_t _GetLineWidth( const std::wstring& sText, bool bFlipX, bool bFlipY, float fZRotate, bool bKerning);

	// draw a single text line
	void _DrawTextLineEx(	const std::wstring& sText, float penX, float penY, float fL, float fT,
							float fR, float fB, uint32_t clrFont, bool bVertical, bool bR2L,bool bFlipX, 
							bool bFlipY, float fZRotate, byte btTrans, bool bKerning, bool bUnderl);

	void _DoDrawBorder(float fX_s, float fX_e, float fY, uint32_t clr, byte btTrans);
};
#endif