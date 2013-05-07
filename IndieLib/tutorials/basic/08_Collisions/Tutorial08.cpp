/*****************************************************************************************
 * Desc: Tutorials a) 08 Collisions
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"

#include "IND_Surface.h"
#include "IND_Animation.h"
#include "IND_Font.h"
#include "IND_Entity2d.h"
#include "../../WorkingPath.h"

/*
==================
Main
==================
*/
int IndieLib()
{
    //Sets the working path as the 'exe' directory. All resource paths are relative to this directory
	if (!WorkingPathSetup::setWorkingPathFromExe(NULL)) {
		std::cout<<"\nUnable to Set the working path !";
	}
	
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;									

	// ----- Surface loading -----

	// Loading Background
	IND_Surface *mSurfaceBack = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBack, "../../resources/twist.jpg", IND_OPAQUE, IND_32)) return 0;

	// Loading Rocket
	IND_Surface *mSurfaceRocket = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceRocket, "../../resources/rocket.png", IND_ALPHA, IND_32)) return 0;

	// Loading Beetleship
	IND_Surface *mSurfaceBeetle = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBeetle, "../../resources/beetleship.png", IND_ALPHA, IND_32)) return 0;
	
	// Sword Master animation, we apply a color key of (0, 255, 0)
	IND_Animation *mSwordMasterAnimation = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mSwordMasterAnimation, "../../resources/animations/sword_master.xml", IND_ALPHA, IND_16, 0, 255, 0)) return 0; //TODO: setAlpha in Ind_Image fails.

	// ----- Font loading -----

	// Font
	IND_Font *mFontSmall = IND_Font::newFont();
	if (!mI->_fontManager->add(mFontSmall, "../../resources/font_small.png", "../../resources/font_small.xml", IND_ALPHA, IND_32)) return 0;

	// ----- Set the surfaces into 2d entities -----

	// Creating 2d entity for the background
	IND_Entity2d *mBack = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mBack);					// Entity adding
	mBack->setSurface(mSurfaceBack);					// Set the surface into the entity

	// Creating 2d entity for the Rocket
	IND_Entity2d *mRocket = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mRocket);					// Entity adding
	mRocket->setSurface(mSurfaceRocket);					// Set the surface into the entity

	// Creating 2d entity for the Beetleship
	IND_Entity2d *mBeetle = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mBeetle);					// Entity adding
	mBeetle->setSurface(mSurfaceBeetle);					// Set the surface into the entity

	// Creating 2d entity for the Sword Master animation
	IND_Entity2d *mSwordMaster = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mSwordMaster);				// Entity adding
	mSwordMaster->setAnimation(mSwordMasterAnimation);			// Set the animation into the entity

	// Text small white
	IND_Entity2d *mTextSmallWhite = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mTextSmallWhite);				// Entity adding
	mTextSmallWhite->setFont(mFontSmall);					// Set the font into the entity

	// ----- Changing the attributes of the 2d entities -----

	// Background
	mBack->setHotSpot(0.5f, 0.5f);
	mBack->setPosition(400, 300, 0);
	mBack->setScale(1.7f, 1.7f);

	// Beetle
	mBeetle->setHotSpot(0.5f, 0.5f);
	mBeetle->setMirrorX(1);
	mBeetle->setBoundingTriangle("beetle_head", 160, 105, 160, 170, 190, 135);
	mBeetle->setBoundingCircle("beetle_boy_head", 85, 52, 55);

	// Rocket
	mRocket->setHotSpot(0.5f, 0.5f);
	mRocket->setPosition(200, 450, 1);
	mRocket->setBoundingAreas("../../resources/rocket_collisions.xml");

	// Sword Master Animation
	mSwordMaster->setHotSpot(0.5f, 0.5f);
	mSwordMaster->setPosition(400, 220, 3);

	// Text
	mTextSmallWhite->setLineSpacing	(18);
	mTextSmallWhite->setCharSpacing	(-7);
	mTextSmallWhite->setPosition(400, 30, 10);
	mTextSmallWhite->setAlign(IND_CENTER);

	// ----- Main Loop -----

	float mAngle = 0;
	float mScale = 1.0f;
	int mSpeedRotation = 5;
	int mSpeedScaling = 1;
	float mDelta;

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Input ----

		mDelta = mI->_render->getFrameTime() / 1000.0f;

		if (mI->_input->isKeyPressed(IND_KEYRIGHT)){
			mScale += mSpeedScaling * mDelta;
		}
		if (mI->_input->isKeyPressed(IND_KEYLEFT)){
			mScale -= mSpeedScaling * mDelta;
		}

		mAngle += mSpeedRotation * mDelta;
		
		if (mScale < 0){
			mScale = 0;
		}

		// ----- Updating entities attributes  -----

		mBack->setAngleXYZ(0, 0, -mAngle);
		mRocket->setAngleXYZ (0, 0, mAngle);
		mRocket->setScale (mScale, mScale);	
		mBeetle->setPosition((float) mI->_input->getMouseX(), (float) mI->_input->getMouseY(), 5);

		// ----- Check collisions -----

		mTextSmallWhite->setText("No collision between the groups we are checking");

		if (mI->_entity2dManager->isCollision(mRocket, "engines", mBeetle, "beetle_boy_head")){
			mTextSmallWhite->setText("Collision between rocket boy head and engines");
		}	

		if (mI->_entity2dManager->isCollision(mRocket, "rocket_head", mBeetle, "beetle_head")){ 
			mTextSmallWhite->setText("Collision between rokect head and beetle head");
		}	
		
		if (mI->_entity2dManager->isCollision(mRocket, "rocket_boy_head", mBeetle, "beetle_boy_head")){ 
			mTextSmallWhite->setText("Collision between rocket boy head and beetle boy head");	
		}

		if (mI->_entity2dManager->isCollision(mBeetle, "beetle_boy_head", mSwordMaster, "sword")){ 
			mTextSmallWhite->setText("Collision between beetle boy head and the sword");
		}

		// ----- Render  -----

		mI->_render->beginScene();
		mI->_render->clearViewPort(0, 0, 0);
		mI->_entity2dManager->renderEntities2d();
		mI->_entity2dManager->renderCollisionAreas(255, 0, 0, 255);
		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}
