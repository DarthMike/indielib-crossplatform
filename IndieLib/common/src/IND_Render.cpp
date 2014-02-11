/*****************************************************************************************
 * File: IND_Render.cpp
 * Desc: Initialization / Destruction of the render.
 *       Hide / wraps the platform specific render.
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


// ----- Includes -----

#include "Global.h"
#include "IND_Math.h"
#include "IND_SurfaceManager.h"
#include "IND_Timer.h"
#include "IND_Render.h"
#include "dependencies/SDL-2.0/include/SDL.h"

// ----- Libs -----
#ifdef INDIERENDER_DIRECTX
#include "render/directx/DirectXRender.h"
#endif

#ifdef INDIERENDER_GLES_IOS
#include "render/gles/ios/OpenGLES2Render.h"
#endif

#ifdef INDIERENDER_OPENGL
#include "render/opengl/OpenGLRender.h"
#endif

// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
@b Parameters:

@arg @b windowproperties         Properties of window to be created

@b Operation:

 Creates a render and window, with supplied window params. Window params need to be compatible with platform
 running on, as engine will not try to fit display params to something 'similar'. 
 
 
 If device does not support given resolution, specially important for mobile platforms (running in an iPad and asking for a
 screen with 800x600), will still create a full screen window, and ignore the supplied width and height.
 
  This function returns a pointer to correctly created window if success. NULL if failed
*/
IND_Window* IND_Render::initRenderAndWindow(IND_WindowProperties& windowproperties) {
	resetTimer();
	_fpsCounter = 0;
	_currentTimeFps = 0.0f;
	_lastTimeFps = 0.0f;
	_lastFps = 0;

	//Actually create real render object
	return(createRender(windowproperties));
}
/**
@b Parameters:

@b Operation:

This function returns 1 (true) if the render is initialized sucessfully,
0 (false) otherwise.
*/
bool IND_Render::isOK() {
	return (_wrappedRenderer->isOK());
}

/**
@b Parameters:

@arg @b pTitle                  Title of the window
@arg @b pWidth                  Width of the window
@arg @b pHeight                 Height of the window
@arg @b pBpp                    Quality of color (32 o 16 bits). 32 bits offers better quality. 16 bits offers better speed.
@arg @b pVsync                  Wait for vertical sync. (1 / 0) = (on / off).
@arg @b pFullscreen             Full screen. (1 / 0) = (on / off).

@b Operation:

This function returns 1 (true) if the application window resets to the attributes passed as parameters. This method
is useful when you want to change the application window on runtime, 0 (false) if it is not possible to create
the new window.
*/
bool IND_Render::reset(IND_WindowProperties& props) {
#ifdef DEBUG
    std::cout<<"Resetting window parameters"<<std::endl;
    std::cout<<"Title: "<<props._title<<std::endl;
    std::cout<<"Width: "<<props._width<<std::endl;
    std::cout<<"Height: "<<props._height<<std::endl;
    std::cout<<"Bpp: "<<props._bpp<<std::endl;
    std::cout<<"Vsync: "<<props._vsync<<std::endl;
    std::cout<<"Full screen: "<<props._fullscreen<<std::endl;
#endif
    
	return (_wrappedRenderer->reset(props));
}


/**
@b Operation:

This function returns 1 (true) if the application window toggles to fullscreen or windowed, 0 (false) if it is not possible
to create the new window.
*/
bool IND_Render::toggleFullScreen() {
	return (_wrappedRenderer->toggleFullScreen());
}


/**
@b Operation:

This function frees the manager and all the objects that it contains.
*/
void IND_Render::end() {
	if (_wrappedRenderer) {
		_wrappedRenderer->end();
		DISPOSE(_wrappedRenderer);
	}
}


// --------------------------------------------------------------------------------
//							        Public methods
// --------------------------------------------------------------------------------
/*
 ==================
 TODO: CHECK THE USE OF THIS METHOD IN THE FUTURE, CURRENTLY IS DEPRECATED. ¿LET IT BE UNTIL HAVING PIXEL SHADER SUPPORT?
 
 @b Parameters:
 
 @b pSwitch       Activates or deactivates the antialiasing. (true = antialiasing on, false = antialiasing of)
 
 Operation:
 
 This method activates or deativates the antialiasing when drawing primitives. It doesn't affect
 to other graphical objects, only to primitives.
 
 This function will return 1 if the antialiasing is activated or deactivated correctly and 0 if the
 graphic card of the user doesn't support this feature.
 ==================
 */
inline bool IND_Render::setAntialiasing(bool pSwitch) {
	return (_wrappedRenderer->setAntialiasing(pSwitch));
}

/**
@b Parameters:

@arg @b pR                  Byte R (Red)
@arg @b pG                  Byte G (Green)
@arg @b pB                  Byte B (Blue)

@b Operation:

Clean the viewport with a color expressed in RGB
*/
void IND_Render::clearViewPort(unsigned char pR,
                               unsigned char pG,
                               unsigned char pB) {
	_wrappedRenderer->clearViewPort(pR, pG, pB);
}

/**
@b Parameters:

@arg @b pFov                                        Vertical fov
@arg @b pAspect                                     Aspect ratio (usually the width of the viewport divided by the height)
@arg @b pNearClippingPlane                          Near clipping plane
@arg @b pFarClippingPlane                           Far clipping plane

Operation:

This function sets a fov projection matrix.

This method is equivalent to use a combination of methods of the class setCamera3d.
*/
inline void IND_Render::perspectiveFov(float pFov, float pAspect, float pNearClippingPlane, float pFarClippingPlane) {
	_wrappedRenderer->perspectiveFov(pFov, pAspect, pNearClippingPlane, pFarClippingPlane);
}


/**
@b Parameters:

@arg @b pWidth                                      Width
@arg @b pHeight                                     Height
@arg @b pNearClippingPlane                          Near clipping plane
@arg @b pFarClippingPlane                           Far clipping plane

Operation:

This function sets a orthographic projection matrix.

Using this method is equivalent to using a combination of the methods of the class setCamera3d.
*/
inline void IND_Render::perspectiveOrtho(float pWidth, float pHeight, float pNearClippingPlane, float pFarClippingPlane) {
	_wrappedRenderer->perspectiveOrtho(pWidth, pHeight, pNearClippingPlane, pFarClippingPlane);
}

/**
@b Operation:

Preparing for render. This function must be called before drawing any graphical object.
*/
void IND_Render::beginScene() {

	// ----- Time counter -----
	float currenttime = static_cast<float>(_timer.getTicks());

	_last = (currenttime - _lastTime);
	_lastTime = currenttime;

	// ----- Fps counter ------

	_fpsCounter++;
	_currentTimeFps = currenttime;

	// After each second passed
	if (_currentTimeFps - _lastTimeFps > 1000) {
		_lastTimeFps = _currentTimeFps;
		_lastFps = _fpsCounter;
		_fpsCounter = 0;
	}

	// Set culling region
	reCalculateFrustrumPlanes();

	_wrappedRenderer->beginScene();
}


/**
@b Operation:

Finish the scene. This function must be called after drawing all the graphical objects.
*/
void IND_Render::endScene() {
	_wrappedRenderer->endScene();
}
/**
@addtogroup Graphical_2d_Objects
 @ingroup Advanced
*/
/**@{*/

/**
@b Parameters:

@arg @b pFo                                         Pointer to a ::IND_Font object
@arg @b pText                                       Text to write to the screen
@arg @b pX, @b pY                                   Position
@arg @b pOffset                                     Char spacing
@arg @b pLineSpacing                                Line spacing
@arg @b pR, @b pG, @b pB                            R, G, B components of the tinting color
@arg @b pA                                          Transparency level. (255 = complety opaque)
@arg @b pFadeR, @b pFadeG, @b pFadeB, @b pFadeA     Fade to a color.
@arg @b pFilter                                     ::IND_Filter type
@arg @b pSo                                         Source blending, see (::IND_BlendingType).
@arg @b pDs                                         Destiny blending, see (::IND_BlendingType).
@arg @b pAlign                                      Text alignment, see::IND_Align.

@b Operation:

This function blits text directly to the screen using the ::IND_Font object.

Important: you cannot change the transformation or color attributes of a font using IND_Render::setTransform2d() or IND_Render::SetRainbow().

Remember that you can use IND_Entity2d object for drawing fonts to the screen without having to use this
advanced method directly. This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using a combination of these methods:
- IND_Entity2d::setFont()
- IND_Entity2d::setPosition()
- IND_Entity2d::setText()
- IND_Entity2d::setCharSpacing()
- IND_Entity2d::setLineSpacing()
- IND_Entity2d::setAlign()
*/
void IND_Render::blitText(IND_Font *pFo,
                                 char *pText,
                                 int pX,
                                 int pY,
                                 int pOffset,
                                 int pLineSpacing,
                                 float pScaleX,  //MIGUEL: New param
                                 float pScaleY,  //MIGUEL: New param
                                 unsigned char pR,
                                 unsigned char pG,
                                 unsigned char pB,
                                 unsigned char pA,
                                 unsigned char pFadeR,
                                 unsigned char pFadeG,
                                 unsigned char pFadeB,
                                 unsigned char pFadeA,
                                 IND_Filter pLinearFilter,
                                 IND_BlendingType pSo,
                                 IND_BlendingType pDs,
                                 IND_Align pAlign) {
	_wrappedRenderer->blitText(pFo,
	                           pText,
	                           pX,
	                           pY,
	                           pOffset,
	                           pLineSpacing,
	                           pScaleX,  //MIGUEL: New param
	                           pScaleY,  //MIGUEL: New param
	                           pR,
	                           pG,
	                           pB,
	                           pA,
	                           pFadeR,
	                           pFadeG,
	                           pFadeB,
	                           pFadeA,
	                           pLinearFilter,
	                           pSo,
	                           pDs,
	                           pAlign);
}

/**
@b Parameters:

@arg @b pAn                         Pointer to a ::IND_Animation object
@arg @b pSequence                   Number of the sequence to blit (the first sequence is 0)
@arg @b pX, @b pY                   Upper left coordinate of the region
@arg @b pWidth, @b pHeight          Width and Height of the region
@arg @b pToggleWrap                 Wraping on (1) / off (0)
@arg @b pUOffset, @b pVOffset   Horizontal and vertical displacement of the image

@b Operation:

This function blits directly to the screen a certain sequence of a ::IND_Animation object.

Each frame of the animation will be blited to the screen the number of milliseconds that are
defined in the animation script file. The sequecen starts in the frame 0 and finishes in the last frame
specefied in the animation script. The animation will be displayed only one time, after that
it will stop in the last frame (bliting it permanently).

This functions returns -1 when the animation finishes, 0 if there is any error (for example trying to
blit an invalid IND_Animation pointer) and 1 if is in the middle of the animation and there are no errors.

In order to change the transformations
and color attributes of the animation you have to use the IND_Render::setTransform2d() and IND_Render::setRainbow2d() methods before
calling to this function. Remember that you can use IND_Entity2d object for drawing animations to the screen without having to use this
advanced methods directly. This method is only useful for advanced users with really concrete purposes.

Special remark: if you specify a region this function only works with ::IND_Surface objects that only have ONE texture
assigned (you can check this using::IND_Surface::getNumTextures() method). So, it will work only
with images that are power of two and lower than the maximum texture size allowed by your card
(you can check this parameter using ::IND_Render::getMaxTextureSize()). The method will return 0
otherwise.

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setAnimation()
- IND_Entity2d::setRegion()
- IND_Entity2d::toggleWrap()
- IND_Entity2d::setWrapOffset()
*/
int IND_Render::blitAnimation(IND_Animation *pAn,
			      unsigned int pSequence,
                              int pX,
      			      int pY,
                              int pWidth,
                              int pHeight,
                              bool pToggleWrap,
                              float pUOffset,
                              float pVOffset) {
	return (_wrappedRenderer->blitAnimation(pAn,
	                                        pSequence,
	                                        pX,
	                                        pY,
	                                        pWidth,
	                                        pHeight,
	                                        pToggleWrap,
	                                        pUOffset,
	                                        pVOffset));
}
/**@}*/

/**
@b Parameters:

@arg @b pSu                         Pointer to a ::IND_Surface object
@arg @b pX, @b pY                   Upper left coordinate of the region
@arg @b pWidth, @b pHeight          Width and Height of the region
@arg @b pUOffset, @b pVOffset   Horizontal and vertical displacement of the image

@b Operation:

This function returns 1 (true) if it blits directly to the screen a ::IND_Surface object tiling it both
in X and Y coordinates.

This method is useful when we want to render a tiled texture or background.

Special remark: this function only works with ::IND_Surface objects that only have ONE texture
assigned (you can check this using::IND_Surface::getNumTextures() method). So, it will work only
with images that are power of two and lower than the maximum texture size allowed by your card
(you can check this parameter using ::IND_Render::getMaxTextureSize()). The method will return 0
otherwise.

In order to change the transformations and color attributes of the surface you have to use the IND_Render::setTransform2d() and IND_Render::setRainbow2d() methods before
calling to this function. Remember that you can use IND_Entity2d object for drawing surfaces to the screen without having to use this
advanced methods directly. This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setSurface()
- IND_Entity2d::setRegion()
- IND_Entity2d::toggleWrap()
- IND_Entity2d::setWrapOffset()
*/
bool IND_Render::blitWrapSurface(IND_Surface *pSu,
                                 int pWidth,
                                 int pHeight,
                                 float pUOffset,
                                 float pVOffset) {
	return (_wrappedRenderer->blitWrapSurface(pSu,
	        pWidth,
	        pHeight,
	        pUOffset,
	        pVOffset));
}

/**
@b Parameters:

@arg @b pSu                     Pointer to a ::IND_Surface object

@b Operation:

This function blits directly to the screen a ::IND_Surface object.

In order to change the transformations
and color attributes of the surface you have to use the IND_Render::setTransform2d() and IND_Render::setRainbow2d() methods before
calling this function. Remember that you can use IND_Entity2d object for drawing surfaces to the screen without having to use these
advanced methods directly. This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using:
- IND_Entity2d::setSurface()
*/
void IND_Render::blitSurface(IND_Surface *pSu) {
	_wrappedRenderer->blitSurface(pSu);
}


/**
@b Parameters:

@arg @b pSu                     Pointer to a ::IND_Surface object

@b Operation:

This function blits directly to the screen the grid of an ::IND_Surface object.

Using this method is equivalent to using both of these methods:
- IND_Entity2dManager::renderGridAreas()
- IND_Entity2d::showGridAreas()
*/
void IND_Render::blitGrid(IND_Surface *pSu,
			  unsigned char pR,
                          unsigned char pG,
                          unsigned char pB,
                          unsigned char pA) {
	_wrappedRenderer->blitGrid(pSu, pR, pG, pB, pA);
}


/**
@b Parameters:

@arg @b pSu                     Pointer to a ::IND_Surface object
@arg @b pX, @b pY               Upper left coordinate of the region
@arg @b pWidth, @b pHeight      Width and Height of the region

@b Operation:

This method is useful when we want to render only a certain region of a ::IND_Surface.

If the region that we chose is out of the range of the sprite, the function will return false and no
region will be rendered.

Special remark: this function only works with ::IND_Surface objects that only have ONE texture
assigned (you can check this using::IND_Surface::getNumTextures() method). So, it will work only
with images that are power of two and lower than the maximum texture size allowed by your card
(you can check this parameter using ::IND_Render::getMaxTextureSize()). The method will return 0
otherwise.

In order to change the transformations and color attributes of the surface you have to use the IND_Render::setTransform2d() and IND_Render::setRainbow2d() methods before
calling to this function. Remember that you can use IND_Entity2d object for drawing surfaces to the screen without having to use this
advanced methods directly. This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using both of these methods:
- IND_Entity2d::setSurface()
- IND_Entity2d::setRegion()
*/
void IND_Render::blitRegionSurface(IND_Surface *pSu,
        			   int pX,
        			   int pY,
        			   int pWidth,
				   int pHeight) {
	_wrappedRenderer->blitRegionSurface(pSu, pX, pY, pWidth, pHeight);
}

/**@}*/

/**
@addtogroup Primitives 
With these methods you can directly blit to the screen primitives using IND_Render class. Remember that you can also use IND_Entity2d with primitives joined to this object, in order to
draw primitives.
*/
/**@{*/

/**
@b Parameters:

@arg @b pX, @b pY               Position in the screen
@arg @b pR, @b pG, @b pB        R, G, B components of the color
@arg @b pA                      Level of transparency. (255 = completly opaque)

@b Operation:

This function draws a pixel into the screen. This is a really slow method when the number of pixels is
big.

This method is equivalent to use a combination of these methods:
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setLine()
- IND_Entity2d::setTint()
- IND_Entity2d::setTransparency()
*/
void IND_Render::blitPixel(int pX,
                           int pY,
                           unsigned char pR,
                           unsigned char pG,
                           unsigned char pB,
                           unsigned char pA) {
	_wrappedRenderer->blitPixel(pX, pY, pR, pG, pB, pA);
}


/**
@b Parameters:

@arg @b pX1, @b pY1             Origin point
@arg @b pX2, pY2                Destiny point
@arg @b pR, @b pG, @b pB        R, G, B components of the color
@arg @b pA                      Level of transparency. (255 = completly opaque)

@b Operation:

This function draws a line into the screen

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setLine()
- IND_Entity2d::setTint()
- IND_Entity2d::setTransparency()
*/
void IND_Render::blitLine(int pX1,
                          int pY1,
                          int pX2,
                          int pY2,
                          unsigned char pR,
                          unsigned char pG,
                          unsigned char pB,
                          unsigned char pA) {
	_wrappedRenderer->blitLine(pX1, pY1, pX2, pY2, pR, pG, pB, pA);
}


/**
@b Parameters:

@arg @b pX1, @b pY1             Upper left corner of the rectangle
@arg @b pX2, @b pY2             Lower right corner of the rectangle
@arg @b pR, @b pG, @b pB        R, G, B components of the color
@arg @b pA                      Level of transparency. (255 = completly opaque)

@b Operation:

This function draws a rectangle into the screen

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setRectangle()
- IND_Entity2d::setTint()
- IND_Entity2d::setTransparency()
*/
void IND_Render::blitRectangle(int pX1,
                               int pY1,
                               int pX2,
                               int pY2,
                               unsigned char pR,
                               unsigned char pG,
                               unsigned char pB,
                               unsigned char pA) {
	_wrappedRenderer->blitRectangle(pX1, pY1, pX2, pY2, pR, pG, pB, pA);
}


/**
@b Parameters:

@arg @b pX1, @b pY1                 Upper left corner of the rectangle
@arg @b pX2, @b pY2                 Lower right corner of the rectangle
@arg @b pR, @b pG, @b pB            R, G, B components of the color
@arg @b pA                          Level of transparency. (255 = completly opaque)

@b Operation:

This function draws a rectangle filled with a color into the screen. The A component is the
transparency level (255 = complety opaque).

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setRectangle()
- IND_Entity2d::setTint()
- IND_Entity2d::setTransparency()
*/
void IND_Render::blitFillRectangle(int pX1,
        			   int pY1,
        			   int pX2,
        			   int pY2,
        			   unsigned char pR,
        			   unsigned char pG,
        			   unsigned char pB,
        			   unsigned char pA) {
	_wrappedRenderer->blitFillRectangle(pX1, pY1, pX2, pY2, pR, pG, pB, pA);
}

/**
@b Parameters:

@arg @b pTrianglePoints             Triangle Points allocated array
@arg @b pNumPoints                  Number of points passed (numtriangles =  pNumPoints - 2)
@arg @b pR, @b pG, @b pB            R, G, B components of the color in outer vertexs
@arg @b pA                          Level of transparency. (255 = completly opaque)

@b Operation:

This function draws a complete triangle fan. The triangle fan is a set of triangles which
share a central vertex (http://msdn.microsoft.com/en-us/library/ee422512(VS.85).aspx)
The A parameter is transparency (255 = complety opaque).
*/
void IND_Render::blitTriangleList(IND_Point *pTrianglePoints,
        			  int pNumPoints,
        			  unsigned char pR,
        			  unsigned char pG,
        			  unsigned char pB,
        			  unsigned char pA) {
				  _wrappedRenderer->blitTriangleList(pTrianglePoints, pNumPoints, pR, pG, pB, pA);
}

/**
@b Parameters:

@arg @b pX1, @b pY1                     Triangle corner #1
@arg @b pX2, @b pY2                     Triangle corner #2
@arg @b pX3, @b pY3                     Triangle corner #3
@arg @b pR1, @b pG1, @b pB1             R, G, B components of the color for corner #1
@arg @b pR2, @b pG2, @b pB2             R, G, B components of the color for corner #2
@arg @b pR3, @b pG3, @b pB3             R, G, B components of the color for corner #3
@arg @b pZ                              Z depth of coords in all triangle
@arg @b pA                              Level of transparency. (255 = completly opaque)

@b Operation:

This function draws a triangle filled with a color given in three corners. The A component is the
transparency level (255 = complety opaque).
*/

void IND_Render::blitColoredTriangle(int pX1,
        			     int pY1,
        			     int pX2,
        			     int pY2,
        			     int pX3,
        			     int pY3,
        			     unsigned char pR1, unsigned char pG1, unsigned char pB1,
        			     unsigned char pR2, unsigned char pG2, unsigned char pB2,
        			     unsigned char pR3, unsigned char pG3, unsigned char pB3,
        			     unsigned char pA) {
					       _wrappedRenderer->blitColoredTriangle(pX1, pY1, pX2, pY2, pX3, pY3, pR1, pG1, pB1, pR2, pG2, pB2, pR3, pG3, pB3, pA);
}


/**
@b Parameters:

@arg @b pPixel                      Pointer to a points array ::IND_Point. Example: ::IND_Point mPoly3 [ ] = { {60, 10},  {20, 15},  {50, 90},  {170, 190} } => Sets 3 points (each one with x and y coordinates).
@arg @b pNumLines                   Number of edges to draw
@arg @b pR, @b pG, @b pB            R, G, B components of the color
@arg @b pA                          Level of transparency. (255 = completly opaque)

@b Operation:

This function draws a 2d poly

Using this method is equivalent to using all of these methods:
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setPolyPoints()
- IND_Entity2d::setNumSides()
- IND_Entity2d::setTint()
- IND_Entity2d::setTransparency()
*/
bool IND_Render::blitPoly2d(IND_Point *pPolyPoints,
                            int pNumLines,
                            unsigned char pR,
                            unsigned char pG,
                            unsigned char pB,
                            unsigned char pA) {
	return (_wrappedRenderer->blitPoly2d(pPolyPoints, pNumLines, pR, pG, pB, pA));
}


/**
@b Parameters:

@arg @b pX, @b pY                   Position in the screen
@arg @b pRadius                     Radius
@arg @b pN                          Number of sides
@arg @b pAngle                      Angle in degrees (if you change this parameter the polygon
                                    will rotate)
@arg @b pR, @b pG, @b pB            R, G, B components of the color
@arg @b pA                          Level of transparency. (255 = completly opaque)

@b Operation:

This function draws 2d regunr poly of n sides. If you need to draw circles you can use this method
using 30 or more sides.

This method is equivalent to use a combination of these methods:
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setRadius()
- IND_Entity2d::setPosition()
- IND_Entity2d::setTint()
- IND_Entity2d::setTransparency()
*/
bool IND_Render::blitRegularPoly(int pX,
                                 int pY,
                                 int pRadius,
                                 int pN,
                                 float pAngle,
                                 unsigned char pR,
                                 unsigned char pG,
                                 unsigned char pB,
                                 unsigned char pA) {
	return (_wrappedRenderer->blitRegularPoly(pX, pY, pRadius, pN, pAngle, pR, pG, pB, pA));
}
/**@}*/

/**
 @addtogroup Graphical_2d_Objects
 @ingroup Advanced
 */
/**@{*/

/**
@b Parameters:

@arg @b pX              Upper left X coordinate of the Viewport
@arg @b pY              Upper left Y coordinate of the Viewport
@arg @b pWidth          Viewport width
@arg @b pHeight         Viewport height

@b Operation:

This function returns 1 (true) if a 2d ViewPort is created in the specified area.

The ViewPort is the drawing region. By default, IndieLib is initialized with a Viewport
that has the same area as the window. Using this method you can define different viewports before
calling to IND_Entity2dManager::renderEntities2d().

After using this method, all the bliting methods will render the graphical objects inside the Viewport.
Objects or blocks drawn outside the viewport will be discarded.

This method returns 0 (false) if the user tries to create a Viewport outside the window area.
*/
bool IND_Render::setViewPort2d(int pX,
                                      int pY,
                                      int pWidth,
                                      int pHeight) {
	return (_wrappedRenderer->setViewPort2d(pX, pY, pWidth, pHeight));
}


/**
@b Parameters:

@arg @b pCamera2d               ::IND_Camera2d object that defines a camera.

@b Operation:

This function sets a 2d camera. See the methods of ::IND_Camera2d for information on how you can manipulate the camera.
*/
void IND_Render::setCamera2d(IND_Camera2d *pCamera2d) {
	_wrappedRenderer->setCamera2d(pCamera2d);
}

/**
@b Parameters:

@arg @b pX                          Translation in the X coordinate. The (0, 0) position is the
                                    upper-left corner of the Viewport
@arg @b pY                          Translation in the Y coordinate. The (0, 0) position is the
                                    upper-left corner of the Viewport
@arg @b pAngleX                     Rotation in the angle x in degrees
@arg @b pAngleY                     Rotation in the angle y in degrees
@arg @b pAngleZ                     Rotation in the angle z in degrees
@arg @b pScaleX                     Scaling in the x coordinate. 1 for maintaining the original size
@arg @b pScaleY                     Scaling in the y coordinate. 1 for maintaining the original size
@arg @b pAxisCalX, @b pAxisCalY     Parameters that indicates the displacement that the graphical
                                    object undergoes due to the HotSpot. If the HotSpot is not specified,
                                    the value should be 0 for both of them. All the transformation
                                    will be aplied from the upper-left corner of the object.
@arg @b pMirrorX                    Horizontal mirroring. (true / false) = (activated / deactivated).
@arg @b pMirrorY                    Vertical mirroring. (true / false) = (activated / deactivated).
@arg @b pWidth                      Width of the graphical object that we are going to blit just after
                                    applying the transformation. You shoud use the getWidth() method
                                    of the object.
@arg @b pHeight                     Height of the graphical object that we are going to blit just after
                                    applying the transformation. You shoud use the getHeight() method
                                    on the object.
@arg @b pMatrix                     Pointer to a ::IND_Matrix matrix. In this parameter will be
                                    returned by reference the world matrix transformation that
                                    will be aplied to the graphical object. This matrix can be useful
                                    for advanced programmers that need the algebraic description
                                    of the transformation. It is possible to use the value 0
                                    if it not necessary to have this matrix information.
@b Operation:

This function sets the 2d transformation (translation, rotation, scaling, mirroring and hotspot)
of the following 2d graphical objects that will be rendered
by the engine. You should use this method before calling to any of the Bliting methods.

Remember that you can use IND_Entity2d object for applying 2d transformations to the graphical
objects without having to use this advanced method directly. This method is only useful for advanced
users with really concrete purposes.

Using this method is equivalent to using a combination of these methods:
- IND_Entity2d::setAnimation()
- IND_Entity2d::setSurface()
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setFont()
- IND_Entity2d::setPosition()
- IND_Entity2d::setAngleXYZ()
- IND_Entity2d::setScale()
- IND_Entity2d::setBackCull()
- IND_Entity2d::setMirrorX()
- IND_Entity2d::setMirrorY()
- IND_Entity2d::setFilter()
- IND_Entity2d::setHotSpot()
- IND_Entity2d::setRegion()
- IND_Entity2d::toggleWrap()
*/
void IND_Render::setTransform2d(int pX,
                                       int pY,
                                       float pAngleX,
                                       float pAngleY,
                                       float pAngleZ,
                                       float pScaleX,
                                       float pScaleY,
                                       int pAxisCalX,
                                       int pAxisCalY,
                                       bool pMirrorX,
                                       bool pMirrorY,
                                       int pWidth,
                                       int pHeight,
                                       IND_Matrix *pMatrix) {
	_wrappedRenderer->setTransform2d(pX, pY, pAngleX, pAngleY, pAngleZ, pScaleX, pScaleY, pAxisCalX, pAxisCalY, pMirrorX, pMirrorY, pWidth, pHeight, pMatrix);
}

/**
@b Parameters:

@arg @b pTransformMatrix                            Translation Matrix (IND_Matrix) to apply to render

@b Operation:

This function sets the 2d transformation (via transformation Matrix)
of the following 2d graphical objects that will be rendered
by the engine. You should use this method before calling to any of the Bliting methods.

Remember that you can use IND_Entity2d object for applying 2d transformations to the graphical
objects without having to use this advanced method directly. This method is only useful for advanced
users with really concrete purposes. This method only modifies the view-world transform. In other words, 
it doesn't modify camera transformations that you have in place, just object transforms inside your world.

Using this method is equivalent to using a combination of these methods:
- IND_Entity2d::setAnimation()
- IND_Entity2d::setSurface()
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setFont()
- IND_Entity2d::setPosition()
- IND_Entity2d::setAngleXYZ()
- IND_Entity2d::setScale()
- IND_Entity2d::setBackCull()
- IND_Entity2d::setMirrorX()
- IND_Entity2d::setMirrorY()
- IND_Entity2d::setFilter()
- IND_Entity2d::setHotSpot()
- IND_Entity2d::setRegion()
- IND_Entity2d::toggleWrap()
*/
void IND_Render::setTransform2d(IND_Matrix &pTransformMatrix) {
	_wrappedRenderer->setTransform2d(pTransformMatrix);
}

/**
@b Operation:

This function sets the 2d transformation to identity. That is no transformation.

Remember that you can use IND_Entity2d object for applying 2d transformations to the graphical
objects without having to use this advanced method directly. This method is only useful for advanced
users with really concrete purposes.

Using this method is equivalent to using a combination of these methods:
- IND_Entity2d::setAnimation()
- IND_Entity2d::setSurface()
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setFont()
- IND_Entity2d::setPosition()
- IND_Entity2d::setAngleXYZ()
- IND_Entity2d::setScale()
- IND_Entity2d::setBackCull()
- IND_Entity2d::setMirrorX()
- IND_Entity2d::setMirrorY()
- IND_Entity2d::setFilter()
- IND_Entity2d::setHotSpot()
- IND_Entity2d::setRegion()
- IND_Entity2d::toggleWrap()
*/
void IND_Render::setIdentityTransform2d () {
	_wrappedRenderer->setIdentityTransform2d();
}

/**
@b Parameters:

@arg @b pType                                       ::IND_Type type of the following graphical object.
@arg @b pCull                                       Backface culling. (true / false) = (on / off). This feature can
                                                    be deactivated when we are going to rotate the graphical object
                                                    in the x or y coordinates. This way it is possible to draw for
                                                    example the leaf of a tree that falls down rotating and shows
                                                    both faces. In cases that you don't want to draw back faces
                                                    deactivate this for faster render times.
@arg @b pMirrorX                                    Horizontal mirroring. (true / false) = (activated / deactivated).
@arg @b pMirrorY                                    Vertical mirroring. (true / false) = (activated / deactivated).
@arg @b pFilter                                     Type of filter ::IND_Filter. There are two types of filters that
                                                    can be applied to the graphical object when it suffers a rotation
                                                    or scaling:
                                                    - ::IND_FILTER_POINT (Nearest point filter). Less quality, but
                                                    bigger performance. It is possible to use this one without
                                                    quality loss in graphical objects that don't rotate or are
                                                    affected by scaling.
                                                    - ::IND_FILTER_LINEAR (Bilinear filter). More quality, but less
                                                    performance. Used in graphical objects that be rotated or
                                                    affected by scaling.
@arg @b pR, pG, pB                                  Color BYTEs FadeR, FadeG, FadeB
                                                    Used for fading the image to a certain color.
                                                    For example, if we use RGB = (255, 0, 0),
                                                    the image will progressively become red.
@arg @b pA                                          The A unsigned char indicates the level of transparency.
                                                    If a value of 0 is used, the following graphical
                                                    object will be completely transparent, as opposed
                                                    to the value 255 that will cause the object
                                                    to be drawn completely opaque. It is possible to use all the
                                                    intermediate values for different levels of
                                                    transparency.
@arg @b pFadeR, @b pFadeG, @b pFadeB, @b pFadeA     Bytes FadeR, FadeG, FadeB
                                                    Used for fading the image to a certain color.
                                                    For example, if we use RGB = (255, 0, 0),
                                                    the image will progressively become red.
@arg @b pSo                                         Indicates the blending source,
                                                    see (::IND_BlendingType).
@arg @b pDs                                         Indicates the blending destiny,
                                                    see (::IND_BlendingType).

Operation:

This functions sets the color and blending attributes of a graphical object. It should
be used before any calling to the Blit methods.

Important: It is not possible to use tinting and fading at the same time, the fading will overide the tinting.

Remember that you can use IND_Entity2d objects for applying color
transformations to the graphical objects without having to use this advanced method directly.
This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using a combination of these methods:
- IND_Entity2d::setTransparency()
- IND_Entity2d::setFade()
- IND_Entity2d::setTint()
- IND_Entity2d::setBlendSource()
- IND_Entity2d::setBlendDest()
*/
void IND_Render::setRainbow2d(IND_Type pType,
                              bool pCull,
                              bool pMirrorX,
                              bool pMirrorY,
                              IND_Filter pFilter,
                              unsigned char pR,
                              unsigned char pG,
                              unsigned char pB,
                              unsigned char pA,
                              unsigned char pFadeR,
                              unsigned char pFadeG,
                              unsigned char pFadeB,
                              unsigned char pFadeA,
                              IND_BlendingType pSo,
                              IND_BlendingType pDs) {
	_wrappedRenderer->setRainbow2d(pType,
	                               pCull,
	                               pMirrorX,
	                               pMirrorY,
	                               pFilter,
	                               pR,
	                               pG,
	                               pB,
	                               pA,
	                               pFadeR,
	                               pFadeG,
	                               pFadeB,
	                               pFadeA,
	                               pSo,
	                               pDs);
}

/**@}*/

/**
 @addtogroup Graphical_3d_Objects
 @ingroup Advanced
 */
/**@{*/

/**
@b Parameters:

@arg @b p3dMesh                     Pointer to a ::IND_3dMesh object

@b Operation:

This function blits directly to the screen a ::IND_3dMesh object.

In order to change the transformations
and color attributes of the 3d mesh you have to use the IND_Render::setTransform3d() and IND_Render::SetRainbow()
methods before calling to this function. Remember that you can use IND_Entity3d object for drawing 3d meshes to
the screen without having to use this advanced methods directly. This method is only useful for advanced users
with really concrete purposes.

Using this method is equivalent to using:
- IND_Entity3d::set3dMesh()
*/
inline void IND_Render::blit3dMesh(IND_3dMesh *p3dMesh) {
	_wrappedRenderer->blit3dMesh(p3dMesh);
}


/**
@b Parameters:

@arg @b p3dMesh                     Pointer to a ::IND_3dMesh object
@arg @b pIndex                      Index of the animation

@b Operation:

This function sets the animation sequence that will be rendered. When you use this method, the transition between the
current animation and the following will not be abrubt. IndieLib will fade out the effect of the first animation
and fade in the second. Use IND_3dMesh::setTransitionSpeed() in order set the speed of that transition.
*/
void IND_Render::set3dMeshSequence(IND_3dMesh *p3dMesh, unsigned int pIndex) {
	_wrappedRenderer->set3dMeshSequence(p3dMesh, pIndex);
}
/**@}*/

/**
@b Parameters:

@arg @b pX              Upper left X coordinate of the Viewport
@arg @b pY              Upper left Y coordinate of the Viewport
@arg @b pWidth          Viewport width
@arg @b pHeight         Viewport height

@b Operation:

This function returns 1 (true) if a 3d ViewPort is created in the specified area.

The ViewPort is the drawing region. You have to call to this method before
calling to IND_Entity3dManager::renderEntities3d().

After using this method, all the 3d bliting methods will render the graphical objects inside this Viewport.

This method returns 0 (false) if the user tries to create a Viewport outside the window area.
*/
inline bool IND_Render::setViewPort3d(int pX,
                                      int pY,
                                      int pWidth,
                                      int pHeight) {
	return (_wrappedRenderer->setViewPort3d(pX, pY, pWidth, pHeight));
}


/**
@b Parameters:

@arg @b pCamera3d               ::IND_Camera3d object that defines a camera.

@b Operation:

This function sets a 3d camera. See the methods of ::IND_Camera3d for information on how you can manipulate the camera.
*/
inline void IND_Render::setCamera3d(IND_Camera3d *pCamera3d) {
	_wrappedRenderer->setCamera3d(pCamera3d);
}

/**
@b Parameters:

@arg @b pX                          Translation in the X coordinate.
@arg @b pY                          Translation in the Y coordinate.
@arg @b pZ                          Translation in the Z coordinate.
@arg @b pAngleX                     Rotation in the angle x in degrees
@arg @b pAngleY                     Rotation in the angle y in degrees
@arg @b pAngleZ                     Rotation in the angle z in degrees
@arg @b pScaleX                     Scaling in the x coordinate. 1 to maintain the original size
@arg @b pScaleY                     Scaling in the y coordinate. 1 to maintain the original size
@arg @b pScaleZ                     Scaling in the z coordinate. 1 to maintain the original size
@arg @b pMatrix                     Pointer to a ::IND_Matrix matrix. In this parameter will be
                                    returned by reference the world matrix transformation that
                                    will be aplied to the graphical object. This matrix can be useful
                                    for advanced programmers that need the algebraic description
                                    of the transformation done. It is possible to use the value 0
                                    if it not necessary to have this matrix information.
@b Operation:

This function sets the 3d transformation (translation, rotation, scaling and mirroring)
of the following 3d graphical objects that will be rendered
by the engine. You should use this method before calling to any of the Bliting methods.

Remember that you can use IND_Entity3d object for applying 3d transformations to the graphical
objects without having to use this advanced method directly. This method is only useful for advanced
users for really concrete purposes.

This method is equivalent to use a combination of this methods:
- IND_Entity3d::set3dMesh()
- IND_Entity3d::setPosition()
- IND_Entity3d::setAngleXYZ()
- IND_Entity3d::setScale()
- IND_Entity2d::setBackCull()
- IND_Entity2d::setFilter()
*/
inline void IND_Render::setTransform3d(float pX,
                                       float  pY,
                                       float  pZ,
                                       float pAngleX,
                                       float pAngleY,
                                       float pAngleZ,
                                       float pScaleX,
                                       float pScaleY,
                                       float pScaleZ,
                                       IND_Matrix *pMatrix)


{
	_wrappedRenderer->setTransform3d(pX, pY, pZ, pAngleX, pAngleY, pAngleZ, pScaleX, pScaleY, pScaleZ, pMatrix);
}


/**
@b Parameters:

@arg @b pCull                                       Backface culling. (true / false) = (on / off). This parameter can
                                                    be deactivated when we are going to go inside a 3dMesh and we want
                                                    to render both faces of the polygons.In any
                                                    other case this parameter must be activated for faster rendering
                                                    times preventing the hardware from drawing back faces.
@arg @b pFilter                                     Type of filter ::IND_Filter. There are two types of filters that
                                                    can be applied to the graphical object when it suffers a rotation
                                                    or scaling:
                                                    - ::IND_FILTER_POINT (Nearest point filter). Less quality, but
                                                    bigger performance. It is possible to use this one without
                                                    quality loss in graphical objects that don't rotate or are
                                                    affected by scaling.
                                                    - ::IND_FILTER_LINEAR (Bilinear filter). More quality, but less
                                                    performance. Used in graphical objects that be rotated or
                                                    affected by scaling.
@arg @b pR, pG, pB                                  Color bytes FadeR, FadeG, FadeB
                                                    Used for fading the image to a certain color.
                                                    For example, if we use RGB = (255, 0, 0),
                                                    the image will progressively become red.
@arg @b pA                                          The A byte indicates the level of transparency.
                                                    If a value of 0 is used, the following graphical
                                                    object will be completely transparent, as opposed
                                                    to the value 255 that will cause the object
                                                    to be drawn completely opaque. It is possible to use all the
                                                    intermediate values for different levels of
                                                    transparency.
@arg @b pFadeR, @b pFadeG, @b pFadeB, @b pFadeA     Bytes FadeR, FadeG, FadeB
                                                    Used for fading the image to a certain color.
                                                    For example, if we use RGB = (255, 0, 0),
                                                    the image will progressively become red.
@arg @b pSo                                         Indicates the blending source,
                                                    see (::IND_BlendingType).
@arg @b pDs                                         Indicates the blending destiny,
                                                    see (::IND_BlendingType).

Operation:

This functions sets the color and blending attributes of the following graphical object. It should
be used before any calling to the Blit methods.

Important: It is not possible to use tinting and fading at the same time, the fading will overide the tinting.

Remember that you can use IND_Entity2d objects for applying color
transformations to the graphical objects without having to use this advanced method directly.
This method is only useful for advanced users for really concrete purposes.

Using this method is equivalent to using a combination of these methods:
- IND_Entity2d::setTransparency()
- IND_Entity2d::setFade()
- IND_Entity2d::setTint()
- IND_Entity2d::setBlendSource()
- IND_Entity2d::setBlendDest()
*/
inline void IND_Render::setRainbow3d(bool pCull,
                                     bool pFlipNormals,
                                     IND_Filter pFilter,
                                     unsigned char pR,
                                     unsigned char pG,
                                     unsigned char pB,
                                     unsigned char pA,
                                     unsigned char pFadeR,
                                     unsigned char pFadeG,
                                     unsigned char pFadeB,
                                     unsigned char pFadeA,
                                     IND_BlendingType pSo,
                                     IND_BlendingType pDs) {
	_wrappedRenderer->setRainbow3d(pCull,
	                               pFlipNormals,
	                               pFilter,
	                               pR,
	                               pG,
	                               pB,
	                               pA,
	                               pFadeR,
	                               pFadeG,
	                               pFadeB,
	                               pFadeA,
	                               pSo,
	                               pDs);
}
/**@}*/

/**
@b Operation:

This function shows the fps (frames per second) as the title of the window.

NOTE: The updating of the window title is quite time-consuming, so this is not the correct method for
checking the FPS. It's better to use the methods IND_Render::getFpsInt() or IND_Render::getFpsString() and drawing
the result using an ::IND_Font object.
*/
inline void IND_Render::showFpsInWindowTitle() {
	char fpsstring [MAX_CHARS_IN_INT32_STR];
	getFpsString(fpsstring);
	_wrappedRenderer->showFpsInWindowTitle(fpsstring);
}


/**
 @brief Sets the pixel to point scaling performed by rendering with vertex and position data
 
 Default value is 1.0f. Takes effect after the next setCamera2d call.
 
 By default pixel-to-point scaling is 1, so position of objects and vertex coordinates submitted
 to the renderer are 1 to 1 related to pixels in the backed framebuffer screen. You can change that if
 you want to maintain same 'logical' coordinates of your objects in your scene, independent of the device
 resolution that you actually render on.
 
 @param pNewScale Non-negative, non-zero value to apply scale globally.
 */
void IND_Render::setPointPixelScale (float pNewScale) {
	if (pNewScale > 0.0f) {
		_wrappedRenderer->setPointPixelScale(pNewScale);
	}
}


//! This function returns the x position of the actual viewport
int IND_Render::getViewPortX()      {
	return _wrappedRenderer->getViewPortX();
}

//! This function returns the y position of the actual viewport
int IND_Render::getViewPortY()      {
	return _wrappedRenderer->getViewPortY();
}

//! This function returns the width position of the actual viewport
int IND_Render::getViewPortWidth()      {
	return _wrappedRenderer->getViewPortWidth();
}

//! This function returns the width position of the actual viewport
int IND_Render::getViewPortHeight()      {
	return _wrappedRenderer->getViewPortHeight();
}

//! This function returns the actual version of Direct3d that is using IndiGeLib.
char *IND_Render::getVersion()      {
	return _wrappedRenderer->getVersion();
}

//! This function returns the name of the graphic card vendor.
char *IND_Render::getVendor()      {
	return _wrappedRenderer->getVendor();
}

//! This function returns the name of the graphic card.
char *IND_Render::getRenderer()      {
	return _wrappedRenderer->getRenderer();
}

//! This function returns the maximum texture size allowed by the graphic card.
int  IND_Render::getMaxTextureSize()      {
	return _wrappedRenderer->getMaxTextureSize();
}

void IND_Render::getFpsString(char *pBuffer)     {
	IND_Math::itoa(_lastFps, pBuffer);
}

//! This function returns a pointer to the IND_Window object where the render has been created
IND_Window *IND_Render::getWindow() {
	return _wrappedRenderer->getWindow();
}

void IND_Render::getNumrenderedObjectsString(char* pBuffer)      {
	_wrappedRenderer->getNumrenderedObjectsString(pBuffer);
}

void IND_Render::getNumDiscardedObjectsString(char* pBuffer)      {
	_wrappedRenderer->getNumDiscardedObjectsString(pBuffer);
}

int IND_Render::getNumrenderedObjectsInt()      {
	return _wrappedRenderer->getNumrenderedObjectsInt();
}

int IND_Render::getNumDiscardedObjectsInt()      {
	return _wrappedRenderer->getNumDiscardedObjectsInt();
}

void IND_Render::resetNumrenderedObject()      {
	_wrappedRenderer->resetNumrenderedObject();
}

void IND_Render::resetNumDiscardedObjects()      {
	_wrappedRenderer->resetNumDiscardedObjects();
}

// --------------------------------------------------------------------------------
//							        Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
==================
Creates the render
==================
*/
IND_Window* IND_Render::createRender(IND_WindowProperties& windowProperties) {
	g_debug->header("Creating Render", DebugApi::LogHeaderBegin);

	// If the window is correctly initialized
#ifdef INDIERENDER_DIRECTX
	_wrappedRenderer = new DirectXRender();
#endif

#ifdef INDIERENDER_GLES_IOS
	_wrappedRenderer = new OpenGLES2Render();
#endif

#ifdef INDIERENDER_OPENGL
	_wrappedRenderer = new OpenGLRender();
#endif

	IND_Window* window = NULL;
	if(_wrappedRenderer){
		window = _wrappedRenderer->initRenderAndWindow(windowProperties);
	}

    return window;
}

/*
==================
Reset the timing. This method is used after recovering the focus and in any restart operation
==================
*/
void IND_Render::resetTimer() {
	// Reset timing
	_timer.start();
	_last = 0.0f;
	_lastTime = static_cast<float>(_timer.getTicks());
}


/*
==================
Perform frustum planes calculation in underlying renderer
==================
*/
void IND_Render::reCalculateFrustrumPlanes() {
	_wrappedRenderer->reCalculateFrustrumPlanes();
}

/*
 ==================
 Blits a bounding circle area
 ==================
 */
void IND_Render::blitCollisionCircle(int pPosX, int pPosY, int pRadius, float pScale,  unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA, IND_Matrix pIndWorldMatrix) {
	_wrappedRenderer->blitCollisionCircle(pPosX, pPosY, pRadius, pScale, pR, pG, pB, pA, pIndWorldMatrix);
}


/*
 ==================
 Blits a bounding line
 ==================
 */
void IND_Render::blitCollisionLine(int pPosX1, int pPosY1, int pPosX2, int pPosY2,  unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA, IND_Matrix pIndWorldMatrix) {
	_wrappedRenderer->blitCollisionLine(pPosX1, pPosY1, pPosX2, pPosY2, pR, pG, pB, pA, pIndWorldMatrix);
}

/** @endcond */
