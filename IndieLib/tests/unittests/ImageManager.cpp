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


/*
   	// ----- Init/End -----

	IND_ImageManager(): _ok(false)  { }
	~IND_ImageManager()              {
		end();
	}

	bool    init();
	void    end();
	bool    isOK()  const {
		return _ok;
	}

	// ----- Public methods -----

	add1:   bool add(IND_Image *pNewImage, char *pName);
	add2:   bool add(IND_Image *pNewImage, int pWidth, int pHeight, IND_ColorFormat pColorFormat);
	remove: bool remove(IND_Image *pIm);
	save:   bool save(IND_Image *pIm, char *pName);
	clone:  bool clone(IND_Image *pNewImage, IND_Image *pOldImage);
	load:   FIBITMAP* load(char *pName);
	add3:   bool add(IND_Image *pNewImage, FIBITMAP *pImageToBeCopied);
*/

TEST(add1) {
	CIndieLib *iLib = CIndieLib::instance();
	iLib->init();

	IND_Image *mImageBug = new IND_Image();
	CHECK(iLib->_imageManager->add(mImageBug, const_cast<char*>("../../resources/Enemy Bug.png")));

//	iLib->end();
}
/*
TEST(add2) {
	CIndieLib *iLib = CIndieLib::instance();
}
*/
TEST(remove) {
	CIndieLib *iLib = CIndieLib::instance();
//	iLib->init();
	
	IND_Image *mImageBug = new IND_Image();
	iLib->_imageManager->add(mImageBug, const_cast<char*>("../../resources/Enemy Bug.png"));
	
	CHECK(iLib->_imageManager->remove(mImageBug));

//	iLib->end();
}
/*
TEST(save) {
	CIndieLib *iLib = CIndieLib::instance();
}
*/
TEST(clone) {
	CIndieLib *iLib = CIndieLib::instance();
//	iLib->init();
	
	IND_Image *mImageBug = new IND_Image();
	IND_Image *mImageClone = new IND_Image();
	iLib->_imageManager->add(mImageBug, const_cast<char*>("../../resources/Enemy Bug.png"));
	
	CHECK(iLib->_imageManager->clone(mImageClone,mImageBug));
	CHECK(mImageClone->getFreeImageHandle() != NULL);

//	iLib->end();
}


TEST(load) {
	CIndieLib *iLib = CIndieLib::instance();

	FIBITMAP* bitmap = iLib->_imageManager->load(const_cast<char*>("../../resources/Enemy Bug.png"));
	CHECK(bitmap != NULL);
}
/*
TEST(add3) {
	CIndieLib *iLib = CIndieLib::instance();
}
*/