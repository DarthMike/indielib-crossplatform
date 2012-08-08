/*****************************************************************************************
 * File: IND_3dMeshManager.cpp
 * Desc: This class is used for managing 3D Meshes
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

// ----- Includes -----

#include "Global.h"
#include "Defines.h"
#include "render/directx/XMeshHierarchy.h"
#include "IND_3dMeshManager.h"
#include "IND_Render.h"
#include "IND_3DMesh.h"

// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 * Init. Returns 1 (true) if the administrator is successfully initialized.
 * Must be called before using any method.
 * @param pRender		TODO: describtion.
 */
bool IND_3dMeshManager::init(IND_Render *pRender) {
	end();
	initVars();

	// Checking IND_render
	if (pRender->isOK()) {
		g_debug->header("Checking IND_Render", 1);
		_render = pRender;
		_ok = true;
	} else {
		g_debug->header("IND_Render is not correctly initialized", 2);
		_ok = false;
		return _ok;
	}

	g_debug->header("Initializing IND_3dMeshManager", 5);
	g_debug->header("IND_3dMeshManager OK", 6);

	return _ok;
}


/**
 * Frees the manager and all the objects that it contains.
 */
void IND_3dMeshManager::end() {
	if (_ok) {
		// Free the subject if it has already loaded a mesh
		g_debug->header("Finalizing IND_3dMeshManager", 5);
		g_debug->header("Freeing 3d Meshes" , 5);
		freeVars();
		g_debug->header("3d meshes freed", 6);
		g_debug->header("IND_3dMeshManager finalized", 6);

		_ok = false;
	}
}


// --------------------------------------------------------------------------------
//									  Public methods
// --------------------------------------------------------------------------------

/**
 * Adds a 3d Mesh from a .x file. You can use ::IND_Entity3d in order to display the 3d mesh on the screen and to change its
 * attributes.
 * @param pNew3dMesh		Pointer to a new IND_3dMesh object.
 * @param pName			Name of the 3d mesh.
 * @param pPath			Path where the 3d mesh is stored.
 */
bool IND_3dMeshManager::add(IND_3dMesh *pNew3dMesh, char *pName, char *pPath) {
	g_debug->header("Loading 3d Mesh", 5);
	g_debug->header("File name:", 3);
	g_debug->dataChar(pName, 1);

	if (!_ok) {
		writeMessage();
		return 0;
	}

	// Create our mesh hierarchy class to control the allocation of memory - only used temporarily
	XMeshHierarchy *memoryAllocator = new XMeshHierarchy;
	memoryAllocator->_path = pPath;

	// This is the function that does all the .x file loading. We provide a pointer to an instance of our
	// memory allocator class to handle memory allocation during the frame and mesh loading
	//TODO: SOLVE DEPENDANCE IN DIRECT3D ONLY BY MOVING CODE SOMEWHERE ELSE?
	HRESULT hr = 0; //= D3DXLoadMeshHierarchyFromX (pName, D3DXMESH_MANAGED, _render->GetDevice(),
	//memoryAllocator, NULL, &pNew3dMesh->_3dMesh._frameRoot, &pNew3dMesh->_3dMesh._animController);

	delete memoryAllocator;
	memoryAllocator = 0;

	if (hr != D3D_OK) {
		g_debug->header("3d Mesh not found or error loading the mesh", 2);
		return 0;
	}

	// Name
	pNew3dMesh->_3dMesh._attributes._name = pName;

	// If the x file contains any animation remember how many sets there are
	if (pNew3dMesh->_3dMesh._animController) {
		pNew3dMesh->_3dMesh._attributes._numSequences = pNew3dMesh->_3dMesh._animController->GetMaxNumAnimationSets();
	}

	// Bones for skining
	if (pNew3dMesh->_3dMesh._frameRoot) {
		// Set the bones up
		setupBoneMatrices((D3DXFRAME_EXTENDED *) pNew3dMesh->_3dMesh._frameRoot, NULL, pNew3dMesh);

		// Create the bone matrices array for use during FrameMove to hold the final transform
		pNew3dMesh->_3dMesh._boneMatrices  = new D3DXMATRIX [pNew3dMesh->_3dMesh._maxBones];
		ZeroMemory(pNew3dMesh->_3dMesh._boneMatrices, sizeof(D3DXMATRIX)*pNew3dMesh->_3dMesh._maxBones);
	}

	// ----- Sets first sequence -----

	if (pNew3dMesh->_3dMesh._animController)
		_render->set3dMeshSequence(pNew3dMesh, 0);

	// ----- Puts the object into the manager -----

	addToList(pNew3dMesh);

	g_debug->header("3d Mesh loaded", 6);

	return 1;
}


/**
 * This function returns 1 (true) if the 3d Mesh object passed as parameter exists and is
 * deleted from the manager successfully.
 * @param pMe			Pointer to an 3d Mesh object.
 */
bool IND_3dMeshManager::remove(IND_3dMesh *pMe) {
	g_debug->header("Freeing 3d Mesh", 5);

	if (!_ok || !pMe) {
		writeMessage();
		return 0;
	}

	// Search object
	bool mIs = 0;
	list <IND_3dMesh *>::iterator _listIter;
	for (_listIter  = _list3dMesh->begin();
	        _listIter != _list3dMesh->end();
	        _listIter++) {
		if ((*_listIter) == pMe) {
			mIs = 1;
			break;
		}
	}

	// Not found
	if (!mIs) {
		writeMessage();
		return 0;
	}

	// ----- Free object -----

	g_debug->header("Freeing 3d mesh:", 3);
	g_debug->dataChar(pMe->GetMeshName(), 1);

	// Quit from list
	delFromlist(pMe);

	// Free mesh
	if (pMe->_3dMesh._animController) {
		pMe->_3dMesh._animController->Release();
		pMe->_3dMesh._animController = 0;
	}

	// free hierarchy
	if (pMe->_3dMesh._frameRoot) {
		// Create a mesh heirarchy class to control the removal of memory for the frame heirarchy
		XMeshHierarchy memoryAllocator;
		D3DXFrameDestroy(pMe->_3dMesh._frameRoot, &memoryAllocator);
		pMe->_3dMesh._frameRoot = 0;
	}

	// Free bones
	if (pMe->_3dMesh._boneMatrices) {
		DISPOSEARRAY(pMe->_3dMesh._boneMatrices);
	}

	// Free the pointer
	DISPOSE(pMe);

	g_debug->header("Ok", 6);

	return 1;
}


// --------------------------------------------------------------------------------
//									  Private methods
// --------------------------------------------------------------------------------

/**
 * Goes through the hierarchy and set the combined matrices calls recursively
 * @param pFrame			The current frame.
 * @param pParentMatrix			The parent frame matrix.
 */
void IND_3dMeshManager::setupBoneMatrices(D3DXFRAME_EXTENDED *pFrame, LPD3DXMATRIX pParentMatrix, IND_3dMesh *pNew3dMesh) {
	// Cast to our extended structure first
	D3DXMESHCONTAINER_EXTENDED *pMesh = (D3DXMESHCONTAINER_EXTENDED *) pFrame->pMeshContainer;

	// If this frame has a mesh
	if (pMesh) {
		// We need to remember which is the first mesh in the hierarchy for later when we
		// update (FrameMove)
		if (!pNew3dMesh->_3dMesh._firstMesh)
			pNew3dMesh->_3dMesh._firstMesh = pMesh;

		// if there is skin info, then setup the bone matrices
		if (pMesh->pSkinInfo && pMesh->MeshData.pMesh) {
			// Create a copy of the mesh to skin into later
			D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
			if (FAILED(pMesh->MeshData.pMesh->GetDeclaration(Declaration)))
				return;
			//TODO: SOLVE DEPENDANCE IN DIRECT3D ONLY BY MOVING CODE SOMEWHERE ELSE?
			//pMesh->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED,
			//  Declaration, _render->GetDevice(),
			//  &pMesh->exSkinMesh);

			// Max bones is calculated for later use (to know how big to make the bone matrices array)
			pNew3dMesh->_3dMesh._maxBones = max(pNew3dMesh->_3dMesh._maxBones, (int)pMesh->pSkinInfo->GetNumBones());

			// For each bone work out its matrix
			for (unsigned int i = 0; i < pMesh->pSkinInfo->GetNumBones(); i++) {
				// Find the frame containing the bone
				D3DXFRAME_EXTENDED *pTempFrame = (D3DXFRAME_EXTENDED *)D3DXFrameFind(pNew3dMesh->_3dMesh._frameRoot,
				                                 pMesh->pSkinInfo->GetBoneName(i));

				// set the bone part - point it at the transformation matrix
				pMesh->_exFrameCombinedMatrixPointer[i] = &pTempFrame->_exCombinedTransformationMatrix;
			}

		}
	}

	// Pass on to sibblings
	if (pFrame->pFrameSibling)
		setupBoneMatrices((D3DXFRAME_EXTENDED *)pFrame->pFrameSibling, pParentMatrix, pNew3dMesh);

	// Pass on to children
	if (pFrame->pFrameFirstChild)
		setupBoneMatrices((D3DXFRAME_EXTENDED *)pFrame->pFrameFirstChild, &pFrame->_exCombinedTransformationMatrix, pNew3dMesh);
}


/**
 * Inserts object into the manager.
 * @param pNew3dMesh			A IND_3dMesh that is going to be held by the manager.	
 */
void IND_3dMeshManager::addToList(IND_3dMesh *pNew3dMesh) {
	_list3dMesh->push_back(pNew3dMesh);
}


/**
 * Removes object from the manager.
 * @param pMe				The IND_3dMesh that is going to be removed from the manager.	
 */
void IND_3dMeshManager::delFromlist(IND_3dMesh *pMe) {
	_list3dMesh->remove(pMe);
	pMe = 0;
}


/**
 * Initialization error message.
 */
void IND_3dMeshManager::writeMessage() {
	g_debug->header("This operation can not be done", 3);
	g_debug->dataChar("", 1);
	g_debug->header("Invalid Id or IND_3dMeshManager not correctly initialized", 2);
}


/**
 * Init manager variables.
 */
void IND_3dMeshManager::initVars() {
	_list3dMesh = new list <IND_3dMesh *>;
}


/**
 * Delete manager variables.
 */
void IND_3dMeshManager::freeVars() {
	// Deletes all the manager entities
	list <IND_3dMesh *>::iterator _listIter;

	for (_listIter  = _list3dMesh->begin();
	        _listIter != _list3dMesh->end();
	        _listIter++) {
		g_debug->header("Freeing 3d mesh:", 3);
		g_debug->dataChar((*_listIter)->GetMeshName(), 1);

		// Free mesh
		if ((*_listIter)->_3dMesh._animController) {
			(*_listIter)->_3dMesh._animController->Release();
			(*_listIter)->_3dMesh._animController = 0;
		}

		// Free heirarchy
		if ((*_listIter)->_3dMesh._frameRoot) {
			// Create a mesh heirarchy class to control the removal of memory for the frame heirarchy
			XMeshHierarchy memoryAllocator;
			D3DXFrameDestroy((*_listIter)->_3dMesh._frameRoot, &memoryAllocator);
			(*_listIter)->_3dMesh._frameRoot = 0;
		}

		// Free bones
		if ((*_listIter)->_3dMesh._boneMatrices) {
			DISPOSEARRAY((*_listIter)->_3dMesh._boneMatrices);
		}
	}

	// Clear list
	_list3dMesh->clear();

	// Free list
	DISPOSE(_list3dMesh);
}
