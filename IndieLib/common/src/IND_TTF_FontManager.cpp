/*****************************************************************************************
 * File: IND_TTF_FontManager.cpp
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


// ----- Includes -----

#include "Global.h"
#include "IND_TTF_FontManager.h"
#include "IND_Math.h"
#include "FreeTypeHandle.h"
#include <assert.h>

#include <ft2build.h>
#include FT_FREETYPE_H


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

IND_TTF_FontManager::IND_TTF_FontManager(void)
: _bInit(false),
_pIndieRender(NULL),
_pIndieImageManager(NULL),
_pIndieSurfaceManager(NULL),
_freetype(NULL)
{
}

IND_TTF_FontManager::~IND_TTF_FontManager(void) {
	if (_bInit) {
		g_debug->header("Finalizing TTF FontManager", DebugApi::LogHeaderBegin);
    }
	end();
}

bool IND_TTF_FontManager::init(IND_Render *pRender, IND_ImageManager *pImageManager, IND_SurfaceManager *pSurfaceManager) {
    
    g_debug->header("Initializing TTF FontManager", DebugApi::LogHeaderBegin);
    
	// Checking IND_Render
	if (pRender->isOK()) {
		g_debug->header("Checking IND_Render", DebugApi::LogHeaderOk);
		_pIndieRender = pRender;
	}
    else {
		g_debug->header("IND_Render is not correctly initialized", DebugApi::LogHeaderError);
		_bInit = false;
		return _bInit;
	}
    
    
	// Checking IND_ImageManager
	if (pImageManager->isOK()) {
		g_debug->header("Checking IND_ImageManager", DebugApi::LogHeaderOk);
		_pIndieImageManager = pImageManager;
    }
    else {
		g_debug->header("IND_ImageManager is not correctly initialized", DebugApi::LogHeaderError);
		_bInit = false;
		return _bInit;
	}

    // Checking IND_SurfaceManager
	if (pSurfaceManager->isOK()) {
		g_debug->header("Checking IND_SurfaceManager", DebugApi::LogHeaderOk);
		_pIndieSurfaceManager = pSurfaceManager;
    }
    else {
		g_debug->header("IND_SurfaceManager is not correctly initialized", DebugApi::LogHeaderError);
		_bInit = false;
		return _bInit;
	}
    
    
	if(_bInit)
		return true;
    
    _freetype = new free_type_ptr_wrapped_impl();

	if(FT_Init_FreeType(&_freetype->_FTLib) != 0) {
        g_debug->header("FreeType library is not correctly initialized", DebugApi::LogHeaderError);
		_bInit = false;
        return _bInit;
    }
    else {
		_bInit = true;
    }

    
    FT_Int major;
    FT_Int minor;
    FT_Int pitch;
    
    FT_Library_Version(_freetype->_FTLib, &major, &minor, &pitch);
    
    char freeTypeVer[15];
    char tempMajor[4];
    char tempMinor[4];
    char tempPitch[4];
	
    
    _math = new IND_Math();                        
	_math->init();
    _math->itoa(major,tempMajor);
    _math->itoa(minor,tempMinor);
    _math->itoa(pitch,tempPitch);
    
    strcat (freeTypeVer, tempMajor);
    strcat (freeTypeVer, ".");
    strcat (freeTypeVer, tempMinor);
    strcat (freeTypeVer, ".");
    strcat (freeTypeVer, tempPitch);
    
    
    const char* freeTypeCopyright = "This program uses FreeType, a freely available software library to render fonts. See http://www.freetype.org for details.";
    
    g_debug->header("Using FreeType version: ",DebugApi::LogHeaderInfo);
	g_debug->dataChar(freeTypeVer, true);
    g_debug->header("Copyright: ", DebugApi::LogHeaderInfo);
    g_debug->dataChar(freeTypeCopyright, true);
	g_debug->header("IND_TTF_FontManager Initialised", DebugApi::LogHeaderEnd);
    

	return _bInit;
}

// --------------------------------------------------------------------------------
//									Public methods
// --------------------------------------------------------------------------------

/**
 * Returns true if the font is allready added to the manager, else
 * tries to load the font and returns true if the font have been
 * sucessfully added to the manager.
 *
 * Note: the iSize value nead to be bigger than 5.
 *
 *
 * @param strName					Name of the font.
 * @param strPath					Path of the font.
 * @param iSize						Size of the font.
 * @param bBold                     Bold true / false.
 * @param bItalic					Italic true / false.
 */
bool IND_TTF_FontManager::addFont(const std::string& strName, const std::string& strPath, int iSize, bool bBold, bool bItalic) {
	if(iSize < 5)
		return false; // too small

	if (!_bInit)
		return false;

	if (isFontLoaded(strName))
		return true;

	IND_TTF_Font* ptrNewFont = new IND_TTF_Font(_freetype, _pIndieRender, _pIndieImageManager, _pIndieSurfaceManager);
	if (!ptrNewFont->loadTTFFontFromDisk(strName, strPath, iSize, bBold, bItalic)) {
		delete ptrNewFont;
		return false;
	}
	_FontList[strName] = ptrNewFont;
	
    return true;
}

/**
 * Returns true if the font is allready added to the manager.
 *
 * @param strName					Name of the font.
 */
bool IND_TTF_FontManager::isFontLoaded(const std::string& strName) {
	return getFontByName(strName) != NULL;
}

/**
 * Unloads and deletes all managed fonts.
 */
void IND_TTF_FontManager::end() {
	if (!_bInit)
		return;

	//free up
	while (!_DTRList.empty()) {
		DrawTextRequestNode* pNode = _DTRList.begin()->second;
		_DTRList.erase(_DTRList.begin());
				
		delete pNode;
	}

	while (!_FontList.empty()) {
		IND_TTF_Font* pFont = _FontList.begin()->second;
		_FontList.erase(_FontList.begin());
		pFont->unloadFont();
		delete pFont;
	}

	if (_bInit) {
		FT_Done_FreeType(_freetype->_FTLib);
		_bInit = false;
	}
    
    // free wrapper object
    delete _freetype;
    
    // free math object;
    _math->end();
    delete _math;
    
}

/**
 * Unloads and delete font.
 *
 * @param strName					Name of the font.
 */
void IND_TTF_FontManager::unloadFont(const std::string& strName) {
	if (!_bInit)
		return;

	IND_TTF_FontListIterator itFont = _FontList.begin();
	
    for (; itFont != _FontList.end(); ++itFont) {
        IND_TTF_Font* pFont = itFont->second;
		
        if (pFont->getFontName() == strName) {
			_FontList.erase(itFont);
			pFont->unloadFont();
			delete pFont;
		}
	}
}

/**
 * Returns an IND_TTF_Font if the search is sucessfull, otherwise NULL
 *
 * @param strName					Name of the font.
 */
IND_TTF_Font* IND_TTF_FontManager::getFontByName(const std::string& strName) {
	if (!_bInit)
		return NULL;

	IND_TTF_FontListIterator it = _FontList.find(strName);
	if(it == _FontList.end())
		return NULL;
	else
		return it->second;
}

/**
 * TODO:describtion
 *
 * @param uiIndex					TODO: describtion
 * @param strFontName				Name of the font.
 * @param x                         TODO: describtion
 * @param y                         TODO: describtion
 * @param clrFont					TODO: describtion
 * @param bFlipX                    TODO: describtion
 * @param bFlipY                    TODO: describtion
 * @param fZRotate                  TODO: describtion
 * @param btTrans                   TODO: describtion
 * @param bKerning                  TODO: describtion
 * @param bUnderl                   TODO: describtion
 * @param format                    TODO: describtion
 */
void IND_TTF_FontManager::drawText(	uint32_t uiIndex, const std::string strFontName, float x, float y,
									uint32_t clrFont,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
									bool bKerning, bool bUnderl, const wchar_t* format, ...) {
	va_list ArgPtr;

	va_start(ArgPtr, format);
	std::size_t Length = vwprintf(format, ArgPtr) + 1;
	std::wstring m_WBuffer;
	m_WBuffer.resize(Length);
	vswprintf(&m_WBuffer[0], Length, format, ArgPtr);
		
	va_end(ArgPtr);

	drawText(uiIndex, strFontName, m_WBuffer, x, y, clrFont, bFlipX, bFlipY, fZRotate, btTrans, bKerning,bUnderl);
}

/**
 * TODO:describtion
 *
 * @param uiIndex					TODO: describtion
 * @param strFontName				Name of the font.
 * @param s                         TODO: describtion
 * @param x                         TODO: describtion
 * @param y                         TODO: describtion
 * @param clrFont					TODO: describtion
 * @param bFlipX                    TODO: describtion
 * @param bFlipY                    TODO: describtion
 * @param fZRotate                  TODO: describtion
 * @param btTrans                   TODO: describtion
 * @param bKerning                  TODO: describtion
 * @param bUnderl                   TODO: describtion
 */
void IND_TTF_FontManager::drawText(uint32_t uiIndex, const std::string strFontName,const std::wstring s,
								   float x, float y, uint32_t clrFont,bool bFlipX, bool bFlipY, float fZRotate, 
								   byte btTrans, bool bKerning, bool bUnderl) {
	DrawTextRequestNode *pNewReq = NULL;

	DTRListIterator it = _DTRList.find(uiIndex);
	if(it == _DTRList.end()){
		pNewReq = new DrawTextRequestNode;
		assert(pNewReq);
	
    } else {
		pNewReq = it->second;
	}

	pNewReq->bEx			= false;
	pNewReq->sFont			= strFontName;
	pNewReq->sText			= s;
	pNewReq->xPos			= x;
	pNewReq->yPos			= y;
	//formats
	pNewReq->clrFont		= clrFont;
	pNewReq->bMirrorX		= bFlipX;
	pNewReq->bMirrorY		= bFlipY;
	pNewReq->fZRotate		= fZRotate;
	pNewReq->btTransparency	= btTrans;
	pNewReq->bUseKerning	= bKerning;
	pNewReq->bUnderline		= bUnderl;
	

	if(it == _DTRList.end())
		_DTRList.insert(std::pair<uint32_t, DrawTextRequestNode*>(uiIndex, pNewReq));
}

/*
void IND_TTF_FontManager::SetFontColor(const std::string& strFontName, unsigned int uiClr)
{
	IND_TTF_Font *pFont = GetFontByName(strFontName);
	if(pFont)
		pFont->SetColor(uiClr);
}
*/

/**
 * TODO:describtion
 *
 * @param strFontName				Name of the font.
 * @param s                         TODO: describtion
 */
bool IND_TTF_FontManager::CacheFontString(const std::string& strFontName, const std::wstring& s) {
	IND_TTF_Font *pFont = getFontByName(strFontName);
	if(pFont)
		return pFont->buildStringCache(s);

	return false;
}

/**
 * TODO:describtion
 *
 * @param strFontName				Name of the font.
 * @param ba                        TODO: describtion
 */
void IND_TTF_FontManager::setFontAutoCache(const std::string& strFontName, bool ba) {
	IND_TTF_Font *pFont = getFontByName(strFontName);
	if(pFont)
		pFont->setAutoCache(ba);
}

/**
 * TODO:describtion
 *
 * @param strFontName				Name of the font.
 * @param hsx                       TODO: describtion
 * @param hsy                       TODO: describtion
 */
void IND_TTF_FontManager::setFontHotSpot(const std::string& strFontName, float hsx, float hsy) {
	IND_TTF_Font *pFont = getFontByName(strFontName);
	if(pFont) {
		pFont->setXHotspot(hsx);
		pFont->setYHotspot(hsy);
	}
}

/**
 * TODO:describtion
 *
 * @param strFontName				Name of the font.
 * @param sx                        TODO: describtion
 * @param sy                        TODO: describtion
 */
void IND_TTF_FontManager::setFontScale(const std::string& strFontName, float sx, float sy) {
	IND_TTF_Font *pFont = getFontByName(strFontName);
	if(pFont) {
		pFont->setXScale(sx);
		pFont->setYScale(sy);
	}
}

/**
 * TODO:describtion
 *
 * @param uiIndex					TODO: describtion
 * @param strFontName				Name of the font.
 * @param sText                     TODO: describtion
 * @param fLeft                     TODO: describtion
 * @param fTop                      TODO: describtion
 * @param fRight					TODO: describtion
 * @param fBottom                   TODO: describtion
 * @param nFormat                   TODO: describtion
 * @param clrFont                   TODO: describtion
 * @param clrBorder                 TODO: describtion
 * @param clrBack                   TODO: describtion
 * @param btBorderTrans             TODO: describtion
 * @param btBackTrans               TODO: describtion
 * @param bFlipX                    TODO: describtion
 * @param bFlipY                    TODO: describtion
 * @param fZRotate                  TODO: describtion
 * @param btTrans                   TODO: describtion
 * @param bKerning                  TODO: describtion
 * @param bUnderl                   TODO: describtion
 */
void IND_TTF_FontManager::drawTextEx(uint32_t uiIndex, const std::string& strFontName,const std::wstring& sText,
									float fLeft, float fTop, float fRight, float fBottom, 
									uint32_t nFormat, uint32_t clrFont,uint32_t clrBorder, uint32_t clrBack,
									byte btBorderTrans, byte btBackTrans, bool bFlipX, bool bFlipY, 
									float fZRotate, byte btTrans, bool bKerning, bool bUnderl) {
	DrawTextRequestNode *pNewReq = NULL;

	DTRListIterator it = _DTRList.find(uiIndex);
	if(it == _DTRList.end()) {
		pNewReq = new DrawTextRequestNode;
		assert(pNewReq);
	
    } else {
		pNewReq = it->second;
	}

	pNewReq->bEx			= true;
	pNewReq->sFont			= strFontName;
	pNewReq->sText			= sText;
	pNewReq->xPos			= fLeft;
	pNewReq->yPos			= fTop;
	//formats ex
	pNewReq->clrFont		= clrFont;
	pNewReq->rPos			= fRight;
	pNewReq->bPos			= fBottom;
	pNewReq->nFmt			= nFormat;
	pNewReq->clrBdr			= clrBorder;
	pNewReq->clrBak			= clrBack; 
	pNewReq->btBdrTrans		= btBorderTrans;
	pNewReq->btBakTrans		= btBackTrans;
	//formats
	pNewReq->bMirrorX		= bFlipX;
	pNewReq->bMirrorY		= bFlipY;
	pNewReq->fZRotate		= fZRotate;
	pNewReq->btTransparency	= btTrans;
	pNewReq->bUseKerning	= bKerning;
	pNewReq->bUnderline		= bUnderl;
	

	if(it == _DTRList.end())
		_DTRList.insert(std::pair<uint32_t, DrawTextRequestNode*>(uiIndex, pNewReq));
}

/**
 * TODO:describtion
 */
void IND_TTF_FontManager::renderAllTexts() {
	// render simple text from DrawText method
	DrawTextRequestNode *pReq = NULL;
	for(DTRListIterator it = _DTRList.begin() ; it != _DTRList.end() ; it++) {
		pReq = it->second;
		if(pReq->bEx)
			doDrawTextEx(	pReq->sFont, pReq->sText, pReq->xPos, pReq->yPos,
							pReq->rPos, pReq->bPos, pReq->nFmt, pReq->clrFont, 
							pReq->clrBdr, pReq->clrBak, 
							pReq->btBdrTrans, pReq->btBakTrans, 
							pReq->bMirrorX,
							pReq->bMirrorY,
							pReq->fZRotate,
							pReq->btTransparency,
							pReq->bUseKerning,
							pReq->bUnderline);
		else
			doDrawText(pReq->sFont, pReq->sText, pReq->xPos, pReq->yPos,
					pReq->clrFont, 
					pReq->bMirrorX,
					pReq->bMirrorY,
					pReq->fZRotate,
					pReq->btTransparency,
					pReq->bUseKerning,
					pReq->bUnderline);
	}
	
}

/**
 * TODO:describtion
 *
 * @param uiIndex					TODO: describtion
 */
void IND_TTF_FontManager::removeText(uint32_t uiIndex) {
	DTRListIterator it = _DTRList.find(uiIndex);
	if(it != _DTRList.end()) {
		DrawTextRequestNode *pNewReq = it->second;
		_DTRList.erase(it);
		delete pNewReq;
	}
}

// --------------------------------------------------------------------------------
//									Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

void IND_TTF_FontManager::doDrawText(const std::string& strFontName,const std::wstring& s, float x, float y,
									  uint32_t clrFont,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
									  bool bKerning, bool bUnderl) {
	IND_TTF_Font *pFont = getFontByName(strFontName);
	if(pFont)
		pFont->drawText(s,x,y,clrFont, bFlipX,bFlipY,fZRotate,btTrans,bKerning,bUnderl);
}

int IND_TTF_FontManager::doDrawTextEx(const std::string& strFontName,const std::wstring& sText,
									float fLeft, float fTop, float fRight, float fBottom, 
									uint32_t nFormat, uint32_t clrFont, uint32_t clrBorder, uint32_t clrBack,
									byte btBorderTrans, byte btBackTrans, bool bFlipX, bool bFlipY, 
									float fZRotate, byte btTrans, bool bKerning, bool bUnderl) {
	IND_TTF_Font *pFont = getFontByName(strFontName);
	if(pFont)
		pFont->drawTextEx(	sText, fLeft, fTop, fRight, fBottom, nFormat, clrFont, clrBorder,
							clrBack,btBorderTrans, btBackTrans,
							bFlipX,bFlipY,fZRotate,btTrans,bKerning,bUnderl);

	return 0;
}

/** @endcond */