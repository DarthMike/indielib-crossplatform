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
	CHECK(iLib->_imageManager->add(testImage, "Enemy Bug.png"));
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
	iLib->_imageManager->add(testImage, "Enemy Bug.png");
	
	IND_Image *copied = IND_Image::newImage();
	CHECK(iLib->_imageManager->add(copied,testImage->getFreeImageHandle()));
	CHECK(copied->getFreeImageHandle() != NULL);
}

TEST_FIXTURE(fixture,Image_Paste) {
	iLib->_imageManager->add(testImage, "Enemy Bug.png");
	
	IND_Image *pastedTo = IND_Image::newImage();
	iLib->_imageManager->add(pastedTo,testImage->getWidth(),testImage->getHeight(),IND_RGBA);
	pastedTo->clear(0,0,0,0);
	CHECK(testImage->pasteImage(pastedTo,0,0,255));
}

TEST_FIXTURE(fixture,ImageManager_remove) {	
	iLib->_imageManager->add(testImage, "Enemy Bug.png");
	
	CHECK(iLib->_imageManager->remove(testImage));
}

TEST_FIXTURE(fixture,ImageManager_clone) {	
	IND_Image *testClone = IND_Image::newImage();
	iLib->_imageManager->add(testImage, "Enemy Bug.png");
	
	CHECK(iLib->_imageManager->clone(testClone,testImage));
	CHECK(testClone->getFreeImageHandle() != NULL);
}

TEST_FIXTURE(fixture,ImageManager_load) {
	FIBITMAP* bitmap = iLib->_imageManager->load("Enemy Bug.png");
	CHECK(bitmap != NULL);
}