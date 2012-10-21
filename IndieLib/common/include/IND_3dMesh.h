/*****************************************************************************************
 * File: IND_3dMesh.h
 * Desc: 3d Mesh object
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
