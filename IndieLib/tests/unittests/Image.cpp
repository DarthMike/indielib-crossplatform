/*****************************************************************************************
 * File: Image.cpp
 * Desc: Test class for the coresponding Indielib library class IND_Image.cpp
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
#include "IND_Image.h"

struct fixture {
    fixture() {
        iLib = CIndieLib::instance();
        iLib->init();
		testImage = IND_Image::newImage();
    }
    ~fixture() {
        iLib->end();
        
    }
	IND_Image* testImage;
    CIndieLib* iLib;
};

TEST_FIXTURE(fixture,clone) {

}
