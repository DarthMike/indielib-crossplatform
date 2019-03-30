/*****************************************************************************************
 * File: EditorMap.h
 * Desc: EditorMap class, for loading and savin maps and creating, cloning and deleting nodes
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

#ifndef _EDITORMAP_
#define _EDITORMAP_

// ------ Includes -----

#include <vector>
#include "dependencies/tinyxml/tinyxml.h"
#include "Resources.h"
#include "Node.h"
#include "CIndieLib.h"
#include "Defines.h"
using namespace std; 


// --------------------------------------------------------------------------------
//								     	EditorMap
// --------------------------------------------------------------------------------

class EditorMap
{
public:

	EditorMap										();	
	void						Free				();

	void						CreateNode			(int pX, int pY, int pZ, int pId, int pLayer, IND_Surface *pSurface);
	void						DeleteNode			(Node *pNode);
	void						CloneNode			(Node *pNode);
	bool						SaveMap				(char *pTilesetPath);
	char						*GetPathToTileset	();
	bool						LoadMap				(Resources *pResources);
	vector		<Node*>			*GetVectorNodes		()	{ return &mVectorNodes; }
	void						FreeMap				();

private:

	CIndieLib *mI;								// IndieLib pointer
	vector <Node*> mVectorNodes;				// Map nodes

	// ----- Private methods -----

	char		*OpenFileDialog				(char *pFilter, bool pAction, char *pTitle);
};

#endif // _EDITORMAP_
