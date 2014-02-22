/*****************************************************************************************
 * File: EditorMap.cpp
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

// ------ Includes -----

#include "EditorMap.h"


/* 
======================================									
Init
====================================== 
*/
EditorMap::EditorMap() 
{
	// Get IndieLib instante
	mI = CIndieLib::instance();
}


/* 
======================================									
Free memory
====================================== 
*/
void EditorMap::Free() 
{
	// Free all the map nodes
	FreeMap ();
}


/* 
======================================									
Create node
====================================== 
*/
void EditorMap::CreateNode (int pX, int pY, int pZ, int pId, int pLayer, IND_Surface *pSurface)
{
	Node *mNode = new Node (pX, pY, pZ, pId, pLayer, pSurface);
	mNode->GetEntity()->setPosition ((float) pX, (float) pY, 0);
	mVectorNodes.push_back (mNode);
}


/* 
======================================									
Clone node
====================================== 
*/
void EditorMap::CloneNode (Node *pNode)
{
	// Create a new node
	Node *mNewNode = new Node	((int) pNode->GetEntity()->getPosX(), 
								(int) pNode->GetEntity()->getPosY(), 
								(int) pNode->GetEntity()->getPosZ() - 1, 
								pNode->GetSurfaceId(),
								pNode->GetLayer(), 
								pNode->GetEntity()->getSurface());

	// Copy attributes from the source entity (original node) to the new cloned entity (new node)
	mNewNode->GetEntity()->setAngleXYZ (0, 0, pNode->GetEntity()->getAngleZ());
	mNewNode->GetEntity()->setScale (pNode->GetEntity()->getScaleX(), pNode->GetEntity()->getScaleX());
	mNewNode->GetEntity()->setTransparency (pNode->GetEntity()->getTransparency());
	mNewNode->GetEntity()->setMirrorX (pNode->GetEntity()->getMirrorX());
	mNewNode->GetEntity()->setMirrorY (pNode->GetEntity()->getMirrorY());
	mNewNode->GetEntity()->setTint (pNode->GetEntity()->getTintR(), pNode->GetEntity()->getTintG(), pNode->GetEntity()->getTintB());
	mNewNode->GetEntity()->toggleWrap (pNode->GetEntity()->ifWrap());
	mNewNode->GetEntity()->setRegion (0, 0, pNode->GetEntity()->getRegionWidth(), pNode->GetEntity()->getRegionHeight());

	// Add the node to the vector
	mVectorNodes.push_back (mNewNode);
}


/* 
======================================									
Delete node
====================================== 
*/
void EditorMap::DeleteNode (Node *pNode)
{
	vector <Node*>::iterator mIter;

	Node *mTemp;

	for (mIter  = mVectorNodes.begin();
		mIter != mVectorNodes.end();
		mIter++)
	{	
		if ((*mIter) == pNode)
		{	
			mTemp = *mIter;
			mIter = mVectorNodes.erase (mIter);
			delete mTemp;
			break;
		}	
	}
}


/* 
======================================									
Save the map (the vector of nodes containing each entity) to an XML file
====================================== 
*/
bool EditorMap::SaveMap (char *pTilesetPath)
{
	char *mPath = OpenFileDialog ("XML\0*.xml;", false, "Choose a location to save the map file");

	if (!strcmp (mPath, "")) 
	{
		delete [] mPath;
		return false;
	}

	TiXmlDocument mXmlDoc;

	// XML Declaration
 	TiXmlDeclaration *mDecl = new TiXmlDeclaration ("1.0", "", "");  
	mXmlDoc.LinkEndChild (mDecl);  
 
	// Map tag
	TiXmlElement *mXRoot = new TiXmlElement ("map");  
	mXmlDoc.LinkEndChild (mXRoot);  

	// Tileset
	TiXmlElement *mXTileset = new TiXmlElement ("tileset");  
	mXRoot->LinkEndChild (mXTileset);  

	char mCurrentDir[MAX_PATH];
	GetCurrentDirectory (MAX_PATH, mCurrentDir);									// Get current directory path
	mXTileset->SetAttribute("tileset_file", pTilesetPath + strlen (mCurrentDir));	// Only gets the relative path to the tileset

	// Iterate the nodes vector and write them to the XML
	TiXmlElement *mXNodes = new TiXmlElement ("nodes");   
	mXRoot->LinkEndChild (mXNodes); 

	vector <Node*>::iterator mIter;

	for (mIter  = mVectorNodes.begin();
		mIter != mVectorNodes.end();
		mIter++)
	{	
		TiXmlElement *mXNode;
		mXNode = new TiXmlElement ("node");  
		mXNode->SetAttribute ("surface_id", (*mIter)->GetSurfaceId());
		mXNode->SetAttribute ("x", (int) (*mIter)->GetEntity()->getPosX());
		mXNode->SetAttribute ("y", (int) (*mIter)->GetEntity()->getPosY());
		mXNode->SetAttribute ("z", (*mIter)->GetEntity()->getPosZ());
		mXNode->SetAttribute ("layer", (*mIter)->GetLayer());
		mXNode->SetDoubleAttribute ("angle", (*mIter)->GetEntity()->getAngleZ());
		mXNode->SetDoubleAttribute ("scale", (*mIter)->GetEntity()->getScaleX());
		mXNode->SetAttribute ("trans", (*mIter)->GetEntity()->getTransparency());
		mXNode->SetAttribute ("mirror_x", (*mIter)->GetEntity()->getMirrorX());
		mXNode->SetAttribute ("mirror_y", (*mIter)->GetEntity()->getMirrorY());
		mXNode->SetAttribute ("tint_r", (*mIter)->GetEntity()->getTintR());
		mXNode->SetAttribute ("tint_g", (*mIter)->GetEntity()->getTintG());
		mXNode->SetAttribute ("tint_b", (*mIter)->GetEntity()->getTintB());
		mXNode->SetAttribute ("if_wrap", (*mIter)->GetEntity()->ifWrap());
		mXNode->SetAttribute ("region_width", (*mIter)->GetEntity()->getRegionWidth());
		mXNode->SetAttribute ("region_height", (*mIter)->GetEntity()->getRegionHeight());
		mXNodes->LinkEndChild (mXNode); 		
	}

	// Save the map to an XML file
	mXmlDoc.SaveFile (mPath);  

	// Free memory (XML nodes are deleted by TiXmlDocument destructor)
	delete mPath;

	return true;
}


/* 
======================================									
Load a Map from a XML file and create a node vector
====================================== 
*/
bool EditorMap::LoadMap (Resources *pResources)
{
	char *mPath = OpenFileDialog ("XML\0*.xml;", true, "Choose a map file");

	if (!strcmp (mPath, "")) 
	{
		delete [] mPath;
		return true;
	}

	// Free current map
	FreeMap();

	// Initializa XML doc
	TiXmlDocument mXmlDoc (mPath);

	// Fatal error, cannot load
	if (!mXmlDoc.LoadFile())
	{
		return false;
	}

	// Document root
	TiXmlElement *mXMap = 0;
	mXMap = mXmlDoc.FirstChildElement("map");

	if (!mXMap)
	{
		// No "<map>" tag
		return false;
	}

	// ----------------- Parse tileset -----------------

	// Tileset
	TiXmlElement *mXTileset = 0;
	mXTileset = mXMap->FirstChildElement("tileset");

	if (!mXTileset)
	{
		// No "<tileset>" tag
		return false;
	}

	// Id
	if (mXTileset->Attribute("tileset_file"))
	{
		char mPath [MAX_PATH];
		GetCurrentDirectory (MAX_PATH, mPath);								// Get the path to the current directory
		strcat (mPath, (char *) mXTileset->Attribute("tileset_file"));		// Add the name of the tileset to the path
		if (!pResources->LoadTileset (mPath)) return 0;						// It tries to load the tiles
	}

	// ----------------- Parse nodes -----------------

	// Nodes
	TiXmlElement *mXNodes = 0;
	mXNodes = mXMap->FirstChildElement("nodes");

	if (!mXNodes)
	{
		// No "<nodes>" tag
		return false;
	}

	TiXmlElement *mXNode = 0;
	mXNode = mXNodes->FirstChildElement("node");

	if (!mXNode)
	{
		// No nodes to parse
		return false;
	}
	
	// Parse all the nodes

	while (mXNode)
	{
		// Parameters to parse
		int mSurfaceId, mX, mY, mZ, mLayer, mTrans, mTintR, mTintG, mTintB, mRegionWidth, mRegionHeight;
		bool mMirrorX, mMirrorY, mIfWrap;
		float mAngle, mScale;
	

		// Surface Id
		if (mXNode->Attribute("surface_id"))
		{
			mSurfaceId = atoi (mXNode->Attribute("surface_id"));
		}
		else
		{
			return false;
		}

		// Pos X
		if (mXNode->Attribute("x"))
		{
			mX = atoi (mXNode->Attribute("x"));
		}
		else
		{
			return false;
		}

		// Pos Y
		if (mXNode->Attribute("y"))
		{
			mY = atoi (mXNode->Attribute("y"));
		}
		else
		{
			return false;
		}

		// Pos Z
		if (mXNode->Attribute("z"))
		{
			mZ = atoi (mXNode->Attribute("z"));
		}
		else
		{
			return false;
		}

		// Layer
		if (mXNode->Attribute("layer"))
		{
			mLayer = atoi (mXNode->Attribute("layer"));
		}
		else
		{
			return false;
		}

		// Angle
		if (mXNode->Attribute("angle"))
		{
			mAngle = (float) atof (mXNode->Attribute("angle"));
		}
		else
		{
			return false;
		}

		// Scale
		if (mXNode->Attribute("scale"))
		{
			mScale = (float) atof (mXNode->Attribute("scale"));
		}
		else
		{
			return false;
		}

		// Transparency
		if (mXNode->Attribute("trans"))
		{
			mTrans = atoi (mXNode->Attribute("trans"));
		}
		else
		{
			return false;
		}

		// Mirror x
		if (mXNode->Attribute("mirror_x"))
		{
			mMirrorX = (bool) atoi (mXNode->Attribute("mirror_x"));
		}
		else
		{
			return false;
		}

		// Mirror y
		if (mXNode->Attribute("mirror_y"))
		{
			mMirrorY = atoi (mXNode->Attribute("mirror_y"));
		}
		else
		{
			return false;
		}

		// Tint r
		if (mXNode->Attribute("tint_r"))
		{
			mTintR = atoi (mXNode->Attribute("tint_r"));
		}
		else
		{
			return false;
		}

		// Tint g
		if (mXNode->Attribute("tint_g"))
		{
			mTintG = atoi (mXNode->Attribute("tint_g"));
		}
		else
		{
			return false;
		}

		// Tint b
		if (mXNode->Attribute("tint_b"))
		{
			mTintB = atoi (mXNode->Attribute("tint_b"));
		}
		else
		{
			return false;
		}

		// If wrap
		if (mXNode->Attribute("if_wrap"))
		{
			mIfWrap = atoi (mXNode->Attribute("if_wrap"));
		}
		else
		{
			return false;
		}

		// Region width
		if (mXNode->Attribute("region_width"))
		{
			mRegionWidth = atoi (mXNode->Attribute("region_width"));
		}
		else
		{
			return false;
		}

		// Region height
		if (mXNode->Attribute("region_height"))
		{
			mRegionHeight = atoi (mXNode->Attribute("region_height"));
		}
		else
		{
			return false;
		}

		// Get the surface of the tile already loaded in Resources class by passing the Id
		IND_Surface *mSurface = pResources->GetSurfaceById(mSurfaceId);
		if (mSurface == (IND_Surface *) -1) return 0;

		// Node creation, we have to set the surface later
		Node *mNewNode = new Node (mX, mY, mZ, mSurfaceId, mLayer, mSurface);

		// Set new node attributes
		mNewNode->GetEntity()->setAngleXYZ (0, 0, mAngle);
		mNewNode->GetEntity()->setScale (mScale, mScale);
		mNewNode->GetEntity()->setTransparency ((BYTE) mTrans);
		mNewNode->GetEntity()->setMirrorX ((BYTE) mMirrorX);
		mNewNode->GetEntity()->setMirrorY ((BYTE) mMirrorY);
		mNewNode->GetEntity()->setTint ((BYTE) mTintR, (BYTE) mTintG, (BYTE) mTintB);
		mNewNode->GetEntity()->toggleWrap (mIfWrap);
		mNewNode->GetEntity()->setRegion (0, 0, mRegionWidth, mRegionHeight);

		// Push the node into the nodes vector
		mVectorNodes.push_back (mNewNode);
	
		// Move to the next declaration
		mXNode = mXNode->NextSiblingElement("node");
	}

	// Free memory, XML nodes are deleted by TiXmlDocument destructor
	delete mPath;

	return true;
}


/* 
======================================									
Gets the path to the tileset file that we are going to load
====================================== 
*/
char *EditorMap::GetPathToTileset ()
{
	char *mPath = OpenFileDialog ("XML\0*.xml;", true, "Choose a Tileset file");

	if (!strcmp (mPath, "")) 
	{
		delete [] mPath;
		return 0;
	}

	return mPath;
}


/* 
======================================									
Delete all the map nodes
====================================== 
*/
void EditorMap::FreeMap ()
{
	vector <Node*>::iterator mIter;

	for (mIter  = mVectorNodes.begin();
		mIter != mVectorNodes.end();
		++mIter)
	{
		mI->_entity2dManager->remove ((*mIter)->GetEntity());
		delete (*mIter);
	}

	mVectorNodes.clear();
}


// --------------------------------------------------------------------------------
//									Private methods
// --------------------------------------------------------------------------------

/*
==================
Open a Windows open / save dialog
pAction = if true => open dialog if false => save dialog
==================
*/
//TODO: THIS IS NOT PORTABLE!!!!
char *EditorMap::OpenFileDialog (char *pFilter, bool pAction, char *pTitle)
{	
	// If we are in full screen mode, we have to change to screen mode
	bool mKeepFullScreen = false;
	if (mI->_window->isFullScreen())
	{
		mI->_render->toggleFullScreen ();
		mKeepFullScreen = true;
	}

	OPENFILENAME mOpenFileName;
	char szFile[MAX_PATH];
	char mCurrentDir[MAX_PATH];

	szFile[0] = 0;
	GetCurrentDirectory (MAX_PATH, mCurrentDir);

	mOpenFileName.lStructSize = sizeof (OPENFILENAME);

	//FIXME: BROKEN IN WINDOWS BECAUSE OF CROSSPLATFORM CODE. NEED GOOD HANDLING OF FILE OPEN DIALOGS 
	//mOpenFileName.hwndOwner = mI->_window->getWind();
	mOpenFileName.lpstrFilter = pFilter;
	mOpenFileName.lpstrCustomFilter = NULL;
	mOpenFileName.nMaxCustFilter = 0;
	mOpenFileName.nFilterIndex = 0;
	mOpenFileName.lpstrFile = szFile;
	mOpenFileName.nMaxFile = sizeof( szFile );
	mOpenFileName.lpstrFileTitle = 0;
	mOpenFileName.nMaxFileTitle = 0;
	mOpenFileName.lpstrInitialDir = mCurrentDir;
	mOpenFileName.lpstrTitle = pTitle;
	mOpenFileName.nFileOffset = 0;
	mOpenFileName.nFileExtension = 0;
	mOpenFileName.lpstrDefExt = NULL;
	mOpenFileName.lCustData = 0;
	mOpenFileName.lpfnHook = NULL;
	mOpenFileName.lpTemplateName = NULL;
	mOpenFileName.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; // Keep the relative part intact.

	char *mResult = new char [MAX_PATH];

	if (pAction)
	{
		if (GetOpenFileName (&mOpenFileName))
		{
			if (mKeepFullScreen)	mI->_render->toggleFullScreen ();
			strcpy (mResult, szFile);
			return mResult;
		}
		else
			return "";
	}
	else
	{
		if (GetSaveFileName (&mOpenFileName))
		{
			if (mKeepFullScreen)	mI->_render->toggleFullScreen ();
			strcpy (mResult, szFile);
			return mResult;
		}
		else
			return "";
	}
}