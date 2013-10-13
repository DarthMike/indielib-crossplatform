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
	CHECK(iLib->_surfaceManager->add(testSurf,const_cast<char *>("blue_background.jpg"), IND_OPAQUE, IND_32));
}

TEST_FIXTURE(fixture,SURFACEMANAGER_ADDNONEXISTING_ADDFAILS) {
	CHECK(!iLib->_surfaceManager->add(testSurf,const_cast<char *>("BADBADBAD.jpg"), IND_OPAQUE, IND_32));
}

TEST_FIXTURE(fixture,SURFACEMANAGER_ADDEXISTING_REMOVEIT_NOFAIL) {
	iLib->_surfaceManager->add(testSurf,const_cast<char *>("blue_background.jpg"), IND_OPAQUE, IND_32);
    CHECK(iLib->_surfaceManager->remove(testSurf));
}

TEST_FIXTURE(fixture,SURFACEMANAGER_REMOVENONEXISTING_FAILS) {
    CHECK(!iLib->_surfaceManager->remove(testSurf));
}
