/*****************************************************************************************
 * File: IND_Entity2d.h
 * Desc: Entity 2d object
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

#ifndef _IND_ENTITY2D_
#define _IND_ENTITY2D_

// ----- Includes -----

#include "Defines.h"
#include <list>
#include "IND_Object.h"

// ----- Forward declarations -----
class CollisionParser;
class IND_Animation;
class IND_Surface;
class IND_Font;

// --------------------------------------------------------------------------------
//									 IND_Entity2d
// --------------------------------------------------------------------------------

/**
@defgroup IND_Entity2d IND_Entity2d
@ingroup EntityManagers
2d entity class managed by IND_Entity2dManager for drawing graphical objects into the screen and manipulating their attributes, click in ::IND_Entity2d to see all the methods of this class.
*/
/**@{*/

/**
@b IND_Entity2d is an 2d entity object of the class ::IND_Entity2dManager. This class, together with ::IND_Entity3d (object
of ::IND_Entity3dManager), are the most important classes of IndieLib.

An entity is an object that can have a graphical object associated to it. Using the methods of this class you will be able to change
the attributes of the graphical object that it contains.

For example, in a game, a bullet can be a ::IND_Entity2d that has a ::IND_Surface associated to it. You can have as many bullets
(::IND_Entity2d objects) in the screen as you want, all of them sharing the same IND_Surface (the sprite). In system memory
you will have only one 3d mesh loaded, the rest will be instances. The cool thing is that you can
change the attributes of each of these different ::IND_Entity2d objects. For example, you can change the size, rotation, color or transparency
of the bullets. So, having only one sprite, you can have lot of different bullets in the screen, with different sizes, colors, positions, etc.

IND_Entity2d can have so many different graphical objects associated to it:
- Surfaces (Sprites, scrolls, images, etc)
- Animations
- Fonts
- Primitives (lines, rectangles, circles, polys, etc)
*/
class LIB_EXP IND_Entity2d : public IND_Object {
public:

	// ----- Init -----
    static IND_Entity2d* newEntity2d();

    void destroy();
    
	// ----- Public sets ------

	/** @name Graphical objects assignation to the 2d entity
	*
	*/
	/**@{*/
	void    setSurface(IND_Surface *pNewSurface);
	void    setAnimation(IND_Animation *pNewAnimation);
	void    setPrimitive2d(IND_Primitive2d pPri2d);
	void    setFont(IND_Font *pFont);
	/**@}*/

	/** @name Showing
	*
	*/
	/**@{*/
	void    setShow(bool pShow);
	/**@}*/

	/** @name Animations
	*
	*/
	/**@{*/
	void    setSequence(unsigned int pSequence);
	void    setNumReplays(int pNumReplays);
	/**@}*/

	/** @name Primitives
	*
	*/
	/**@{*/
	void    setLine(int pX1, int pY1, int pX2, int pY2);
	void    setRectangle(int pX1, int pY1, int pX2, int pY2);
	void    setRadius(int pRadius);
	void    setNumSides(int pNumSides);
	void    setPolyAngle(float pPolyAngle);
	void    setPolyPoints(IND_Point *pPolyPoints);
	void    setNumLines(int pNumLines);
	/**@}*/

	/** @name Fonts
	*
	*/
	/**@{*/
	void    setAlign(IND_Align pAlign);
	void    setCharSpacing(int pCharSpacing);
	void    setLineSpacing(int pLineSpacing);
	void    setText(const char *pText);
	/**@}*/


	/** @name Space transformations (Some of these methods dont affect the fonts or the primitives)
	*
	*/
	/**@{*/
	void    setPosition(float pX, float pY, int pZ);
	void    setAngleXYZ(float pAnX, float pAnY, float pAnZ);
	void    setScale(float pSx, float pSy);
	void    setMirrorX(bool pMx);
	void    setMirrorY(bool pMy);
	void    setFilter(IND_Filter pF);
	bool    setHotSpot(float pX, float pY);
	bool    setRegion(int pOffX, int pOffY, int pRegionWidth, int pRegionHeight);
	bool    toggleWrap(bool pWrap);
	void    setWrapDisplacement(float pUDisplace, float pVDisplace);
	void    setLayer(int pLayer);
	/**@}*/


	/** @name Color transformations, blending and back face culling (Some of these methods dont affect the fonts or the primitives)
	*
	*/
	/**@{*/
	void    setBackCull(bool pCull);
	void    setTint(BYTE pR, BYTE pG, BYTE pB);
	void    setTransparency(BYTE pA);
	void    setFade(BYTE pR, BYTE pG, BYTE pB, BYTE pA);
	void    setBlendSource(IND_BlendingType pSo);
	void    setBlendDest(IND_BlendingType pDs);
	/**@}*/


	/** @name Collisions
	*
	*/
	/**@{*/
	bool setBoundingAreas(const char *pFile);
	bool setBoundingTriangle(const char *pId, int pAx, int pAy, int pBx, int pBy, int pCx, int pCy);
	bool setBoundingCircle(const char *pId, int pOffsetX, int pOffsetY, int pRadius);
	bool setBoundingRectangle(const char *pId, int pOffsetX, int pOffsetY, int pWidth, int pHeight);
	bool deleteBoundingAreas(const char *pId);
	void showCollisionAreas(bool pShowCollisionAreas);
	void showGridAreas(bool pShowGridAreas);
	/**@}*/


	// ----- Public Gets ------

	/** @name Gets
	*
	*/
	/**@{*/

	//! A unique id assigned for this entity instance.
    unsigned int getId() {
        return _id;
    }
    
    //! If the entity has a surface assigned, it returns a pointer to this surface.
	IND_Surface     *getSurface()      {
		return _su;
	}
	//! If the entity has a animation assigned, it returns a pointer to this animation.
	IND_Animation   *getAnimation()      {
		return _an;
	}
	//! Returns true if the entity is being showed, false if not
	bool    isShow()      {
		return _show;
	}
	//! Returns the sequence number that has been assigned to the animation.
	int     getSequence()      {
		return _sequence;
	}
	//! Returns the number of repetitions the animation has to do. If this value is equal or less than zero, it indicates that the amination is looping.
	int     getNumReplays()      {
		return _numReplays;
	}
	//! Returns X position of the entity.
	float   getPosX()      {
		return _x;
	}
	//! Returns Y position of the entity.
	float   getPosY()      {
		return _y;
	}
	//! Returns Z position of the entity.
	int     getPosZ()      {
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
	//! Indicate if the entity is making backface culling 0 = No, 1 = Yes.
	bool    getBackCull()      {
		return _cull;
	}
	//!Indicate if the entity makes horizontal mirror. 0 = No, 1 = Yes.
	bool    getMirrorX()      {
		return _mirrorX;
	}
	//! Indicate if the entity makes vertical mirror. 0 = No, 1 = Yes.
	bool    getMirrorY()      {
		return _mirrorY;
	}
	//! Returns the type of filter ::IND_Filter which uses the graphical object assignated to the entity.
	IND_Filter  getFilter()      {
		return _filter;
	}
	//! Returns the type ::IND_Type which uses the graphical object assignated to the entity.
	IND_Type    getType();
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
	//! Returns the assigned layer of the entity
	int     getLayer()      {
		return _layer;
	}
	//! Returns the X HotSpot of the entity.
	float   getHotSpotX()      {
		return _hotSpotX;
	}
	//! Returns the Y HotSpot of the entity.
	float   getHotSpotY()      {
		return _hotSpotY;
	}
	//! Returns the X position of the upper left corner of the region assigned to the entity by IND_Entity2d::setRegion()
	int     getRegionX()      {
		return _offX;
	}
	//! Returns the Y position of the upper left corner of the region assigned to the entity by IND_Entity2d::setRegion()
	int     getRegionY()      {
		return _offY;
	}
	//! Returns the width of the region assignated to the entity by IND_Entity2d::setRegion()
	int     getRegionWidth()      {
		return _regionWidth;
	}
	//! Returns the height of the region assignated to the entity by IND_Entity2d::setRegion()
	int     getRegionHeight()      {
		return _regionHeight;
	}
	//! Returns the position 1 (true) if the image is being repeated in the X,Y axis
	bool    ifWrap()          {
		return _wrap;
	}
	//! Returns the position in x from the first point of a primitive line.
	int     getLineX1()      {
		return _x1;
	}
	//! Returns the position in y from the first point of a primitive line.
	int     getLineY1()      {
		return _y1;
	}
	//! Returns the position in x from the second point of a primitive line.
	int     getLineX2()      {
		return _x2;
	}
	//! Returns the position in y from the second point of a primitive line.
	int     getLineY2()      {
		return _y2;
	}
	//! Returns the radius of the primitives  ::IND_REGULAR_POLY
	int     getRadius()      {
		return _radius;
	}
	//! Returns the numbers of sides of the primitive ::IND_REGULAR_POLY
	int     getNumSides()      {
		return _numSides;
	}
	//! Returns the primitive angle ::IND_REGULAR_POLY
	float   getPolyAngle()      {
		return _polyAngle;
	}
	//!  Returns the  points array which draw the primitive ::IND_POLY2D
	IND_Point *getPolyPoints()      {
		return _polyPoints;
	}
	//! Returns the number of lines which draw the primitive ::IND_POLY2D
	int     getNumLines()      {
		return _numLines;
	}
	//! Returns the align ::IND_Align of the text
	IND_Align getAlign()      {
		return _align;
	}
	//! Returns the additional space between characters when using IND_Font
	int     getCharSpacing()      {
		return _charSpacing;
	}
	//!  Returns the line spacing between lines when using IND_Font
	int     getLineSpacing()      {
		return _lineSpacing;
	}
	//! Returns the text string of the entity
	char    *getText()      {
		return _text;
	}
	//! Returs true if the collision areas are being shown
	bool    isShowCollisionAreas()      {
		return _showCollisionAreas;
	}
	//! Returs true if the grid areas are being shown
	bool    isShowGridAreas()              {
		return _showGridAreas;
	}
	/**@}*/

private:
	/** @cond DOCUMENT_PRIVATEAPI */
	// ----- Private ------
	IND_Entity2d();
	virtual ~IND_Entity2d();
    
	// ----- Objects -----

	CollisionParser *_collisionParser;

	// ----- Entity attributes -----

	IND_Surface     *_su;   // Pointer to a surface
	IND_Animation   *_an;   // Pointer to an animation
	IND_Primitive2d _pri2d; // Primitive type
	IND_Font        *_font; // Fonts

	// Show
	bool _show;             // Flag for showing / hiding the entity

	// Space transformation attributes
	bool _updateTransFlag;  // Flag for knowing when to recalculate the transformation matrix of an entity
	float _x;               // x Coordinate
	float _y;               // y Coordinate
	int _z;                 // Depth (indicates which object will be blitted upon which other)
	float _angleX;          // Angle in the x axis
	float _angleY;          // Angle in the y axis
	float _angleZ;          // Angle in the z axis
	float _scaleX;          // Horizontal scaling
	float _scaleY;          // Vertical scaling
	bool _cull;             // Back Face Culling ON / OFF
	float _hotSpotX;        // Hotspot x
	float _hotSpotY;        // Hotspot y
	int _axisCalX;          // Horizontal hotspot precalculated
	int _axisCalY;          // Vertical hotspot precalculated
	bool _mirrorX;          // Horizontal mirroring
	bool _mirrorY;          // Vertical mirroring
	IND_Filter _filter;     // Filter type (0 = Nearest Point || 1 = Linear)
	IND_Matrix  _mat;       // World matrix

	// Color, transperency and fading attribute
	BYTE _r;                // R component for tinting
	BYTE _g;                // G component for tinting
	BYTE _b;                // B component for tinting
	BYTE _a;                // Transparency
	BYTE _fadeR;            // R component for the fade to color
	BYTE _fadeG;            // G component for the fade to color
	BYTE _fadeB;            // B component for the fade to color
	BYTE _fadeA;            // Amount of fade
	IND_BlendingType _so;   // Blending source
	IND_BlendingType _ds;   // Blending destination

	// Region
	int _offX;              // x coordinate
	int _offY;              // y coordinate
	int _regionWidth;       // Witdh
	int _regionHeight;      // Height

	// Tiling
	bool _wrap;             // Wrapping
	float _uDisplace;       // U Coordinate displacement
	float _vDisplace;       // V Coordinate displacement

	// Animation attributes
	unsigned int _sequence;          // Index of the sequence
	int _numReplays;        // Num of replays of the sequence
	int _firstTime;         // Flag

	// 2d primitive attributes
	int _x1, _y1, _x2, _y2; // Line
	int _radius;            // Radius
	IND_Point *_polyPoints; // Point list
	int _numLines;          // Number of sides of the polygon
	int _numSides;          // Number of sides of the regular polygon
	float _polyAngle;       // Regular polygon angle

	// Fonts attributes
	IND_Align _align;       // Font align
	int _charSpacing;       // Additional space between letters
	int _lineSpacing;       // Space between lines
	char *_text;            // Text

	// Collision attributes
	bool _showCollisionAreas;

	// Grid areas attributes
	bool _showGridAreas;

	// Collision list for surfaces (the collision list for animations is in IND_AnimationManager.h)
	list <BOUNDING_COLLISION *> *_listBoundingCollision; // Vector of bounding areas for collision checking

	int _layer;
    
    unsigned int _id;

	// ----- Private methods -----

	void    initAttrib();

	// ----- Friends -----

	friend class IND_Entity2dManager;
    /** @endcond */
};
/**@}*/

#endif // _IND_ENTITY2D_
