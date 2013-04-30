/*****************************************************************************************
 * File: Defines.h
 * Desc: Types, defines, modos, etc
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

/**
@mainpage
@section main_general Menu
<A HREF="modules.html">Click here to access the Menu</A>
*/

/**
 * @defgroup Classes Objects used in library
 *
 */


/**
 * @defgroup Init_End Initialization / Destruction
 * @ingroup Classes
 */

/**
 * @defgroup IndieLib_Init_End Init / End
 * @ingroup Init_End
 */


/**
 * @defgroup Main Main
 * @ingroup Classes
 */

/**
 * @defgroup IND_Window IND_Window
 * @ingroup Main
 */

/**
 * @defgroup IND_Render IND_Render
 * @ingroup Main
 */

/**
 * @defgroup Math Math
 * @ingroup Classes
 */

/**
 * @defgroup EntityManagers Entity Objects and Entity Managers (the most important concept of IndieLib)
 * @ingroup Classes
 */

/**
 * @defgroup Managers Object Managers
 * @ingroup Classes
 */

/**
 * @defgroup Objects Graphical Objects (that can be inserted into the entities)
 * @ingroup Classes
 */

/**
 * @defgroup Cameras Cameras
 * @ingroup Classes
 */

/**
 * @defgroup Input Input from keyboard and mouse
 * @ingroup Classes
 */

/**
 * @defgroup Timer Timer
 * @ingroup Classes
 */

/**
 * @defgroup Advanced Advanced methods for bliting directly to the screen without using entities. 
 */

/**
 * @defgroup Primitives Bliting Primitives
 * @ingroup Advanced
 */

/**
 * @defgroup Graphical_2d_Objects Bliting Surfaces, Animations and Fonts and setting the transformations directly
 * @ingroup Advanced
With these methods you can set the 2d transformations (using IND_Render::Set2dTransform) and color
attributes (using IND_Render::SetRainbow()) and blit directly
to the screen surfaces, animations, and fonts using IND_Render class.

Remember that you can use ::IND_Entity2d together with ::IND_Entity2dManager for drawing these graphical objects
to the screen without having to use this advanced method directly. This method is only useful for advanced
users for really concrete purposes.
 */


/**
 * @defgroup Graphical_3d_Objects Bliting 3d Animated 3d Models and setting the transformations directly
 * @ingroup Advanced
With these methods you can set the 3d transformations (using IND_Render::Set3dTransform) and color
attributes (using IND_Render::SetRainbow()) and to blit directly
to the screen 3d animated meshes using IND_Render class.

Remember that you can use ::IND_Entity3d together with ::IND_Entity3dManager for drawing these graphical objects
to the screen without having to use this advanced method directly. This method is only useful for advanced
users for really concrete purposes.
 */


/**
 * @defgroup Types Types used by library
 */



#ifndef _DEFINES_
#define _DEFINES_

// ----- Platform checkings -----
//Platform definition checkings
#include "IndiePlatforms.h"

#ifdef PLATFORM_LINUX
#include <cstring>
#include <stdio.h> 
#endif

// ----- Renderer settings -----
//****Renderer choosing in static time****
//YOU SHOULD CHANGE HERE THE DEFINITION OF WHICH RENDERER TO COMPILE WITH
//#define INDIERENDER_DIRECTX
#define INDIERENDER_OPENGL
//#define INDIERENDER_GLES_IOS

// ----- Renderer set checkings -----
//Only one render type per configuration!
//DirectX vs. OPENGLES
#if defined (INDIERENDER_DIRECTX) && defined (INDIERENDER_GLES_IOS)
#error Multiple renderers defined. Check Defines.h
#endif
//DirectX vs. OPENGL
#if defined (INDIERENDER_DIRECTX) && defined (INDIERENDER_OPENGL)
#error Multiple renderers defined. Check Defines.h
#endif
//OPENGL vs. OPENGLES
#if defined (INDIERENDER_GLES_IOS) && defined (INDIERENDER_OPENGL)
#error Multiple renderers defined. Check Defines.h
#endif

//Undefine symbols depending on chosen renderer
#ifdef INDIERENDER_OPENGL
#undef INDIERENDER_DIRECTX
#undef INDIERENDER_GLES_IOS
#undef PLATFORM_IOS			//We need this as iOS is a subset of MAC in apple terms (both defined)
#endif //INDIERENDER_OPENGL

#ifdef INDIERENDER_DIRECTX
#undef INDIERENDER_OPENGL
#undef INDIERENDER_GLES_IOS  
#endif //INDIERENDER_DIRECTX

#ifdef INDIERENDER_GLES_IOS
#undef INDIERENDER_OPENGL
#undef INDIERENDER_DIRECTX
#undef PLATFORM_OSX          //We need this as iOS is a subset of MAC in apple terms (both defined)
#endif //INDIERENDER_GLES_IOS
//****************************************

// ---- Config checkings -----
//Win32 platform vs. render settings
#if defined (PLATFORM_WIN32) && defined (INDIERENDER_GLES_IOS)
#error Render GLES_IOS defined for platform WIN32! Check Defines.h
#endif
//iOS platform vs. render setting
#if defined (PLATFORM_IOS) && !defined (INDIERENDER_GLES_IOS)
#error Render GLES_IOS NOT defined for platform iOS! Check Defines.h
#endif
//OSX platform vs. render setting
#if defined (PLATFORM_OSX) && !defined (INDIERENDER_OPENGL)
#error Render INDIERENDER_OPENGL NOT defined for platform OSX! Check Defines.h
#endif
//Linux platform vs. render setting
#if defined (PLATFORM_LINUX) && !defined (INDIERENDER_OPENGL)
#error Render GL NOT defined for platform Linux! Check Defines.h
#endif
// ----- Lib export -----

#ifdef PLATFORM_WIN32

#ifdef INDIELIB_DLLBUILD
#define DLL_EXP __declspec(dllexport)
#else
#define DLL_EXP __declspec(dllimport)
#endif //INDIELIB_DLLBUILD

#define LIB_EXP DLL_EXP
#define IndieLib() WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif //PLATFORM_WIN32

#ifdef PLATFORM_IOS
typedef unsigned char BYTE;    // HACK: Fixes some code regarding surfaces, fixes same problem as in Linux.
#if defined (__GNUC__) && __GNUC__ >= 4
#define LIB_EXP __attribute__ ((visibility("default")))
#else
#define LIB_EXP
#endif
#define IndieLib() main(int argc, char **argv)
#endif //PLATFORM_IOS

#ifdef PLATFORM_OSX
typedef unsigned char BYTE;    // HACK: Fixes some code regarding surfaces, fixes same problem as in Linux.
#if defined (__GNUC__) && __GNUC__ >= 4
#define LIB_EXP __attribute__ ((visibility("default")))
#else
#define LIB_EXP
#endif
#define IndieLib() main(int argc, char **argv)
#endif //PLATFORM_OSX

#ifdef PLATFORM_LINUX
#define LIB_EXP
typedef unsigned char BYTE;    // HACK: Fixes some code regarding surfaces, BYTE is not defined in Linux (and remember Linux is case sensitive BYTE != byte )
#define IndieLib() main(int argc, char * argv[])
#endif //PLATFORM_LINUX

// ----- Const -----

#define PI 3.14159265358979323846f

// --------------------------------------------------------------------------------
//							       Warnings OFF
// --------------------------------------------------------------------------------

//Visual C++ compilation warnings
#ifdef _MSC_VER
// ' ' needs to have dll-interface to be used by clients of class ' '
#pragma warning (disable : 4251)
// Identifier was truncated to '255' characters in the debug information
#pragma warning (disable : 4786)
// Defaultlib "MSVCRT" conflicts with use of other libs
#pragma warning (disable : 4098)
// all references to "GDI32.dll" discarded by /OPT:REF
#pragma warning (disable : 4089)
// 'sprintf': This function or variable may be unsafe.
#pragma warning (disable : 4996)
//Disables 'unreferenced formal parameter'
#pragma warning( disable : 4100 )  
#endif

//---------------------------------------------------------------------------------
//									Standard C/C++
//---------------------------------------------------------------------------------
#include <iostream>   //To use printf and other logging utilities


// --------------------------------------------------------------------------------
//								SDL 1.2 compatibility mode off
// --------------------------------------------------------------------------------
#define SDL_NO_COMPAT

//Vector3d utility
#include "IND_Vector3.h"
/**
 * @defgroup Pixel_Vertex Pixel and vertex formats
 * @ingroup Types
 */
/**@{*/
//DirectX
#ifdef INDIERENDER_DIRECTX
//!Pixel - When not rendering textures
struct structPixel {
    float _x; ///< Point position x
    float _y; ///< Point position y
    float _z; ///< Point position z
    unsigned long _color; ///< Point color, contains RGBA value
};
//! Alias for the pixel structure
typedef struct structPixel PIXEL;
#define D3DFVF_PIXEL (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#endif

//OPENGL
#ifdef INDIERENDER_OPENGL
//!Pixel - When not rendering textures
struct structPixelPos {
	float _x; ///< Point position x
    float _y; ///< Point position y
    float _z; ///< Point position z
	//Color
	float _colorR; ///< Point color R
    float _colorG; ///< Point color G
    float _colorB; ///< Point color B
    float _colorA; ///< Point color A
};
//! Alias for the pixel structure
typedef struct structPixelPos PIXEL;
#endif

//Win32 (DirectX used)
#ifdef INDIERENDER_DIRECTX
//!Vertex - When rendering with textures
struct structVertex2d {
	float _x; ///< Point position x
    float _y; ///< Point position y
    float _z; ///< Point position z
	float _u; ///< Texture mapping coordinate u
    float _v; ///< Texture mapping coordinate v
};
//! Alias for the 2d vertex structure
typedef struct structVertex2d CUSTOMVERTEX2D;

#define D3DFVF_CUSTOMVERTEX2D (D3DFVF_XYZ | D3DFVF_TEX1)
#endif

#ifdef INDIERENDER_OPENGL
//!Vertex - When rendering with textures
struct structVertex2d {
	float _x; ///< Point position x
    float _y; ///< Point position y
    float _z; ///< Point position z
	float _u; ///< Texture mapping coordinate u
    float _v; ///< Texture mapping coordinate v
};
//! Alias for the 2d vertex structure
typedef struct structVertex2d CUSTOMVERTEX2D;
#endif
/**@}*/

/**
 * @defgroup Math_strucutures Mathematical data structures
 * @ingroup Types
 */
/**@{*/

/**
 @brief Matrix representation used by all Indielib entities.
 
 It abstracts coordinate system handedness and how the matrices are stored in memory in the underlying renderer.
 
 Matrix (OpenGL COLUM-MAJOR ORDER!) (Note indices are the same, just store order in memory is different)
 Be aware that DirectX matrixes are stored in memory using ROW-MAJOR ORDER!. Use methods here to convert
 between them safely.
 
 Matrix would look like this:
 _11, _21, _31, _41
 _12, _22, _32, _42
 _13, _23, _33, _43
 _14, _24, _34, _44
 
 For example, Matrix in openGL is stored in array using colum-major order
 A Matrix 4x4\n (_11, _21, _31, _41,\n _12, _22, _32, _42,\n _13, _23, _33, _43\n _14, _24, _34, _44)
 */
struct structMatrix {
	
	float _11;      //!< Matrix element
    float _21;      //!< Matrix element
    float _31;      //!< Matrix element
    float _41;      //!< Matrix element
	float _12;      //!< Matrix element
    float _22;      //!< Matrix element
    float _32;      //!< Matrix element
    float _42;      //!< Matrix element
	float _13;      //!< Matrix element
    float _23;      //!< Matrix element
    float _33;      //!< Matrix element
    float _43;      //!< Matrix element
	float _14;      //!< Matrix element
    float _24;      //!< Matrix element
    float _34;      //!< Matrix element
    float _44;      //!< Matrix element

    /**
     @brief Default constructor
     */
	structMatrix() {
		_11 = _12 = _13 = _14 = _21 = _22 = _23 = _24 = _31 = _32 = _33 = _34 = _41 = _42 = _43 = _44 = 0;
	}
	
    /**
     @brief Constructor to initialize struct with an array
     
     It's useful to get data from matrices in render side
     
     @param matrixArray An array containing as many elements as 16 (4x4) to read matrix from.
     */
	structMatrix(float* matrixArray){
		if (!matrixArray) {
			_11 = _12 = _13 = _14 = _21 = _22 = _23 = _24 = _31 = _32 = _33 = _34 = _41 = _42 = _43 = _44 = 0.0f;
            return;
		}
		readFromArray(matrixArray);
	}

    /**
     @brief Writes to passed matrixArray the values inside the IND_Matrix structure
     Elements are interpreted this way:
     Matrix 4x4\n (_11, _12, _13, _14,\n _21, _22, _23, _24,\n _31, _32, _33, _34\n _41, _42, _43, _44)
     
     @param matrixArray An array which will be written with the representation of this matrix.
     */
	void arrayRepresentation(float* matrixArray) {
		if (!matrixArray) {
			return;
		}
		matrixArray[0] = _11;
		matrixArray[1] = _21;
		matrixArray[2] = _31;
		matrixArray[3] = _41;
		matrixArray[4] = _12;
		matrixArray[5] = _22;
		matrixArray[6] = _32;
		matrixArray[7] = _42;
		matrixArray[8] = _13;
		matrixArray[9] = _23;
		matrixArray[10] = _33;
		matrixArray[11] = _43;
		matrixArray[12] = _14;
		matrixArray[13] = _24;
		matrixArray[14] = _34;
		matrixArray[15] = _44;
	}

    /**
     @brief Operator override which does the right thing, that is matrix sum
     
     @param rhs Other matrix
     @return Sum matrix
     */
	structMatrix & operator=(const structMatrix &rhs) {
		if (this != &rhs)  {
			_11 = rhs._11;
			_21 = rhs._21;
			_31 = rhs._31;
			_41 = rhs._41;
			_12 = rhs._12;
			_22 = rhs._22;
			_32 = rhs._32;
			_42 = rhs._42;
			_13 = rhs._13;
			_23 = rhs._23;
			_33 = rhs._33;
			_43 = rhs._43;
			_14 = rhs._14;
			_24 = rhs._24;
			_34 = rhs._34;
			_44 = rhs._44;
		}
		return *this;
	}
    
    /**
     @brief Writes to passed matrixArray the values inside the IND_Matrix structure
     
     Elements are interpreted this way:
     Matrix 4x4\n (_11, _12, _13, _14,\n _21, _22, _23, _24,\n _31, _32, _33, _34\n _41, _42, _43, _44)
     
     @param matrixArray An array containing as many elements as 16 (4x4) to read matrix from.
     */
	void readFromArray(float* matrixArray) {
		_11 = matrixArray[0];
		_21 = matrixArray[1];
		_31 = matrixArray[2];
		_41 = matrixArray[3];
		_12 = matrixArray[4];
		_22 = matrixArray[5];
		_32 = matrixArray[6];
		_42 = matrixArray[7];
		_13 = matrixArray[8];
		_23 = matrixArray[9];
		_33 = matrixArray[10];
		_43 = matrixArray[11];
		_14 = matrixArray[12];
		_24 = matrixArray[13];
		_34 = matrixArray[14];
		_44 = matrixArray[15];
	}

    //!Logs itself to console.
    void description () const {
        std::cout << "\nMATRIX :\n";
        std::cout << _11 << " ";
        std::cout << _21 << " ";
        std::cout << _31 << " ";
        std::cout << _41 << " ";
        std::cout << std::endl;
        std::cout << _12 << " ";
        std::cout << _22 << " ";
        std::cout << _32 << " ";
        std::cout << _42 << " ";
        std::cout << std::endl;
        std::cout << _13 << " ";
        std::cout << _23 << " ";
        std::cout << _33 << " ";
        std::cout << _43 << " ";
        std::cout << std::endl;
        std::cout << _14 << " ";
        std::cout << _24 << " ";
        std::cout << _34 << " ";
        std::cout << _44 << " ";
        std::cout << std::endl;
    }
};
//!Alias for the matrix structure
typedef struct structMatrix IND_Matrix;


//! 2d Point 2d\n (x, y)
struct structPoint {
	int x;      //!< Coordinate x
    int y;      //!< Coordinate y
};
//! Alias for the 2d point structure
typedef struct structPoint IND_Point;

/**@}*/

// --------------------------------------------------------------------------------
//									Free memory operations
// --------------------------------------------------------------------------------

#define DISPOSE(x)      if (x)      { delete    x;  x = NULL; }
#define DISPOSEARRAY(x) if (x)      { delete[] x;  x = NULL; }
#define DISPOSEMANAGED(x) if (x)   { x->destroy(); x = NULL; }

// --------------------------------------------------------------------------------
//									Color Formats
// --------------------------------------------------------------------------------

/**
 * @defgroup ColorFormats Color definitions
 * @ingroup Types
 */
/**@{*/

//! Color formats

/**
IND_Image objects can have different formats depending on the number of colors. To have @b truecolor, the range of colors
that the human eye can perceive,  3 bytes per pixel are necessary. These are   @b R (Red), @b G (Green)
and @b B (Blue) bytes.

When we want images to have per pixel transparency, it is necessary an extra
byte. The A byte, also called alpha chanel. This byte doesn't show a color, but a level of transparency
 (0 full transparent, 255 full opaque).

It is also possible to define images with less colors and to obtain good results.
For example using gray range (they only use one byte per pixel).

<b>Types IND_ColorFormat</b>

@arg ::IND_RGB
@arg ::IND_RGBA
@arg ::IND_BGR
@arg ::IND_BGRA
@arg ::IND_LUMINANCE
@arg ::IND_COLOUR_INDEX
*/
typedef int IND_ColorFormat;
//! For unknown types
#define IND_UNKNOWN							0
//! Colour index
#define IND_COLOUR_INDEX                    1
//! Real color, using 3 bytes per pixel.
#define IND_RGB                             2
//! Real color, using 4 bytes per pixel. The fourth byte is used to the transparency variable.
#define IND_RGBA                            3
//! Gray range using 1 byte per pixel.
#define IND_LUMINANCE                       4

// --------------------------------------------------------------------------------
//								     Colour quality
// --------------------------------------------------------------------------------

//! Colour quality

/**
@b IND_Quality shows the quality of the entity that it references.

You can consume less memory by adjusting this parameter, and as a result losing some quality in the colors.

This atributte alone let us to put entities (fonts, surfaces, etc) in gray color range.
*/
typedef int IND_Quality;

//! It uses 8 bits for gray range. This is the attribute that wastes less memory.
#define IND_GREY_8                          8
//! This is like  ::IND_GREY_8 but it uses 16 bits for gray range, getting better quality for images that we want to render in "black and white".
#define IND_GREY_16                         15
//! It uses 16 bits colour. Sometimes the diference between this attribute and  ::IND_32 is indiscernible, but this one wastes less memory.
#define IND_16                              16
//! It uses a 32 bits of real colour. This is the maximum colour quality that can be get with this library and consistently the one that wastes more memory.
#define IND_32                              32
/**@}*/


// --------------------------------------------------------------------------------
//								     Light types
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_LightType Light types
 * @ingroup Types
 */
/**@{*/

//! Light types

/**
@b IND_LightType is the type of the light

You can define different type of lights using this type.
*/
typedef int IND_LightType;

/**
Ambient light represents the background light in a scene. In the real world light bounces off many objects and creates
a low light level so even geometry facing away from any directional light is still partially lit. Ambient light is very
inexpensive in terms of frame rate - almost free. It is applied to all surfaces irrespective of the direction they are
facing. There can be only one ambient light enabled at a time.
*/
#define IND_AMBIENT_LIGHT                           100

/**
Directional light is used to simulate distant light sources like the sun. It has a direction but no position. e.g.
you could set it to point downward (0,-1,0) which would simulate the sun being directly overhead. Directional light is
relatively inexpensive although if you add lots of them you may find your frame rate dropping somewhat. The lighting
effect on objects depends on which way they are facing relative to the light direction (defined in the vertex normal).
So a triangle with normal pointing straight up (0,1,0) would be lit by our sun (0,-1,0) fully - it would receive the
maximum amount of light. As the angle of the normal to light increases less light is applied. If the triangle normal
faces away from the light it is not lit at all.

Methods you can use for changing the attributes of these type of lights:
- IND_Light::SetDirection()
- IND_Light::SetRange()
- IND_Light::SetColor()
*/
#define IND_DIRECTIONAL_LIGHT                       101

/**
A point light represents a point source. It has a position in the world and radiates light in all directions. A bare
bulb on a stand would be an example of this. It radiates light in every direction and has a position in the world. This
is a more expensive type of light in terms of frame rate than a directional light.

A point light has an attenuation (use IND_Light::SetAttenuation()) that defines how the light level decreases over distance
and a range. The range (use IND_Light::SetRange()) is the maximum distance the light will travel.

Methods you can use for changing the attributes of these type of lights:
- IND_Light::setPosition()
- IND_Light::SetRange()
- IND_Light::SetColor()
- IND_Light::SetAttenuation()
- IND_Light::SetRange()
*/
#define IND_POINT_LIGHT                             102

/**
Spot light is is the most complex light and is the most expensive type in
terms of frame rate so use sparingly. A spot light has a position in space and a direction. It emits a cone of light
with two degrees of intensity. It has a central brightly lit section and a surrounding dimly lit section that merges
with the surrounding shadow. Only objects within the light cone are illuminated.

To set a spot light you need to provide position, direction, cone size and a number of parameters determining the spread of
the cone. Theta (use IND_Light::SetTheta()) is the angle that defines the inner cone while Phi (use IND_Light::SetPhi())
defines the outer cone. Falloff (use IND_Light::SetFalloff()) defines the decrease in illumination between the outside of
the inner cone and the outside of the outer cone

Methods you can use for changing the attributes of these type of lights:
- IND_Light::setPosition()
- IND_Light::SetRange()
- IND_Light::SetColor()
- IND_Light::SetAttenuation()
- IND_Light::SetRange()
- IND_Light::SetFalloff()
- IND_Light::SetPhi()
- IND_Light::SetTheta()
*/
#define IND_SPOT_LIGHT                              103
/**@}*/

/**
 * @defgroup IND_Type Surface types
 * @ingroup Types
 */
/**@{*/

//! Surface types

/**
@b IND_Type (type of surface).

Specifying the type  (opaque or transparent), you can consume less memory. For example,
when drawing the background of a game, that background usually doesn't
need alpha chanel for transparency (dosent't have transparent pixels), so it can be loaded
as  ::IND_OPAQUE.

However, another entity of your game could need transparency in some pixels of the image, then
you have to use the type
::IND_ALPHA. This type consumes more memory that ::IND_OPAQUE but both will be equally fast
when rendering.

<b>Types IND_Type</b>

@arg ::IND_OPAQUE
@arg ::IND_ALPHA
*/
typedef int IND_Type;

//! It  consumes less memory than IND_ALPHA, but it doesn't allow transparency. Usually it is used for backgrounds or entities that haven't transparent pixels inside the image. This method isn't faster when rendering than ::IND_ALPHA.
#define IND_OPAQUE                          200
//! It allows per pixel transparency, but consumes more memory that IND_OPAQUE.
#define IND_ALPHA                           202
/**@}*/

/**
 * @defgroup IND_Align Font alignment
 * @ingroup Types
 */
/**@{*/

//! Text alignment

/**
Used for the alignment of the fonts.
*/
typedef int IND_Align;

//! Center alignment.
#define IND_CENTER                          300
//! Right alignment.
#define IND_RIGHT                           301
//! Left alignment.
#define IND_LEFT                            302
/**@}*/

/**
 * @defgroup IND_Primitive2d 2d Primitives
 * @ingroup Types
 */
/**@{*/

//! Type of primitives

/**
IND_Primitive2d defines a primitive (lines, circles, rectangles, etc).
*/
typedef int IND_Primitive2d;

//! Pixel (dot).
#define IND_PIXEL                           400
//! Line.
#define IND_LINE                            401
//! Rectangle.
#define IND_RECTANGLE                       402
//! Polygon. The vertex list is an array of ::IND_Point, that type consist on two coordinates (x, y).
#define IND_POLY2D                          403
//! Regular polygon with n sides.
#define IND_REGULAR_POLY                    404
//! Filled rectangle.
#define IND_FILL_RECTANGLE                  405
/**@}*/

/**
 * @defgroup IND_BlendingType Blending types
 * @ingroup Types
 */
/**@{*/

//! Blending type

/**
Type @b IND_BlendigType define the type of blend (to do alphablending). Alphablending is a powerful tool that can be used
to create lots of graphical efects ( burst, electricity, speck efects, rain, etc).

The more usual factors of  blending are:

@arg (source, destination) = (::IND_SRCALPHA, ::IND_INVSRCALPHA) => This is the usual method of  blending to draw picture with alpha channel. It regards the alpha channel to draw pictures that have variable transparency.
@arg (source, destination) = (::IND_SRCALPHA, ::IND_ONE) => It is used to speck effects. In the source picture it has black areas than later will be transparent in the screen.

You can try diferents values to the source and to the destination and you can get a big amount of
diverse efects as much additive as subtractive (relative to RGBA values) and a great visual impact.
*/
typedef int IND_BlendingType;

//! Blend factor  (0, 0, 0, 0).
#define IND_ZERO                            500
//! Blend factor (1, 1, 1, 1).
#define IND_ONE                             501
//! Blend factor (Rs,Gs,Bs,As).
#define IND_SRCCOLOR                        502
//! Blend factor (1 - Rs, 1 - Gs, 1 - Bs, 1 - As).
#define IND_INVSRCCOLOR                     503
//! Blend factor (As, As, As, As).
#define IND_SRCALPHA                        504
//! Blend factor ( 1 - As, 1 - As, 1 - As, 1 - As).
#define IND_INVSRCALPHA                     505
//! Blend factor (Ad, Ad, Ad, Ad).
#define IND_DESTALPHA                       506
//! Blend factor (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad).
#define IND_INVDESTALPHA                    507
//! Blend factor (Rd, Gd, Bd, Ad).
#define IND_DESTCOLOR                       508
//! Blend factor (1 - Rd, 1 - Gd, 1 - Bd, 1 - Ad).
#define IND_INVDESTCOLOR                    509
//! Blend factor (f, f, f, 1); f = min(A, 1 - Ad).
#define IND_SRCALPHASAT                     510
//! Obsoleto.
#define IND_BOTHSRCALPHA                    511
//! Blend factor (1 - As, 1 - As, 1 - As, 1 - As), y el destino (As, As, As, As);
#define IND_BOTHINVSRCALPHA                 512
//! Blend colour.
#define IND_BLENDFACTOR                     513
//! Blend opposite colour.
#define IND_INVBLENDFACTOR                  514
/**@}*/

/**
 * @defgroup IND_Filter Filtering when rendering
 * @ingroup Types
 */
/**@{*/

//! Texture filters

/**
These filters affects the surfaces when they are rendered on the screen. When we rotate or
we resize a surface, the quality of the image will be better or worse depending on the filter used.

<b>Types IND_Filter</b>

@arg ::IND_FILTER_NONE
@arg ::IND_FILTER_POINT
@arg ::IND_FILTER_LINEAR
*/
typedef int IND_Filter;

//! No filter applied.
#define IND_FILTER_NONE                     0
//! Nearest point filter. This is faster than  IND_FILTER_LINEAR, but has less quality.
#define IND_FILTER_POINT                    1
//! Bilinear interpolation filter. This is quite fast, and has better quality than IND_FILTER_POINT.
#define IND_FILTER_LINEAR                   2
/**@}*/

#ifndef PLATFORM_IOS   //KEYS FOR DESKTOP OS

/**
 * @defgroup IND_Key Input keys
 * @ingroup Types
 */
/**@{*/

//! Keys


/**
Input keys of the keyboard.
*/
typedef int IND_Key;


enum {
	//! a
	IND_A,
	//! b
	IND_B,
	//! c
	IND_C,
	//! d
	IND_D,
	//! e
	IND_E,
	//! f
	IND_F,
	//! g
	IND_G,
	//! h
	IND_H,
	//! i
	IND_I,
	//! j
	IND_J,
	//! k
	IND_K,
	//! l
	IND_L,
	//! m
	IND_M,
	//! n
	IND_N,
	//! o
	IND_O,
	//! p
	IND_P,
	//! q
	IND_Q,
	//! r
	IND_R,
	//! s
	IND_S,
	//! t
	IND_T,
	//! u
	IND_U,
	//! v
	IND_V,
	//! w
	IND_W,
	//! x
	IND_X,
	//! y
	IND_Y,
	//! z
	IND_Z,

	//! 0
	IND_0,
	//! 1
	IND_1,
	//! 2
	IND_2,
	//! 3
	IND_3,
	//! 4
	IND_4,
	//! 5
	IND_5,
	//! 6
	IND_6,
	//! 7
	IND_7,
	//! 8
	IND_8,
	//! 9
	IND_9,

	//! backspace key
	IND_BACKSPACE,
	//! Tab
	IND_TAB,
	//! Clear
	IND_CLEAR,
	//! Return
	IND_RETURN,
	//! Pause
	IND_PAUSE,
	//! Escape
	IND_ESCAPE,
	//! Space
	IND_SPACE,
	//! Exclaim
	IND_EXCLAIM,
	//! Exclaim
	IND_QUOTEDBL,
	//! hash
	IND_HASH,
    //! percent
    IND_PERCENT,
	//! Dollar
	IND_DOLLAR,
	//! ampersand
	IND_AMPERSAND,
	//! quote
	IND_QUOTE,
	//! left parenthesis
	IND_LEFTPAREN,
	//! right parenthesis
	IND_RIGHTPAREN,
	//! Asterisk
	IND_ASTERISK,
	//! plus
	IND_PLUS,
	//! Comma
	IND_COMMA,
	//! Minus
	IND_MINUS,
	//! Period
	IND_PERIOD,
	//! slash
	IND_SLASH,

	//! colon
	IND_COLON,
	//! semicolon
	IND_SEMICOLON,
	//! less than
	IND_LESS,
	//! equals
	IND_EQUALS,
	//! greater than
	IND_GREATER,
	//! question
	IND_QUESTION,
	//! at
	IND_AT,
	//! left bracket
	IND_LEFTBRACKET,
	//! Back slash
	IND_BACKSLASH,
	//! right bracket
	IND_RIGHTBRACKET,
	//! caret
	IND_CARET,
	//! underscore
	IND_UNDERSCORE,
	//! back quote
	IND_BACKQUOTE,

	//! Delete
	IND_DELETE,

	//! 0 (numeric keyboard area)
	IND_K0,
	//! 1 (numeric keyboard area)
	IND_K1,
	//! 2 (numeric keyboard area)
	IND_K2,
	//! 3 (numeric keyboard area)
	IND_K3,
	//! 4 (numeric keyboard area)
	IND_K4,
	//! 5 (numeric keyboard area)
	IND_K5,
	//! 6 (numeric keyboard area)
	IND_K6,
	//! 7 (numeric keyboard area)
	IND_K7,
	//! 8 (numeric keyboard area)
	IND_K8,
	//! 9 (numeric keyboard area)
	IND_K9,

	//! Period (numeric keyboard area)
	IND_KPERIOD,
	//! Divide (numeric keyboard area)
	IND_KDIVIDE,
	//! Multiply (numeric keyboard area)
	IND_KMULTIPLY,
	//! Minus (numeric keyboard area)
	IND_KMINUS,
	//! Plus (numeric keyboard area)
	IND_KPLUS,
	//! Enter (numeric keyboard area)
	IND_KENTER,
	//! Equals (numeric keyboard area)
	IND_KEQUALS,

	//! Key up
	IND_KEYUP,
	//! Key down
	IND_KEYDOWN,
	//! Key right
	IND_KEYRIGHT,
	//! Key left
	IND_KEYLEFT,

	//! Insert
	IND_INSERT,
	//! Home
	IND_HOME,
	//! End
	IND_END,
	//! Page up
	IND_PAGEUP,
	//! Page down
	IND_PAGEDOWN,

	//! F1
	IND_F1,
	//! F2
	IND_F2,
	//! F3
	IND_F3,
	//! F4
	IND_F4,
	//! F5
	IND_F5,
	//! F6
	IND_F6,
	//! F7
	IND_F7,
	//! F8
	IND_F8,
	//! F9
	IND_F9,
	//! F10
	IND_F10,
	//! F11
	IND_F11,
	//! F12
	IND_F12,
	//! F13
	IND_F13,
	//! F14
	IND_F14,
	//! F15
	IND_F15,

	//! Numlock
	IND_NUMLOCK,
	//! Capslock
	IND_CAPSLOCK,
	//! Scrollock
	IND_SCROLLOCK,
	//! Right shift
	IND_RSHIFT,
	//! Left shift
	IND_LSHIFT,
	//! Right control
	IND_RCTRL,
	//! Left control
	IND_LCTRL,
	//! Right Alt
	IND_RALT,
	//! Left Alt
	IND_LALT,
	//! Right Meta
	IND_RMETA,
	//! Left Meta
	IND_LMETA,
	//! Mode shift
	IND_MODE,
	//! Help
	IND_HELP,
	//! Print
	IND_PRINT,
	//! Sysreq
	IND_SYSREQ,
	//! Break
	IND_BREAK,
	//! Menu
	IND_MENU,
	//! Power
	IND_POWER,
	//! Currency unit in keyboard
	IND_CURRENCYUNIT
};
/**@}*/

/**
 * @defgroup IND_KeyState Input key states
 * @ingroup Types
 */
/**@{*/

//! Key state

/**
Key state.
*/
typedef int IND_KeyState;

//! Pressed key
#define IND_KEY_PRESSED 1
//! Not pressed key
#define IND_KEY_NOT_PRESSED 0
/**@}*/

/**
 * @defgroup IND_MouseButton Mouse buttons
 * @ingroup Types
 */
/**@{*/

//! Mouse Buttons

/**
Mouse input buttons.
*/
typedef int IND_MouseButton;

//! Left mouse button
#define IND_MBUTTON_LEFT 0
//! Right mouse button
#define IND_MBUTTON_RIGHT 1
//! Middle mouse button
#define IND_MBUTTON_MIDDLE 2
/**@}*/

/**
 * @defgroup IND_MouseButtonState Mouse button state
 * @ingroup Types
 */
/**@{*/

//! Mouse button states

/**
States of mouse buttons.
*/
typedef int IND_MouseButtonState;

//! Button pressed
#define IND_MBUTTON_PRESSED 1
//! Button not pressed
#define IND_MBUTTON_NOT_PRESSED 0
/**@}*/

#endif  //KEYS FOR DESKTOP OS
    
/**
 * @defgroup IND_Debug Debug mode
 * @ingroup Types
 */
/**@{*/

//! g_debug

/**
Using IndieLib::init() method, the library can be initialized with the modes:
::IND_DEBUG_MODE (debug mode) , ::IND_RELEASE_MODE (release mode).

::IND_DEBUG_MODE starts the library in debug mode. In this mode at the end of the execution
a file named debug.txt is generated. It contains details about the project execution. Here you can verify
if the resources are correctly loaded/freed. Please check this file if any problem happen after the execution of
an IndieLib application.

::IND_RELEASE_MODE is the mode that should be chosen for a finished project.
*/
typedef int IND_InitializationMode;

//! Iniatializes the engine in debug mode.
#define IND_DEBUG_MODE                      1
//! Iniatializes the engine in release mode.
#define IND_RELEASE_MODE                    0
/**@}*/


/**
 * @defgroup BoundingCollisions Bounding collisions
 * @ingroup Types
 */
/**@{*/
//! Encapsulates information about a parsed bounding collision information from xml
struct structBoundingCollision {
public:
	int _posX;                          //!<_posX Position
    int _posY;                          //!< _posY Position
	int _radius;                        //!< Radius of the circle
	int _ax;                            //!< Vertex of the triangle
    int _ay;                            //!< Vertex of the triangle
    int _bx;                            //!< Vertex of the triangle
    int _by;                            //!< Vertex of the triangle
    int _cx;                            //!< Vertex of the triangle
    int _cy;                            //!< Vertex of the triangle
private:
    int _type;                          //!< 0 = Triange, 1 = Circle
	char *_id;                          //!< Group Id for grouping bounding areas
    
public:
    //! Default constructor
	structBoundingCollision() {
		_type = _posX = _posY = _radius = _ax = _ay = _bx = _by = _cx = _cy = 0;
	}
    
    /**
     Designater constructor
     @param type The type for this collision. Can't be changed after creation
     @param identifier A string to match with a collision zone. Can't be changed after
    */
    structBoundingCollision(int type, const char* identifier) {
        _type = type;
        _id = new char[strlen(identifier) + 1];
        strcpy(_id, identifier);
        _posX = _posY = _radius = _ax = _ay = _bx = _by = _cx = _cy = 0;
    }
    ~structBoundingCollision() {
        DISPOSEARRAY(_id);
    }
    
    
    //!Returns the ID for this collision
    const char* const getId() {
        return _id;
    }

    //!Returns the type of this collision
    int getType() {
        return _type;
    }
};
//! Alias for the bounding collision structure
typedef struct structBoundingCollision BOUNDING_COLLISION;
/**@}*/

/**
 * @defgroup Culling
 * @ingroup Types
 */
/**@{*/
    
//! Frustrum plane representation via normal and a distance from origin. Generally represented as Ax+By+Cz+D=0
struct StructFrustrumPlane {
    IND_Vector3 _normal; 			//!<_normal The normal vector describing the plane (A,B,C)
    float _distance;				//!<_distance The distance of the point in plane from origin (D)
    
    /**
     Calculates the distance between the plane to a 3d point in space.
     @param pPnt The point to calculate distance to
     @return Signed distance module between point and plane
     */
    float distanceToPoint(IND_Vector3 &pPnt) {
        return  _normal.dotProduct(pPnt) + _distance;
    }
    
    //! Normalises the plane. Better to execute signed distance calculations
    void normalise() {
        float denom = 1 / sqrt((_normal._x * _normal._x) + (_normal._y * _normal._y) + (_normal._z * _normal._z));
        _normal._x = _normal._x * denom;
        _normal._y = _normal._y * denom;
        _normal._z = _normal._z * denom;
        _distance = _distance * denom;
    }
    
    //! Prints itself to console.
    void description() {
        printf("\nFRUSTRUMPLANE: Normal: (%.5f,%.5f,%.5f) Distance:%.5f",_normal._x,_normal._y,_normal._z,_distance);
    }
};

//! A frustrum is composed of 6 planes in a bounding volume.
struct StructFrustrum {
	StructFrustrumPlane _planes[6];			//!<planes The planes consisting the frustrum

    //! Prints itself to console.
	void description() {
		for (int i = 0;i < 6;++i) {
			_planes[i].description();
		}
	}
};

typedef StructFrustrum FRUSTRUMPLANES;
/**@}*/
/**
 * @defgroup GlobalConstants Global constants
 * @ingroup Types
 */
/**@{*/
//! Determines the max characters in a string
const int MAX_CHARS_IN_INT32_STR = (10 + 1); // +1 is for '\0' character

//! Determines the max characters in a string
const int MAX_CHARS_IN_INT64_STR = (20 + 1);
    
//! Determines how a circle is renderered
const int SIDES_PER_CIRCLE = 30;   //Number of sides a circle has (will blit as many lines as specified here when blitting a circle primitive)

//! Default size for allocated char buffers
const int MAX_TOKEN = 1024;
/**@}*/

#endif // _DEFINES_
