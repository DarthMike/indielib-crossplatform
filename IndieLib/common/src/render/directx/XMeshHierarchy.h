/*****************************************************************************************
 * File: XMeshHierarchy.h
 * Desc: Modification of the Keith Ditchburn class for loading .x 3d models.
 *****************************************************************************************/

#ifndef _IND_XMESHHIERARCHY_
#define _IND_XMESHHIERARCHY_

// ----- Includes -----

#include <d3d9.h>
#include <d3dx9.h>
#include "XMeshStructures.h"

/** @cond DOCUMENT_PRIVATEAPI */
// --------------------------------------------------------------------------------
//									 XMeshHierarchy
// --------------------------------------------------------------------------------

/*
==================
This class defines a number of callbacks required by the D3DXLoadMeshHierarchyFromX function.
The required functions are defined by the abstract interface class: ID3DXAllocateHierarchy
==================
*/
class XMeshHierarchy : public ID3DXAllocateHierarchy {
public:

	char *_path;

	// Callback to create a D3DXFRAME extended object and initialize it
	STDMETHOD(CreateFrame)(LPCSTR Name, LPD3DXFRAME *retNewFrame);

	// Callback to create a D3DXMESHCONTAINER extended object and initialize it
	STDMETHOD(CreateMeshContainer)(LPCSTR Name, CONST D3DXMESHDATA *meshData,
	                               CONST D3DXMATERIAL *materials, CONST D3DXEFFECTINSTANCE *effectInstances,
	                               DWORD numMaterials, CONST DWORD *adjacency, LPD3DXSKININFO skinInfo,
	                               LPD3DXMESHCONTAINER *retNewMeshContainer);

	// Callback to release a D3DXFRAME extended object
	STDMETHOD(DestroyFrame)(LPD3DXFRAME frameToFree);

	// Callback to release a D3DXMESHCONTAINER extended object
	STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER meshContainerToFree);
};

#endif // _IND_XMESHHIERARCHY_

/** @endcond */
