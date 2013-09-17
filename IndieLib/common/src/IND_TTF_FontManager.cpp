/*****************************************************************************************
 * File: IND_TTF_FontManager.cpp
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

#include "Global.h"
#include "IND_TTF_FontManager.h"
#include <assert.h>

//#include <ft2build.h>
//#include FT_FREETYPE_H

IND_TTF_FontManager::IND_TTF_FontManager(void)
: m_bInit(false), 
m_pIndieRender(NULL),
m_pIndieImageManager(NULL),
m_pIndieSurfaceManager(NULL)
{
}

IND_TTF_FontManager::~IND_TTF_FontManager(void)
{
	End();
}

bool IND_TTF_FontManager::Init(IND_Render *pRender, IND_ImageManager *pImageManager, IND_SurfaceManager *pSurfaceManager)
{
    
    g_debug->header("Initializing TTF FontManager", DebugApi::LogHeaderBegin);
    
	// Checking IND_Render
	if (pRender->isOK()) {
		g_debug->header("Checking IND_Render", DebugApi::LogHeaderOk);
		m_pIndieRender = pRender;
	}
    else {
		g_debug->header("IND_Render is not correctly initialized", DebugApi::LogHeaderError);
		m_bInit = false;
		return m_bInit;
	}
    
    
	// Checking IND_ImageManager
	if (pImageManager->isOK()) {
		g_debug->header("Checking IND_ImageManager", DebugApi::LogHeaderOk);
		m_pIndieImageManager = pImageManager;
    }
    else {
		g_debug->header("IND_ImageManager is not correctly initialized", DebugApi::LogHeaderError);
		m_bInit = false;
		return m_bInit;
	}

    // Checking IND_SurfaceManager
	if (pSurfaceManager->isOK()) {
		g_debug->header("Checking IND_SurfaceManager", DebugApi::LogHeaderOk);
		m_pIndieSurfaceManager = pSurfaceManager;
    }
    else {
		g_debug->header("IND_SurfaceManager is not correctly initialized", DebugApi::LogHeaderError);
		m_bInit = false;
		return m_bInit;
	}

    
	if(m_bInit)
		return true;

	if(FT_Init_FreeType(&m_FTLib) != 0)
		m_bInit = false;
	else
		m_bInit = true;

	return m_bInit;
}

bool IND_TTF_FontManager::AddFont(const std::string& strName, const std::string& strPath, int iSize, bool bBold, bool bItalic)
{
	if(iSize < 5)
		return false; // too small

	if (!m_bInit)
		return false;

	if (IsFontLoaded(strName))
		return true;

	IND_TTF_Font* ptrNewFont = new IND_TTF_Font(m_FTLib, m_pIndieRender, m_pIndieImageManager, m_pIndieSurfaceManager);
	if (!ptrNewFont->LoadTTFFontFromDisk(strName, strPath, iSize, bBold, bItalic))
	{
		delete ptrNewFont;
		return false;
	}
	m_FontList[strName] = ptrNewFont;
	return true;
}

bool IND_TTF_FontManager::IsFontLoaded(const std::string& strName)
{
	return GetFontByName(strName) != NULL;
}


void IND_TTF_FontManager::End(void)
{
	if (!m_bInit)
		return;

	//free up
	while (!m_DTRList.empty())
	{
		DrawTextRequestNode* pNode = m_DTRList.begin()->second;
		m_DTRList.erase(m_DTRList.begin());
				
		delete pNode;
	}

	while (!m_FontList.empty())
	{
		IND_TTF_Font* pFont = m_FontList.begin()->second;
		m_FontList.erase(m_FontList.begin());
		pFont->UnloadFont();
		delete pFont;
	}

	if (m_bInit)
	{
		FT_Done_FreeType(m_FTLib);
		m_bInit = false;
	}
}

void IND_TTF_FontManager::UnloadFont(const std::string& strName)
{
	if (!m_bInit)
		return;

	IND_TTF_FontListIterator itFont = m_FontList.begin();
	for (; itFont != m_FontList.end(); ++itFont)
	{
		IND_TTF_Font* pFont = itFont->second;
		if (pFont->GetFontName() == strName)
		{
			m_FontList.erase(itFont);
			pFont->UnloadFont();
			delete pFont;
		}
	}
}

IND_TTF_Font* IND_TTF_FontManager::GetFontByName(const std::string& strName)
{
	if (!m_bInit)
		return NULL;

	IND_TTF_FontListIterator it = m_FontList.find(strName);
	if(it == m_FontList.end())
		return NULL;
	else
		return it->second;
}

void IND_TTF_FontManager::DrawText(	uint32_t uiIndex, const std::string strFontName, float x, float y, 
									uint32_t clrFont,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
									bool bKerning, bool bUnderl, const wchar_t* format, ...)
{
	va_list ArgPtr;

	va_start(ArgPtr, format);
	std::size_t Length = vwprintf(format, ArgPtr) + 1;
	std::wstring m_WBuffer;
	m_WBuffer.resize(Length);
	vswprintf(&m_WBuffer[0], Length, format, ArgPtr);
		
	va_end(ArgPtr);

	DrawText(uiIndex, strFontName, m_WBuffer, x, y, clrFont, bFlipX, bFlipY, fZRotate, btTrans, bKerning,bUnderl);
}

void IND_TTF_FontManager::DrawText(uint32_t uiIndex, const std::string strFontName,const std::wstring s, 
								   float x, float y, uint32_t clrFont,bool bFlipX, bool bFlipY, float fZRotate, 
								   byte btTrans, bool bKerning, bool bUnderl)
{
	DrawTextRequestNode *pNewReq = NULL;

	DTRListIterator it = m_DTRList.find(uiIndex);
	if(it == m_DTRList.end())
	{
		pNewReq = new DrawTextRequestNode;
		assert(pNewReq);
	}
	else
	{
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
	

	if(it == m_DTRList.end())
		m_DTRList.insert(std::pair<uint32_t, DrawTextRequestNode*>(uiIndex, pNewReq));
}
/*
void IND_TTF_FontManager::SetFontColor(const std::string& strFontName, unsigned int uiClr)
{
	IND_TTF_Font *pFont = GetFontByName(strFontName);
	if(pFont)
		pFont->SetColor(uiClr);
}
*/

bool IND_TTF_FontManager::CacheFontString(const std::string& strFontName, const std::wstring& s)
{
	IND_TTF_Font *pFont = GetFontByName(strFontName);
	if(pFont)
		return pFont->BuildStringCache(s);

	return false;
}

void IND_TTF_FontManager::SetFontAutoCache(const std::string& strFontName, bool ba)
{
	IND_TTF_Font *pFont = GetFontByName(strFontName);
	if(pFont)
		pFont->SetAutoCache(ba);
}

void IND_TTF_FontManager::SetFontHotSpot(const std::string& strFontName, float hsx, float hsy)
{
	IND_TTF_Font *pFont = GetFontByName(strFontName);
	if(pFont)
	{
		pFont->SetXHotspot(hsx);
		pFont->SetYHotspot(hsy);
	}
}

void IND_TTF_FontManager::SetFontScale(const std::string& strFontName, float sx, float sy)
{
	IND_TTF_Font *pFont = GetFontByName(strFontName);
	if(pFont)
	{
		pFont->SetXScale(sx);
		pFont->SetYScale(sy);
	}
}

void IND_TTF_FontManager::DrawTextEx(uint32_t uiIndex, const std::string& strFontName,const std::wstring& sText, 
									float fLeft, float fTop, float fRight, float fBottom, 
									uint32_t nFormat, uint32_t clrFont,uint32_t clrBorder, uint32_t clrBack,
									byte btBorderTrans, byte btBackTrans, bool bFlipX, bool bFlipY, 
									float fZRotate, byte btTrans, bool bKerning, bool bUnderl)
{
	DrawTextRequestNode *pNewReq = NULL;

	DTRListIterator it = m_DTRList.find(uiIndex);
	if(it == m_DTRList.end())
	{
		pNewReq = new DrawTextRequestNode;
		assert(pNewReq);
	}
	else
	{
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
	

	if(it == m_DTRList.end())
		m_DTRList.insert(std::pair<uint32_t, DrawTextRequestNode*>(uiIndex, pNewReq));
}

void IND_TTF_FontManager::RenderAllTexts(void)
{
	// render simple text from DrawText method
	DrawTextRequestNode *pReq = NULL;
	for(DTRListIterator it = m_DTRList.begin() ; it != m_DTRList.end() ; it++)
	{
		pReq = it->second;
		if(pReq->bEx)
			_DoDrawTextEx(	pReq->sFont, pReq->sText, pReq->xPos, pReq->yPos,
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
			_DoDrawText(pReq->sFont, pReq->sText, pReq->xPos, pReq->yPos,
					pReq->clrFont, 
					pReq->bMirrorX,
					pReq->bMirrorY,
					pReq->fZRotate,
					pReq->btTransparency,
					pReq->bUseKerning,
					pReq->bUnderline);
	}
	
}

void IND_TTF_FontManager::RemoveText(uint32_t uiIndex)
{
	DTRListIterator it = m_DTRList.find(uiIndex);
	if(it != m_DTRList.end())
	{
		DrawTextRequestNode *pNewReq = it->second;
		m_DTRList.erase(it);
		delete pNewReq;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

void IND_TTF_FontManager::_DoDrawText(const std::string& strFontName,const std::wstring& s, float x, float y,
									  uint32_t clrFont,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
									  bool bKerning, bool bUnderl)
{
	IND_TTF_Font *pFont = GetFontByName(strFontName);
	if(pFont)
		pFont->DrawText(s,x,y,clrFont, bFlipX,bFlipY,fZRotate,btTrans,bKerning,bUnderl);
}

int IND_TTF_FontManager::_DoDrawTextEx(const std::string& strFontName,const std::wstring& sText, 
									float fLeft, float fTop, float fRight, float fBottom, 
									uint32_t nFormat, uint32_t clrFont, uint32_t clrBorder, uint32_t clrBack,
									byte btBorderTrans, byte btBackTrans, bool bFlipX, bool bFlipY, 
									float fZRotate, byte btTrans, bool bKerning, bool bUnderl)
{
	IND_TTF_Font *pFont = GetFontByName(strFontName);
	if(pFont)
		pFont->DrawTextEx(	sText, fLeft, fTop, fRight, fBottom, nFormat, clrFont, clrBorder, 
							clrBack,btBorderTrans, btBackTrans,
							bFlipX,bFlipY,fZRotate,btTrans,bKerning,bUnderl);

	return 0;
}

