/*****************************************************************************************
 * File: RenderText2dOpenGL.cpp
 * Desc: Bitmap fonts text blitting
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
#include "Defines.h"

#ifdef INDIERENDER_GLES_IOS
// ----- Includes -----

#include "Global.h"
#include "OpenGLES2Render.h"
#include "IND_SurfaceManager.h"
#include "IND_Font.h"
#include "IND_Surface.h"

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

void OpenGLES2Render::blitText(IND_Font *pFo,
                            char *pText,
                            int pX,
                            int pY,
                            int pOffset,
                            int pLineSpacing,
                            float pScaleX,
                            float pScaleY,
                            unsigned char pR,
                            unsigned char pG,
                            unsigned char pB,
                            unsigned char pA,
                            unsigned char pFadeR,
                            unsigned char pFadeG,
                            unsigned char pFadeB,
                            unsigned char pFadeA,
                            IND_Filter pLinearFilter,
                            IND_BlendingType pSo,
                            IND_BlendingType pDs,
                            IND_Align pAlign) {
	// ----- Transform -----
	bool correctParams = true;
	if(!pFo->getSurface()) {
		correctParams = false;
	}
    
	if (correctParams) {
		setTransform2d(pX, pY, 0, 0, 0, pScaleX, pScaleY, 0, 0, 0, 0, pFo->getSurface()->getWidthBlock(), pFo->getSurface()->getHeightBlock(), 0);
		setRainbow2d(pFo->getSurface()->getTypeInt(), 1, 0, 0, pLinearFilter, pR, pG, pB, pA, pFadeR, pFadeG, pFadeB, pFadeA, pSo, pDs);
        
        
        switch (pFo->getFontType()) {
            case IND_Font::FONTTYPE_AngelCode:
                blitCharsAngelCodeFont(pFo, pText, pAlign, pOffset, pScaleX, pScaleY, pLineSpacing);
                break;
                
            case IND_Font::FONTTYPE_MudFont:
                blitCharsMudFont(pFo, pText, pAlign, pOffset, pScaleX, pScaleY, pLineSpacing);
                break;
                
            default:
                break;
        }
        
	}
}


// --------------------------------------------------------------------------------
//							         Private methods
// --------------------------------------------------------------------------------

void OpenGLES2Render::blitCharsAngelCodeFont(IND_Font *pFo, char *pText, IND_Align pAlign, int pOffset, float pScaleX, float pScaleY, int pLineSpacing) {
    
//    // ----- Drawing text   -----
//    
//    unsigned char mChar1;
//    unsigned char mChar2;
//    int mCont1 = 0;
//    int mCont2 = 0;
//    int mTranslationX = 0;
//    int mTranslationY = 0;
//    bool mErrorChar;    // Char that doesn't exist
//    int mSentencePos;
//    int mLongActualSentence;
//    IND_Matrix mLineTransform;  //Maintains a transform inside the entity coord system
//    IND_Matrix mBaseEntityTransform; //Maintains transform for the whole entity coord system
//    mCont1 = 0;
//    mChar1 = pText [mCont1++];
//    
//    //Store entity transform
//    mBaseEntityTransform = _shaderModelViewMatrix;
//    
//    //LOOP - Blit character by character
//    while (mChar1 != 0) {
//        // If it's a new line or it's the first line
//        if (mChar1 == '\n' || mCont1 == 1) {
//            // First line
//            if (mCont1 == 1) {
//                mSentencePos = 0;
//				//Any other line
//            } else {
//                mSentencePos = mCont1;
//            }
//            
//            // Set the alignment
//            switch (pAlign) {
//                case IND_CENTER: {
//                    mLongActualSentence = static_cast<int>(getLongInPixels(pFo, pText, mSentencePos, pOffset) * pScaleX);
//                    mTranslationX = (int)(mLongActualSentence / 2);
//                    break;
//                }
//                    
//                case IND_RIGHT: {
//                    mLongActualSentence = static_cast<int>(getLongInPixels(pFo, pText, mSentencePos, pOffset) * pScaleX);
//                    mTranslationX = (int)(mLongActualSentence);
//                    
//                    break;
//                }
//                    
//                case IND_LEFT: {
//                    break;
//                }
//            }
//            
//            //Load line transform matrix taking into account entity transform
//            //This effectively resets transform to first character in the new line
//            IND_Matrix transform;
//            _math.matrix4DSetTranslation(transform, static_cast<float>(-mTranslationX), static_cast<float>(mTranslationY), 0.0f);
//            transform.arrayRepresentation(mLineTransform);
//            glLoadMatrixf(mBaseEntityTransform);
//            glMultMatrixf(mLineTransform);
//            mTranslationY += static_cast<int>((pLineSpacing * pScaleY));
//        } //Was first new line or first line
//        
//        // It's a normal character
//        if (mChar1 != '\n') {
//            
//            mErrorChar = 0;
//            mCont2 = 0;
//            mChar2 = pFo->getLetters() [mCont2++]._letter;
//            
//            // Seek its location in the bitmap
//            while (mChar2 != mChar1 && mCont2 < pFo->getNumChars()) {
//                mChar2 = pFo->getLetters() [mCont2++]._letter;
//            }
//            if (mCont2  ==  pFo->getNumChars())
//                mErrorChar = 1;
//            
//            mCont2--;
//            if (!mErrorChar) {
//                
//                // To avoid that Y displacement transform accumulates for every character in the line,
//                // save the untranslated coordinate system.
//                glPushMatrix();
//                
//                float charTranslateY = static_cast<float>(pFo->getLetters()[mCont2]._yOffset);
//                glTranslatef(0.0f, charTranslateY ,0.0f);
//                
//                //#warning lookout
//                //mvTransformPresetState();  //Need to preset transform state, as the blit operation will reset the state!!!!
//                blitRegionSurface(pFo->getSurface(),
//                                  pFo->getLetters() [mCont2]._x,
//                                  pFo->getLetters() [mCont2]._y,
//                                  pFo->getLetters() [mCont2]._width,
//                                  pFo->getLetters() [mCont2]._height);
//                
//                // Restore the untranslated coordinate system.
//                glPopMatrix();
//            }
//            
//            //X displacement of the character.
//            //X displacement transform accumulates for every character in the line
//            float charTranslateX = ((pFo->getLetters() [mCont2]._width) + pOffset) * pScaleX;
//            glTranslatef(charTranslateX,0.0f,0.0f);
//        }//Was normal character
//        
//        // Advance one character
//        mChar1 = pText [mCont1++];
//    }//LOOP END - Blit character by character
    
}


void OpenGLES2Render::blitCharsMudFont(IND_Font *pFo, char *pText, IND_Align pAlign, int pOffset, float pScaleX, float pScaleY, int pLineSpacing) {
    
    // ----- Drawing text   -----
    
    unsigned char mChar1;
    unsigned char mChar2;
    int mCont1 (0);
    int mCont2 (0);
    int mLine (0);
    bool mErrorChar;    // Char that doesn't exist
    int mSentencePos;
    int mLongActualSentence;
    IND_Matrix mLineTransform;  //Maintains a transform inside the entity coord system
    IND_Matrix mBaseEntityTransform; //Maintains transform for the whole entity coord system
    mCont1 = 0;
    mChar1 = pText [mCont1++];
    
    //Store entity transform
    mBaseEntityTransform = _shaderModelViewMatrix;
    
    
    //LOOP - Blit character by character
    while (mChar1 != 0) {
        // If it's a new line or it's the first line
        if (mChar1 == '\n' || mCont1 == 1) {
            // First line
            if (mCont1 == 1) {
                mSentencePos = 0;
				//Any other line
            } else {
                mSentencePos = mCont1;
            }
            
            float mXAlignmentAdjust (0.f);
            // Set the alignment
            switch (pAlign) {
                case IND_CENTER: {
                    mLongActualSentence = static_cast<int>(getLongInPixels(pFo, pText, mSentencePos, pOffset) * pScaleX);
                    mXAlignmentAdjust = static_cast<int>(mLongActualSentence / 2);
                    break;
                }
                    
                case IND_RIGHT: {
                    mLongActualSentence = static_cast<int>(getLongInPixels(pFo, pText, mSentencePos, pOffset) * pScaleX);
                    mXAlignmentAdjust = static_cast<int>(mLongActualSentence);
                    
                    break;
                }
                    
                case IND_LEFT: {
                    break;
                }
            }
            
            //Load line transform matrix taking into account entity transform
            //This effectively resets transform to first character in the new line
            IND_Matrix translation;
            _math.matrix4DSetTranslation(translation, -mXAlignmentAdjust, mLine * pLineSpacing * pScaleY, 0.0f);
            _math.matrix4DMultiply(mBaseEntityTransform, translation, _shaderModelViewMatrix);
            
            ++mLine;

        } //Was first new line or first line
        
        // It's a normal character
        if (mChar1 != '\n') {
            
            mErrorChar = 0;
            mCont2 = 0;
            mChar2 = pFo->getLetters() [mCont2++]._letter;
            
            // Seek its location in the bitmap
            while (mChar2 != mChar1 && mCont2 < pFo->getNumChars()) {
                mChar2 = pFo->getLetters() [mCont2++]._letter;
            }
            if (mCont2  ==  pFo->getNumChars())
                mErrorChar = 1;
            
            mCont2--;
            if (!mErrorChar) {
                blitRegionSurface(pFo->getSurface(),
                                  pFo->getLetters() [mCont2]._x + 1,
                                  pFo->getLetters() [mCont2]._y + 1,
                                  pFo->getLetters() [mCont2]._width - 1,
                                  pFo->getLetters() [mCont2]._height - 1);
            }
            
            //Displacement of the character.
            //Displacement transform accumulates for every character in the line
            float charTranslateX = ((pFo->getLetters() [mCont2]._width) + pOffset) * pScaleX;
            IND_Matrix translation;
            _math.matrix4DSetTranslation(translation, charTranslateX, 0, 0);
            _math.matrix4DMultiplyInPlace(_shaderModelViewMatrix, translation);
        }//Was normal character
        
        // Advance one character
        mChar1 = pText [mCont1++];
    }//LOOP END - Blit character by character
    
}


/*
 ==================
 Returns a long in pixels of the sentence that starts in pPos
 A sentence is an array of chars that ends in \n or \0
 ==================
 */
int OpenGLES2Render::getLongInPixels(IND_Font *pFo, char *pText, int pPos, int pOffset) {
	unsigned char mChar1;
	unsigned char mChar2;
	int mCont1 = pPos;
	int mCont2 = 0;
	bool mErrorChar;    // Error when nonexistant char
    
	int mWidthSentence = 0;
	mChar1 = pText [mCont1++];
	while (mChar1 != '\0' && mChar1 != '\n') {
		mErrorChar = 0;
		mCont2 = 0;
		mChar2 = pFo->getLetters() [mCont2++]._letter;
        
		// Search the position
		while (mChar2 != mChar1 && mCont2 < pFo->getNumChars()) mChar2 = pFo->getLetters() [mCont2++]._letter;
		if (mCont2  ==  pFo->getNumChars())
			mErrorChar = 1;
        
		if (!mErrorChar)
			mWidthSentence += pFo->getLetters() [mCont2 - 1]._width + pOffset;
        
		mChar1 = pText [mCont1++];
	}
    
	return mWidthSentence;
}
/** @endcond */
#endif //INDIERENDER_GLES_IOS
