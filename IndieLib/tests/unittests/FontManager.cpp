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


#include "dependencies/unittest++/src/UnitTest++.h"
#include "CIndieLib.h"
#include "IND_Font.h"

struct fontFixture {
    fontFixture() {
        iLib = CIndieLib::instance();
        iLib->init();
        testFont = IND_Font::newFont();
    }
    ~fontFixture() {
        iLib->end();
        
    }
    IND_Font *testFont;
    CIndieLib* iLib;
};



TEST_FIXTURE(fontFixture,FONTMANAGER_ADDEXISTING_ADDOK) {
	CHECK(iLib->_fontManager->addMudFont(testFont, "font/bitmap/MudFont/font_small.png", "font/bitmap/MudFont/font_small.xml", IND_ALPHA, IND_32));
}

TEST_FIXTURE(fontFixture,FONTMANAGER_ADDNONEXISTINGIMG_ADDFAILS) {
	CHECK(!iLib->_fontManager->addMudFont(testFont, "BADBADBAD.png", "font/bitmap/MudFont/font_small.xml", IND_ALPHA, IND_32));
}

TEST_FIXTURE(fontFixture,FONTMANAGER_ADDNONEXISTINGXML_ADDFAILS) {
	CHECK(!iLib->_fontManager->addMudFont(testFont, "font/bitmap/MudFont/font_small.png", "BADBADBAD.xml", IND_ALPHA, IND_32));
}

TEST_FIXTURE(fontFixture,FONTMANAGER_ADDEXISTING_REMOVEIT_NOFAIL) {
	CHECK(iLib->_fontManager->addMudFont(testFont, "font/bitmap/MudFont/font_small.png", "font/bitmap/MudFont/font_small.xml", IND_ALPHA, IND_32));
    CHECK(iLib->_fontManager->remove(testFont));
}

TEST_FIXTURE(fontFixture,FONTMANAGER_REMOVENONEXISTING_FAILS) {
    CHECK(!iLib->_fontManager->remove(testFont));
}
