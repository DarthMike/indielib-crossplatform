/*****************************************************************************************
 * File: IND_TTF_Font.cpp
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


// ----- Includes -----

#include "IND_TTF_Font.h"
//#include "IND_TTF_FontManager.h"
#include "FreeTypeHandle.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

//! wrap of the TrueType library, so that the Indielib user does not need to include this.
class free_type_impl {
public:
    //! FreeType lib handle
    FT_Library				_FTLib;
    //! THIS font face
	FT_Face					_Face;
    //! Transformation matrix for italic
    FT_Matrix				_matItalic;

public:
    friend class IND_TTF_Font;
};


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

IND_TTF_Font::IND_TTF_Font( free_type_ptr_wrapped_impl *freetype_wrapped, IND_Render *pIndieRender,
                            IND_ImageManager *pIndieImageManager, IND_SurfaceManager *pIndieSurfaceManager) {
    
    _pIndieRender           = pIndieRender;
    _pIndieImageManager     = pIndieImageManager;
    _pIndieSurfaceManager   = pIndieSurfaceManager;
    _CharWidth              = 20;
    _CharHeight             = 20;
    _bAutoCache             = true;
    _bHasKerning            = false;
    _fXScale                = 1.0f;
    _fYScale                = 1.0f;
    _fXHotSpot              = 0.5f;
    _fYHotSpot              = 0.5f;
    _bBold                  = false;
    _bItalic                = false;

    _impl = new free_type_impl();               // TODO: remember to delete this
    _impl->_FTLib = freetype_wrapped->_FTLib;
    _impl->_Face = NULL;
	_impl->_matItalic.xx = 1 << 16;
	_impl->_matItalic.xy = 0x5800;
	_impl->_matItalic.yx = 0;
	_impl->_matItalic.yy = 1 << 16;
    
}

IND_TTF_Font::~IND_TTF_Font() {
	unloadFont();
}

// --------------------------------------------------------------------------------
//									Public methods
// --------------------------------------------------------------------------------

bool IND_TTF_Font::loadTTFFontFromDisk(const std::string& strname, const std::string& strpath,
										int iSize, bool bBold, bool bItalic) {
	unloadFont();

	//create new face
	if (FT_New_Face(_impl->_FTLib, strpath.c_str(), 0, &_impl->_Face) != 0)
		return false;

	if (!_impl->_Face->charmap || !FT_IS_SCALABLE(_impl->_Face)) {
		FT_Done_Face(_impl->_Face);
		return false;
	}

	_strFilePath = strpath;
	if (FT_HAS_KERNING(_impl->_Face))
		_bHasKerning = true;

	_CharWidth	= iSize;
	_CharHeight = iSize;

	if (FT_Set_Pixel_Sizes(_impl->_Face, _CharWidth, _CharHeight) != 0)
		return false;

    _fFaceAscender = _impl->_Face->ascender * _impl->_Face->size->metrics.y_scale * float(1.0/64.0) * (1.0f/65536.0f);

	_bBold = bBold;
	_bItalic = bItalic;
	
	return true;
}

void IND_TTF_Font::unloadFont() {
	clearAllCache();

	if (_impl->_Face) {
		FT_Done_Face(_impl->_Face);
		_impl->_Face = NULL;
	}
}


bool IND_TTF_Font::buildStringCache(const std::wstring& str) {
	bool bRet = true;

	for (std::size_t i = 0; i < str.length(); i++) {
		if (!buildCharCache(str[i]))
			bRet = false;
	}
	return bRet;
}

bool IND_TTF_Font::isCharCached(wchar_t charCode) {
	return getCharCacheNode(charCode) != NULL;
}

void IND_TTF_Font::clearAllCache() {
	while (!_FontCharCache.empty()) {
		CharCacheNode* pNode = _FontCharCache.begin()->second;
		_FontCharCache.erase(_FontCharCache.begin());
		
		// delete surface object firstly
		if(pNode->pSurface) {
			_pIndieSurfaceManager->remove(pNode->pSurface);
			//DISPOSEMANAGED(pNode->pSurface); // TODO: figure out why a diposemanaged gives an exeption, ... the surface needs to be deleted
		}
				
		delete pNode;
	}
}


bool IND_TTF_Font::drawText(const std::wstring& s, float x, float y, uint32_t clrFont, bool bFlipX, bool bFlipY,
							float fZRotate, byte btTrans, bool bKerning, bool bUnderl) {
	bool Ret = true;
	float penX = x, penY = y;
	uint32_t previousGlyph = 0;
	FT_Vector Delta;
	CharCacheNode* pNode = NULL;

	int nSpace = getSpaceAdvance();

	//std::size_t Length = s.length();
	float original_Pen_x = x;

	for (std::size_t i = 0; i < s.length(); ++i) {
		
        //Special cases
		switch (s[i]) {
		
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
			if(bUnderl) {
				doDrawBorder(original_Pen_x, penX, penY + _CharHeight, clrFont, btTrans);
			}
			penY += _CharHeight;
			penX = x;	
			original_Pen_x = x;
			previousGlyph = 0; 
			continue;
		}

		if (_bAutoCache)
			buildCharCache(s[i]);

		pNode = getCharCacheNode(s[i]);
		if (!pNode) {
			Ret = false;
			previousGlyph = 0;
			continue;
		}
		//Kerning
		if (previousGlyph != 0 && _bHasKerning && bKerning && !bFlipX && !bFlipY && fZRotate == 0) {
			FT_Get_Kerning(_impl->_Face, previousGlyph, pNode->charGlyphIndex, FT_KERNING_DEFAULT, &Delta);
			penX += Delta.x >> 6;
			penY += Delta.y >> 6;
		}
		if (!renderChar(s[i], penX, penY, clrFont, bFlipX, bFlipY, fZRotate, btTrans, bKerning, bUnderl))
			Ret = false;

		penX += pNode->charAdvance;
		previousGlyph = pNode->charGlyphIndex;
	}

	// Draw underline
	if(bUnderl && ((penX - original_Pen_x) > 0.1f)) {
		doDrawBorder(original_Pen_x, penX, penY + _CharHeight, clrFont, btTrans);
	}
	return Ret;
}


// return value
// 1	-	ok
// 0	-	failed
// -1	-	too small to draw
// -2	-	format invalid
// -3	-	vertical layout is not supported by current font face
int IND_TTF_Font::drawTextEx(const std::wstring& sText, float fLeft, float fTop, float fRight, float fBottom,
					uint32_t nFormat, uint32_t clrFont, uint32_t clrBorder, uint32_t clrBack,byte btBorderTrans, 
					byte btBackTrans,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans,bool bKerning, bool bUnderl) {
	//1. Check parameters
	if(fLeft >= fRight || fTop >= fBottom)
		return 0;
	float fAreaWidth = fRight - fLeft;
	float fAreaHeight = fBottom - fTop;

	if(fAreaWidth < _CharWidth || fAreaHeight < _CharHeight) {
		return -1;
	}

	//the format must contain one para for horizontal align and one for vertical
	if( !(nFormat & (DT_EX_LEFT | DT_EX_CENTER | DT_EX_RIGHT)) ||
		!(nFormat & (DT_EX_TOP | DT_EX_VCENTER | DT_EX_BOTTOM)))
		return -2;
	
	//2. Display background if reauired
	if(nFormat & DT_EX_BACKCOLOR) {
		byte r,g,b;
		r = clrBack & 0xFF;
		g = (clrBack >> 8) & 0xFF;
		b = (clrBack >> 16) & 0xFF;
		_pIndieRender->blitFillRectangle(
											(int)fLeft, 
											(int)fTop,
											(int)fRight,
											(int)fBottom,
											r,g,b,btBackTrans);
	}


	bool bVertical = false;
	//3. check vertical layout
	if(nFormat & DT_EX_VERTICAL) {
        // vertical layout
		// mainly for you guys speaking Chinese, Japanese and Korean
		if(!FT_HAS_VERTICAL(_impl->_Face)){
            // ooops, font face doesn't support vertical layout
			return -3;
		}
		bVertical = true;
	}
	
	// 4. check the right to left property
	bool bR2L = false;
	if(nFormat & DT_EX_RTOLREADING) {
        // right to left reading
		// mainly for you guys speaking Arabic
		bR2L = true;
	}

	// 5. check the line wrap property
	bool bWrap = false;
	if(nFormat & DT_EX_LINEWRAP) {
        // do the line wrap
		bWrap = true;
	}
	
	// 6. text format
	float fTextWidth, fTextHeight;
	int iTotalLines;
	std::wstring sTarget = textFormat( sText, bVertical?fAreaHeight:fAreaWidth, fTextWidth, fTextHeight,
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
				pen_X = start_X - _CharWidth * line;
			}
			else
			{
				pen_X = start_X + _CharWidth * line;
			}

			if(nFormat & DT_EX_TOP)
			{
				pen_Y = fTop;
			}
			else if(nFormat & DT_EX_VCENTER)
			{
				pen_Y = fTop  + (fAreaHeight - getLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning)) / 2;
			}
			else if(nFormat & DT_EX_BOTTOM)
			{
				pen_Y = fTop  + fAreaHeight - getLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning);
			}
            else{
                pen_Y = 0.0f; // TODO: this is added to fix "warning C4701: potentially uninitialized local variable 'pen_Y'"

            }
		}
		else
		{
			pen_X = fLeft;
			pen_Y = start_Y + (float)(_CharHeight * line);

			if(nFormat & DT_EX_LEFT)
			{
				if(bR2L)
				{
					pen_X = fRight - (fAreaWidth - getLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning));
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
					pen_X = fRight - (fAreaWidth - getLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning)) / 2;
				}
				else
				{
					pen_X = fLeft + (fAreaWidth - getLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning)) / 2;
				}
			}
			else if(nFormat & DT_EX_RIGHT)
			{
				if(bR2L)
				{
					pen_X = fRight - (fAreaWidth - getLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning));
				}
				else
				{
					pen_X = fLeft + fAreaWidth - getLineWidth(curline, bFlipX, bFlipY, fZRotate, bKerning);
				}
			}	
		}

		// draw this line
		drawTextLineEx(curline, pen_X, pen_Y, fLeft, fTop, fRight, fBottom, clrFont, bVertical, bR2L, bFlipX,
						bFlipY, fZRotate, btTrans, bKerning, bUnderl);
		line++;
	}

	//8. Display border if reauired
	if(nFormat & DT_EX_BORDER) {
		byte r,g,b;
		r = clrBorder & 0xFF;
		g = (clrBorder >> 8) & 0xFF;
		b = (clrBorder >> 16) & 0xFF;
		_pIndieRender->blitRectangle(
											(int)(fLeft - 1), 
											(int)(fTop - 1),
											(int)(fRight + 1),
											(int)(fBottom + 1),
											r,g,b,btBorderTrans);
	}

	return 1;
}

// --------------------------------------------------------------------------------
//									Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

bool IND_TTF_Font::renderChar(wchar_t charCode, float x, float y, uint32_t clrFont, bool bFlipX, bool bFlipY,
							   float fZRotate, byte btTrans, bool bKerning, bool bUnderl) {
	if (_bAutoCache)
		buildCharCache(charCode);

	CharCacheNode* pNode = getCharCacheNode(charCode);
	if (!pNode || !pNode->pSurface)
		return false;

	float origin_x, origin_y;
	origin_x = x;
	origin_y = y;

	x += pNode->charLeftBearing;
	y += _fFaceAscender - pNode->charTopBearing;
	
	//Bliting the font surfaces to screen
	// 1) We apply the world space transformation (translation, rotation, scaling).
	// If you want to recieve the transformation in a single matrix you can pass
	// and IND_Matrix object by reference.

	int mWidth = pNode->pSurface->getWidth();
	int mHeight = pNode->pSurface->getHeight();

	/*
	if( (x + mWidth  <= 0) || (y + mHeight <= 0) ||
		(x > m_pIndieRender->GetWindow()->GetWidth()) || (y > m_pIndieRender->GetWindow()->GetHeight()))
		return false;//not visible, just drop it
	*/
	//
	IND_Matrix mMatrix;
	// We want the start position (x,y) to be the top left corner 
	_pIndieRender->setTransform2d(
									(int)(x + _fXHotSpot * mWidth),		// x pos
									(int)(y + _fYHotSpot * mHeight),	// y pos
									0,                                  // Angle x
									0,                                  // Angle y
									fZRotate,                           // Angle z
									_fXScale,                           // Scale x
									_fYScale,                           // Scale y
									(int) (_fXHotSpot * mWidth * -1),	// Axis cal x
									(int) (_fYHotSpot * mHeight * -1),	// Axis cal y
									bFlipX,                             // Mirror x
									bFlipY,                             // Mirror y
									mWidth,                             // Width
									mHeight,                            // Height
									&mMatrix);                          // Matrix in wich the transformation will be applied (optional)			
 
	//2) We apply the color, blending and culling transformations.
	//modified by Joel Gao Mar,4th 2009
	byte r,g,b;
	r = clrFont & 0xFF;
	g = (clrFont >> 8) & 0xFF;
	b = (clrFont >> 16) & 0xFF;

	_pIndieRender->setRainbow2d(
									IND_ALPHA,			// IND_Type
									1,					// Back face culling 0/1 => off / on
									bFlipX,				// Mirror x
									bFlipY,				// Mirror y
									IND_FILTER_LINEAR,	// IND_Filter
									r,                  // R Component	for tinting
									g,                  // G Component	for tinting
									b,                  // B Component	for tinting
									btTrans,			// A Component	for tinting
									0,					// R Component	for fading to a color		
									0,					// G Component	for fading to a color		
									0,					// B Component	for fading to a color			
									255,//btTrans,		// Amount of fading
									IND_SRCALPHA,		// IND_BlendingType (source)
									IND_INVSRCALPHA);	// IND_BlendingType (destination)
	//


	// 3) Blit the IND_Surface
	//m_pIndieLib->Render->BlitRegionSurface(theGlyph->pSurface, (int)x, (int)y, mWidth,mHeight);
	_pIndieRender->blitSurface(pNode->pSurface);
	
	return true;
}


bool IND_TTF_Font::buildCharCache(wchar_t charCode) {
	if (isCharCached(charCode))
		return true;

	CharCacheNode* pNode = new CharCacheNode;
	//////////
	//init the struct pointers
	pNode->pSurface = NULL;
	//////////
	pNode->charCode = charCode;
	pNode->charGlyphIndex = FT_Get_Char_Index(_impl->_Face, charCode);

	if (pNode->charGlyphIndex == 0) {
		delete pNode;
		return false;
	}

	FT_Load_Char(_impl->_Face, charCode, FT_LOAD_DEFAULT /*| FT_LOAD_NO_BITMAP*/);

	// Bold
	if(_bBold) {
		int strength = 1 << 6;
		FT_Outline_Embolden(&_impl->_Face->glyph->outline, strength);
	}

	// Italic
	if(_bItalic) {
		// set transformation 
		FT_Outline_Transform(&_impl->_Face->glyph->outline, &_impl->_matItalic);
	}
	

	if(FT_Render_Glyph(_impl->_Face->glyph, FT_RENDER_MODE_NORMAL)) {
		delete pNode;
		return false;
	}

	// build an image
	IND_Image *pImage = IND_Image::newImage();
	assert(pImage);

	// render the glyph image to IND_Image
	if(!renderGlyph(_impl, pImage)) // &_impl->_Face->glyph->bitmap, pImage
		return false;
		
	//building the surface from image
	pNode->pSurface = IND_Surface::newSurface();

	bool bOK = true;
	if (!_pIndieSurfaceManager->add (pNode->pSurface, pImage, IND_ALPHA, IND_32)) {
		bOK = false;
	}
    
	// free the image
	_pIndieImageManager->remove(pImage);
	//DISPOSEMANAGED(pImage);  // TODO: figure out why a diposemanaged gives an exeption, ... the image needs to be deleted
    // pImage->destroy();      // TODO: ... this also does not work ..
    
	if(!bOK) {
		delete pNode;
		return false;
	}

	pNode->charLeftBearing = _impl->_Face->glyph->bitmap_left;
	pNode->charTopBearing = _impl->_Face->glyph->bitmap_top;
	pNode->charAdvance = _impl->_Face->glyph->advance.x / 64;
	
	_FontCharCache.insert(std::pair<wchar_t, CharCacheNode*>(charCode, pNode));

	//cache entry built
	return true;
}

IND_TTF_Font::CharCacheNode* IND_TTF_Font::getCharCacheNode(wchar_t charCode) {
	CharCacheMapIterator it = _FontCharCache.find(charCode);
	if(it == _FontCharCache.end())
		return NULL;
	else
		return it->second;
}

bool IND_TTF_Font::renderGlyph(free_type_impl* impl, IND_Image *pImage) {
	
    //free_type_impl* impl
    
    FT_Bitmap* ftBMP = &impl->_Face->glyph->bitmap;
    
    uint32_t glyphWidth = ftBMP->width;
	uint32_t glyphHeight = ftBMP->rows;

	if (glyphWidth == 0 || glyphHeight == 0)
		return false;

	_pIndieImageManager->add(pImage, glyphWidth, glyphHeight, IND_RGBA);
	if(pImage == NULL)
		return false;

	byte *pSrc = ftBMP->buffer;
	/*
	byte r,g,b;
	r = m_FontColor & 0xFF;
	g = (m_FontColor >> 8) & 0xFF;
	b = (m_FontColor >> 16) & 0xFF;
	*/

	for(uint32_t x = 0 ; x <  glyphWidth; x++) {
		
        for(uint32_t y = 0 ; y <  glyphHeight; y++) {
			
            switch (ftBMP->pixel_mode) {
				
                case FT_PIXEL_MODE_GRAY:
					//pImage->PutPixel(x, y, r,g,b,pSrc[y * glyphWidth + x]);
					pImage->putPixel(x, y, 255,255,255,pSrc[y * glyphWidth + x]);
					break;
				case FT_PIXEL_MODE_MONO:
					pSrc = ftBMP->buffer + (y * ftBMP->pitch);
					if((pSrc [x / 8] & (0x80 >> (x & 7))))
						//pImage->PutPixel(x, y, r,g,b,0xFF);
						pImage->putPixel(x, y, 255,255,255,0xFF);
					else
						//pImage->PutPixel(x, y, r,g,b,0x00);
						pImage->putPixel(x, y, 255,255,255,0x00);
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

IND_TTF_Font::uint32_t IND_TTF_Font::getSpaceAdvance() {
	//We use the advance value of 'A' as the space value
	buildCharCache(L'A');
	CharCacheNode* pNode = getCharCacheNode(L'A');
	if(pNode)
		return pNode->charAdvance;
	else// no 'A' in this face
		return _CharWidth / 2;
}

std::wstring IND_TTF_Font::textFormat(	const std::wstring& sText, float &fLineWidth, float &fTotalWidth, float &fTotalHeight,
										int &iTotalLineNum, bool bLineWrap,bool bFlipX, bool bFlipY, float fZRotate, byte btTrans, 
										bool bKerning, bool bUnderl) {
	fTotalWidth = 0.0f;
	fTotalHeight = 0.0f;
	iTotalLineNum = 0;

	CharCacheNode* pNode = NULL;
	//For Kerning
	FT_Vector Delta;
	uint32_t previousGlyph = 0;
	//

	float fCurrentLineWidth = 0.0f;

	uint32_t nSpace = getSpaceAdvance();

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
		if (_bAutoCache)
			buildCharCache(sText[i]);

		pNode = getCharCacheNode(sText[i]);
		if (!pNode)
		{
			previousGlyph = 0;
			continue;
		}
		if (previousGlyph != 0 && _bHasKerning && bKerning && !bFlipX && !bFlipY && fZRotate == 0)
		{
			FT_Get_Kerning(_impl->_Face, previousGlyph, pNode->charGlyphIndex, FT_KERNING_DEFAULT, &Delta);
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

	fTotalHeight = (float)(_CharHeight * iTotalLineNum);
	return sRet;
}

IND_TTF_Font::uint32_t IND_TTF_Font::getLineWidth(const std::wstring& sText, bool bFlipX, bool bFlipY,
												   float fZRotate, bool bKerning) {
	CharCacheNode* pNode = NULL;
	//For Kerning
	FT_Vector Delta;
	uint32_t previousGlyph = 0;
	//
	uint32_t nSpace = getSpaceAdvance();
	uint32_t nRet = 0;
	std::size_t nLength = sText.length();

	for (std::size_t i = 0; i < nLength; i++) {
        
		switch (sText[i]) {
                
		case L' ':	
			nRet += nSpace;
			previousGlyph = 0; 
			continue;
		case L'\t': 
			nRet += nSpace * nTabSize;	
			previousGlyph = 0; 
			continue;
		}

		pNode = getCharCacheNode(sText[i]);
		
        if (!pNode) {
			previousGlyph = 0;
			continue;
		}
        
		if (previousGlyph != 0 && _bHasKerning  && bKerning && !bFlipX && !bFlipY && fZRotate == 0) {
			FT_Get_Kerning(_impl->_Face, previousGlyph, pNode->charGlyphIndex, FT_KERNING_DEFAULT, &Delta);
			nRet += Delta.x >> 6;
		}

		nRet += pNode->charAdvance;

		previousGlyph = pNode->charGlyphIndex;
	}
	return nRet;
}

void IND_TTF_Font::drawTextLineEx(const std::wstring& sText, float penX, float penY, float fL, float fT,
							  float fR, float fB, uint32_t clrFont,bool bVertical, bool bR2L,bool bFlipX, 
							  bool bFlipY, float fZRotate, byte btTrans, bool bKerning, bool bUnderl) {
	uint32_t previousGlyph = 0;
	FT_Vector Delta;

	CharCacheNode* pNode = NULL;
	std::size_t nLength = sText.length();

	uint32_t nSpace = getSpaceAdvance();

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

		pNode = getCharCacheNode(sText[i]);
		if (!pNode)
		{
			previousGlyph = 0;
			continue;
		}
		//Kerning
		if (!bR2L && previousGlyph != 0 && _bHasKerning  && bKerning && !bFlipX && !bFlipY && fZRotate == 0)
		{
			FT_Get_Kerning(_impl->_Face, previousGlyph, pNode->charGlyphIndex, FT_KERNING_DEFAULT, &Delta);
			penX += Delta.x >> 6;
			penY += Delta.y >> 6;
		}
		

		//bool bDraw = false;
		if(bVertical)
		{
			if(bR2L)
			{
				if(	((penX - _CharWidth) >= fL) && 
					(penX <= fR) &&
					(penY >= fT ) && 
					((penY + pNode->charAdvance) <= fB))
					renderChar(sText[i], penX - _CharWidth, penY, clrFont, bFlipX, bFlipY, fZRotate, btTrans,
								bKerning, bUnderl);
			}
			else
			{
				if(	(penX >= fL) && 
					((penX + _CharWidth) <= fR) &&
					(penY >= fT ) && 
					((penY + pNode->charAdvance) <= fB))
					renderChar(sText[i], penX, penY, clrFont, bFlipX, bFlipY, fZRotate, btTrans,
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
					((penY + _CharHeight) <= fB))
					renderChar(sText[i], penX - pNode->charAdvance, penY, clrFont, bFlipX, bFlipY, fZRotate, btTrans,
								bKerning, bUnderl);
			}
			else
			{
				if(	(penX >= fL) && 
					((penX + pNode->charAdvance) <= fR) &&
					(penY >= fT ) &&
					((penY + _CharHeight) <= fB))
					renderChar(sText[i], penX, penY, clrFont, bFlipX, bFlipY, fZRotate, btTrans,
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
		doDrawBorder(original_Pen_x, penX, penY + _CharHeight, clrFont, btTrans);
	}
}

void IND_TTF_Font::doDrawBorder(float fX_s, float fX_e, float fY, uint32_t clr, byte btTrans) {
	byte r,g,b;
	r = clr & 0xFF;
	g = (clr >> 8) & 0xFF;
	b = (clr >> 16) & 0xFF;
	_pIndieRender->blitLine((int)(fX_s),
							(int)(fY),
							(int)(fX_e),
							(int)(fY),
							r,g,b,btTrans);
}

/** @endcond */
