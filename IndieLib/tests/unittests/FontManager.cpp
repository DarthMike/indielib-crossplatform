#include "dependencies/unittest++/src/UnitTest++.h"
#include "CIndieLib_vc2008.h"
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
	CHECK(iLib->_fontManager->add(testFont, "../../resources/font_small.png", "../../resources/font_small.xml", IND_ALPHA, IND_32));
}

TEST_FIXTURE(fontFixture,FONTMANAGER_ADDNONEXISTINGIMG_ADDFAILS) {
	CHECK(!iLib->_fontManager->add(testFont, "../../resources/BADBADBAD.png", "../../resources/font_small.xml", IND_ALPHA, IND_32));
}

TEST_FIXTURE(fontFixture,FONTMANAGER_ADDNONEXISTINGXML_ADDFAILS) {
	CHECK(!iLib->_fontManager->add(testFont, "../../resources/font_small.png", "../../resources/BADBADBAD.xml", IND_ALPHA, IND_32));
}

TEST_FIXTURE(fontFixture,FONTMANAGER_ADDEXISTING_REMOVEIT_NOFAIL) {
	CHECK(iLib->_fontManager->add(testFont, "../../resources/font_small.png", "../../resources/font_small.xml", IND_ALPHA, IND_32));
    CHECK(iLib->_fontManager->remove(testFont));
}

TEST_FIXTURE(fontFixture,FONTMANAGER_REMOVENONEXISTING_FAILS) {
    CHECK(!iLib->_fontManager->remove(testFont));
}
