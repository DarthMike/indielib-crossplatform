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

// ------ Includes -----

#include "Resources.h"

/* 
======================================									
Init
====================================== 
*/
Resources::Resources() 
{
	// Get IndieLib instante
	mI = CIndieLib::instance();

	mMouseSurface = IND_Surface::newSurface();
	mMouseEntity = IND_Entity2d::newEntity2d();
	mFont = IND_Font::newFont();
	mFontEntity = IND_Entity2d::newEntity2d();

}


/* 
======================================									
End
====================================== 
*/
void Resources::Free() 
{
	// Free all the loaded tiles
	FreeTileset();

	// Note, the font bitmap and cursor are freed in Main.cpp calling to mI->End()
}


/* 
======================================									
Load Resources
====================================== 
*/
bool Resources::LoadResources (char *pTilesetFile)
{
	if (!LoadEditorElements())							return false;
	if (!LoadTileset (pTilesetFile))					return false;

	return true;
}


/* 
======================================									
Return the IndieLib surface that corresponds with the ID passed as parameter. It returns -1 if the ID is not found
====================================== 
*/
IND_Surface *Resources::GetSurfaceById (int pId)
{
	vector <SURFACEWITHID*>::iterator mIter;

	// Iterate the vector
	for (mIter  = mVectorTiles.begin();
		mIter != mVectorTiles.end();
		mIter++)
	{
		// Check if the ID is the correct one
		if ((*mIter)->mId == pId)
			return (*mIter)->mSurface;
	}

	// Returns error
	return (IND_Surface *) -1;
}


// --------------------------------------------------------------------------------
//									Private methods
// --------------------------------------------------------------------------------

/* 
======================================									
Load Resources
====================================== 
*/
bool Resources::LoadEditorElements ()
{
	// Load the mouse pointer, it is loaded to and IndieLib surface
	if (!mI->_surfaceManager->add (mMouseSurface, "../../resources/images/editor/cursor.png", IND_ALPHA, IND_32)) return 0;
	
	// Add the Mouse entity to the IndieLib Entity Manager
	mI->_entity2dManager->add (BRUSH_LAYER, mMouseEntity);
	mMouseEntity->setSurface (mMouseSurface);

	// Create a collision bounding rectangle for the mouse. It will be used in order to be able to select 
	// the editor elements (by checking if there is a collision
	mMouseEntity->setBoundingRectangle ("cursor", 0, 0, 20, 20);
	mMouseEntity->setHotSpot (0.5f, 0.5f);

	// Font loading
	if (!mI->_fontManager->add (mFont, "../../resources/fonts/font_small.png", "../../resources/fonts/font_small.xml", IND_ALPHA, IND_32)) return 0;
	mI->_entity2dManager->add	(GUI_LAYER, mFontEntity);
	mFontEntity->setFont			(mFont);
	mFontEntity->setLineSpacing	(18);
	mFontEntity->setCharSpacing	(-8);
	mFontEntity->setPosition		(0, 0, 1);
	mFontEntity->setAlign		(IND_LEFT);

	return true;
}


/* 
======================================									
Parse file and load backdrop images (tiles)
====================================== 
*/
bool Resources::LoadTileset (char *pTilesetFile)
{
	// If there is a tileset already, free it
	if (!mVectorTiles.empty()) FreeTileset();

	strcpy (mTilesetPath, pTilesetFile);

	TiXmlDocument mXmlDoc (mTilesetPath);

	// Fatal error, cannot load
	if (!mXmlDoc.LoadFile())
	{
		return false;
	}

	// Document root
	TiXmlElement *mXResources = 0;
	mXResources = mXmlDoc.FirstChildElement("resources");

	if (!mXResources)
	{
		// No "<resources>" tag
		return false;
	}

	// ----------------- Parse surfaces from the file and load them into a vector -----------------

	// Surfaces
	TiXmlElement *mXSurfaces = 0;
	mXSurfaces = mXResources->FirstChildElement("surfaces");

	if (!mXSurfaces)
	{
		// No "<surfaces>" tag
		return false;
	}

	TiXmlElement *mXSurface = 0;
	mXSurface = mXSurfaces->FirstChildElement("surface");

	if (!mXSurface)
	{
		// No surfaces to parse
		return false;
	}
	
	// Parse all the surfaces
	char mFileName [1024];
	mFileName [0] = 0;

	while (mXSurface)
	{
		SURFACEWITHID *mNewSurface = new SURFACEWITHID;

		// Id
		if (mXSurface->Attribute("id"))
		{
			mNewSurface->mId = atoi (mXSurface->Attribute("id"));
		}
		else
		{
			delete mNewSurface;
			return false;
		}

		// Path to the image
		if (mXSurface->Attribute("image"))
		{
			strcpy (mFileName, mXSurface->Attribute("image"));	

			// Load surface
			if (!mI->_surfaceManager->add (mNewSurface->mSurface, mFileName, IND_ALPHA, IND_32)) return 0;
		}
		else
		{
			delete mNewSurface;
			return false;
		}

		// Push the surface into the surfaces vector
		mVectorTiles.push_back (mNewSurface);
	
		// Move to the next declaration
		mXSurface = mXSurface->NextSiblingElement("surface");
	}


	// Note: XML nodes are deleted by TiXmlDocument destructor

	return true;
}


/* 
======================================									
Free current tileset
====================================== 
*/
void  Resources::FreeTileset ()
{
	vector <SURFACEWITHID*>::iterator mIter;

	for (mIter  = mVectorTiles.begin();
		mIter != mVectorTiles.end();
		mIter++)
	{
		mI->_surfaceManager->remove((*mIter)->mSurface);
		delete (*mIter);
	}

	mVectorTiles.clear();
}
