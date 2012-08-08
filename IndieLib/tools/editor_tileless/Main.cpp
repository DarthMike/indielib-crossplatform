/*****************************************************************************************
 * Desc: No-tile editor tutorial
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

#include "CIndieLib.h"
#include "Resources.h"
#include "Map.h"
#include "Listener.h"

/*
==================
Main
==================
*/
int IndieLib()		
{
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init ()) return 0;	

	// ----- Editor classes initialization -----

	// Resource loading
	Resources mResources;
	char mPath [MAX_PATH];
	GetCurrentDirectory (MAX_PATH, mPath);			// Get the path to the current directory
	strcat (mPath, "../../resources/tileset_01.xml");		// Add the name of the tileset to the path
	if (!mResources.LoadResources (mPath)) exit (0);

	// Map initialization
	Map mMap;

	// Listener initialization
	Listener mListener (&mResources, &mMap);

	// ----- Main Loop -----

	while (!mI->_input->onKeyPress (IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input Update ----

		mI->_input->update ();

		// -------- Atributes -------

		mListener.Listen ();

		// -------- Render -------

		// Reset the counting of rendered and discarded objects
		mI->_render->resetNumDiscardedObjects();
		mI->_render->resetNumrenderedObject();

		mI->_render->beginScene ();

		// Render banckground (two triangles)
		mI->_render->blitColoredTriangle (0, 0, mI->_window->getWidth(), 0, 0, mI->_window->getHeight(), 
										255, 128, 128, 
										255, 128, 128, 
										27, 27, 204,
										255);

		mI->_render->blitColoredTriangle (0, mI->_window->getHeight(), mI->_window->getWidth(), 0, mI->_window->getWidth(), mI->_window->getHeight(), 
										27, 27, 204, 
										255, 128, 128, 
										27, 27, 204,
										255);

		// --- Render parallax layer B ---

		mI->_render->setCamera2d (mListener.GetCameraB());
		mI->_entity2dManager->renderEntities2d (0);

		// --- Render parallax layer N ---

		mI->_render->setCamera2d (mListener.GetCameraN());
		mI->_entity2dManager->renderEntities2d (1);

		// --- Render parallax layer M ---

		mI->_render->setCamera2d (mListener.GetCameraM());
		mI->_entity2dManager->renderEntities2d (2);

		// --- Render backdrop elements of Layers from 1 to 9 ---

		mI->_render->setCamera2d (mListener.GetCameraLayers());

		for (int i = 3; i < NUM_EDITOR_LAYERS; i++)
			mI->_entity2dManager->renderEntities2d (i);

		// --- Render editor elements (like the brush and areas) ---

		// If editing mode
		if (mListener.GetMode())
		{
			switch (mListener.GetCurrentLayer())
			{
				case 0:		mI->_render->setCamera2d (mListener.GetCameraB());		break;
				case 1:		mI->_render->setCamera2d (mListener.GetCameraN());		break;
				case 2:		mI->_render->setCamera2d (mListener.GetCameraM());		break;
				default:	mI->_render->setCamera2d (mListener.GetCameraLayers());		break;
			}

			// Render
			mI->_entity2dManager->renderEntities2d (BRUSH_LAYER);

			// Render the collision areas of the working layer
			mI->_entity2dManager->renderCollisionAreas (mListener.GetCurrentLayer(), 255, 255, 255, 30);
			mI->_entity2dManager->renderGridAreas (mListener.GetCurrentLayer(), 255, 0, 0, 255);
		}

		// --- Render texts ---

		// Render gui elements (text, mouse cursor)
		mI->_render->setCamera2d (mListener.GetCameraGui());
		mI->_entity2dManager->renderEntities2d (GUI_LAYER);	

		// --- End Scene ---

		mI->_render->endScene ();

		//mI->_render->ShowFpsInWindowTitle();
	}

	// ----- Free memory (we don't use destructors becase IndieLib pointers would be pointing to null -----

	mListener.Free ();
	mResources.Free ();
	mMap.Free ();

	// ----- Indielib End -----

	mI->end ();

	return 0;
}
