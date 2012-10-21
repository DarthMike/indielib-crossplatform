/*****************************************************************************************
 * File: XMeshStructures.h
 * Desc: Modification of the Keith Ditchburn class for loading .x 3d models.
 *****************************************************************************************/

#ifndef _IND_XMESHSTRUCTURES_
#define _IND_XMESHSTRUCTURES_

// ----- Includes -----

#include <d3d9.h>
#include <d3dx9.h>


// --------------------------------------------------------------------------------
//								 XMesh Structures
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
==================
Structure derived from D3DXMESHCONTAINER and extended with some app-specific
info that will be stored with each mesh. To help detect which data is extended and which is
rom the base each variable begins with ex
==================
*/
struct D3DXMESHCONTAINER_EXTENDED: public D3DXMESHCONTAINER {
	// The base D3DXMESHCONTAINER has a pMaterials member which is a D3DXMATERIAL structure
	// that contains a texture filename and material data. It is easier to ignore this and
	// instead store the data in arrays of textures and materials in this extended structure:
	IDirect3DTexture9  **_exTextures;        // Array of texture pointers
	D3DMATERIAL9        *_exMaterials;       // Array of materials

	// Skinned mesh variables
	ID3DXMesh           *_exSkinMesh;            // The skin mesh
	D3DXMATRIX          *_exBoneOffsets;         // The bone matrix Offsets, one per bone
	D3DXMATRIX         **_exFrameCombinedMatrixPointer;  // Array of frame matrix pointers
};


/*
==================
Structure derived from D3DXFRAME and extended so we can add some app-specific
info that will be stored with each frame. To help detect which data is extended and which is
from the base each variable begins with ex
==================
*/
struct D3DXFRAME_EXTENDED: public D3DXFRAME {
	D3DXMATRIX _exCombinedTransformationMatrix;
};

/** @endcond */

#endif // _IND_XMESHSTRUCTURES_
