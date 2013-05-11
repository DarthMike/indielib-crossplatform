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
#include "CIndieLib.h"
#include "IND_Image.h"

TEST(add1) {
	CIndieLib *iLib = CIndieLib::instance();
	iLib->init();

	IND_Image *mImageBug = IND_Image::newImage();
	CHECK(iLib->_imageManager->add(mImageBug, "../../resources/Enemy Bug.png"));
}

TEST(remove) {
	CIndieLib *iLib = CIndieLib::instance();
	
	IND_Image *mImageBug = IND_Image::newImage();
	iLib->_imageManager->add(mImageBug, "../../resources/Enemy Bug.png");
	
	CHECK(iLib->_imageManager->remove(mImageBug));
}

TEST(clone) {
	CIndieLib *iLib = CIndieLib::instance();
	
	IND_Image *mImageBug = IND_Image::newImage();
	IND_Image *mImageClone = IND_Image::newImage();
	iLib->_imageManager->add(mImageBug, "../../resources/Enemy Bug.png");
	
	CHECK(iLib->_imageManager->clone(mImageClone,mImageBug));
	CHECK(mImageClone->getFreeImageHandle() != NULL);
}

TEST(load) {
	CIndieLib *iLib = CIndieLib::instance();

	FIBITMAP* bitmap = iLib->_imageManager->load("../../resources/Enemy Bug.png");
	CHECK(bitmap != NULL);
}