/*****************************************************************************************
 * File: IND_Entity2d.cpp
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

// ----- Includes -----

#include "Global.h"
#include "IND_Entity2d.h"
#include "CollisionParser.h"
#include "IND_Animation.h"
#include "IND_Surface.h"
#include "IND_Font.h"

#if defined (PLATFORM_LINUX)
#include <stdlib.h>
#include <string.h>
#endif


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------


IND_Entity2d::IND_Entity2d() : _text(NULL),  _listBoundingCollision(NULL) {
	initAttrib();
}


IND_Entity2d::~IND_Entity2d() {
    if (_listBoundingCollision) {
        list<BOUNDING_COLLISION*>::iterator itr;
        for (itr = _listBoundingCollision->begin(); itr != _listBoundingCollision->end();++itr) {
            delete  (*itr);
        }
    }
    
	DISPOSE(_listBoundingCollision);
    DISPOSEARRAY(_text);
}



// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

/**
 * Returns the type of the underlying related object.
 */
IND_Type IND_Entity2d::getType() {
	//Resolve the type of the underlying related object.
	IND_Type type (IND_OPAQUE);
	if (_su) {
		type = _su->getTypeInt();
	} else if (_an) {
		if (_an->getSurface(0))
			type = _an->getSurface(0)->getTypeInt();
	} else if (_font) {
		type = IND_ALPHA;
	} else if (_pri2d) {
        	type = IND_ALPHA;
	}

	return type;
}

/**
 * Inserts a surface in the entity. If that entity has another graphical object allocated it will be replaced (but not freed) by the present surface.
 * In the next call to IND_Entity2dManager::renderEntities2d() the surface will be drawn.
 * In addition, all the attributes of the entity will be restarted.
 * @param pNewSurface				Pointer to an IND_Surface object.
 */
void IND_Entity2d::setSurface(IND_Surface *pNewSurface) {
	if(!pNewSurface) {
		return;
	}
	
	initAttrib();
	_su = pNewSurface;

}

/**
 * Inserts an animation in the entity. If that entity has another graphical object allocated it will be replaced (but not freed)by the present animation.
 * In the next call to IND_Entity2dManager::renderEntities2d() this animation will be drawn.
 * In addition, all the attributes of the entity will be restarted.
 * @param pNewAnimation				Pointer to an IND_Animation object.
 */
void IND_Entity2d::setAnimation(IND_Animation *pNewAnimation) {
	if(!pNewAnimation) {
		return;
	}

	initAttrib();
	_an = pNewAnimation;
}

/**
 * Inserts a primitive in the entity. If that entity has another graphical object allocated it will be replaced (but not freed) by the present primitive.
 * In the next call to IND_Entity2dManager::renderEntities2d() this primitive will be drawn.
 * In addition, all the parameters of the entity will be restarted.
 * @param pPri2d				Pointer to an IND_Primitive2d object.
 */
void IND_Entity2d::setPrimitive2d(IND_Primitive2d pPri2d) {
	initAttrib();
	_pri2d = pPri2d;
}

/**
 * Inserts a font into the entity. If that entity has another graphical object allocated, it will be replaced (but not freed) by the present font.
 * In the next call to IND_Entity2dManager::renderEntities2d() the font will be drawn.
 * In addition, all the parameters of the entity will be restarted.
 * Important: Fonts can't be sized and can't be rotated.
 * @param pFont					Pointer to an IND_Font object.
 */
void IND_Entity2d::setFont(IND_Font *pFont) {
	if(!pFont) {
		return;
	}
	
	initAttrib();
	_font = pFont;
}

/**
 * Shows / hides the entity. It dosen't delete it.
 * Default: 0.
 * @param pShow					True = show the entity / False = hide entity.
 */
void IND_Entity2d::setShow(bool pShow) {
	_show = pShow;
}

/**
 * This function sets the frame number of the sequence that will be drawn.
 * Default: 0.
 * @param pSequence				Frame number of the sequence to draw.
 */
void IND_Entity2d::setSequence(unsigned int pSequence) {
	if (_an) {
		_an->setActualFramePos(_sequence, 0);
		_an->setIsActive(_sequence, false); //Reset
	}
	_sequence = pSequence;
	_an->setIsActive(_sequence, true); //Set
}

/**
 * Sets the number of times that the animation will be drawn on the screen.
 * When the parameter takes values less or equals 0 the animation repeats forever.
 * When the parameter takes values greater or equals 1, this number is the number of times that the animation will be repeated.
 * Default: -1 (repeat forever),
 * @param pNumReplays				Number of repetitions of the animation.
 */
void IND_Entity2d::setNumReplays(int pNumReplays) {
	if (pNumReplays < 0) {
		_numReplays = -1;
		return;
	}

	if (_firstTime && pNumReplays != -1) {
		//pNumReplays--;
		_firstTime = 0;
	}
	_numReplays = pNumReplays;
}

/**
 * Sets the coordinates of the ::IND_LINE primitive that will be drawn on screen.
 * Default: (0, 0, 0, 0).
 * @param pX1					X value of the startpoint of the line.
 * @param pY1					Y value of the startpoint of the line.
 * @param pX2					X value of the endpoint of the line.
 * @param pY2					Y value of the endpoint of the line.
 */
void IND_Entity2d::setLine(int pX1, int pY1, int pX2, int pY2) {
	_x1 = pX1;
	_y1 = pY1;
	_x2 = pX2;
	_y2 = pY2;
}

/**
 * Sets the coordinates of the ::IND_RECTANGLE primitive that will be drawn on screen.
 * Default: (0, 0, 0, 0).
 * @param pX1					X value of the top left point of the rectangle.
 * @param pY1					Y value of the top left point of the rectangle.
 * @param pX2					X value of the lower right point of the rectangle.
 * @param pY2					Y value of the lower right point of the rectangle.
 */
void IND_Entity2d::setRectangle(int pX1, int pY1, int pX2, int pY2) {
	_x1 = pX1;
	_y1 = pY1;
	_x2 = pX2;
	_y2 = pY2;
}

/**
 * Sets the length of the radius of the primitive ::IND_REGULAR_POLY that will be drawn on screen.
 * Default: 0.
 * @param pRadius				Length of the radius of the primitive ::IND_REGULAR_POLY.
 */
void IND_Entity2d::setRadius(int pRadius) {
	_radius = pRadius;
}

/**
 * Sets the number of sides of the regular polygon ::IND_REGULAR_POLY.
 * Default: 0.
 * @param pNumSides				Number of sides of the regular polygon ::IND_REGULAR_POLY.
 */
void IND_Entity2d::setNumSides(int pNumSides) {
	_numSides = pNumSides;
}

/**
 * Sets the angle of the regular polygon ::IND_REGULAR_POLY. With incremental values it is possible to rotate the polygon.
 * Default: 0.
 * @param pPolyAngle				Angle of the regular polygon ::IND_REGULAR_POLY.
 */
void IND_Entity2d::setPolyAngle(float pPolyAngle) {
	_polyAngle = pPolyAngle;
}

/**
 * Sets the array of points that will be used to draw the primitive ::IND_POLY2D.
 * Every point consists of two coordinates (x, y). This function must be used together with setNumLines()
 * that is used to indicate the number of edges to draw between the specified points.
 * Default: 0.
 * @param pPolyPoints				Pointer to an array of points ::IND_Point. Example: ::IND_Point mPoly3 [] = {{60, 10}, {20, 15}, {50, 90}, {170, 190}} =>  Indicates three points (each one with its x and y coordinates).
 */
void IND_Entity2d::setPolyPoints(IND_Point *pPolyPoints) {
	if(!pPolyPoints) {
		return;
	}
	_polyPoints = pPolyPoints;
}

/**
 * Sets the number of edges to draw by the primitive ::IND_POLY2D. This function must used together with setPolyPoints() that is used to indicate the array of points.
 * Default: 0.
 * @param pNumLines				Number of edges to draw.
 */
void IND_Entity2d::setNumLines(int pNumLines) {
	_numLines = pNumLines;
}

/**
 * Sets the alignment of the text.
 * Default: ::IND_RIGHT.
 * @param pAlign				Text alignment. See ::IND_Align.
 */
void IND_Entity2d::setAlign(IND_Align pAlign) {
	_align = pAlign;
}

/**
 * Sets an additional space between characters when using a IND_Font in a 2d entity.
 * Debault: 0.
 * @param pCharSpacing				Width of the additional space between letters.
 */
void IND_Entity2d::setCharSpacing(int pCharSpacing) {
	_charSpacing = pCharSpacing;
}

/**
 * Sets the line spacing between lines when using a IND_Font in a 2d entity.
 * Default: 0.
 * @param pLineSpacing				Height of the line spacing between lines.
 */
void IND_Entity2d::setLineSpacing(int pLineSpacing) {
	_lineSpacing = pLineSpacing;
}

/**
 * Sets the text that is going to be shown on screen.
 * Default: "" (Empty String).
 * @param pText					Text to draw in the screen.
 */
void IND_Entity2d::setText(const char *pText) {
	if (pText) {
		DISPOSEARRAY(_text);

		size_t textSize = strlen(pText);
		//Reallocate text on heap memory. Characters in string +1 (line ending)
		_text = new char [textSize + 1];
		//Cooy string contents
		strcpy(_text,pText);
	}
}

/**
 * Sets the position where the entity will be drawn.
 * The parameter Z controls the depth. This way it is possible to choose that certain entities are drawn over others.
 * An entity with Z = 1 will be drawn over other one with Z = 0.
 * Default: (0, 0, 0).
 * @param pX, pY				Translation in the x and y axis. The position (0,0) corresponds to the top left corner of the ViewPort.
 * @param pZ					Depth of the entity.
 */
void IND_Entity2d::setPosition(float pX, float pY, int pZ) {
	// If updated
	if (pX != _x || pY != _y) {
		_x = pX;
		_y = pY;
		_updateTransFlag = 1;
	}
	_z = pZ;
}

/**
 * Sets the rotation of the entity in each of its axes.
 * Default: (0, 0, 0).
 * @param pAnX					Angle of rotation in the x axis in degrees.
 * @param pAnY					Angle of rotation in the y axis in degrees.
 * @param pAnZ					Angle of rotation in the z axis in degrees.
 */
void IND_Entity2d::setAngleXYZ(float pAnX, float pAnY, float pAnZ) {
	// If updated
	if (pAnX != _angleX || pAnY != _angleY || pAnZ != _angleZ) {
		_angleX = pAnX;
		_angleY = pAnY;
		_angleZ = pAnZ;
		_updateTransFlag = 1;
	}
}

/**
 * Sets the scaling of the entity in the x and y axis. With this method it is possible to get bigger or smaller entities.
 * Default: (1.0f, 1.0f).
 * @param pSx, pSy				Level of scaling in the x and y axis. (1,1) to keep the normal size.
 */
void IND_Entity2d::setScale(float pSx, float pSy) {
	// If updated
	if (pSx != _scaleX || pSy != _scaleY) {
		_scaleX = pSx;
		_scaleY = pSy;
		_updateTransFlag = 1;
	}
}

/**
 * Activates or deactivates the backface culling. (true / false) = (activated / deactivated).
 * This feature can be deactivated when we are going to rotate the graphical object in the x or y coordinates.
 * This way it is possible to draw for example the leaf of a tree that falls down rotating and shows both faces.
 * In cases that you don't want to draw back faces deactivate this for faster render times.
 * Default: Activated (true).
 * @param pCull					backface culling activated / deactivated.
 */
void IND_Entity2d::setBackCull(bool pCull) {
	// If updated
	if (pCull != _cull) {
		_cull = pCull;
		_updateTransFlag = 1;
	}
}

/**
 * Activates or deactivates the horizontal flip.
 * Deactivated by default (0).
 * @param pMx					Mirror (flip) in X. (1 / 0) = (activated / deactivated).
 */
void IND_Entity2d::setMirrorX(bool pMx) {
	// If updated
	if (pMx != _mirrorX) {
		_mirrorX = pMx;
		_updateTransFlag = 1;
	}
}

/**
 * Activates or deactivates the vertical flip.
 * Deactivated by default (0).
 * @param pMy					Mirror (flip) in Y. (1 / 0) = (activated / deactivated).
 */
void IND_Entity2d::setMirrorY(bool pMy) {
	// If updated
	if (pMy != _mirrorY) {
		_mirrorY = pMy;
		_updateTransFlag = 1;
	}
}

/**
 * Sets the filter that will affect the graphical object when drawing.
 * Default: ::IND_FILTER_LINEAR
 * @param pF					Type of filter ::IND_Filter. There are two types of filters that
						can be applied to the graphical object when it suffers a rotation
						or scaling:
						- ::IND_FILTER_POINT (Nearest point filter). Less quality, but
						bigger performance. It is possible to use this one without
						quality loss in graphical objects that don't rotate or are
						affected by scaling.
						- ::IND_FILTER_LINEAR (Bilinear filter). More quality, but less
						performance. Used in graphical objects that be rotated or
						affected by scaling.
 */
void IND_Entity2d::setFilter(IND_Filter pF) {
	// If updated
	if (pF != _filter) {
		_filter = pF;
		_updateTransFlag = 1;
	}
}

/**
 * Sets the hotspot of the graphical object (only works if the entity has an ::IND_Surface or
 * ::IND_Animation assigned). When we talk about hotspot we are refering to
 * an imaginary point from which the transformations are applied (trasnlation, rotation and scaling).
 * So, we can choose that certain surface rotates on his center, on one of his vertex, or on any another
 * intermediate position.
 * To establish the hotspot we use pX and pY in a range from 0 to 1 (a point inside the surface area)
 * or higher (a point outside the surface area). Refering respectively to the horizontal and vertical
 * position of the surface. Thus (0.5f, 0.5f)  would be the center of the image, (1.0f, 0.0f) would be
 * the top right corner, etc.
 * You can also set a hotspot out of the image, for example using (2.0f, 4.0f) values. This way you can make the entity
 * rotate around a point.
 * Default value (0, 0) = Upper left corner.
 * @param pX, pY				Values that indicates the position of the hotspot. The top left corner of the entity corresponds to (0, 0).
 */
bool IND_Entity2d::setHotSpot(float pX, float pY) {
	//if (pX < 0 || pX > 1) pX = 0;
	//if (pY < 0 || pY > 1) pY = 0;

	// If updated
	if (pX != _hotSpotX || pY != _hotSpotY) {
		_updateTransFlag = 1;

		if (_su) {
			_hotSpotX = pX;
			_hotSpotY = pY;
			_axisCalX = (int)(pX * _su->getWidth() * -1);
			_axisCalY = (int)(pY * _su->getHeight() * -1);
		} else if (_an) {
			_hotSpotX = pX;
			_hotSpotY = pY;
			_axisCalX = (int)(pX * _an->getHighWidth(_sequence) * -1);
			_axisCalY = (int)(pY * _an->getHighHeight(_sequence) * -1);
		} else
			return 0;
	}

	return 1;
}


/**
 * This function has two use uses:
 * 
 * 1) First, it can be used to indicate the rectangular region which is going to be drawn from
 * the surface or animation assigned to the entity. So, instead of drawing all the surface, it can
 * be chosen which rectangular area is going to be rendered in the screen. This method doesn't affect
 * primitives or fonts. If the requested region stays partly or totally out of the surface,
 * the region will not be applied.
 *
 * 2) Second, if IND_Entity2d::toggleWrap() has been used on the entity, the image will be
 * repeated (tiling) in the region specified.
 *
 * The 1st method is useful in the example of the classic loading bar, which shows itself
 * from left side to right progressively changing the width of the region.
 *
 * The 2nd method is useful for multiple things. For example a background made by repeating a tile.
 *
 * Special remark: this function only works with ::IND_Surface objects that only have ONE texture
 * assigned (you can check this using::IND_Surface::getNumTextures() method). So, it will work only
 * with images that are power of two and lower than the maximum texture size allowed by your card
 * (you can check this parameter using ::IND_Render::getMaxTextureSize()). The method will return 0
 * otherwise.
 * 
 * Default: (0, 0, 0, 0) (Indicates that there is no region, the whole surface will be drawn)
 *
 * @param pOffX, pOffY					Position of the upper left corner of the region.
 * @param pRegionWidth					Width of the region.
 * @param pRegionHeight					Height of the region.
 */
bool IND_Entity2d::setRegion(int pOffX, int pOffY, int pRegionWidth, int pRegionHeight) {
	if (!_su && !_an) return 0;

	if (pRegionWidth    <= 0) return 0;
	if (pRegionHeight   <= 0) return 0;

	_offX           = pOffX;
	_offY           = pOffY;
	_regionWidth    = pRegionWidth;
	_regionHeight   = pRegionHeight;

	return 1;
}

/**
 * Causes the repetition of the image in the x and y axis in the specified region
 * by IND_Entity2d::setRegion().
 * @param pWrap						(Activates / Deactivates) = (true, false) the repetition of the image in the x,y axis.
 */
bool IND_Entity2d::toggleWrap(bool pWrap) {
	if (!_su && !_an) return 0;

	_wrap = pWrap;

	return 1;
}

/**
 * Sets the horizontal and vertical displacement of the an image that is being tiled
 * to the screen using the IND_Entity2d::setRegion() and IND_Entity2d::toggleWrap() methods.
 * Using this method you can, for example, create and effect of an animated tiled background.
 * @param pUDisplace				Horizontal displacement of the image (U coordinate).
 * @param pVDisplace				Vertical displacement of the image (V coordinate).
 */
void IND_Entity2d::setWrapDisplacement(float pUDisplace, float pVDisplace) {
	_uDisplace = pUDisplace;
	_vDisplace = pVDisplace;
}

/**
 * This function is not changing internal functionality once IndieLib is working.
 * @param pLayer					New Layer to render in.
 */
void IND_Entity2d::setLayer(int pLayer) {
	_layer = pLayer;
}

/**
 * This function sets the bytes pR, pG, pB for "tinting" the image to a certain color. For example,
 * if we use RGB = (255, 0, 0), the image will be tinted red. Note: It is not the same as
 * fade. The tint produces a colored transparent effect (similar to looking through celophane),
 * whereas the fade completely changes the image to the color selected.
 *
 * It is not possible to use IND_Entity2d::setTint() and IND_Entity2d::setFade() at the same time,
 * the fade will override the tinting.
 * 
 * Default RGBA = (0, 0, 0) (Without tint)
 * @param pR, pG, pB					Bytes R, G, B
 */
void IND_Entity2d::setTint(BYTE pR, BYTE pG, BYTE pB) {
	_r = pR;
	_g = pG;
	_b = pB;
}

/**
 * Sets the level of transparency of the entity.
 *
 * The A byte indicates the level of transparency. If a value of 0 is used, the following graphical object
 * will be completely transparent, as opposed to the value 255 that will cause the object to be drawn completely opaque.
 * It is possible to use all the intermediate values for different levels of transparency.
 *
 * Default A = (255) (Without transparency)
 * @param pA						Byte A.
 */
void IND_Entity2d::setTransparency(BYTE pA) {
	_a = pA;
}

/**
 * Sets the bytes FadeR, FadeG, FadeB used for fading the image to a certain color.
 * For example, if we use RGB = (255, 0, 0), the image will progressively become red. Note: It is not
 * the same as the tint. The tint produces a colored transparent effect (like looking through celophane),
 * whereas the fade changes the image to the selected color completely.
 *
 * The byte A indicates the level of fade. If a value 0 is used, the following graphical object
 * will be filled solid with the selected color, as opposed to the value 255 that would cause
 * the object to be drawn completely normally. It is possible to use all the intermediate values for different
 * levels of fade.
 *
 * It is not possible to use IND_Entity2d::setTint() and IND_Entity2d::setFade() at the same time,
 * the fade will override upon the tinting.
 *
 * Default RGBA = (0, 0, 0, 255) (Without fade)
 * @param pR, pG, pB, pA				Bytes R, G, B, A.
 */
void IND_Entity2d::setFade(BYTE pR, BYTE pG, BYTE pB, BYTE pA) {
	_fadeR = pR;
	_fadeG = pG;
	_fadeB = pB;
	_fadeA = pA;
}

/**
 * Sets the blending source.
 * Default: 0. (Blending source is not in use).
 * @param pSo						Indicates the blending source, see (::IND_BlendingType).
 */
void IND_Entity2d::setBlendSource(IND_BlendingType pSo) {
	_so = pSo;
}

/**
 * Sets the blending destiny.
 * Default 0. (Blending destiny is not in use).
 * @param pDs						Indicates blending destiny, see (::IND_BlendingType).
 */
void IND_Entity2d::setBlendDest(IND_BlendingType pDs) {
	_ds = pDs;
}

/**
 * Return 1 (true) if an xml file that has the definitions of several
 * bounding areas for checking collisions is parsed correctly. This
 * method will return 0 (false) if the entity dosen't have a surface assigned
 * (check setSurface(IND_Surface *pNewSurface). For animations in which you want that each frame has different
 * collision areas don't use this method, define the .xml script directly in the </frame> section.
 *
 * You can define in the xml file all the triangular, rectangular and circular areas that you want.
 * It is possible to "group" the bounding areas of the 2d entity using the "id" parameter. For
 * example you can add 3 triangles and 2 circles using the same id="head". An having at the same time
 * 4 rectangles with the id="foot". Later, you can use this id parameter in the IND_Entity2dManager::isCollision()
 * method for checking collision only in that areas.
 *
 * Use showCollisionAreas() method in order to show or hide the collision areas of this entity.
 * In order to render all the collision areas you should use the
 * IND_Entity2dManager::renderCollisionAreas() method.
 *
 * Default: no bounding areas assigned.
 * @param pFile						Xml file defining the bounding areas.
 */
bool IND_Entity2d::setBoundingAreas(const char *pFile) {
	g_debug->header("Parsing collision file", 5);

	if(!pFile) {
		g_debug->header("No filename provided", 2);
		return 0;
	}

	g_debug->header("File name:", 3);
	g_debug->dataChar(pFile, 1);

	// Only for surfaces
	if (!_su) {
		g_debug->header("The entity hasn't got a surface assigned, for animations you must define the collision scripts in the .xml file", 2);
		return 0;
	}

	char stringTemp[128];
	char *pFileCharTemp = strcpy(stringTemp, pFile);

	// ----- Parsing collision file -----

	if (!_collisionParser->parseCollision(_listBoundingCollision, pFileCharTemp)) {
		g_debug->header("Fatal error, cannot load the collision xml file", 2);
		return 0;
	}

	// ----- g_debug -----

	g_debug->header("Collision file loaded", 6);

	return 1;
}

/**
 * Return 1 (true) if a bounding triangle for checking collisions is defined. This
 * method will return 0 (false) if the entity dosen't have a surface assigned (check setSurface(IND_Surface *pNewSurface).
 *
 * You can assign all the triangle areas (and also circular and rectangular areas, using
 * setBoundingCircle() and setBoundingRectangle()) that you want.
 * It is possible to "group" the bounding areas of the 2d entity using the "id" parameter. For
 * example you can add 3 triangles and 2 circles using the same id="head". An having at the same time
 * 4 rectangles with the id="foot". Later, you can use this id parameter in the IND_Entity2dManager::isCollision()
 * method for checking collision only in that areas.
 * You can also use the setBoundingAreas() method in order to load all the bounding areas directly
 * from an xml file.
 *
 * Use showCollisionAreas() method in order to show or hide the collision areas of this entity.
 * In order to render all the collision areas you should use the
 * IND_Entity2dManager::renderCollisionAreas() method.
 *
 * Default: no bounding triangle assigned.
 * @param pId						Id of the collision group, use this Id for group several bounding areas
							together. Later you can use IND_Entity2dManager::isCollision() method passing
							this parameter to check a collision only in that areas. For
							example a head and ears of one character sprite can be 3 circular
							bounding areas grouped together using the same id="head": one for the head
							and two for the ears.
 * @param pAx, pAy					Vertex A of the triangle.
 * @param pBx, pBy					Vertex B of the triangle.
 * @param pCx, pCy					Vertex C of the triangle.
 */
bool IND_Entity2d::setBoundingTriangle(const char *pId,
                                       int pAx, int pAy,
                                       int pBx, int pBy,
                                       int pCx, int pCy) {
	// Only for surfaces
	if (!_su) return 0;

	if(!pId) return 0;

	char stringTemp[128];
	char *pIdCharTemp = strcpy(stringTemp, pId);

	_collisionParser->setBoundingTriangle(_listBoundingCollision, pIdCharTemp, pAx, pAy, pBx, pBy, pCx, pCy);

	return 1;
}

/**
 * Return 1 (true) if a bounding circle for checking collisions is defined. This
 * method will return 0 (false) if the entity dosen't have a surface assigned
 * (check setSurface(IND_Surface *pNewSurface).
 *
 * You can assign all the circular areas (and also triangle and rectangular areas, using
 * showCollisionAreas() and setBoundingRectangle()) that you want.
 * It is possible to "group" the bounding areas of the 2d entity using the "id" parameter. For
 * example you can add 3 triangles and 2 circles using the same id="head". An having at the same time
 * 4 rectangles with the id="foot". Later, you can use this id parameter in the IND_Entity2dManager::isCollision()
 * method for checking collision only in that areas.
 *
 * You can also use the setBoundingAreas() method in order to load all the bounding areas directly
 * from an xml file.
 *
 * Use showCollisionAreas() method in order to show or hide the collision areas of this entity.
 * In order to render all the collision areas you should use the
 * IND_Entity2dManager::renderCollisionAreas() method.
 *
 * Default: no bounding circle assigned.
 * @param pId						Id of the collision group, use this Id for group several bounding areas
							together. Later you can use IND_Entity2dManager::isCollision() method passing
							this parameter to check a collision only in that areas. For
							example a head and ears of one character sprite can be 3 circular
							bounding areas grouped together using the same id="head": one for the head
							and two for the ears.
 * @param pOffsetX					Horizontal displacement.
 * @param pOffsetY					Vertical displacement.
 * @param pRadius					Radius of the circle area.
 */
bool IND_Entity2d::setBoundingCircle(const char *pId, int pOffsetX, int pOffsetY, int pRadius) {
	// Only for surfaces
	if (!_su) return 0;
	
	if (pRadius <= 0) return 0;

	char stringTemp[128];
	char *pIdCharTemp = strcpy(stringTemp, pId);

	_collisionParser->setBoundingCircle(_listBoundingCollision, pIdCharTemp, pOffsetX, pOffsetY, pRadius);

	return 1;
}

/**
 * Return 1 (true) if a bounding rectangle for checking collisions is defined. This
 * method will return 0 (false) if the entity dosen't have a surface assigned (check setSurface(IND_Surface *pNewSurface).
 *
 * You can assign all the rectangular areas (and also triangle and circular areas, using
 * showCollisionAreas() and setBoundingCircle() that you want.
 * It is possible to "group" the bounding areas of the 2d entity using the "id" parameter. For
 * example you can add 3 triangles and 2 circles using the same id="head". An having at the same time
 * 4 rectangles with the id="foot". Later, you can use this id parameter in the IND_Entity2dManager::isCollision()
 * method for checking collision only in that areas.
 *
 * You can also use the setBoundingAreas() method in order to load all the bounding areas directly
 * from an xml file.
 *
 * Use showCollisionAreas() method in order to show or hide the collision areas of this entity.
 * In order to render all the collision areas you should use the
 * IND_Entity2dManager::renderCollisionAreas() method.
 *
 * Default: no bounding rectangle assigned.
 * @param pId						Id of the collision group, use this Id for group several bounding areas
							together. Later you can use IND_Entity2dManager::isCollision() method passing
							this parameter to check a collision only in that areas. For
							example a head and ears of one character sprite can be 3 circular
							bounding areas grouped together using the same id="head": one for the head
							and two for the ears.
 * @param pOffsetX					Horizontal displacement.
 * @param pOffsetY					Vertical displacement.
 * @param pWidth					Width of the rectangle.
 * @param pHeight					Height of the rectangle.
 */
bool IND_Entity2d::setBoundingRectangle(const char *pId, int pOffsetX, int pOffsetY, int pWidth, int pHeight) {
	// Only for surfaces
	if (!_su) return 0;
	
	if (!pId) return 0;

	if (pWidth <= 0 || pHeight <= 0) return 0;

	char stringTemp[128];
	char *pIdCharTemp = strcpy(stringTemp, pId);

	_collisionParser->setBoundingRectangle(_listBoundingCollision, pIdCharTemp, pOffsetX, pOffsetY, pWidth, pHeight);

	return 1;
}

/**
 * Return 1 (true) if a bounding rectangle for checking collisions is deleted. This
 * method will return 0 (false) if the entity dosen't have a surface assigned (check setSurface(IND_Surface *pNewSurface).
 *
 * Default: no bounding rectangle assigned.
 * @param pId						Id of the collision group to delete, use "*" to delete all groups.
 */
bool IND_Entity2d::deleteBoundingAreas(const char *pId) {
	// Only for surfaces
	if (!_su) return 0;

	if(!pId) return 0;

	char stringTemp[128];
	char *pIdCharTemp = strcpy(stringTemp, pId);

	_collisionParser->deleteBoundingAreas(_listBoundingCollision, pIdCharTemp);

	return 1;
}

/**
 * Turn on / off the visualization of collision areas. This can be useful
 * for checking that your collisions are accuarte.
 *
 * In order to render all the collision areas you should use the Entity2dManager::renderCollisionAreas() method.
 * Default: true.
 * @param pShowCollisionAreas				(true / false) = (activate / deactivate).
 */
void IND_Entity2d::showCollisionAreas(bool pShowCollisionAreas) {
	_showCollisionAreas = pShowCollisionAreas;
}

/**
 * Turn on / off the visualization of grid areas.
 * In order to render all the collision areas you should use the Entity2dManager::renderGridAreas() method.
 *
 * Default: true.
 * @param pShowGridAreas				(true / false) = (activate / deactivate).
 */
void IND_Entity2d::showGridAreas(bool pShowGridAreas) {
	_showGridAreas = pShowGridAreas;
}


// --------------------------------------------------------------------------------
//									 Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
==================
Attributes initialization
==================
*/
void IND_Entity2d::initAttrib() {
    //Id
    _id = 0;
	// Assigned object
	_su = 0;
	_an = 0;
	_pri2d = 0;
	_font = 0;

	// Showing
	_show = 1;

	// Space transformation attributes
	_updateTransFlag = 1;
	_x = 0;
	_y = 0;
	_z = 0;
	_angleX = 0;
	_angleY = 0;
	_angleZ = 0;
	_scaleX = 1;
	_scaleY = 1;
	_cull = 1;
	_hotSpotX = 0;
	_hotSpotY = 0;
	_axisCalX = 0;
	_axisCalY = 0;
	_mirrorX = 0;
	_mirrorY = 0;
	_filter = IND_FILTER_LINEAR;

	// Color, transperency and fading attributes
	_r = 255;
	_g = 255;
	_b = 255;
	_a = 255;
	_fadeR = 0;
	_fadeG = 0;
	_fadeB = 0;
	_fadeA = 255;
	_so = 0;
	_ds = 0;

	// Region
	_offX = 0;
	_offY = 0;
	_regionWidth = 0;
	_regionHeight = 0;

	// Tiling
	_wrap = 0;
	_uDisplace = 0.0f;
	_vDisplace = 0.0f;

	// Animation attributes
	_sequence = 0;
	_numReplays = -1;
	_firstTime = 1;

	// 2d primitive attributes
	_x1 = 0;
	_y1 = 0;
	_x2 = 0;
	_y2 = 0;
	_radius = 0;
	_polyPoints = 0;
	_numLines = 0;
	_numSides = 0;
	_polyAngle = 0;

	// Fonts attributes
	_align = IND_RIGHT;
	_charSpacing = 0;
	_lineSpacing = 20;

	const char* defText = "";
	setText(defText);

	// Collision attributes
	_showCollisionAreas = 1;
	_collisionParser = CollisionParser::instance();
    DISPOSE(_listBoundingCollision);
	_listBoundingCollision = new list <BOUNDING_COLLISION *>;

	// Show grid areas
	_showGridAreas = 1;
}

/** @endcond */
