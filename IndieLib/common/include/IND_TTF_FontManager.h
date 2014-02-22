/*****************************************************************************************
 * File: IND_TTF_FontManager.h
 * Desc: TrueType Fontobject manager
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


#ifndef IND_TTF_FONTMANAGER_H
#define IND_TTF_FONTMANAGER_H

// ----- Includes -----

#include <string>
#include <map>
#include "IND_TTF_Font.h"

class IND_Math;
class free_type_ptr_wrapped_impl;  // forward-declare private "implementation" class.

// NOTE that this class uses STL, the performance will be a lot better in Release version

/**
@defgroup IND_TTF_FontManager IND_TTF_FontManager
@ingroup Managers
Manager of IND_TTF_Font objects. Click in ::IND_TTF_FontManager to see all the methods of this class.
*/
/**@{*/

/**
TODO: Describtion.
*/


class LIB_EXP IND_TTF_FontManager {

public:

    //! Typedef byte - TODO : should be moved to the defines setup
	typedef unsigned char byte;

    //! Typedef uint32_t - TODO : should be moved to the defines setup
	typedef unsigned int uint32_t;

	// ----- Init/End -----    
    
	IND_TTF_FontManager();
	~IND_TTF_FontManager();

	// ----- Public methods -----	
    
	bool init(IND_Render *pIndieRender, IND_ImageManager *pIndieImageManager, IND_SurfaceManager *pIndieSurfaceManager);
	void end();
	bool isInitialized() {return _bInit;}

	bool addFont(	const std::string& strName, const std::string& strPath, int iSize = 20,
					bool bBold = false, bool bItalic = false);
	bool isFontLoaded(const std::string& strName);
	void unloadFont(const std::string& strName);
	IND_TTF_Font* getFontByName(const std::string& strName);

	bool CacheFontString(const std::string& strFontName, const std::wstring& s);

	void drawText(	uint32_t uiIndex, const std::string strFontName, float x, float y, uint32_t clrFont,
					bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, bool bKerning, 
					bool bUnderl, const wchar_t* format, ...);
	void drawText(	uint32_t uiIndex, const std::string strFontName,const std::wstring s, float x, float y,
					uint32_t clrFont = 0xFFFFFF,bool bFlipX = false, bool bFlipY = false, float fZRotate = 0, 
					byte btTrans = 255, bool bKerning = false, bool bUnderl = false);

	void drawTextEx(uint32_t uiIndex, const std::string& strFontName,const std::wstring& sText, float fLeft,
					float fTop, float fRight, float fBottom, uint32_t nFormat = DT_EX_LEFT | DT_EX_TOP, 
					uint32_t clrFont = 0xFFFFFF, uint32_t clrBorder = 0, uint32_t clrBack = 0,
					byte btBorderTrans = 255, byte btBackTrans = 255, bool bFlipX = false, bool bFlipY = false, 
					float fZRotate = 0, byte btTrans = 255, bool bKerning = false, bool bUnderl = false);

	void removeText(uint32_t uiIndex);

	void renderAllTexts();
	
	void setFontAutoCache(const std::string& strFontName, bool ba);
	void setFontHotSpot(const std::string& strFontName, float hsx, float hsy);
	void setFontScale(const std::string& strFontName, float sx, float sy);

private:
	
    /** @cond DOCUMENT_PRIVATEAPI */

	// ----- Private -----

	typedef std::map<const std::string, IND_TTF_Font*> IND_TTF_FontList;
	typedef IND_TTF_FontList::iterator IND_TTF_FontListIterator;

    // ----- Objects -----
    
	IND_TTF_FontList            _FontList;
	bool                        _bInit;
    IND_Math                    *_math;
	free_type_ptr_wrapped_impl	*_freetype;
	IND_Render                  *_pIndieRender;
	IND_ImageManager            *_pIndieImageManager;
	IND_SurfaceManager          *_pIndieSurfaceManager;

    // ----- Structures ------
    
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

	DTRList					_DTRList;

    // ----- Private methods -----
    
	void doDrawText(const std::string& strFontName,const std::wstring& s, float x, float y,
					 uint32_t clrFont, bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
					 bool bKerning, bool bUnderl);

	int doDrawTextEx(	const std::string& strFontName,const std::wstring& sText, float fLeft,
					float fTop, float fRight, float fBottom, uint32_t nFormat = DT_EX_LEFT | DT_EX_TOP, 
					uint32_t clrFont = 0xFFFFFF, uint32_t clrBorder = 0, uint32_t clrBack = 0,
					byte btBorderTrans = 255, byte btBackTrans = 255, bool bFlipX = false, 
					bool bFlipY = false, float fZRotate = 0, byte btTrans = 255, bool bKerning = false, 
					bool bUnderl = false);

	/** @endcond */

};
/**@}*/

#endif // IND_TTF_FONTMANAGER_H