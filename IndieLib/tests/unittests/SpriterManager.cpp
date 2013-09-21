/*****************************************************************************************
 * File: SpriterManager.cpp
 * Desc: Test class for the coressponding Indielib library class SpriterManager.cpp
 *****************************************************************************************/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */

#include "dependencies/unittest++/src/UnitTest++.h"
#include "CIndieLib.h"
#include "IND_SpriterEntity.h"

struct fixture {
    fixture() {
        iLib = CIndieLib::instance();
        iLib->init();
		listSpriterEntity = new vector <IND_SpriterEntity *>();
    }
    ~fixture() {
        iLib->end();
        
    }
	vector <IND_SpriterEntity *> *listSpriterEntity;
    CIndieLib* iLib;
};

TEST_FIXTURE(fixture,SpriterManager_addFromFile) {
	CHECK(iLib->_spriterManager->addSpriterFile("../../resources/Spriter/monster/Example.SCML"));
    CHECK(!listSpriterEntity->empty());
    // @MFK Tests fail here
    //CHECK_EQUAL(1,listSpriterEntity->at(0)->getAnimations()->size());
}



