/*****************************************************************************************
 * File: IND_TTF_FontManager.h
 * Desc: TrueType Fontobject manager
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

#ifndef IND_TTF_FONTMANAGER_H
#define IND_TTF_FONTMANAGER_H

#include <string>
#include <map>
#include "IND_TTF_Font.h"

// NOTE that this class uses STL, the perfermance will be a lot better in Release version 

class IND_TTF_FontManager
{
public:
	typedef unsigned char byte;
	typedef unsigned int uint32_t;

	IND_TTF_FontManager(void);
	~IND_TTF_FontManager(void);
public:
	
	bool Init(IND_Render *pIndieRender, IND_ImageManager *pIndieImageManager, IND_SurfaceManager *pIndieSurfaceManager);
	void End(void);
	bool IsInitialized() {return m_bInit;}

	bool AddFont(	const std::string& strName, const std::string& strPath, int iSize = 20, 
					bool bBold = false, bool bItalic = false);
	bool IsFontLoaded(const std::string& strName);
	void UnloadFont(const std::string& strName);
	IND_TTF_Font* GetFontByName(const std::string& strName);

	bool CacheFontString(const std::string& strFontName, const std::wstring& s);

	void DrawText(	uint32_t uiIndex, const std::string strFontName, float x, float y, uint32_t clrFont,
					bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, bool bKerning, 
					bool bUnderl, const wchar_t* format, ...);
	void DrawText(	uint32_t uiIndex, const std::string strFontName,const std::wstring s, float x, float y, 
					uint32_t clrFont = 0xFFFFFF,bool bFlipX = false, bool bFlipY = false, float fZRotate = 0, 
					byte btTrans = 255, bool bKerning = false, bool bUnderl = false);

	void DrawTextEx(uint32_t uiIndex, const std::string& strFontName,const std::wstring& sText, float fLeft, 
					float fTop, float fRight, float fBottom, uint32_t nFormat = DT_EX_LEFT | DT_EX_TOP, 
					uint32_t clrFont = 0xFFFFFF, uint32_t clrBorder = 0, uint32_t clrBack = 0,
					byte btBorderTrans = 255, byte btBackTrans = 255, bool bFlipX = false, bool bFlipY = false, 
					float fZRotate = 0, byte btTrans = 255, bool bKerning = false, bool bUnderl = false);

	void RemoveText(uint32_t uiIndex);

	void RenderAllTexts(void);
	
	void SetFontAutoCache(const std::string& strFontName, bool ba);
	void SetFontHotSpot(const std::string& strFontName, float hsx, float hsy);
	void SetFontScale(const std::string& strFontName, float sx, float sy);

private:
	typedef std::map<const std::string, IND_TTF_Font*> IND_TTF_FontList;
	typedef IND_TTF_FontList::iterator IND_TTF_FontListIterator;

	IND_TTF_FontList		m_FontList;

	bool					m_bInit;
	FT_Library				m_FTLib;
	
	IND_Render				*m_pIndieRender;
	IND_ImageManager		*m_pIndieImageManager;
	IND_SurfaceManager		*m_pIndieSurfaceManager;

	// Struct for every cached draw text request
	struct DrawTextRequestNode
	{
		bool				bEx;				// ex mode
		std::string			sFont;				// font name
		std::wstring		sText;				// text to draw
		float				xPos;				// x pos
		float				yPos;				// y pos
		// Formats
		uint32_t			clrFont;			// font color
		bool				bMirrorX;			// mirror x
		bool				bMirrorY;			// mirror y
		float				fZRotate;			// rotate angle in Z axis	
		byte				btTransparency;		// transparency
		bool				bUseKerning;		// use kerning?
		bool				bUnderline;			// underline
		// Formats EX
		float				rPos;				// right
		float				bPos;				// bottom
		uint32_t			nFmt;				// ex format
		uint32_t			clrBdr;				// border color
		uint32_t			clrBak;				// background color
		byte				btBdrTrans;			// border trans
		byte				btBakTrans;			// back trans
	};
	typedef std::map<const uint32_t, DrawTextRequestNode*> DTRList;
	typedef DTRList::iterator DTRListIterator;

	DTRList					m_DTRList;

private:
	void _DoDrawText(const std::string& strFontName,const std::wstring& s, float x, float y,
					 uint32_t clrFont, bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
					 bool bKerning, bool bUnderl);

	int _DoDrawTextEx(	const std::string& strFontName,const std::wstring& sText, float fLeft, 
					float fTop, float fRight, float fBottom, uint32_t nFormat = DT_EX_LEFT | DT_EX_TOP, 
					uint32_t clrFont = 0xFFFFFF, uint32_t clrBorder = 0, uint32_t clrBack = 0,
					byte btBorderTrans = 255, byte btBackTrans = 255, bool bFlipX = false, 
					bool bFlipY = false, float fZRotate = 0, byte btTrans = 255, bool bKerning = false, 
					bool bUnderl = false);

};
#endif