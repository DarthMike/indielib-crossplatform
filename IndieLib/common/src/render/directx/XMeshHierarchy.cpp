/*****************************************************************************************
 * File: XMeshHierarchy.cpp
 * Desc: Modification of the Keith Ditchburn class for loading .x 3d models.
 *****************************************************************************************/

// ----- Includes -----

#include "XMeshHierarchy.h"
#include "Global.h"
#include <string>

// --------------------------------------------------------------------------------
//										Methods
// --------------------------------------------------------------------------------

/*
==================
Callback called when a new frame is encountered during the .x file load
Name - name of the frame
ppNewFrame - output pointer assign our newly created frame
==================
*/
HRESULT XMeshHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *retNewFrame) {
	// Always a good idea to initialise a return pointer before proceeding
	*retNewFrame = 0;

	// Create a new frame using the derived version of the structure
	D3DXFRAME_EXTENDED *newFrame = new D3DXFRAME_EXTENDED;
	ZeroMemory(newFrame, sizeof(D3DXFRAME_EXTENDED));

	// Now fill in the data members in the frame structure

	// Now initialize other data members of the frame to defaults
	D3DXMatrixIdentity(&newFrame->TransformationMatrix);
	D3DXMatrixIdentity(&newFrame->_exCombinedTransformationMatrix);

	newFrame->pMeshContainer = 0;
	newFrame->pFrameSibling = 0;
	newFrame->pFrameFirstChild = 0;

	// Assign the return pointer to our newly created frame
	*retNewFrame = newFrame;

	// The frame name (note: may be 0 or zero length)
	if (Name && strlen(Name)) {
		newFrame->Name = g_debug->duplicateCharString(Name);
		g_debug->header("Added frame:", 3);
		g_debug->dataChar((char *) Name, 1);
	} else {
		g_debug->header("Added frame:", 3);
		g_debug->dataChar("No name given", 1);
	}

	return S_OK;
}


/*
==================
Callback called when a mesh data is encountered during the .x file load
Name - name of the Mesh (const char*)
meshData - the mesh data
materials - material array
effectInstances - effect files / settings for the mesh
numMaterials - number of materials in the mesh
adjacency - adjacency array
pSkinInfo - skin info.
etNewMeshContainer - output pointer to assign our newly created mesh container
==================
*/
HRESULT XMeshHierarchy::CreateMeshContainer(
    LPCSTR Name,
    CONST D3DXMESHDATA *meshData,
    CONST D3DXMATERIAL *materials,
    CONST D3DXEFFECTINSTANCE *effectInstances,
    DWORD numMaterials,
    CONST DWORD *adjacency,
    LPD3DXSKININFO pSkinInfo,
    LPD3DXMESHCONTAINER *retNewMeshContainer) {
	// Create a mesh container structure to fill and initilaise to zero values
	// Note: I use my extended version of the structure (D3DXMESHCONTAINER_EXTENDED) defined in MeshStructures.h
	D3DXMESHCONTAINER_EXTENDED *newMeshContainer = new D3DXMESHCONTAINER_EXTENDED;
	ZeroMemory(newMeshContainer, sizeof(D3DXMESHCONTAINER_EXTENDED));

	// Always a good idea to initialise return pointer before proceeding
	*retNewMeshContainer = 0;

	// The mesh name (may be 0) needs copying over
	if (Name && strlen(Name)) {
		newMeshContainer->Name = g_debug->duplicateCharString(Name);
	} else {

	}

	// The mesh type (D3DXMESHTYPE_MESH, D3DXMESHTYPE_PMESH or D3DXMESHTYPE_PATCHMESH)
	if (meshData->Type != D3DXMESHTYPE_MESH) {
		// This demo does not handle mesh types other than the standard
		// Other types are D3DXMESHTYPE_PMESH (progressive mesh) and D3DXMESHTYPE_PATCHMESH (patch mesh)
		DestroyMeshContainer(newMeshContainer);
		return E_FAIL;
	}

	newMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	// Adjacency data - holds information about triangle adjacency, required by the ID3DMESH object
	DWORD dwFaces = meshData->pMesh->GetNumFaces();
	newMeshContainer->pAdjacency = new DWORD[dwFaces * 3];
	memcpy(newMeshContainer->pAdjacency, adjacency, sizeof(DWORD) * dwFaces * 3);

	// Get the Direct3D device, luckily this is held in the mesh itself (Note: must release it when done with it)
	LPDIRECT3DDEVICE9 pd3dDevice = 0;
	meshData->pMesh->GetDevice(&pd3dDevice);

	// Can just assign pointer and add a ref rather than need to clone
	newMeshContainer->MeshData.pMesh = meshData->pMesh;
	newMeshContainer->MeshData.pMesh->AddRef();

	// Create material and texture arrays. Note that I always want to have at least one
	newMeshContainer->NumMaterials = max(numMaterials, 1);
	newMeshContainer->_exMaterials = new D3DMATERIAL9[newMeshContainer->NumMaterials];
	newMeshContainer->_exTextures  = new LPDIRECT3DTEXTURE9[newMeshContainer->NumMaterials];

	ZeroMemory(newMeshContainer->_exTextures, sizeof(LPDIRECT3DTEXTURE9) * newMeshContainer->NumMaterials);

	if (numMaterials > 0) {
		// Load all the textures and copy the materials over
		for (DWORD i = 0; i < numMaterials; ++i) {
			newMeshContainer->_exTextures[i] = 0;
			newMeshContainer->_exMaterials[i] = materials[i].MatD3D;

			if (materials[i].pTextureFilename) {
				char mTexturePath [2048];
				mTexturePath [0] = 0;
				strcat(mTexturePath, _path);
				strcat(mTexturePath, "\\");
				strcat(mTexturePath, materials[i].pTextureFilename);

				// Use the D3DX function to load the texture
				if (FAILED(D3DXCreateTextureFromFile(pd3dDevice, mTexturePath,
				                                     &newMeshContainer->_exTextures[i]))) {
					g_debug->header("Could not load texture:", 3);
					g_debug->dataChar(mTexturePath, 1);
					return 0;
				}
			}
		}
	} else
		// make a default material in the case where the mesh did not provide one
	{
		ZeroMemory(&newMeshContainer->_exMaterials[0], sizeof(D3DMATERIAL9));
		newMeshContainer->_exMaterials[0].Diffuse.r = 0.5f;
		newMeshContainer->_exMaterials[0].Diffuse.g = 0.5f;
		newMeshContainer->_exMaterials[0].Diffuse.b = 0.5f;
		newMeshContainer->_exMaterials[0].Specular = newMeshContainer->_exMaterials[0].Diffuse;
		newMeshContainer->_exTextures[0] = 0;
	}


	// If there is skin data associated with the mesh copy it over
	if (pSkinInfo) {
		// save off the SkinInfo
		newMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		// Need an array of offset matrices to move the vertices from the figure space to the bone's space
		UINT numBones = pSkinInfo->GetNumBones();
		newMeshContainer->_exBoneOffsets = new D3DXMATRIX[numBones];

		// Create the arrays for the bones and the frame matrices
		newMeshContainer->_exFrameCombinedMatrixPointer = new D3DXMATRIX*[numBones];

		// get each of the bone offset matrices so that we don't need to get them later
		for (UINT i = 0; i < numBones; i++)
			newMeshContainer->_exBoneOffsets[i] = *(newMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i));

		g_debug->header("Mesh has skinning info. Number of bones is:", 3);
		g_debug->dataInt(numBones, 1);

		// Note: in the Microsoft samples a GenerateSkinnedMesh function is called here in order to prepare
		// the skinned mesh data for optimial hardware acceleration. As mentioned in the notes this sample
		// does not do hardware skinning but instead uses software skinning.
	} else {
		g_debug->header("Mesh has no skinning info", 1);

		// No skin info so 0 all the pointers
		newMeshContainer->pSkinInfo = 0;
		newMeshContainer->_exBoneOffsets = 0;
		newMeshContainer->_exSkinMesh = 0;
		newMeshContainer->_exFrameCombinedMatrixPointer = 0;
	}

	// When we got the device we caused an internal reference count to be incremented
	// So we now need to release it
	pd3dDevice->Release();

	// The mesh may contain a reference to an effect file
	if (effectInstances) {
		if (effectInstances->pEffectFilename)
			g_debug->header("This .x file references an effect file. Effect files are not handled by IndieLib", 1);
	}

	// Set the output mesh container pointer to our newly created one
	*retNewMeshContainer = newMeshContainer;

	return S_OK;
}


/*
==================
Callback called to deallocate the frame data
frameToFree - the frame to free
==================
*/
HRESULT XMeshHierarchy::DestroyFrame(LPD3DXFRAME frameToFree) {
	// Convert to our extended type. OK to do this as we know for sure it is:
	D3DXFRAME_EXTENDED *frame = (D3DXFRAME_EXTENDED *)frameToFree;

	if (frame->Name)
		delete []frame->Name;
	delete frame;

	return S_OK;
}


/*
==================
Callback called to deallocate the mesh container data
meshContainerBase - the mesh data to free
==================
*/
HRESULT XMeshHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER meshContainerBase) {
	// Convert to our extended type. OK as we know for sure it is:
	D3DXMESHCONTAINER_EXTENDED *meshContainer = (D3DXMESHCONTAINER_EXTENDED *)meshContainerBase;
	if (!meshContainer)
		return S_OK;

	// Name
	if (meshContainer->Name) {
		delete []meshContainer->Name;
		meshContainer->Name = 0;
	}

	// Material array
	if (meshContainer->_exMaterials) {
		delete []meshContainer->_exMaterials;
		meshContainer->_exMaterials = 0;
	}

	// Release the textures before deleting the array
	if (meshContainer->_exTextures) {
		for (UINT i = 0; i < meshContainer->NumMaterials; ++i) {
			if (meshContainer->_exTextures[i])
				meshContainer->_exTextures[i]->Release();
		}
	}

	// Texture array
	if (meshContainer->_exTextures)
		delete []meshContainer->_exTextures;

	// Adjacency data
	if (meshContainer->pAdjacency)
		delete []meshContainer->pAdjacency;

	// Bone parts
	if (meshContainer->_exBoneOffsets) {
		delete []meshContainer->_exBoneOffsets;
		meshContainer->_exBoneOffsets = 0;
	}

	// Frame matrices
	if (meshContainer->_exFrameCombinedMatrixPointer) {
		delete []meshContainer->_exFrameCombinedMatrixPointer;
		meshContainer->_exFrameCombinedMatrixPointer = 0;
	}

	// Release skin mesh
	if (meshContainer->_exSkinMesh) {
		meshContainer->_exSkinMesh->Release();
		meshContainer->_exSkinMesh = 0;
	}

	// Release the main mesh
	if (meshContainer->MeshData.pMesh) {
		meshContainer->MeshData.pMesh->Release();
		meshContainer->MeshData.pMesh = 0;
	}

	// Release skin information
	if (meshContainer->pSkinInfo) {
		meshContainer->pSkinInfo->Release();
		meshContainer->pSkinInfo = 0;
	}

	// Finally delete the mesh container itself
	delete meshContainer;
	meshContainer = 0;

	return S_OK;
}
