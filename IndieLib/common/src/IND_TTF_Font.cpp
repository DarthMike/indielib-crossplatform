/*****************************************************************************************
 * File: IND_TTF_Font.cpp
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

#include "IND_TTF_Font.h"

#include "freetype\ttunpat.h"
#include "freetype\ftoutln.h"

IND_TTF_Font::IND_TTF_Font(FT_Library ftlib, IND_Render *pIndieRender, IND_ImageManager *pIndieImageManager, 
							IND_SurfaceManager *pIndieSurfaceManager)
:m_FTLib(ftlib),
m_pIndieRender(pIndieRender),
m_pIndieImageManager(pIndieImageManager),
m_pIndieSurfaceManager(pIndieSurfaceManager),
m_Face(NULL),
m_CharWidth(20),
m_CharHeight(20),
m_bAutoCache(true),
m_bHasKerning(false),
m_fXScale(1.0f),
m_fYScale(1.0f),
m_fXHotSpot(0.5f),
m_fYHotSpot(0.5f),
m_bBold(false),
m_bItalic(false)
{
	m_matItalic.xx = 1 << 16;
	m_matItalic.xy = 0x5800;
	m_matItalic.yx = 0;
	m_matItalic.yy = 1 << 16;
}

IND_TTF_Font::~IND_TTF_Font(void)
{
	UnloadFont();
}

bool IND_TTF_Font::LoadTTFFontFromDisk(const std::string& strname, const std::string& strpath, 
										int iSize, bool bBold, bool bItalic)
{
	UnloadFont();

	//create new face
	if (FT_New_Face(m_FTLib, strpath.c_str(), 0, &m_Face) != 0)
		return false;

	if (!m_Face->charmap || !FT_IS_SCALABLE(m_Face))
	{
		FT_Done_Face(m_Face);
		return false;
	}

	m_strFilePath = strpath;
	if (FT_HAS_KERNING(m_Face))
		m_bHasKerning = true;

	m_CharWidth	= iSize;
	m_CharHeight = iSize;

	if (FT_Set_Pixel_Sizes(m_Face, m_CharWidth, m_CharHeight) != 0)
		return false;

    m_fFaceAscender = m_Face->ascender * m_Face->size->metrics.y_scale * float(1.0/64.0) * (1.0f/65536.0f);

	m_bBold = bBold;
	m_bItalic = bItalic;
	
	return true;
}

void IND_TTF_Font::UnloadFont(void)
{
	ClearAllCache();

	if (m_Face)
	{
		FT_Done_Face(m_Face);
		m_Face = NULL;
	}
}


bool IND_TTF_Font::BuildStringCache(const std::wstring& str)
{
	bool bRet = true;

	for (std::size_t i = 0; i < str.length(); i++)
	{
		if (!_BuildCharCache(str[i]))
			bRet = false;
	}
	return bRet;
}

bool IND_TTF_Font::IsCharCached(wchar_t charCode)
{
	return _GetCharCacheNode(charCode) != NULL;
}

void IND_TTF_Font::ClearAllCache(void)
{
	while (!m_FontCharCache.empty())
	{
		CharCacheNode* pNode = m_FontCharCache.begin()->second;
		m_FontCharCache.erase(m_FontCharCache.begin());
		
		// delete surface object firstly
		if(pNode->pSurface)
		{
			m_pIndieSurfaceManager->Delete(pNode->pSurface);
			delete pNode->pSurface;
		}
				
		delete pNode;
	}
}

///////////////////
bool IND_TTF_Font::DrawText(const std::wstring& s, float x, float y, uint32_t clrFont, bool bFlipX, bool bFlipY, 
							float fZRotate, byte btTrans, bool bKerning, bool bUnderl)
{
	bool Ret = true;
	float penX = x, penY = y;
	uint32_t previousGlyph = 0;
	FT_Vector Delta;
	CharCacheNode* pNode = NULL;

	int nSpace = _GetSpaceAdvance();

	//std::size_t Length = s.length();
	float original_Pen_x = x;

	for (std::size_t i = 0; i < s.length(); ++i)
	{
		//Special cases
		switch (s[i])
		{
		case L' ':
			penX += nSpace;	
			previousGlyph = 0; 
			continue;
		case L'\t': 
			penX += nSpace * nTabSize;	
			previousGlyph = 0; 
			continue;
		case L'\n':	
			// Draw underline
			if(bUnderl)
			{
				_DoDrawBorder(original_Pen_x, penX, penY + m_CharHeight, clrFont, btTrans);
			}
			penY += m_CharHeight; 
			penX = x;	
			original_Pen_x = x;
			previousGlyph = 0; 
			continue;
		}

		if (m_bAutoCache)
			_BuildCharCache(s[i]);

		pNode = _GetCharCacheNode(s[i]);
		if (!pNode)
		{
			Ret = false;
			previousGlyph = 0;
			continue;
		}
		//Kerning
		if (previousGlyph != 0 && m_bHasKerning && bKerning && !bFlipX && !bFlipY && fZRotate == 0)
		{
			FT_Get_Kerning(m_Face, previousGlyph, pNode->charGlyphIndex, FT_KERNING_DEFAULT, &Delta);
			penX += Delta.x >> 6;
			penY += Delta.y >> 6;
		}
		if (!_RenderChar(s[i], penX, penY, clrFont, bFlipX, bFlipY, fZRotate, btTrans, bKerning, bUnderl))
			Ret = false;

		penX += pNode->charAdvance;
		previousGlyph = pNode->charGlyphIndex;
	}

	// Draw underline
	if(bUnderl && ((penX - original_Pen_x) > 0.1f))
	{
		_DoDrawBorder(original_Pen_x, penX, penY + m_CharHeight, clrFont, btTrans);
	}
	return Ret;
}


// return value
// 1	-	ok
// 0	-	failed
// -1	-	too small to draw
// -2	-	format invalid
// -3	-	vertical layout is not supported by current font face
int IND_TTF_Font::DrawTextEx(const std::wstring& sText, float fLeft, float fTop, float fRight, float fBottom, 
					uint32_t nFormat, uint32_t clrFont, uint32_t clrBorder, uint32_t clrBack,byte btBorderTrans, 
					byte btBackTrans,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans,bool bKerning, bool bUnderl)
{
	//1. Check parameters
	if(fLeft >= fRight || fTop >= fBottom)
		return 0;
	float fAreaWidth = fRight - fLeft;
	float fAreaHeight = fBottom - fTop;

	if(fAreaWidth < m_CharWidth || fAreaHeight < m_CharHeight)
	{//
		return -1;
	}

	//the format must contain one para for horizontal align and one for vertical
	if( !(nFormat & (DT_EX_LEFT | DT_EX_CENTER | DT_EX_RIGHT)) ||
		!(nFormat & (DT_EX_TOP | DT_EX_VCENTER | DT_EX_BOTTOM)))
		return -2;
	
	//2. Display background if reauired
	if(nFormat & DT_EX_BACKCOLOR)
	{
		byte r,g,b;
		r = clrBack & 0xFF;
		g = (clrBack >> 8) & 0xFF;
		b = (clrBack >> 16) & 0xFF;
		m_pIndieRender->BlitFillRectangle(
											(int)fLeft, 
											(int)fTop,
											(int)fRight,
											(int)fBottom,
											r,g,b,btBackTrans);
	}


	bool bVertical = false;
	//3. check vertical layout
	if(nFormat & DT_EX_VERTICAL)
	{	// vertical layout 
		// mainly for you guys speaking Chinese, Japanese and Korean
		if(!FT_HAS_VERTICAL(m_Face))
		{// ooops, font face doesn't support vertical layout
			return -3;
		}
		bVertical = true;
	}
	
	// 4. check the right to left property
	bool bR2L = false;
	if(nFormat & DT_EX_RTOLREADING)
	{	// right to left reading 
		// mainly for you guys speaking Arabic
		bR2L = true;
	}

	// 5. check the line wrap property
	bool bWrap = false;
	if(nFormat & DT_EX_LINEWRAP)
	{	// do the line wrap 
		bWrap = true;
	}
	
	// 6. text format
	float fTextWidth, fTextHeight;
	int iTotalLines;
	std::wstring sTarget = _TextFormat( sText, bVertical?fAreaHeight:fAreaWidth, fTextWidth, fTextHeight,
										iTotalLines, bWrap,bFlipX, bFlipY, fZRotate, btTrans, bKerning,
										bUnderl);
	// determin the proper start point
	float pen_X, pen_Y, start_X, start_Y;
	start_X = fLeft;
	start_Y = fTop;
	if(bVertical)
	{	
		if(nFormat & DT_EX_LEFT)
		{
			if(bR2L)
			{
				start_X = fLeft + fTextHeight;
			}
		}
		else if(nFormat & DT_EX_CENTER)
		{
			if(bR2L)
			{
				start_X = fRight - (fAreaWidth - fTextHeight) / 2;
			}
			else
			{
				start_X = fLeft + (fAreaWidth - fTextHeight) / 2;
			}
		}
		else if(nFormat & DT_EX_RIGHT)
		{
			if(bR2L)
			{
				start_X = fRight;// - (fAreaWidth - fTextHeight);
			}
			else
			{
				start_X = fRight - fTextHeight;
			}
		}	
	}
	else
	{
		if(nFormat & DT_EX_VCENTER)
		{
			start_Y = fTop  + (fAreaHeight - fTextHeight) / 2;
		}
		else if(nFormat & DT_EX_BOTTOM)
		{
			start_Y = fBottom  - fTextHeight;
		}
	}
	// 7. draw the string content
	std::size_t start = 0, end = 0, line = 0;
	std::wstring curline;

	while ( end < sTarget.length())
	{
		end = sTarget.find_first_of(L'\n', start);
		if(end == std::wstring::npos)
		{// no '\n found'
			end = sTarget.length();
		}
		curline = sTarget.substr(start, end - start);
        start = end + 1;
		
		if(bVertical)
		{
			if(bR2L)
			{
				pen_X = start_X - m_CharWidth * line;
			}
			else
			{
				pen_X = start_X + m_CharWidth * line;
			}

			if(nFormat & DT_EX_TOP)
			{
				pen_Y = fTop;
			}
			else if(nFormat & DT_EX_VCENTER)
			{
				pen_Y = fTop  + (fAreaHeight - _GetLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning)) / 2;
			}
			else if(nFormat & DT_EX_BOTTOM)
			{
				pen_Y = fTop  + fAreaHeight - _GetLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning);
			}
		}
		else
		{
			pen_X = fLeft;
			pen_Y = start_Y + (float)(m_CharHeight * line);

			if(nFormat & DT_EX_LEFT)
			{
				if(bR2L)
				{
					pen_X = fRight - (fAreaWidth - _GetLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning));
				}
				else
				{
					pen_X = fLeft;
				}
			}
			else if(nFormat & DT_EX_CENTER)
			{
				if(bR2L)
				{
					pen_X = fRight - (fAreaWidth - _GetLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning)) / 2;
				}
				else
				{
					pen_X = fLeft + (fAreaWidth - _GetLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning)) / 2;
				}
			}
			else if(nFormat & DT_EX_RIGHT)
			{
				if(bR2L)
				{
					pen_X = fRight - (fAreaWidth - _GetLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning));
				}
				else
				{
					pen_X = fLeft + fAreaWidth - _GetLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning);
				}
			}	
		}

		// draw this line
		_DrawTextLineEx(curline, pen_X, pen_Y, fLeft, fTop, fRight, fBottom, clrFont, bVertical, bR2L, bFlipX, 
						bFlipY, fZRotate, btTrans, bKerning, bUnderl);
		line++;
	}

	//8. Display border if reauired
	if(nFormat & DT_EX_BORDER)
	{
		byte r,g,b;
		r = clrBorder & 0xFF;
		g = (clrBorder >> 8) & 0xFF;
		b = (clrBorder >> 16) & 0xFF;
		m_pIndieRender->BlitRectangle(
											(int)(fLeft - 1), 
											(int)(fTop - 1),
											(int)(fRight + 1),
											(int)(fBottom + 1),
											r,g,b,btBorderTrans);
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

bool IND_TTF_Font::_RenderChar(wchar_t charCode, float x, float y, uint32_t clrFont, bool bFlipX, bool bFlipY, 
							   float fZRotate, byte btTrans, bool bKerning, bool bUnderl)
{
	if (m_bAutoCache)
		_BuildCharCache(charCode);

	CharCacheNode* pNode = _GetCharCacheNode(charCode);
	if (!pNode || !pNode->pSurface)
		return false;

	float origin_x, origin_y;
	origin_x = x;
	origin_y = y;

	x += pNode->charLeftBearing;
	y += m_fFaceAscender - pNode->charTopBearing;
	
	//Bliting the font surfaces to screen
	// 1) We apply the world space transformation (translation, rotation, scaling).
	// If you want to recieve the transformation in a single matrix you can pass
	// and IND_Matrix object by reference.

	int mWidth = pNode->pSurface->GetWidth();
	int mHeight = pNode->pSurface->GetHeight();

	/*
	if( (x + mWidth  <= 0) || (y + mHeight <= 0) ||
		(x > m_pIndieRender->GetWindow()->GetWidth()) || (y > m_pIndieRender->GetWindow()->GetHeight()))
		return false;//not visible, just drop it
	*/
	//
	IND_Matrix mMatrix;
	// We want the start position (x,y) to be the top left corner 
	m_pIndieRender->SetTransform2d(
									(int)(x + m_fXHotSpot * mWidth),		// x pos
									(int)(y + m_fYHotSpot * mHeight),		// y pos
									0,					// Angle x	
									0,					// Angle y
									fZRotate,			// Angle z
									m_fXScale,			// Scale x
									m_fYScale,			// Scale y
									(int) (m_fXHotSpot * mWidth * -1),		// Axis cal x
									(int) (m_fYHotSpot * mHeight * -1),		// Axis cal y
									bFlipX,					// Mirror x
									bFlipY,					// Mirror y
									mWidth,				// Width
									mHeight,			// Height
									&mMatrix);			// Matrix in wich the transformation will be applied (optional)			
 
	//2) We apply the color, blending and culling transformations.
	//modified by Joel Gao Mar,4th 2009
	byte r,g,b;
	r = clrFont & 0xFF;
	g = (clrFont >> 8) & 0xFF;
	b = (clrFont >> 16) & 0xFF;

	m_pIndieRender->SetRainbow2d(
									IND_ALPHA,			// IND_Type
									1,					// Back face culling 0/1 => off / on
									bFlipX,					// Mirror x
									bFlipY,					// Mirror y
									IND_FILTER_LINEAR,	// IND_Filter
									r,				// R Component	for tinting
									g,				// G Component	for tinting
									b,				// B Component	for tinting			
									btTrans,				// A Component	for tinting
									0,					// R Component	for fading to a color		
									0,					// G Component	for fading to a color		
									0,					// B Component	for fading to a color			
									255,//btTrans,				// Amount of fading	
									IND_SRCALPHA,		// IND_BlendingType (source)
									IND_INVSRCALPHA);	// IND_BlendingType (destination)
	//


	// 3) Blit the IND_Surface
	//m_pIndieLib->Render->BlitRegionSurface(theGlyph->pSurface, (int)x, (int)y, mWidth,mHeight);
	m_pIndieRender->BlitSurface(pNode->pSurface);
	
	return true;
}


bool IND_TTF_Font::_BuildCharCache(wchar_t charCode)
{
	if (IsCharCached(charCode))
		return true;

	CharCacheNode* pNode = new CharCacheNode;
	//////////
	//init the struct pointers
	pNode->pSurface = NULL;
	//////////
	pNode->charCode = charCode;
	pNode->charGlyphIndex = FT_Get_Char_Index(m_Face, charCode);

	if (pNode->charGlyphIndex == 0)
	{
		delete pNode;
		return false;
	}

	FT_Load_Char(m_Face, charCode, FT_LOAD_DEFAULT /*| FT_LOAD_NO_BITMAP*/);

	//
	if(m_bBold)
	{
		int strength = 1 << 6;
		FT_Outline_Embolden(&m_Face->glyph->outline, strength);

	}
	//
	if(m_bItalic)
	{
		// set transformation 
		FT_Outline_Transform(&m_Face->glyph->outline, &m_matItalic);
	}
	//

	if(FT_Render_Glyph(m_Face->glyph, FT_RENDER_MODE_NORMAL))
	{
		delete pNode;
		return false;
	}

	// build an image
	IND_Image *pImage = new IND_Image;
	_ASSERT(pImage);

	// render the glyph image to IND_Image
	if(!_RenderGlyph(&m_Face->glyph->bitmap, pImage))
		return false;
		
	//building the surface from image
	pNode->pSurface = new IND_Surface;

	bool bOK = true;
	if (!m_pIndieSurfaceManager->Add (pNode->pSurface, pImage, IND_ALPHA, IND_32)) 
	{
		bOK = false;
	}
	// free the image
	m_pIndieImageManager->Delete(pImage);
	delete pImage;

	if(!bOK)
	{
		delete pNode;
		return false;
	}

	pNode->charLeftBearing = m_Face->glyph->bitmap_left;
	pNode->charTopBearing = m_Face->glyph->bitmap_top;
	pNode->charAdvance = m_Face->glyph->advance.x / 64;
	
	m_FontCharCache.insert(std::pair<wchar_t, CharCacheNode*>(charCode, pNode));

	//cache entry built
	return true;
}

IND_TTF_Font::CharCacheNode* IND_TTF_Font::_GetCharCacheNode(wchar_t charCode)
{
	CharCacheMapIterator it = m_FontCharCache.find(charCode);
	if(it == m_FontCharCache.end())
		return NULL;
	else
		return it->second;
}

bool IND_TTF_Font::_RenderGlyph(FT_Bitmap* ftBMP, IND_Image *pImage)
{
	uint32_t glyphWidth = ftBMP->width;
	uint32_t glyphHeight = ftBMP->rows;

	if (glyphWidth == 0 || glyphHeight == 0)
		return false;

	m_pIndieImageManager->Add(pImage, glyphWidth, glyphHeight, IND_RGBA);
	if(pImage == NULL)
		return false;

	byte *pSrc = ftBMP->buffer;
	/*
	byte r,g,b;
	r = m_FontColor & 0xFF;
	g = (m_FontColor >> 8) & 0xFF;
	b = (m_FontColor >> 16) & 0xFF;
	*/

	for(uint32_t x = 0 ; x <  glyphWidth; x++)
	{
		for(uint32_t y = 0 ; y <  glyphHeight; y++)
		{
			switch (ftBMP->pixel_mode)
			{
				case FT_PIXEL_MODE_GRAY:
					//pImage->PutPixel(x, y, r,g,b,pSrc[y * glyphWidth + x]);
					pImage->PutPixel(x, y, 255,255,255,pSrc[y * glyphWidth + x]);
					break;
				case FT_PIXEL_MODE_MONO:
					pSrc = ftBMP->buffer + (y * ftBMP->pitch);
					if((pSrc [x / 8] & (0x80 >> (x & 7))))
						//pImage->PutPixel(x, y, r,g,b,0xFF);
						pImage->PutPixel(x, y, 255,255,255,0xFF);
					else
						//pImage->PutPixel(x, y, r,g,b,0x00);
						pImage->PutPixel(x, y, 255,255,255,0x00);
					break;
				default:
					break;
			}
		}
	}
	
	//Test effects here
	//pImage->Pixelize(2);
	//
	return true;
}

IND_TTF_Font::uint32_t IND_TTF_Font::_GetSpaceAdvance()
{
	//We use the advance value of 'A' as the space value
	_BuildCharCache(L'A');
	CharCacheNode* pNode = _GetCharCacheNode(L'A');
	if(pNode)
		return pNode->charAdvance;
	else// no 'A' in this face
		return m_CharWidth / 2;
}

std::wstring IND_TTF_Font::_TextFormat(	const std::wstring& sText, float &fLineWidth, float &fTotalWidth, float &fTotalHeight,
										int &iTotalLineNum, bool bLineWrap,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
										bool bKerning, bool bUnderl)
{
	fTotalWidth = 0.0f;
	fTotalHeight = 0.0f;
	iTotalLineNum = 0;

	CharCacheNode* pNode = NULL;
	//For Kerning
	FT_Vector Delta;
	uint32_t previousGlyph = 0;
	//

	float fCurrentLineWidth = 0.0f;

	uint32_t nSpace = _GetSpaceAdvance();

	std::wstring sRet = L"";
	std::size_t nLength = sText.length();

	for (std::size_t i = 0; i < nLength; i++)
	{
		switch (sText[i])
		{
		case L' ':
			if(bLineWrap && (fCurrentLineWidth + nSpace > fLineWidth))
			{// next line
				sRet += L'\n';
				
				iTotalLineNum++;
				if(fCurrentLineWidth > fTotalWidth)
					fTotalWidth = fCurrentLineWidth;

				fCurrentLineWidth = 0.0f;
			}
			sRet += sText[i];	
			fCurrentLineWidth += nSpace;
			previousGlyph = 0; 
			continue;
		case L'\t': 
			if(bLineWrap && (fCurrentLineWidth + nSpace * nTabSize > fLineWidth))
			{// next line
				sRet += L'\n';
				
				iTotalLineNum++;
				if(fCurrentLineWidth > fTotalWidth)
					fTotalWidth = fCurrentLineWidth;

				fCurrentLineWidth = 0.0f;
			}
			sRet += sText[i];
			fCurrentLineWidth += nSpace * nTabSize;	
			previousGlyph = 0; 
			continue;
		case L'\n':		
			if(fCurrentLineWidth > fTotalWidth)
				fTotalWidth = fCurrentLineWidth;

			sRet += sText[i];
			iTotalLineNum++;
			fCurrentLineWidth = 0.0f;

			previousGlyph = 0; 
			continue;
		}
		if (m_bAutoCache)
			_BuildCharCache(sText[i]);

		pNode = _GetCharCacheNode(sText[i]);
		if (!pNode)
		{
			previousGlyph = 0;
			continue;
		}
		if (previousGlyph != 0 && m_bHasKerning && bKerning && !bFlipX && !bFlipY && fZRotate == 0)
		{
			FT_Get_Kerning(m_Face, previousGlyph, pNode->charGlyphIndex, FT_KERNING_DEFAULT, &Delta);
			fCurrentLineWidth += Delta.x >> 6;
		}

		if(bLineWrap && (fCurrentLineWidth + pNode->charAdvance > fLineWidth))
		{// next line
			sRet += L'\n';
				
			iTotalLineNum++;
			if(fCurrentLineWidth > fTotalWidth)
				fTotalWidth = fCurrentLineWidth;

			fCurrentLineWidth = 0.0f;
		}

		sRet += sText[i];	
		fCurrentLineWidth += pNode->charAdvance;

		previousGlyph = pNode->charGlyphIndex;
	}

	// last line
	if(fCurrentLineWidth > 0)
		iTotalLineNum++;

	fTotalHeight = (float)(m_CharHeight * iTotalLineNum);
	return sRet;
}

IND_TTF_Font::uint32_t IND_TTF_Font::_GetLineWidth(const std::wstring& sText, bool bFlipX, bool bFlipY, 
												   float fZRotate, bool bKerning)
{
	CharCacheNode* pNode = NULL;
	//For Kerning
	FT_Vector Delta;
	uint32_t previousGlyph = 0;
	//
	uint32_t nSpace = _GetSpaceAdvance();
	uint32_t nRet = 0;
	std::size_t nLength = sText.length();

	for (std::size_t i = 0; i < nLength; i++)
	{
		switch (sText[i])
		{
		case L' ':	
			nRet += nSpace;
			previousGlyph = 0; 
			continue;
		case L'\t': 
			nRet += nSpace * nTabSize;	
			previousGlyph = 0; 
			continue;
		}

		pNode = _GetCharCacheNode(sText[i]);
		if (!pNode)
		{
			previousGlyph = 0;
			continue;
		}
		if (previousGlyph != 0 && m_bHasKerning  && bKerning && !bFlipX && !bFlipY && fZRotate == 0)
		{
			FT_Get_Kerning(m_Face, previousGlyph, pNode->charGlyphIndex, FT_KERNING_DEFAULT, &Delta);
			nRet += Delta.x >> 6;
		}

		nRet += pNode->charAdvance;

		previousGlyph = pNode->charGlyphIndex;
	}
	return nRet;
}

void IND_TTF_Font::_DrawTextLineEx(const std::wstring& sText, float penX, float penY, float fL, float fT,
							  float fR, float fB, uint32_t clrFont,bool bVertical, bool bR2L,bool bFlipX, 
							  bool bFlipY, float fZRotate, byte btTrans, bool bKerning, bool bUnderl)
{
	uint32_t previousGlyph = 0;
	FT_Vector Delta;

	CharCacheNode* pNode = NULL;
	std::size_t nLength = sText.length();

	uint32_t nSpace = _GetSpaceAdvance();

	float original_Pen_x = penX;

	for (std::size_t i = 0; i < nLength; i++)
	{
		//Special cases
		switch (sText[i])
		{
		case L' ':
			if(bVertical)
			{
				penY += nSpace;
			}
			else
			{
				if(bR2L)
					penX -= nSpace;
				else
					penX += nSpace;
			}
			
			previousGlyph = 0; 
			continue;
		case L'\t': 
			if(bVertical)
			{
				penY += nSpace * nTabSize;
			}
			else
			{
				if(bR2L)
					penX -= nSpace * nTabSize;
				else
					penX += nSpace * nTabSize;
			}
			previousGlyph = 0; 
			continue;
		}

		pNode = _GetCharCacheNode(sText[i]);
		if (!pNode)
		{
			previousGlyph = 0;
			continue;
		}
		//Kerning
		if (!bR2L && previousGlyph != 0 && m_bHasKerning  && bKerning && !bFlipX && !bFlipY && fZRotate == 0)
		{
			FT_Get_Kerning(m_Face, previousGlyph, pNode->charGlyphIndex, FT_KERNING_DEFAULT, &Delta);
			penX += Delta.x >> 6;
			penY += Delta.y >> 6;
		}
		

		//bool bDraw = false;
		if(bVertical)
		{
			if(bR2L)
			{
				if(	((penX - m_CharWidth) >= fL) && 
					(penX <= fR) &&
					(penY >= fT ) && 
					((penY + pNode->charAdvance) <= fB))
					_RenderChar(sText[i], penX - m_CharWidth, penY, clrFont, bFlipX, bFlipY, fZRotate, btTrans,
								bKerning, bUnderl);
			}
			else
			{
				if(	(penX >= fL) && 
					((penX + m_CharWidth) <= fR) &&
					(penY >= fT ) && 
					((penY + pNode->charAdvance) <= fB))
					_RenderChar(sText[i], penX, penY, clrFont, bFlipX, bFlipY, fZRotate, btTrans,
								bKerning, bUnderl);
			}
		}
		else
		{
			if(bR2L)
			{
				if(	((penX - pNode->charAdvance) >= fL) && 
					(penX <= fR) &&
					(penY >= fT ) && 
					((penY + m_CharHeight) <= fB))
					_RenderChar(sText[i], penX - pNode->charAdvance, penY, clrFont, bFlipX, bFlipY, fZRotate, btTrans,
								bKerning, bUnderl);
			}
			else
			{
				if(	(penX >= fL) && 
					((penX + pNode->charAdvance) <= fR) &&
					(penY >= fT ) && 
					((penY + m_CharHeight) <= fB))
					_RenderChar(sText[i], penX, penY, clrFont, bFlipX, bFlipY, fZRotate, btTrans,
								bKerning, bUnderl);
			}
		}		

		if(bVertical)
		{
			penY += pNode->charAdvance;
		}
		else
		{
			if(bR2L)
				penX -= pNode->charAdvance;
			else
				penX += pNode->charAdvance;
		}
		previousGlyph = pNode->charGlyphIndex;
	}
	// Draw underline
	if(bUnderl && !bVertical && ((penX - original_Pen_x) > 0.1f))
	{
		_DoDrawBorder(original_Pen_x, penX, penY + m_CharHeight, clrFont, btTrans);
	}
}

void IND_TTF_Font::_DoDrawBorder(float fX_s, float fX_e, float fY, uint32_t clr, byte btTrans)
{
	byte r,g,b;
	r = clr & 0xFF;
	g = (clr >> 8) & 0xFF;
	b = (clr >> 16) & 0xFF;
	m_pIndieRender->BlitLine((int)(fX_s), 
							(int)(fY),
							(int)(fX_e),
							(int)(fY),
							r,g,b,btTrans);
}
