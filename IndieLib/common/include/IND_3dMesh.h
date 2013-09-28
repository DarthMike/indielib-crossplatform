/*****************************************************************************************
 * File: IND_3dMesh.h
 * Desc: 3d Mesh object
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


#ifndef _IND_3DMESH_
#define _IND_3DMESH_

// ----- Includes -----

#include <d3d9.h>
#include <d3dx9.h>
#include "XMeshStructures.h"

// --------------------------------------------------------------------------------
//									 IND_3dMesh
// --------------------------------------------------------------------------------

/**
@defgroup IND_3dMesh IND_3dMesh
@ingroup Objects
IND_3dMesh class managed by IND_3dMeshManager for rendering 3d meshes. Click in IND_3dMesh to see all the methods of this class.
*/
/**@{*/

/**
@b IND_3dMesh is an 3d mesh object from the class ::IND_3dMeshManager. Read the explanation in ::IND_3dMeshManager for having more details.
*/
class LIB_EXP IND_3dMesh {
public:

	// ----- Public sets ------

	void                setTransitionSpeed(float pSpeed);
	void                setAnimationSpeed(float pSpeed);

	// ----- Public gets ------

	//! Return the name of the loaded 3d Mesh
	char                *GetMeshName()  {
		return _3dMesh._attributes._name;
	}
	//! Return the number of the sequence that is being rendered at this moment
	unsigned int        GetCurrentSequence()  {
		return _3dMesh._currentAnimationSet;
	}
	//! Return the number of sequence animations of the 3d Mesh
	int                 getNumSequences()  {
		return _3dMesh._attributes._numSequences;
	}
	//! Return the animation speed value
	float               GetAnimationSpeed()  {
		return _3dMesh._speedAdjust;
	}
	//! Return the transition speed value
	float               GetTransitionSpeed()  {
		return _3dMesh._moveTransitionTime;
	}

private:

	// ----- Private ------
	/** @cond DOCUMENT_PRIVATEAPI */
	// Attributes
	struct structAttributes {
		char *_name;
		int _numSequences;

		structAttributes() {
			_name = 0;
			_numSequences = 0;
		}
	};
	typedef struct structAttributes ATTRIBUTES;

	// TYPE
	struct struct3dMesh {
		// Direct3D objects required for animation
		LPD3DXFRAME                 _frameRoot;
		LPD3DXANIMATIONCONTROLLER   _animController;
		D3DXMESHCONTAINER_EXTENDED *_firstMesh;

		// Bone data
		D3DXMATRIX *_boneMatrices;
		int _maxBones;

		// Animation variables
		unsigned int _currentAnimationSet;
		unsigned int _currentTrack;
		float _currentTime;
		float _speedAdjust;

		// The time to change from one animation set to another
		// To see how the merging works - increase this time value to slow it down
		float _moveTransitionTime;

		// Attributes
		ATTRIBUTES      _attributes;

		struct3dMesh() {
			_maxBones = _currentAnimationSet = _currentTrack = 0;
			_speedAdjust = 1.0f;
			_currentTime = 0.0f;
			_boneMatrices = 0;
			_moveTransitionTime = 0.25f;
			_firstMesh = 0;
		}
	};
	typedef struct struct3dMesh MESH3D;
	
	MESH3D _3dMesh;

	// ----- Friends -----

	friend class DirectXRender;
	friend class IND_3dMeshManager;
    /** @endcond */
};
/**@}*/

#endif // _IND_3DMESH_
