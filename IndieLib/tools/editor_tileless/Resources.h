/*****************************************************************************************
 * File: Resources.h
 * Desc: Resources class, for loading the initial tileset of brushes and the editor graphics
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

#ifndef _RESOURCES_
#define _RESOURCES_

// ------ Includes -----

#include "CIndieLib.h"
#include "dependencies/tinyxml/tinyxml.h"
#include "Defines.h"
#include "IND_Font.h"
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
