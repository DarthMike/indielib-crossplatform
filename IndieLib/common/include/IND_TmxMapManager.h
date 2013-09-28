/*****************************************************************************************
 * File: IND_TmxMapManager.h
 * Desc: TmxMap manager
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


#ifndef _IND_TMXMAPMANAGER_
#define _IND_TMXMAPMANAGER_

// ----- Includes -----
#include <list>

// ----- Forward declarations ----

class IND_TmxMap;
class IND_Surface;
class IND_Render;

// ----- Defines -----

#define MAX_EXT_TMXMAP 1


// --------------------------------------------------------------------------------
//									IND_ImageManager
// --------------------------------------------------------------------------------

/*!
\defgroup IND_ImageManager IND_ImageManager
\ingroup Managers
Manager of IND_Image objects. Click in ::IND_ImageManager to see all the methods of this class.
*/
/**@{*/

/*!
This manager that stores IND_Image objects works as a mediator between the image (bitmap) loaded
in memory and the IND_Surface object which finally appears in the screen. The IND_Image objects
which are stored in this class can not be rendered directly to the screen.
But this class is very useful for applying <b>filters</b> (like in Gimp or Photoshop programs) to
loaded images, and later they can be converted to IND_Surface objects, which can be directly
rendered.

Other possible uses of this class are:
- <b>Hardness maps</b>. Used for making collisions or path finding.
- GetPixel() / PutPixel()
- <b>Colorkey</b>. By changing a certain color into transparent using SetAlpha().
- Generation of new images (procedurally).
- Convert formats. Changing the number of bytes per pixel using Convert()
- Direct accesing to the bitmap. Using IND_Image::getPointer()
*/
class LIB_EXP IND_TmxMapManager {
public:

	// ----- Init/End -----

	IND_TmxMapManager(): _ok(false)  { }
	~IND_TmxMapManager()              {
		end();
	}

	bool    init(IND_Render* render);
	void    end();
	bool    isOK()  const {
		return _ok;
	}

	// ----- Public methods -----

	bool add(IND_TmxMap *pNewMap,const char *pName);
	bool remove(IND_TmxMap *pMap);
	bool save(IND_TmxMap *pMap,const char *pName);
	bool clone(IND_TmxMap *pNewMap, IND_TmxMap *pOldMap);
	//FIXME Tmx::Map* load(char *pName);
	bool add(IND_TmxMap *pNewMap, IND_TmxMap *pMapToBeCopied);

	void renderIsometricMap(IND_TmxMap *map,IND_Surface *mSurfaceTiles, int kMapCenterOffset);
	void renderOrthogonalMap(IND_TmxMap *orthogonalMap,IND_Surface *mSurfaceOrthogonalTiles, int kMapCenterOffset);
	void renderStaggeredMap(IND_TmxMap *staggeredMap,IND_Surface *mSurfaceStaggeredTiles, int kMapCenterOffset);

private:
	/** @cond DOCUMENT_PRIVATEAPI */
	// ----- Private -----
	IND_Render *_render;
	bool _ok;                                // Manager initialization flag
	char *_supportedExt [MAX_EXT_TMXMAP];    // File extensions supported

	// ----- Containers -----

	list <IND_TmxMap *> *_listMaps;

	// ----- Private Methods -----

	void getExtensionFromName(const char *pName,char* pMap);
	bool checkExtImage(const char *pMap);

	void addToList(IND_TmxMap *pNewMap);
	void delFromlist(IND_TmxMap *pMap);
	void writeMessage();
	void initVars();
	void freeVars();
    /** @endcond */
};
/**@}*/

#endif // _IND_TMXMAPMANAGER_
