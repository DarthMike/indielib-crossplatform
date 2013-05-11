/*****************************************************************************************
 * File: ImageManager.cpp
 * Desc: Test class for the coressponding Indielib library class IMageManager.cpp
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
#include "CIndieLib_vc2008.h"
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

TEST_FIXTURE(fixture,ImageManager_addFromFile) {
	CHECK(iLib->_imageManager->add(testImage, "../../resources/Enemy Bug.png"));
}

TEST_FIXTURE(fixture,ImageManager_AddProcedural_RGBA) {
	CHECK(iLib->_imageManager->add(testImage,800,600,IND_RGBA));
	CHECK_EQUAL(32,testImage->getBpp());
}

TEST_FIXTURE(fixture,ImageManager_AddProcedural_RGB) {
	CHECK(iLib->_imageManager->add(testImage,800,600,IND_RGB));
	CHECK_EQUAL(24,testImage->getBpp());
}

TEST_FIXTURE(fixture,ImageManager_AddProcedural_COLOURINDEX) {
	CHECK(iLib->_imageManager->add(testImage,800,600,IND_COLOUR_INDEX));
	CHECK_EQUAL(16,testImage->getBpp());
}

TEST_FIXTURE(fixture,ImageManager_AddProcedural_LUMINANCE) {
	CHECK(iLib->_imageManager->add(testImage,800,600,IND_LUMINANCE));
	CHECK_EQUAL(8,testImage->getBpp());
}

TEST_FIXTURE(fixture,ImageManager_addCopying) {
	iLib->_imageManager->add(testImage, "../../resources/Enemy Bug.png");
	
	IND_Image *copied = IND_Image::newImage();
	CHECK(iLib->_imageManager->add(copied,testImage->getFreeImageHandle()));
	CHECK(copied->getFreeImageHandle() != NULL);
}

TEST_FIXTURE(fixture,Image_Paste) {
	iLib->_imageManager->add(testImage, "../../resources/Enemy Bug.png");
	
	IND_Image *pastedTo = IND_Image::newImage();
	iLib->_imageManager->add(pastedTo,testImage->getWidth(),testImage->getHeight(),IND_RGBA);
	pastedTo->clear(0,0,0,0);
	CHECK(testImage->pasteImage(pastedTo,0,0,255));
}

TEST_FIXTURE(fixture,ImageManager_remove) {	
	iLib->_imageManager->add(testImage, "../../resources/Enemy Bug.png");
	
	CHECK(iLib->_imageManager->remove(testImage));
}

TEST_FIXTURE(fixture,ImageManager_clone) {	
	IND_Image *testClone = IND_Image::newImage();
	iLib->_imageManager->add(testImage, "../../resources/Enemy Bug.png");
	
	CHECK(iLib->_imageManager->clone(testClone,testImage));
	CHECK(testClone->getFreeImageHandle() != NULL);
}

TEST_FIXTURE(fixture,ImageManager_load) {
	FIBITMAP* bitmap = iLib->_imageManager->load("../../resources/Enemy Bug.png");
	CHECK(bitmap != NULL);
}