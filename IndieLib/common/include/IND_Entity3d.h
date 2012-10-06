/*****************************************************************************************
 * File: IND_Entity3d.h
 * Desc: Entity 3d object
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

#ifndef _IND_ENTITY3D_
#define _IND_ENTITY3D_

// ----- Includes -----
#include "IND_3dMesh.h"

// ----- Forward Declarations -----

class IND_SurfaceManager;

// --------------------------------------------------------------------------------
//									 IND_Entity3d
// --------------------------------------------------------------------------------

/**
@defgroup IND_Entity3d IND_Entity3d
@ingroup EntityManagers
3d entity class for drawing graphical objects into the screen and manipulating their attributes, click in ::IND_Entity3d to see all the methods of this class.
*/
/*@{*/

/**
@b ::IND_Entity3d is an 3d entity object of the class ::IND_Entity3dManager. This class, together with ::IND_Entity2d (object
of ::IND_Entity2dManager), are the most important classes of IndieLib.

An entity is an object that can have a graphical object associated to it. In this case this graphical objects
are 3d meshes. Using the methods of this class you will be able to change
the attributes of that 3d meshes.

For example, in a game, a 3d monster can be a ::IND_Entity3d that has a ::IND_3dMesh associated to it. You can have as many monsters
(::IND_3dMesh objects) in the screen as you want, all of them sharing the same ::IND_3dMesh (the 3d model). In system memory you will
have only one 3d mesh loaded, the rest will be instances. The cool thing is that you can
change the attributes of each of these different ::IND_Entity3d objects. For example, you can change the size, rotation, color or transparency
of the monsters. So, having only one 3d mesh, you can have lot of different monsters in the screen, with different sizes, colors, positions, etc.
*/
class LIB_EXP IND_Entity3d {
public:

	// ----- Init -----

	IND_Entity3d() {
		initAttrib();
	}

	// ----- Public sets ------

	/** @name Graphical objects assignation to the 3d entity
	*
	*/
	//@{
	void set3dMesh(IND_3dMesh *pNewMesh);
	//@}

	/** @name Showing
	*
	*/
	//@{
	void    setShow(bool pShow);
	//@}

	/** @name Animations
	*
	*/
	//@{
	bool    setSequence(int pS);
	void    setAnimationSpeed(float pSpeed);
	void    setTransitionSpeed(float pSpeed);
	//@}

	/** @name Space transformations
	*
	*/
	//@{
	void    setPosition(float pX, float pY, float pZ);
	void    setAngleXYZ(float pAnX, float pAnY, float pAnZ);
	void    setScale(float pSx, float pSy, float pSz);
	void    setFilter(IND_Filter pF);
	//@}

	/** @name Color transformations, blending and back face culling
	*
	*/
	//@{
	void    setBackCull(bool pCull);
	void    flipNormals(bool pFlipNormals);
	void    setTint(BYTE pR, BYTE pG, BYTE pB, BYTE pA);
	void    setTransparency(BYTE pA);
	void    setFade(BYTE pR, BYTE pG, BYTE pB, BYTE pA);
	void    setBlendSource(IND_BlendingType pSo);
	void    setBlendDest(IND_BlendingType pDs);
	//@}


	// ----- Public gets ------

	/** @name Gets
	*
	*/
	//@{
	//! If the entity has a surface assigned, it returns a pointer to this surface.
	IND_3dMesh      *get3dMesh()      {
		return _me;
	}
	//! Returns true if the entity is being showed, false if not
	bool    isShow()      {
		return _show;
	}
	//! Returns the sequence number that has been assigned to the animation.
	int     getSequence()      {
		return _sequence;
	}
	//! Return the animation speed value
	float   getAnimationSpeed()      {
		return _me->GetAnimationSpeed();
	}
	//! Return the transition speed value
	float   getTransitionSpeed()      {
		return _me->GetTransitionSpeed();
	}
	//! Returns X position of the entity.
	float       getPosX()      {
		return _x;
	}
	//! Returns Y position of the entity.
	float       getPosY()      {
		return _y;
	}
	//! Returns Z position of the entity.
	float       getPosZ()      {
		return _z;
	}
	//! Returns the angle in the X axis of the entity.
	float   getAngleX()      {
		return _angleX;
	}
	//! Returns the angle in the Y axis of the entity.
	float   getAngleY()      {
		return _angleY;
	}
	//! Returns the angle in the Z axis of the entity.
	float   getAngleZ()      {
		return _angleZ;
	}
	//! Returns the X scale of the entity.
	float   getScaleX()      {
		return _scaleX;
	}
	//! Returns the Y scale of the entity.
	float   getScaleY()      {
		return _scaleY;
	}
	//! Returns the Z scale of the entity.
	float   getScaleZ()      {
		return _scaleZ;
	}
	//! Indicates if the entity is making backface culling 0 = No, 1 = Yes.
	bool    getBackCull()      {
		return _cull;
	}
	//! Returns the type of filter ::IND_Filter which uses the graphical object assignated to the entity.
	IND_Filter  getFilter()      {
		return _filter;
	}
	//! Returns the tint level in the R (red) channel of the entity.
	BYTE    getTintR()      {
		return _r;
	}
	//! Returns the tint level in the G (green) channel of the entity.
	BYTE    getTintG()      {
		return _g;
	}
	//! Returns the tint level in the B (blue) channel of the entity.
	BYTE    getTintB()      {
		return _b;
	}
	//! Returns the transparency level of the entity
	BYTE    getTransparency()      {
		return _a;
	}
	//! Returns the fade level in R (red) channel of the entity.
	BYTE    getFadeR()      {
		return _fadeR;
	}
	//! Returns the fade level in G (green) channel of the entity.
	BYTE    getFadeG()      {
		return _fadeG;
	}
	//! Returns the fade level in B (blue) channel of the entity.
	BYTE    getFadeB()      {
		return _fadeB;
	}
	//! Returns the fade level of the entity.
	BYTE    getFadeA()      {
		return _fadeA;
	}
	//! Returns the blending type ::IND_BlendingType for the source
	IND_BlendingType    getBlendSource()      {
		return _so;
	}
	//! Returns the blending type ::IND_BlendingType for the destination
	IND_BlendingType    getDestSource()      {
		return _ds;
	}
	//@}

private:

	// ----- Private ------

	// ----- Entity attributes -----

	IND_3dMesh      *_me;       // Pointer to a mesh

	// Show
	bool _show;                 // Flag for showing / hiding the entity

	// Space transformation attributes
	bool _flipNormals;          // Flip normals 0 / 1 of / off
	float _x;                   // x Coordinate
	float _y;                   // y Coordinate
	float _z;                   // z Coordinate
	float _angleX;              // x Axis angle
	float _angleY;              // y Axis angle
	float _angleZ;              // z Axis angle
	float _scaleX;              // x Scaling
	float _scaleY;              // y Scaling
	float _scaleZ;              // z Scaling
	bool _cull;                 // Back Face Culling (1 = Activated, 0 = Deactivated)
	IND_Filter _filter;         // Tipo filtro (0 = Nearest Point || 1 = Linear)
	IND_Matrix  _mat;           // Global Matrix

	// Color, transperency and fading attributes
	BYTE _r;                    // Tint R
	BYTE _g;                    // Tint G
	BYTE _b;                    // Tint B
	BYTE _a;                    // Tint A
	BYTE _fadeR;                // Fade to color R
	BYTE _fadeG;                // Fade to color G
	BYTE _fadeB;                // Fade to color B
	BYTE _fadeA;                // Fade to color A (quantity of fading)
	IND_BlendingType _so;       // Blending Type
	IND_BlendingType _ds;       // Blending Source

	// Animations attributes
	bool _changeSequenceFlag;   // Activated when changing the sequence
	int _sequence;              // Number of sequences to draw

	// ----- Private methods -----

	void    initAttrib();

	// ----- Friends -----

	friend class IND_Entity3dManager;
};
/*@}*/

#endif // _IND_ENTITY3D_

