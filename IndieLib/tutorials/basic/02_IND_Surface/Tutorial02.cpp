/*****************************************************************************************
 * Desc: Tutorial 02 IND_Surface
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"
#include "IND_Surface.h"
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
	if (!mI->_surfaceManager->add(mSurfaceBack, "../../resources/blue_background.jpg", IND_OPAQUE, IND_32)) return 0;

	// Loading sprite of a warrior
	IND_Surface *mSurfaceWarrior = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceWarrior, "../../resources/derekyu_sprite.png", IND_ALPHA, IND_32, 255, 0, 255)) return 0; // TODO: This calls setAlpha in IND_Image, which is broken, please fix.

	// Loading sprite of a star
	IND_Surface *mSurfaceStar = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceStar, "../../resources/star.png", IND_ALPHA, IND_32)) return 0;


	// ----- Set the surfaces into 2d entities -----
	
	// Creating 2d entity for the background
	IND_Entity2d *mBack = IND_Entity2d::newEntity2d();				
	mI->_entity2dManager->add(mBack);					// Entity adding
	mBack->setSurface(mSurfaceBack);					// Set the surface into the entity

	// Creating 2d entity for the warrior
	IND_Entity2d *mWarrior = IND_Entity2d::newEntity2d();				
	mI->_entity2dManager->add(mWarrior);					// Entity adding
	mWarrior->setSurface(mSurfaceWarrior);					// Set the surface into the entity

	// Creating 2d entity for the star 1
	IND_Entity2d *mStar1 = IND_Entity2d::newEntity2d();		
	mI->_entity2dManager->add(mStar1);					// Entity adding
	mStar1->setSurface(mSurfaceStar);					// Set the surface into the entity

	// Creating 2d entity for the star 2 (big and a bit orange)
	IND_Entity2d *mStar2 = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mStar2);					// Entity adding
	mStar2->setSurface(mSurfaceStar);					// Set the surface into the entity

	// Creating 2d entity for the star 3 
	IND_Entity2d *mStar3 = IND_Entity2d::newEntity2d();					
	mI->_entity2dManager->add(mStar3);					// Entity adding
	mStar3->setSurface(mSurfaceStar);					// Set the surface into the entity

	// ----- Changing the attributes of the 2d entities -----

	// Warrior
	mWarrior->setPosition(400, 170, 0);					// Set the position of the entity

	// Original Star without chaning it's attributes
	mStar1->setPosition(100, 270, 0);					// Set the position of the entity
	
	// We change the attributes of this entity in order 
	// to create a big rotated semitransparent star with 
	// an orange tint
	mStar2->setPosition(280, 200, 0);					// Set the position of the entity
	mStar2->setScale(2, 2);							// Set the scale of the entity
	mStar2->setTint(240, 160, 230);						// Set tint to color RGB = (240, 160, 230)
	mStar2->setTransparency(200);						// Level of transparency 200 (255 will be opaque)
	mStar2->setAngleXYZ(0, 0, 45);						// Rotation in Z angle = 45º

	// A bigger star than the original, faded to pink.
	// We only draw a region of 50x50 pixels
	mStar3->setHotSpot(0.5f, 0.5f);						// We change the reference point of the entity
	mStar3->setPosition(400, 470, 0);					// Set the position of the entity
	mStar3->setScale(1.5f, 1.5f);						// Set the scale of the entity
	mStar3->setFade(230, 0, 230, 128);					// Set fade to pink color, the amout of fade is 128 (255 will be complety pink)
	mStar3->setRegion(20, 20, 50, 50);					// Region we want to draw


	// ----- Main Loop -----

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		mI->_input->update();
		mI->_render->beginScene();
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}
