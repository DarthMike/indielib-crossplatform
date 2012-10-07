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
\mainpage
\section main_general Menu
<A HREF="modules.html">Click here to access the Menu</A>
*/

/**
 * @defgroup Classes
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
 * @defgroup EntityManagers Entity Objects and Entity Managers (the most important concept of IndieLib)
 * @ingroup Classes
 */

/**
 * @defgroup Managers Managers
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
 * @defgroup Advances Advances methods for bliting directly to the screen without using entities. It uses IND_Render methods (recommended only for advanced IndieLib users)
 */

/**
 * @defgroup Primitives Bliting Primitives
 * @ingroup Advances
 */

/**
 * @defgroup Graphical_Objects Bliting Surfaces, Animations and Fonts and setting the transformations directly
 * @ingroup Advances
With these methods you can set the 2d transformations (using IND_Render::Set2dTransform) and color
attributes (using IND_Render::SetRainbow()) and blit directly
to the screen surfaces, animations, and fonts using IND_Render class.

Remember that you can use ::IND_Entity2d together with ::IND_Entity2dManager for drawing these graphical objects
to the screen without having to use this advanced method directly. This method is only useful for advanced
users for really concrete purposes.
 */


/**
 * @defgroup Graphical_3d_Objects Bliting 3d Animated 3d Models and setting the transformations directly
 * @ingroup Advances
With these methods you can set the 3d transformations (using IND_Render::Set3dTransform) and color
attributes (using IND_Render::SetRainbow()) and to blit directly
to the screen 3d animated meshes using IND_Render class.

Remember that you can use ::IND_Entity3d together with ::IND_Entity3dManager for drawing these graphical objects
to the screen without having to use this advanced method directly. This method is only useful for advanced
users for really concrete purposes.
 */


/**
 * @defgroup Types
 */



#ifndef _DEFINES_
#define _DEFINES_

// ----- Platform checkings -----
//Platform definition checkings
#include "IndiePlatforms.h"

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
#define DLL_EXP __declspec(dllexport)
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

// --------------------------------------------------------------------------------
//									Pixel and vertex formats
// --------------------------------------------------------------------------------
//Vector3d utility
#include "IND_Vector3.h"

//DirectX
#ifdef INDIERENDER_DIRECTX
		// Pixel - When not rendering textures
		struct structPixel {
			// Position
			float _x, _y, _z;

			// Color
			unsigned long _color;
		};
typedef struct structPixel PIXEL;
#define D3DFVF_PIXEL (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#endif

//OPENGL
#ifdef INDIERENDER_OPENGL
// Pixel - When not rendering textures
struct structPixelPos {
	// Position
	float _x, _y, _z;
	//Color
	float _colorR, _colorG, _colorB, _colorA;
};
typedef struct structPixelPos PIXEL;
#endif


//iOS (GL ES)
#ifdef INDIERENDER_GLES_IOS
// Pixel - When not rendering textures
struct structPixelPos {
	// Position
	float _x, _y, _z;
	//Color
	float _colorR, _colorG, _colorB, _colorA;
};
typedef struct structPixelPos PIXEL;
#endif

//Win32 (DirectX used)
#ifdef INDIERENDER_DIRECTX
// Vertex format - When rendering with textures
struct structVertex2d {
	float _x, _y, _z;           // Position
	float _u, _v;               // Mapping coordinates
};
typedef struct structVertex2d CUSTOMVERTEX2D;

#define D3DFVF_CUSTOMVERTEX2D (D3DFVF_XYZ | D3DFVF_TEX1)
#endif

//iOS (GL ES)
#ifdef INDIERENDER_OPENGL
// Vertex format - When rendering with textures
struct structVertex2d {
	float _x, _y, _z;           // Position
	float _u, _v;               // Mapping coordinates
};
typedef struct structVertex2d CUSTOMVERTEX2D;
#endif

//iOS (GL ES)
#ifdef INDIERENDER_GLES_IOS
// Vertex format - When rendering with textures
struct structVertex2d {
	float _x, _y, _z;           // Position
	float _u, _v;               // Mapping coordinates
};
typedef struct structVertex2d CUSTOMVERTEX2D;
#endif

// --------------------------------------------------------------------------------
//										Structures
// --------------------------------------------------------------------------------

/**
 * @defgroup Additional structures
 * @ingroup Types
 */
/*@{*/

struct structMatrix {
	//Matrix (OpenGL COLUM-MAJOR ORDER!) (Note indices are the same, just store order in memory is different)
	//Be aware that DirectX matrixes are stored in memory using ROW-MAJOR ORDER!. Use methods here to convert
	//between them safely.
	float _11, _21, _31, _41;
	float _12, _22, _32, _42;
	float _13, _23, _33, _43;
	float _14, _24, _34, _44;

	structMatrix() {
		_11 = _12 = _13 = _14 = _21 = _22 = _23 = _24 = _31 = _32 = _33 = _34 = _41 = _42 = _43 = _44 = 0;
	}
	//Initializes the structure with a 4x4 matrix in array format. 
	//Elements are interpreted this way:
	//Matrix 4x4\n (_11, _12, _13, _14,\n _21, _22, _23, _24,\n _31, _32, _33, _34\n _41, _42, _43, _44)
	structMatrix(float* matrixArray){
		if (!matrixArray) {
			_11 = _12 = _13 = _14 = _21 = _22 = _23 = _24 = _31 = _32 = _33 = _34 = _41 = _42 = _43 = _44 = 0;
            return;
		}
		readFromArray(matrixArray);
	}

	//Writes to passed matrixArray the values inside the IND_Matrix structure
	//Elements are interpreted this way:
	//Matrix 4x4\n (_11, _12, _13, _14,\n _21, _22, _23, _24,\n _31, _32, _33, _34\n _41, _42, _43, _44)
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
    
	//Writes to passed matrixArray the values inside the IND_Matrix structure
	//Elements are interpreted this way:
	//Matrix 4x4\n (_11, _12, _13, _14,\n _21, _22, _23, _24,\n _31, _32, _33, _34\n _41, _42, _43, _44)
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

    //Logs itself to console
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
//! Matrix 4x4\n (_11, _21, _31, _41,\n _12, _22, _32, _42,\n _13, _23, _33, _43\n _14, _24, _34, _44)
typedef struct structMatrix IND_Matrix;


// Point
struct structPoint {
	int x, y;
};
//! 2d Point 2d\n (x, y)
typedef struct structPoint IND_Point;

/*@}*/

// --------------------------------------------------------------------------------
//									Free memory operations
// --------------------------------------------------------------------------------

#define DISPOSE(x)      if (x)      { delete    x;  x = NULL; }
#define DISPOSEARRAY(x) if (x)      { delete[] x;  x = NULL; }


// --------------------------------------------------------------------------------
//									Color Formats
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_ColorFormat
 * @ingroup Types
 */
/*@{*/

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
//TODO: MIGUEL - REVIEW THAT ALL DEFINED TYPES ARE USED. I MISS WHY THERE IS IND_BGR AND IND_BGRA
typedef int IND_ColorFormat;
//! For unknown types
#define IND_UNKNOWN							0
//! Colour index
#define IND_COLOUR_INDEX                    1
//! Real color, using 3 bytes per pixel.
#define IND_RGB                             2
//! Real color, using 4 bytes per pixel. The fourth byte is used to the transparency variable.
#define IND_RGBA                            3
//! Real colour using 3 bytes per pixel.
#define IND_BGR                             4
//! Real color using 3 bytes per pixel. The fourth byte is used to the transparency variable.
#define IND_BGRA                            5
//! Gray range using 1 byte per pixel.
#define IND_LUMINANCE                       6

//#define IND_LUMINANCE_ALPHA                   0x190A
/*@}*/


// --------------------------------------------------------------------------------
//								     Colour quality
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_Quality
 * @ingroup Types
 */
/*@{*/

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
/*@}*/


// --------------------------------------------------------------------------------
//								     Light types
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_LightType
 * @ingroup Types
 */
/*@{*/

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
/*@}*/



// --------------------------------------------------------------------------------
//							          Surface types
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_Type
 * @ingroup Types
 */
/*@{*/

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
/*@}*/


// --------------------------------------------------------------------------------
//								Font alignment
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_Align
 * @ingroup Types
 */
/*@{*/

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
/*@}*/


// --------------------------------------------------------------------------------
//									Primitives
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_Primitive2d
 * @ingroup Types
 */
/*@{*/

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
/*@}*/


// --------------------------------------------------------------------------------
//										Blending
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_BlendingType
 * @ingroup Types
 */
/*@{*/

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
/*@}*/


// --------------------------------------------------------------------------------
//									   Filters
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_Filter
 * @ingroup Types
 */
/*@{*/

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
/*@}*/

#ifndef PLATFORM_IOS   //KEYS FOR DESKTOP OS
// --------------------------------------------------------------------------------
//										  Keys
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_Key
 * @ingroup Types
 */
/*@{*/

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
/*@}*/


// --------------------------------------------------------------------------------
//									   Key State
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_KeyState
 * @ingroup Types
 */
/*@{*/

//! Key state

/**
Key state.
*/
typedef int IND_KeyState;

//! Pressed key
#define IND_KEY_PRESSED 1
//! Not pressed key
#define IND_KEY_NOT_PRESSED 0
/*@}*/


// --------------------------------------------------------------------------------
//									   Mouse Button
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_MouseButton
 * @ingroup Types
 */
/*@{*/

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
/*@}*/


// --------------------------------------------------------------------------------
//									   Mouse Button State
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_MouseButtonState
 * @ingroup Types
 */
/*@{*/

//! Mouse button states

/**
States of mouse buttons.
*/
typedef int IND_MouseButtonState;

//! Button pressed
#define IND_MBUTTON_PRESSED 1
//! Button not pressed
#define IND_MBUTTON_NOT_PRESSED 0
/*@}*/

#endif  //KEYS FOR DESKTOP OS
// --------------------------------------------------------------------------------
//										 g_debug
// --------------------------------------------------------------------------------

/**
 * @defgroup IND_Debug
 * @ingroup Types
 */
/*@{*/

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
/*@}*/


// --------------------------------------------------------------------------------
//								 Bounding collision
// --------------------------------------------------------------------------------

struct structBoundingCollision {
	int _type;                          // 0 = Triange, 1 = Circle
	char *_id;                          // Group Id for grouping bounding areas
	int _posX, _posY;                   // Position
	int _radius;                        // Radius of the circle
	int _ax, _ay, _bx, _by, _cx, _cy;   // Vertices of the triangle

	structBoundingCollision() {
		_type = _posX = _posY = _radius = _ax = _ay = _bx = _by = _cx = _cy = 0;
	}
};
typedef struct structBoundingCollision BOUNDING_COLLISION;

// --------------------------------------------------------------------------------
//								 Global constants
// --------------------------------------------------------------------------------
const int MAX_CHARS_IN_INT32_STR = (10 + 1); // +1 is for '\0' character
const int MAX_CHARS_IN_INT64_STR = (20 + 1);
const int SIDES_PER_CIRCLE = 30;   //Number of sides a circle has (will blit as many lines as specified here when blitting a circle primitive)


#endif // _DEFINES_
