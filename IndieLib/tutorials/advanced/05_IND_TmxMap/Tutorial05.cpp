/*****************************************************************************************
 * Desc: Tutorials b) 05 IND_TmxMap
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"
#include "IND_TmxMap.h"
#include "IND_Surface.h"
#include "../../WorkingPath.h"

#include <math.h>
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
	//if (!mI->_tmxMapManager->add(map, "../../resources/tmx/example.tmx")) return 0; // orthogonal
    if (!mI->_tmxMapManager->add(map, "../../resources/tmx/isometric_grass_and_water.tmx")) return 0; // isometric
    
    
    /*
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
        printf("Tile Width: %d\n", tileset->GetTileWidth());
        printf("Tile Height: %d\n", tileset->GetTileHeight());

        
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
	*/
	
    IND_Surface *mSurfaceTiles = new IND_Surface();
    
    if (!mI->_surfaceManager->add(mSurfaceTiles, map->getImagePath(), IND_ALPHA, IND_32)) return 0;

    
    
    const Tmx::Tileset *tileset = map->getTmxMapHandle()->GetTileset(0);
    
    // Print tileset information.
    printf("Name: %s\n", tileset->GetName().c_str());
    printf("Margin: %d\n", tileset->GetMargin());
    printf("Spacing: %d\n", tileset->GetSpacing());
    printf("Tile Width: %d\n", tileset->GetTileWidth());
    printf("Tile Height: %d\n", tileset->GetTileHeight());
    
    const int tileWidth = tileset->GetTileWidth();
    const int tileHeight = tileset->GetTileHeight();
    const int tileMargin = tileset->GetMargin();
    const int tileSpacing = tileset->GetSpacing();
    
    
    //const int mWidth = mSurfaceTiles->getWidth();
    //const int mHeight = mSurfaceTiles->getHeight();
    
//    const int startX = 10;
//    const int startY = 20;
    
//    int sourceX;
//    int sourceY;
//    int sourceWidth;
//    int sourceHeight;
    
    
//    int destX;
//    int destY;
//    int tileGid;
    
    
    IND_Matrix *mMatrix = new IND_Matrix();
    
	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
        // ----- Input update ----
        
		mI->_input->update();
        
        
		// ----- Render  -----
        
		mI->_render->beginScene();
		mI->_render->clearViewPort(255, 60, 60);
        
        
        // at present time the loaded TMX is ISOMETRIC , later TODO: there needs to be draw code for the orthogonal
        
        const Tmx::Layer *layer = map->getTmxMapHandle()->GetLayer(0);  // TODO: later this need to be looped over
       
        for (int x = 0; x < layer->GetWidth(); ++x)
        {
            for (int y = 0; y < layer->GetHeight(); ++y)
            {
                // Get the tile's id.
                int CurTile = layer->GetTileGid(x, y);
                 printf("%03d ", CurTile);
                
                if(CurTile == 0)
                {
                    continue;
                }
                
                const Tmx::Tileset *tileset = map->getTmxMapHandle()->FindTileset(CurTile);
                
                //CurTile = tileset->GetFirstGid() + CurTile;
                CurTile--;
                
                int tileset_col = (CurTile % 4 /*Num_Of_Cols*/); // TODO the number of cols needs to be calculated from where?
                int tileset_row = (CurTile / 4 /*Num_Of_Cols*/);// TODO the number of cols needs to be calculated from where?
                
                int sourceX = (tileset->GetMargin() + (tileset->GetTileWidth() + tileset->GetSpacing()) * tileset_col);
                int sourceY = (tileset->GetMargin() + (tileset->GetTileHeight() + tileset->GetSpacing()) * tileset_row);
                int sourceWidth = tileset->GetTileWidth();
                int sourceHeight = tileset->GetTileHeight();
                
                int destX = ((x * tileset->GetTileWidth()  / 2  ) + (y * tileset->GetTileWidth() / 2  ) ) ;
                int destY = ((y * tileset->GetTileHeight() / 2 ) - (x * tileset->GetTileHeight() / 2  ) ) ;
                
               
         
		mI->_render->setTransform2d(destY + 800,        // x pos    TODO: FIND out why x and y needed to be switched to gain same map as in tiled
									destX - 200,        // y pos    TODO: FIND out why x and y needed to be switched to gain same map as in tiled
									0,					// Angle x
									0,					// Angle y
									0,					// Angle z
									1,					// Scale x
									1,					// Scale y
									0,					// Axis cal x
									0,					// Axis cal y
									0,					// Mirror x
									0,					// Mirror y
									0,		// Width
									0,		// Height
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
		mI->_render->blitRegionSurface(mSurfaceTiles, sourceX, sourceY, sourceWidth, sourceHeight); // 265, 199
        
            }
            
            printf("\n");
        }
        
        printf("\n");
        
        mI->_render->endScene();	        
        
	}

	// ----- Free -----

	mI->end();

	return 0;
}
