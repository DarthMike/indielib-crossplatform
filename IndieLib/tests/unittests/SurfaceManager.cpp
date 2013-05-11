#include "dependencies/unittest++/src/UnitTest++.h"
#include "CIndieLib.h"
#include "IND_Surface.h"

struct fixture {
    fixture() {
        iLib = CIndieLib::instance();
        iLib->init();
        testSurf = IND_Surface::newSurface();
    }
    ~fixture() {
        iLib->end();
        
    }
    IND_Surface *testSurf;
    CIndieLib* iLib;
};


TEST_FIXTURE(fixture,SURFACEMANAGER_ADDEXISTING_ADDOK) {
	CHECK(iLib->_surfaceManager->add(testSurf,const_cast<char *>("../../resources/blue_background.jpg"), IND_OPAQUE, IND_32));
}

TEST_FIXTURE(fixture,SURFACEMANAGER_ADDNONEXISTING_ADDFAILS) {
	CHECK(!iLib->_surfaceManager->add(testSurf,const_cast<char *>("../../resources/BADBADBAD.jpg"), IND_OPAQUE, IND_32));
}

TEST_FIXTURE(fixture,SURFACEMANAGER_ADDEXISTING_REMOVEIT_NOFAIL) {
	iLib->_surfaceManager->add(testSurf,const_cast<char *>("../../resources/blue_background.jpg"), IND_OPAQUE, IND_32);
    CHECK(iLib->_surfaceManager->remove(testSurf));
}

TEST_FIXTURE(fixture,SURFACEMANAGER_REMOVENONEXISTING_FAILS) {
    CHECK(!iLib->_surfaceManager->remove(testSurf));
}
