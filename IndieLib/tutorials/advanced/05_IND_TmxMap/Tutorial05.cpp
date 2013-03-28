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
	   
	IND_TmxMap *map = new IND_TmxMap();
	if (!mI->_tmxMapManager->add(map, "../../resources/tmx/example.tmx")) return 0;

    
    
    // NOTE THE CODE SHOWN HERE IS DIRECTLY TAKEN FROM THE TmxParser example code

    // Iterate through the tilesets.
    for (int i = 0; i < map->getTmxMapHandle()->GetNumTilesets(); ++i) {
        printf("\n");
        printf("                                    \n");
        printf("           Tmx parsing:             \n");
        printf("                                    \n");
        printf("====================================\n");
        printf("Tileset : %02d\n", i);
        printf("====================================\n");
        
        // Get a tileset.
        const Tmx::Tileset *tileset = map->getTmxMapHandle()->GetTileset(i);
        
        // Print tileset information.
        printf("Name: %s\n", tileset->GetName().c_str());
        printf("Margin: %d\n", tileset->GetMargin());
        printf("Spacing: %d\n", tileset->GetSpacing());
        printf("Image Width: %d\n", tileset->GetImage()->GetWidth());
        printf("Image Height: %d\n", tileset->GetImage()->GetHeight());
        printf("Image Source: %s\n", tileset->GetImage()->GetSource().c_str());
        printf("Transparent Color (hex): %s\n", tileset->GetImage()->GetTransparentColor().c_str());
        
        if (tileset->GetTiles().size() > 0) {
            // Get a tile from the tileset.
            const Tmx::Tile *tile = *(tileset->GetTiles().begin());
            
            // Print the properties of a tile.
            std::map< std::string, std::string > list = tile->GetProperties().GetList();
            std::map< std::string, std::string >::iterator iter;
            for (iter = list.begin(); iter != list.end(); ++iter) {
                printf("%s = %s\n", iter->first.c_str(), iter->second.c_str());
            }
        }
    }

    // Iterate through the layers.
    for (int i = 0; i < map->getTmxMapHandle()->GetNumLayers(); ++i) {
        printf("                                    \n");
        printf("====================================\n");
        printf("Layer : %02d/%s \n", i, map->getTmxMapHandle()->GetLayer(i)->GetName().c_str());
        printf("====================================\n");
        
        // Get a layer.
        const Tmx::Layer *layer = map->getTmxMapHandle()->GetLayer(i);
        
        for (int y = 0; y < layer->GetHeight(); ++y) {
            for (int x = 0; x < layer->GetWidth(); ++x) {
                // Get the tile's id.
                printf("%03d", layer->GetTileGid(x, y));
                
                // Find a tileset for that id.
                const Tmx::Tileset *tileset = map->getTmxMapHandle()->FindTileset(layer->GetTileGid(x, y));
                if (layer->IsTileFlippedHorizontally(x, y)){
                    printf("h");
                }else{
                    printf(" ");
                }
                if (layer->IsTileFlippedVertically(x, y)){
                    printf("v");
                }else{
                    printf(" ");
                }
                if (layer->IsTileFlippedDiagonally(x, y)){
                    printf("d ");
                } else {
                    printf("  ");
                }
            }
            
            printf("\n");
        }
    }
    
    printf("\n\n");
    
    // Iterate through all of the object groups.
    for (int i = 0; i < map->getTmxMapHandle()->GetNumObjectGroups(); ++i) {
        printf("                                    \n");
        printf("====================================\n");
        printf("Object group : %02d\n", i);
        printf("====================================\n");
        
        // Get an object group.
        const Tmx::ObjectGroup *objectGroup = map->getTmxMapHandle()->GetObjectGroup(i);
        
        // Iterate through all objects in the object group.
        for (int j = 0; j < objectGroup->GetNumObjects(); ++j) {
            // Get an object.
            const Tmx::Object *object = objectGroup->GetObject(j);
            
            // Print information about the object.
            printf("Object Name: %s\n", object->GetName().c_str());
            printf("Object Position: (%03d, %03d)\n", object->GetX(), object->GetY());
            printf("Object Size: (%03d, %03d)\n", object->GetWidth(), object->GetHeight());
            
            // Print Polygon points.
            const Tmx::Polygon *polygon = object->GetPolygon();
            if (polygon != 0) {
                for (int i = 0; i < polygon->GetNumPoints(); i++) {
                    const Tmx::Point &point = polygon->GetPoint(i);
                    printf("Object Polygon: Point %d: (%d, %d)\n", i, point.x, point.y);
                }
            }
            
            // Print Polyline points.
            const Tmx::Polyline *polyline = object->GetPolyline();
            if (polyline != 0) {
                for (int i = 0; i < polyline->GetNumPoints(); i++) {
                    const Tmx::Point &point = polyline->GetPoint(i);
                    printf("Object Polyline: Point %d: (%d, %d)\n", i, point.x, point.y);
                }
            }
        }
    }
    
    delete map;




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
