/*****************************************************************************************
 * Desc: Tutorials b) 04 Several ViewPorts
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"
#include "IND_Surface.h"
#include "IND_Font.h"
#include "IND_Entity2d.h"
#include "../../WorkingPath.h"

#include <cstring>


static const float K_ZOOMSPEED = 0.001f;
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

	// Loading tile for the terrain
	IND_Surface *mSurfaceBack = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBack, "../../resources/twist.jpg", IND_OPAQUE, IND_32)) return 0;
	
	// Loading beetle
	IND_Surface *mSurfaceBeetle = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBeetle, "../../resources/beetleship.png", IND_ALPHA, IND_32)) return 0;
	
	// Loading octopus
	IND_Surface *mSurfaceOctopus = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceOctopus, "../../resources/octopus.png", IND_ALPHA, IND_32)) return 0;

	// Loading bug
	IND_Surface *mSurfaceBug = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBug, "../../resources/Enemy Bug.png", IND_ALPHA, IND_32)) return 0;

	// Font
	IND_Font *mFontSmall = IND_Font::newFont();
	if (!mI->_fontManager->add(mFontSmall, "../../resources/font_small.png", "../../resources/font_small.xml", IND_ALPHA, IND_32)) return 0;

	// ----- Font creation -----

	IND_Entity2d *mTextSmallWhite = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mTextSmallWhite);			// Entity adding
	mTextSmallWhite->setFont(mFontSmall);				// Set the font into the entity
	mTextSmallWhite->setLineSpacing(18);
	mTextSmallWhite->setCharSpacing(-8);
	mTextSmallWhite->setPosition(5, 5, 1);
	mTextSmallWhite->setAlign(IND_LEFT);

	// ----- Entities -----

	// Terrain
	IND_Entity2d *mTerrain = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mTerrain);
	mTerrain->setSurface(mSurfaceBack);

	// Beetle
	IND_Entity2d *mBeetle = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mBeetle);
	mBeetle->setSurface(mSurfaceBeetle);
	mBeetle->setHotSpot(0.5f, 0.5f);
	mBeetle->setPosition(150, 150, 2);

	// Octopus
	IND_Entity2d *mOctopus = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mOctopus);
	mOctopus->setSurface(mSurfaceOctopus);
	mOctopus->setHotSpot(0.5f, 0.5f);
	mOctopus->setPosition(450, 150, 2);

	// But
	IND_Entity2d *mBug = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mBug);
	mBug->setSurface(mSurfaceBug);
	mBug->setHotSpot(0.5f, 0.5f);
	mBug->setPosition(700, 150, 2);
	
	// ----- Camera ------

	// Camera for the viewport 1
	IND_Camera2d *mCamera1 = new IND_Camera2d(mI->_window->getWidth () / 2, mI->_window->getHeight() / 4);

	// Camera for the viewport 2
	IND_Camera2d *mCamera2 = new IND_Camera2d(mI->_window->getWidth () / 2, mI->_window->getHeight() / 4);

	// ----- Main Loop -----

	float mZoom = 1.0f;
	float mCameraAngle = 0;
	float mBugAngle = 0;
	char mText [2048];
	mText [0] = 0;
	float mSpeedRotation = 50;
	float mDelta;

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Text -----

		strcpy (mText, "Use mouse wheel for zooming in/out\n");
		strcat (mText, "Use mouse buttons for rotating the camera");
		mTextSmallWhite->setText(mText);	

		// ----- Input ----

		mDelta = mI->_render->getFrameTime() / 1000.0f;

		// Camera Zoom in / out
		if (mI->_input->isMouseScroll()) {
			mZoom += mI->_input->getMouseScrollY() * K_ZOOMSPEED;
		}	

		// Camera angle
		if (mI->_input->isMouseButtonPressed(IND_MBUTTON_LEFT))			mCameraAngle += mSpeedRotation * mDelta;
		if (mI->_input->isMouseButtonPressed(IND_MBUTTON_RIGHT))		mCameraAngle -= mSpeedRotation * mDelta;

		// ----- Updating entities attributes  -----

		// Rotation of the beetle and bug
		mBugAngle += mSpeedRotation * mDelta;
		mBeetle->setAngleXYZ(0, 0, mBugAngle);
		mBeetle->setAngleXYZ(0, 0, mBugAngle);
		mBug->setAngleXYZ(0, 0, -mBugAngle);
		mBug->setAngleXYZ(0, 0, -mBugAngle);

		// Zooming and rotating the camera
		if (mZoom < 0.1f) mZoom =  0.1f;
		mCamera2->setAngle(mCameraAngle);
		mCamera2->setZoom(mZoom);

		// ----- Render  -----

		// ----- Upper viewport -----

		mI->_render->beginScene();
		mI->_render->clearViewPort(60, 60, 60);
		mI->_render->setViewPort2d(0, 0, mI->_window->getWidth(), mI->_window->getHeight() / 2);
		mI->_render->setCamera2d(mCamera1);
		mI->_entity2dManager->renderEntities2d();

		// ----- Lower viewport -----

		mI->_render->setViewPort2d(0, mI->_window->getHeight() / 2, mI->_window->getWidth(), mI->_window->getHeight() / 2);
		mI->_render->setCamera2d(mCamera2);
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}

