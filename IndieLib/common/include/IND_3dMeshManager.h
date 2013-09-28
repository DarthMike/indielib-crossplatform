/*****************************************************************************************
 * File: IND_3dMeshManager.h
 * Desc: Manager of 3d mesh objects
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


#ifndef _IND_3DMESHMANAGER_
#define _IND_3DMESHMANAGER_

// ----- Includes -----

#include <list>
#include "XMeshStructures.h"

// ----- Forward Declarations -----
class IND_3dMesh;
class IND_Render;

// --------------------------------------------------------------------------------
//									IND_3dMeshManager
// --------------------------------------------------------------------------------

/**
@defgroup IND_3dMeshManager IND_3dMeshManager
@ingroup Managers
Manager of IND_3dMesh objects. Click in ::IND_3dMeshManager to see all the methods of this class.
*/
/**@{*/

/**
This class stores 3d meshes (IND_3dMesh) that can be inserted into a IND_Entity3d and rendered to the screen using
IND_Entity3dManager::renderEntities3d().
*/
class LIB_EXP IND_3dMeshManager {
public:

	// ----- Init/End -----

	IND_3dMeshManager(): _ok(false)  { }
	~IND_3dMeshManager()              {
		end();
	}

	bool    init(IND_Render *pRender);
	void    end();
	bool    isOK()           const {
		return _ok;
	}

	// ----- Public methods -----

	bool add(IND_3dMesh *pNew3dMesh, char *pName, char *pPath);
	bool remove(IND_3dMesh *pMe);

private:
	/** @cond DOCUMENT_PRIVATEAPI */
	// ----- Private Methods -----

	bool _ok;

	// ----- Objects -----

	IND_Render *_render;

	// ----- Containers -----

	list <IND_3dMesh *> *_list3dMesh;

	// ----- Private methods -----

	void setupBoneMatrices(D3DXFRAME_EXTENDED *pFrame, LPD3DXMATRIX pParentMatrix, IND_3dMesh *pNew3dMesh);

	void addToList(IND_3dMesh *pNew3dMesh);
	void delFromlist(IND_3dMesh *pMe);
	void writeMessage();
	void initVars();
	void freeVars();
    /** @endcond */
};
/**@}*/

#endif // _IND_3DMESHMANAGER_

