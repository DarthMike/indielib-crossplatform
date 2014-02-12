/*****************************************************************************************
 * Desc: FontTests_Pretty class
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
#ifndef _FontTests_Pretty_
#define _FontTests_Pretty_

#include "manual/ManualTests.h"

class IND_Font;
class IND_Entity2d;

class FontTests_Pretty : public ManualTests {
public:

    FontTests_Pretty():
		_surfaceBack(NULL),
	_surfaceTip(NULL),
	_fontSmall(NULL),
	_fontBig(NULL),
    _fontKomika(NULL),
	_back(NULL),
	 _tip(NULL),
	 _textTitle(NULL),
	_textSmallBlack(NULL),					
	_textSmallWhite(NULL),
    _textKomika(NULL){
		init();
	}

	virtual ~FontTests_Pretty() {
		release();
	}
	void prepareTests();
	void performTests(float dt);

    bool isActive();
    void setActive(bool active);

private:
	void init();
	void release();

	IND_Surface *_surfaceBack;
	IND_Surface *_surfaceTip;
	IND_Font* _fontSmall;
	IND_Font* _fontBig;
    IND_Font* _fontKomika;
	IND_Entity2d* _back;
	IND_Entity2d* _tip;
	IND_Entity2d* _textTitle;
	IND_Entity2d* _textSmallBlack;					
	IND_Entity2d* _textSmallWhite;
    IND_Entity2d* _textKomika;
};


#endif // _Camera2dTests_
