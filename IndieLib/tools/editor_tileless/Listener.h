/*****************************************************************************************
 * Desc: Listener class, interface between the user and the editor using keyboard and
 * mouse Input
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

#ifndef _LISTENER_
#define _LISTENER_

// ----- Includes -----

#include "CIndieLib.h"
#include "Resources.h"
#include "Map.h"


// --------------------------------------------------------------------------------
//									    Listener
// --------------------------------------------------------------------------------

class Listener
{
public:

	Listener									(Resources *pResources, Map *mMap);
	void 					Free				();

	void					Listen				();
	IND_Camera2d			*GetCameraLayers	()						{ return mCameraLayers;			}
	IND_Camera2d			*GetCameraB			()						{ return mCameraB;				}
	IND_Camera2d			*GetCameraN			()						{ return mCameraN;				}
	IND_Camera2d			*GetCameraM			()						{ return mCameraM;				}
	IND_Camera2d			*GetCameraGui		()						{ return mCameraGui;			}

	int						GetCurrentLayer		()						{ return mLayer;				}	
	bool					GetMode				()						{ return mMode;					}


private:

	// Classes
	CIndieLib *mI;
	Resources *mResources;
	Map *mMap;

	// Time measurement
	float	mDelta;

	// Mouse position
	int mMouseX, mMouseY;

	// Screen dimensions
	int mScreenWidth, mScreenHeight;

	// Editing mode (true = editing, false = in game)
	bool mMode;

	// Current working layer (there are 13 layers) => b, n, m, 1-9, j (coded from 0 to 12; layers 13 and 14
	// are for the Brushes and the Gui Text
	int mLayer;

	// Cameras (one camera per each parallax layer)
	float mCameraZoomB, mCameraZoomN, mCameraZoomM, mCameraZoomLayers;
	IND_Camera2d *mCameraB;
	IND_Camera2d *mCameraN; 
	IND_Camera2d *mCameraM; 
	IND_Camera2d *mCameraLayers;
	float mCameraBX, mCameraBY;
	float mCameraNX, mCameraNY;
	float mCameraMX, mCameraMY;
	float mCameraLayersX, mCameraLayersY;
	int mCameraSpeedB, mCameraSpeedN, mCameraSpeedM, mCameraSpeedLayers;

	// Camera used for showing the menu options, fps, etc (this camera dosen't change)
	IND_Camera2d *mCameraGui;

	// Editor texts
	char mText [1024];
	char mModeName [64];
	char mLayerName [64];

	// Backdrops
	BRUSH *mBackDropBrushes; 			// Array of entities, one for each brush, for the editing mode
	int mNumBackDropBrushes;			// Number of backdrops images that we can use to draw as brushes

	// Backdrop brush
	float mPosBrushX, mPosBrushY;		// Position of the brush
	int mCurrentBackDropBrush; 			// Current backdrop image that could be dropped (brush image)

	// Backdrop node that is currently selected (the mouse is over it)
	Node *mBackDropNodeOver;

	// Backdrop editing flags
	bool mIsRotatingBackDrop;
	bool mIsTranslatingBackDrop;
	bool mIsScalingBackDrop;
	bool mIsTintingBackDrop;

	// ----- Private methods -----

	void ListenCommon					();
	void ListenEditing					();
	void ListenInGame					();
	void ChangeLayer					(int pNumLayer, char *pLayerName);
	Node *CheckMouseCollisions			(int pLayer);
	void ListenHoverBackDrop			();
	void ListenBackDropBrush			();
	void CreateBackDropBrushes			();
	void DeleteBackDropBrushes			();
	void ResetZoom						();
	bool ResetScreen					(int pScreenWidth, int pScreenHeight);
};
	
#endif // _LISTENER_