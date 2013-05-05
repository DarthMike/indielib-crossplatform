/*****************************************************************************************
 * Desc: Resources class, for loading the initial tileset of brushes and the editor graphics.
 *
 * gametuto.com - Javier López López (javilop.com)
 *
 *****************************************************************************************
 *
 * Creative Commons - Attribution 3.0 Unported
 * You are free:
 *	to Share — to copy, distribute and transmit the work
 *	to Remix — to adapt the work
 *
 * Under the following conditions:
 * Attribution. You must attribute the work in the manner specified by the author or licensor 
 * (but not in any way that suggests that they endorse you or your use of the work).
 *
 *****************************************************************************************/

#ifndef _RESOURCES_
#define _RESOURCES_

// ------ Includes -----

#include "CIndieLib.h"
#include "dependencies/tinyxml/tinyxml.h"
#include "Defines.h"
#include "Ind_Font.h"
#include <vector>
using namespace std; 


// --------------------------------------------------------------------------------
//									  Resources
// --------------------------------------------------------------------------------

class Resources
{
public:

	Resources											();	
	void Free											();

	bool					LoadResources				(char *pResourceFile);
	bool					LoadTileset					(char *pTilesetFile);

	IND_Entity2d			*GetMouseEntity				()							{ return mMouseEntity;			}
	IND_Entity2d			*GetFontEntity				()							{ return mFontEntity;			}
	char					*GetTilesetPath				()							{ return mTilesetPath;			}				
	vector <SURFACEWITHID*>		*GetVectorTiles				()							{ return &mVectorTiles;			}
	IND_Surface				*GetSurfaceById				(int pId);

private:

	// IndieLib pointer
	CIndieLib *mI;							
	
	// Vector of the surfaces (tiles) that can be used in the editor.
	vector <SURFACEWITHID*> mVectorTiles;	

	// Mouse pointer
	IND_Surface* mMouseSurface;
	IND_Entity2d* mMouseEntity;

	// Font
	IND_Font* mFont;
	IND_Entity2d* mFontEntity;

	// Current tileset location
	char mTilesetPath [MAX_PATH];

	// ----- Methods -----

	bool LoadEditorElements		();
	void FreeTileset			();
};

#endif // _RESOURCES_
