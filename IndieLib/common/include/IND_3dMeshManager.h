/*****************************************************************************************
 * File: IND_3dMeshManager.h
 * Desc: Manager de mallas 3d
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
MODIFIED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)

This library is free software; you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
Suite 330, Boston, MA 02111-1307 USA
*/

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

