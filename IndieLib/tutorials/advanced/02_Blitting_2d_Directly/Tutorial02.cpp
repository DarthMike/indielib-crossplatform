/*****************************************************************************************
 * Desc: Tutorial b) 02 Blitting 2d directly
 *****************************************************************************************/

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


#include "CIndieLib.h"
#include "IND_Surface.h"
#include "IND_Animation.h"
//#include "IND_Matrix.h"
#include "../../WorkingPath.h"


/*
==================
Main
==================
*/
Indielib_Main
{
    //Sets the working path at the resources directory. Resources paths are relative to that directory
	if (!WorkingPathSetup::setWorkingPath(WorkingPathSetup::resourcesDirectory())) {
		std::cout<<"\nUnable to Set the working path !";
	}
	
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;		

	// ----- Surface loading -----

	// Loading draco
	IND_Surface *mSurfaceDraco = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceDraco, "draco.png", IND_ALPHA, IND_32)) return 0;

	// Loading gem (this image has a blue rectangle surronding it)
	IND_Surface *mSurfaceGem = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceGem, "gem_squared.png", IND_ALPHA, IND_32)) return 0;

	// Loading bug
	IND_Surface *mSurfaceBug = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBug, "Enemy Bug.png", IND_ALPHA, IND_32)) return 0;

	// ----- Animation loading -----

	IND_Animation *mAnimationUfo = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationUfo, "animations/ufo.xml", IND_ALPHA, IND_32)) return 0;

	// ----- Main Loop -----

	int mWidth, mHeight;
	float mAngle = 0.0f;
	float mHotSpotX, mHotSpotY;
	int mAxisCalX, mAxisCalY;
	IND_Matrix *mMatrix = new IND_Matrix();
	float mSpeed = 50;
	float mDelta;

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Delta -----

		mDelta = mI->_render->getFrameTime() / 1000.0f;

		// ----- Render  -----

		mI->_render->beginScene();
		mI->_render->clearViewPort(60, 60, 60);


		// --------------------------------------------------------------------------------
		//					Blitting Draco directly (IND_Surface object) 
		// --------------------------------------------------------------------------------

		// 1) We apply the world space transformation (translation, rotation, scaling).
		// If you want to recieve the transformation in a single matrix you can pass
		// and IND_Matrix object by reference.

		mWidth = mSurfaceDraco->getWidth();
		mHeight = mSurfaceDraco->getHeight();
		mI->_render->setTransform2d(50,					// x pos
									70,					// y pos
									0,					// Angle x	
									0,					// Angle y
									0,					// Angle z
									1,					// Scale x
									1,					// Scale y
									0,					// Axis cal x
									0,					// Axis cal y
									0,					// Mirror x
									0,					// Mirror y
									mWidth,				// Width
									mHeight,			// Height
									mMatrix);			// Matrix in wich the transformation will be applied (optional)			
 
		// 2) We apply the color, blending and culling transformations.
		mI->_render->setRainbow2d(IND_ALPHA,								// IND_Type
									1,					// Back face culling 0/1 => off / on
									0,					// Mirror x
									0,					// Mirror y
									IND_FILTER_LINEAR,	// IND_Filter
									255,				// R Component	for tinting
									255,				// G Component	for tinting
									255,				// B Component	for tinting			
									255,				// A Component	for tinting
									0,					// R Component	for fading to a color		
									0,					// G Component	for fading to a color		
									0,					// B Component	for fading to a color			
									255,				// Amount of fading	
									IND_SRCALPHA,		// IND_BlendingType (source)
									IND_INVSRCALPHA);	// IND_BlendingType (destination)


		// 3) Blit the IND_Surface
		mI->_render->blitSurface(mSurfaceDraco);


		// --------------------------------------------------------------------------------
		//		Blitting Bug directly (IND_Surface object with a HotSpot and rotating)
		// --------------------------------------------------------------------------------

		// 1) We apply the world space transformation (translation, rotation, scaling).
		// If you want to recieve the transformation in a single matrix you can pass
		// and IND_Matrix object by reference.

		mAngle += mSpeed *mDelta;
		mHotSpotX = 0.5f;
		mHotSpotY = 0.5f;
		mWidth = mSurfaceBug->getWidth();
		mHeight = mSurfaceBug->getHeight();
		mAxisCalX = (int) (mHotSpotX * mWidth * -1);
		mAxisCalY = (int) (mHotSpotY * mHeight * -1);

		mI->_render->setTransform2d(500,				// x pos
									100,				// y pos
									0,					// Angle x	
									0,					// Angle y
									mAngle,				// Angle z
									1,					// Scale x
									1,					// Scale y
									mAxisCalX,			// Axis cal x
									mAxisCalY,			// Axis cal y
									0,					// Mirror x
									0,					// Mirror y
									mWidth,				// Width
									mHeight,			// Height
									mMatrix);			// Matrix in wich the transformation will be applied (optional)			
 
		// 2) We apply the color, blending and culling transformations.
		mI->_render->setRainbow2d(IND_ALPHA,			// IND_Type
									1,					// Back face culling 0/1 => off / on
									0,					// Mirror x
									0,					// Mirror y
									IND_FILTER_LINEAR,	// IND_Filter
									255,				// R Component	for tinting
									255,				// G Component	for tinting
									255,				// B Component	for tinting			
									255,				// A Component	for tinting
									0,					// R Component	for fading to a color		
									0,					// G Component	for fading to a color		
									0,					// B Component	for fading to a color			
									255,				// Amount of fading	
									IND_SRCALPHA,		// IND_BlendingType (source)
									IND_INVSRCALPHA);	// IND_BlendingType (destination)


		// 3) Blit the IND_Surface
		mI->_render->blitSurface(mSurfaceBug);


		// --------------------------------------------------------------------------------
		//				Blitting a region of Gem directly (IND_Surface region)
		// --------------------------------------------------------------------------------

		// 1) We apply the world space transformation (translation, rotation, scaling).
		// If you want to recieve the transformation in a single matrix you can pass
		// and IND_Matrix object by reference.

		mWidth = mSurfaceGem->getWidth();
		mHeight = mSurfaceGem->getHeight();
		mI->_render->setTransform2d(600,				// x pos
									200,				// y pos
									0,					// Angle x	
									0,					// Angle y
									0,					// Angle z
									1,					// Scale x
									1,					// Scale y
									0,					// Axis cal x
									0,					// Axis cal y
									0,					// Mirror x
									0,					// Mirror y
									mWidth,				// Width
									mHeight,			// Height
									mMatrix);			// Matrix in wich the transformation will be applied (optional)			
 
		// 2) We apply the color, blending and culling transformations.
		mI->_render->setRainbow2d(IND_ALPHA,								// IND_Type
									1,					// Back face culling 0/1 => off / on
									0,					// Mirror x
									0,					// Mirror y
									IND_FILTER_LINEAR,	// IND_Filter
									255,				// R Component	for tinting
									255,				// G Component	for tinting
									255,				// B Component	for tinting			
									255,				// A Component	for tinting
									0,					// R Component	for fading to a color		
									0,					// G Component	for fading to a color		
									0,					// B Component	for fading to a color			
									255,				// Amount of fading	
									IND_SRCALPHA,		// IND_BlendingType (source)
									IND_INVSRCALPHA);	// IND_BlendingType (destination)


		// 3) Blit the IND_Surface
		mI->_render->blitRegionSurface(mSurfaceGem, 10, 10, 70, 70);


		// --------------------------------------------------------------------------------
		//				Blitting a tiled region of Gem directly (IND_Surface wrap)
		// --------------------------------------------------------------------------------

		// 1) We apply the world space transformation (translation, rotation, scaling).
		// If you want to recieve the transformation in a single matrix you can pass
		// and IND_Matrix object by reference.

		mWidth = mSurfaceGem->getWidth();
		mHeight = mSurfaceGem->getHeight();
		mI->_render->setTransform2d(500,							// x pos
									350,				// y pos
									0,					// Angle x	
									0,					// Angle y
									0,					// Angle z
									1,					// Scale x
									1,					// Scale y
									0,					// Axis cal x
									0,					// Axis cal y
									0,					// Mirror x
									0,					// Mirror y
									mWidth,				// Width
									mHeight,			// Height
									mMatrix);			// Matrix in wich the transformation will be applied (optional)			
 
		// 2) We apply the color, blending and culling transformations.
		mI->_render->setRainbow2d(IND_ALPHA,								// IND_Type
									1,					// Back face culling 0/1 => off / on
									0,					// Mirror x
									0,					// Mirror y
									IND_FILTER_LINEAR,	// IND_Filter
									255,				// R Component	for tinting
									255,				// G Component	for tinting
									255,				// B Component	for tinting			
									255,				// A Component	for tinting
									0,					// R Component	for fading to a color		
									0,					// G Component	for fading to a color		
									0,					// B Component	for fading to a color			
									255,				// Amount of fading	
									IND_SRCALPHA,		// IND_BlendingType (source)
									IND_INVSRCALPHA);	// IND_BlendingType (destination)


		// 3) Blit the IND_Surface
		mI->_render->blitWrapSurface(mSurfaceGem, 200, 200, 0, 0);


		// --------------------------------------------------------------------------------
		//				Blitting a ufo animation directly (IND_Animation object)
		// --------------------------------------------------------------------------------

		// 1) We apply the world space transformation (translation, rotation, scaling).
		// If you want to recieve the transformation in a single matrix you can pass
		// and IND_Matrix object by reference.

		mWidth = mAnimationUfo->getActualSurface(0)->getWidth();
		mHeight= mAnimationUfo->getActualSurface(0)->getHeight();
		mI->_render->setTransform2d(650,								// x pos
									70,					// y pos
									0,					// Angle x	
									0,					// Angle y
									0,					// Angle z
									1,					// Scale x
									1,					// Scale y
									0,					// Axis cal x
									0,					// Axis cal y
									0,					// Mirror x
									0,					// Mirror y
									mWidth,				// Width
									mHeight,			// Height
									mMatrix);			// Matrix in wich the transformation will be applied (optional)			
 
		// 2) We apply the color, blending and culling transformations.
		mI->_render->setRainbow2d(IND_ALPHA,								// IND_Type
									1,					// Back face culling 0/1 => off / on
									0,					// Mirror x
									0,					// Mirror y
									IND_FILTER_LINEAR,	// IND_Filter
									255,				// R Component	for tinting
									255,				// G Component	for tinting
									255,				// B Component	for tinting			
									255,				// A Component	for tinting
									0,					// R Component	for fading to a color		
									0,					// G Component	for fading to a color		
									0,					// B Component	for fading to a color			
									255,				// Amount of fading	
									IND_SRCALPHA,		// IND_BlendingType (source)
									IND_INVSRCALPHA);	// IND_BlendingType (destination)


		// 3) Blit the IND_Animation looping
		if (mI->_render->blitAnimation(mAnimationUfo, 0, 0, 0, 0, 0, 0, 0, 0) == -1)
			mAnimationUfo->setActualFramePos (0, 0);

		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	delete mMatrix;

	return 0;
}
