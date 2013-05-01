/*****************************************************************************************
 * Desc: Tutorials b) 05 IND_TmxMap
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"
#include "IND_TmxMap.h"
#include "../../WorkingPath.h"

#include <cstring>

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

	// ----- Map loading -----
	// TODO: map loading goes here...
       
	IND_TmxMap *map = IND_TmxMap::newTmxMap();
	if (!mI->_tmxMapManager->add(map, "example.tmx")) return 0;

			// Get a tileset.
		//const Tmx::Tileset *tileset = map->getTmxMapHandle()->GetTileset(0);

		// Print tileset information.
		printf("Name: %s\n", map->getTmxMapHandle()->GetTileset(0)->GetName().c_str());
		printf("Margin: %d\n", map->getTmxMapHandle()->GetTileset(0)->GetMargin());
		printf("Spacing: %d\n", map->getTmxMapHandle()->GetTileset(0)->GetSpacing());
		printf("Image Width: %d\n", map->getTmxMapHandle()->GetTileset(0)->GetImage()->GetWidth());
		printf("Image Height: %d\n", map->getTmxMapHandle()->GetTileset(0)->GetImage()->GetHeight());
		printf("Image Source: %s\n", map->getTmxMapHandle()->GetTileset(0)->GetImage()->GetSource().c_str());
		printf("Transparent Color (hex): %s\n", map->getTmxMapHandle()->GetTileset(0)->GetImage()->GetTransparentColor().c_str());




	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Text -----

//		strcpy(mText, "Press space to see the grid in action. This is really cool, isn't it?");
//		mTextSmallWhite->setText(mText);

		// ----- Input ----

		// Show / Hide the grid pressing "space"
		if (mI->_input->onKeyPress(IND_SPACE))
		{
//			if (mShowGrid){

//			}else{

//			}
		}

		// ----- Updating entities attributes  -----


		// ----- Render  -----

		mI->_render->beginScene();
		mI->_render->clearViewPort(0, 0, 0);
//		mI->_entity2dManager->renderEntities2d();
//		if (mShowGrid) mI->_entity2dManager->renderGridAreas(0, 0, 0, 255);
		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}
