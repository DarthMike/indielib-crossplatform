/*****************************************************************************************
 * Desc: Test c) 01 Alien BenchMark
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"
#include "CTerrain.h"
#include "CUfo.h"

#include "../../WorkingPath.h"
// #include "IND_Surface.h"
#include "IND_Font.h"
#include "IND_Animation.h"
// #include "IND_Entity2d.h"
// #include "IND_Camera2d.h"

#include <cstring>

#define X_TILES 8								// Horizontal number of tiles of the terrain
#define Y_TILES 6								// Vertical number of tiles of the terrain
#define NUM_HOLES 130								// Number of craters for the terrain
#define CAMERA_SPEED 300							// Camera speed
static const float K_ZOOMSPEED = 0.001f;

#define CAMERA_DISPLACE (CAMERA_SPEED * mI->_render->getFrameTime() / 1000.0f)	// Camera displacement
#define MOVE_TERRAIN_BY_MOUSE_AREA 20						// We move the terrain if the mouse is on this area

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
	if (!mI->init()) 
		return 0;			

	// ----- Surface loading -----

	// Loading cursor
	IND_Surface *mSurfaceCursor = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceCursor, "../../resources/cursor.png", IND_ALPHA, IND_32))
		return 0;

	// Loading tile for the terrain
	IND_Surface *mSurfaceTerrain = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceTerrain, "../../resources/tiled_terrain.png", IND_OPAQUE, IND_32))
		return 0;

	// Loading alien hole 1
	IND_Surface *mSurfaceHole1 = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceHole1, "../../resources/hole1.png", IND_ALPHA, IND_32)) 
		return 0;
	
	// Loading alien hole 2
	IND_Surface *mSurfaceHole2 = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceHole2, "../../resources/hole2.png", IND_ALPHA, IND_32)) 
		return 0;

	// Font
	IND_Font *mFontSmall = IND_Font::newFont();
	if (!mI->_fontManager->add(mFontSmall, "../../resources/font_small.png", "../../resources/font_small.xml", IND_ALPHA, IND_32)) 
		return 0;

	// ----- Animation loading -----

	IND_Animation *mAnimationUfo = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationUfo, "../../resources/animations/ufo.xml", IND_ALPHA, IND_32)) 
		return 0;

	// ----- Cursor creation -----

	IND_Entity2d *mCursor = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(1, mCursor);			// Entity adding in layer 1 (gui)	
	mCursor->setSurface(mSurfaceCursor);

	// ----- Terrain creation -----

	CTerrain *mTerrain = new CTerrain(X_TILES, Y_TILES, NUM_HOLES, mSurfaceTerrain, mSurfaceHole1, mSurfaceHole2);

	// ----- Font creation -----

	IND_Entity2d *mTextSmallWhite = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(1, mTextSmallWhite);		// Entity adding in layer 1 (gui)
	mTextSmallWhite->setFont(mFontSmall);			// Set the font into the entity

	mTextSmallWhite->setLineSpacing(18);
	mTextSmallWhite->setCharSpacing(-8);
	mTextSmallWhite->setPosition(15, 15, 4);
	mTextSmallWhite->setAlign(IND_LEFT);

	// ----- Ufo creation -----

	CUfo *mUfos = new CUfo[X_TILES * Y_TILES * 1024];

	int cont = 0;
	for (int i = 0; i < X_TILES * 512; i += 50) {
		for (int j = 0; j < Y_TILES * 512; j += 50, ++cont) {
			mUfos [cont].Init ((float) i, (float) j, mAnimationUfo);
		
		}
	}
		
	// ----- Camera ------

	// Camera used for moving along the terrain
	IND_Camera2d *mCameraTerrain = new IND_Camera2d(mI->_window->getWidth () / 2, mI->_window->getHeight() / 2);

	// Camera used for showing the menu options, fps, etc (this camera dosen't change)
	IND_Camera2d *mCameraGui = new IND_Camera2d(mI->_window->getWidth () / 2, mI->_window->getHeight() / 2);

	// ----- Main Loop -----

	float mZoom = 1.0f;
	float mAngle = 0;
	char mFpsString [512];
	char mFpsValueStr [15];
	char mRenderedObjectsStr [15];
	char mDiscardedObjectsStr [15];
	mFpsString [0] = 0;
	int mSpeedRotation = 50;
	float mDelta;

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Texts -----

		strcpy (mFpsString, "Fps: ");
		mI->_render->getFpsString(mFpsValueStr);
		strcat (mFpsString, mFpsValueStr);
		strcat (mFpsString, "\n");
		strcat (mFpsString, "Rendered: ");
		mI->_render->getNumrenderedObjectsString(mRenderedObjectsStr);
		strcat (mFpsString, mRenderedObjectsStr); 
		strcat (mFpsString, "\n"); 
		strcat (mFpsString, "Discarded: ");
		mI->_render->getNumDiscardedObjectsString(mDiscardedObjectsStr);
		strcat (mFpsString, mDiscardedObjectsStr);
		mTextSmallWhite->setText(mFpsString);
		

		// ----- Input ----

		// Delta
		mDelta = mI->_render->getFrameTime() / 1000.0f;

		// Cursor position
		mCursor->setPosition((float) mI->_input->getMouseX(), (float) mI->_input->getMouseY(), 3);

		// Change resolution to 640, 480 when pressing 1
		if (mI->_input->onKeyPress(IND_1))
		{
			//if (!mI->_render->reset("Alien BenchMark", 640, 480, 32, 0, 1)) return 0; //FIXME: this is broken - figure out how to use IND_Windowproperties correctly 
			mCameraGui->setPosition(mI->_window->getWidth() / 2, mI->_window->getHeight() / 2);
			mCameraTerrain->setPosition(mI->_window->getWidth() / 2, mI->_window->getHeight() / 2);
		}

		// Change resolution to 800, 600 when pressing 2
		if (mI->_input->onKeyPress(IND_2))
		{
			//if (!mI->_render->reset("Alien BenchMark", 800, 600, 32, 0, 1)) return 0;  //FIXME: this is broken - figure out how to use IND_Windowproperties correctly 
			mCameraGui->setPosition(mI->_window->getWidth() / 2, mI->_window->getHeight() / 2);
			mCameraTerrain->setPosition(mI->_window->getWidth() / 2, mI->_window->getHeight() / 2);
		}

		// Change resolution to 1024, 768 when pressing 3
		if (mI->_input->onKeyPress(IND_3))
		{
			//if (!mI->_render->reset("Alien BenchMark", 1024, 768, 32, 0, 1)) return 0;  //FIXME: this is broken - figure out how to use IND_Windowproperties correctly 
			mCameraGui->setPosition(mI->_window->getWidth() / 2, mI->_window->getHeight() / 2);
			mCameraTerrain->setPosition(mI->_window->getWidth() / 2, mI->_window->getHeight() / 2);
		}

		// Toogle full screen when pressing "space"
		if (mI->_input->onKeyPress(IND_SPACE)) mI->_render->toggleFullScreen();

		// Camera Zoom in / out
		if (mI->_input->isMouseScroll()) {
			mZoom += mI->_input->getMouseScrollY() * K_ZOOMSPEED;
		}		

		// Camera angle
		if (mI->_input->isMouseButtonPressed(IND_MBUTTON_LEFT))	mAngle += mSpeedRotation * mDelta;
		if (mI->_input->isMouseButtonPressed(IND_MBUTTON_RIGHT)) mAngle -= mSpeedRotation * mDelta;		

		// ----- Updating entities attributes  -----

		// Zooming and rotating the camera
		if (mZoom < 0.1f) mZoom =  0.1f;
		mCameraTerrain->setAngle(mAngle);
		mCameraTerrain->setZoom(mZoom);

		// Camera translation
		if (mI->_input->getMouseX() > mI->_window->getWidth() - MOVE_TERRAIN_BY_MOUSE_AREA) mCameraTerrain->setPosition(mCameraTerrain->getPosX() + (int) CAMERA_DISPLACE, mCameraTerrain->getPosY());
		if (mI->_input->getMouseX() < MOVE_TERRAIN_BY_MOUSE_AREA) mCameraTerrain->setPosition(mCameraTerrain->getPosX() - (int) CAMERA_DISPLACE, mCameraTerrain->getPosY());
		if (mI->_input->getMouseY() > mI->_window->getHeight()- MOVE_TERRAIN_BY_MOUSE_AREA) mCameraTerrain->setPosition(mCameraTerrain->getPosX(), mCameraTerrain->getPosY() + (int) CAMERA_DISPLACE);
		if (mI->_input->getMouseY() < MOVE_TERRAIN_BY_MOUSE_AREA) mCameraTerrain->setPosition(mCameraTerrain->getPosX(), mCameraTerrain->getPosY() - (int) CAMERA_DISPLACE);

		// ----- Render  -----

		mI->_render->beginScene();
		mI->_render->clearViewPort(0, 0, 0);
		
		// --- Layer 0 (terrain) ---

		mI->_render->resetNumDiscardedObjects();
		mI->_render->resetNumrenderedObject();
		mI->_render->setViewPort2d(0, 0, mI->_window->getWidth(), mI->_window->getHeight());
		mI->_render->setCamera2d(mCameraTerrain);
		mI->_entity2dManager->renderEntities2d(0);

		// --- Layer 1 (gui) ---

		mI->_render->setViewPort2d(0, 0, mI->_window->getWidth(), mI->_window->getHeight());
		mI->_render->setCamera2d(mCameraGui);
		mI->_render->blitFillRectangle(10, 10, 240, 80, 128, 128, 128, 128);
		mI->_render->blitRectangle(10, 10, 240, 80, 255, 255, 255, 128);
		mI->_entity2dManager->renderEntities2d(1);

		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();
	delete [] mUfos;

	return 0;
}
