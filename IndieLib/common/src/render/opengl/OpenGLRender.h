/****************************************************************************************
 * File: OpenGLRender.h
 * Desc: Render class using OpenGL
 *
 * Divided in:
 * - OpenGLRender.cpp
 * - RenderTransform2dOpenGL.cpp
 * - RenderTransform3dOpenGL.cpp
 * - RenderTransformCommonOpenGL.cpp
 * - RenderObject2dOpenGL.cpp
 * - RenderObject3dOpenGL.cpp
 * - RenderPrimitive2dOpenGL.cpp
 * - RenderText2dOpenGL.cpp
 * - RenderCollision2dOpenGL.cpp
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


#ifndef _OPENGLRENDER_H_
#define _OPENGLRENDER_H_

// ----- Includes -----

#include <string.h>
#include "Defines.h"
#include "IND_Math.h"
#include "IND_Render.h"
#include "IND_Vector2.h"
#include "dependencies/SDL-2.0/include/SDL_opengl.h"

// ----- Forward Declarations -----

class IND_Window;
class IND_FontManager;
class IND_SurfaceManager;
class IND_Surface;
class IND_3dMeshManager;
class IND_AnimationManager;
class IND_FontManager;
class IND_Timer;
class IND_Font;
class IND_3dMesh;
class IND_Animation;
class IND_Camera2d;
class IND_Camera3d;
class OSOpenGLManager;

// ----- Libs -----

// ----- Defines ------

#define MAX_PIXELS 2048

struct InfoStruct {
    InfoStruct():
    _fbWidth(0),
    _fbHeight(0),
    _viewPortX(0),
    _viewPortY(0),
    _viewPortWidth(0),
    _viewPortHeight(0),
    _viewPortApectRatio(0.0f),
    _antialiasing(0),
    _maxTextureSize(0),
    _textureUnits(0),
    _pointPixelScale(1.0f){
        strcpy(_version, "NO DATA");
        strcpy(_vendor, "NO DATA");
        strcpy(_renderer, "NO DATA");
    }
    int _fbWidth;
    int _fbHeight;
    int _viewPortX;
    int _viewPortY;
    int _viewPortWidth;
    int _viewPortHeight;
    float _viewPortApectRatio;
    bool _antialiasing;
    char _version [1024] ;
    char _vendor [1024];
    char _renderer [1024];
    int _maxTextureSize;
    int _textureUnits;
    float _pointPixelScale;
};

struct TextureSamplerState {
    TextureSamplerState() :
    minFilter(GL_NEAREST),
    magFilter(GL_NEAREST),
    wrapS(GL_CLAMP_TO_EDGE),
    wrapT(GL_CLAMP_TO_EDGE)
    {}
    
    GLint minFilter;    //!< Texture filter setting 
    GLint magFilter;    //!< Texture filter setting 
    GLint wrapS;        //!< Texture wrap setting
    GLint wrapT;        //!< Texture wrap setting
};

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//									  OpenGLRender
// --------------------------------------------------------------------------------

class OpenGLRender {
public:

	// ----- Init/End -----

	OpenGLRender():
    	_window(NULL),
    	_osOpenGLMgr(NULL),
		_ok(false),
    	_numrenderedObjects(0),
    	_numDiscardedObjects(0),
		_doubleBuffer(false)
	{ }
	~OpenGLRender()              {
		end();
	}


	IND_Window* initRenderAndWindow(IND_WindowProperties& props);
	bool    reset(IND_WindowProperties& props);
	bool    toggleFullScreen();
	void    end();
	bool    isOK() {
		return _ok;
	}

	// ----- Public methods -----

	void beginScene();
	void endScene();
	void showFpsInWindowTitle(char *pFPSString);
    void setPointPixelScale (float pNewScale);
	
    // ----- Viewports and cameras -----

	void clearViewPort(unsigned char pR,
	                   unsigned char pG,
	                   unsigned char pB);

	bool setViewPort2d(int pX,
	                   int pY,
	                   int pWidth,
	                   int pHeight);

	void setCamera2d(IND_Camera2d *pCamera2d);

	bool setViewPort3d(int pX,
	                   int pY,
	                   int pWidth,
	                   int pHeight);

	void setCamera3d(IND_Camera3d *pCamera3d);


	// ----- Render Primitive 2d -----

	bool setAntialiasing(bool pSwitch);

	void blitPixel(int pX,
	               int pY,
	               unsigned char pR,
	               unsigned char pG,
	               unsigned char pB,
	               unsigned char pA);

	void blitLine(int pX1,
	              int pY1,
	              int pX2,
	              int pY2,
	              unsigned char pR,
	              unsigned char pG,
	              unsigned char pB,
	              unsigned char pA);

	void blitRectangle(int pX1,
	                   int pY1,
	                   int pX2,
	                   int pY2,
	                   unsigned char pR,
	                   unsigned char pG,
	                   unsigned char pB,
	                   unsigned char pA);

	void blitFillRectangle(int pX1,
	                       int pY1,
	                       int pX2,
	                       int pY2,
	                       unsigned char pR,
	                       unsigned char pG,
	                       unsigned char pB,
	                       unsigned char pA);

	void blitColoredTriangle(int pX1,
	                         int pY1,
	                         int pX2,
	                         int pY2,
	                         int pX3,
	                         int pY3,
	                         unsigned char pR1, unsigned char pG1, unsigned char pB1,
	                         unsigned char pR2, unsigned char pG2, unsigned char pB2,
	                         unsigned char pR3, unsigned char pG3, unsigned char pB3,
	                         unsigned char pA);

	void blitTriangleList(IND_Point *pTrianglePoints,
	                      int pNumPoints,
	                      unsigned char pR,
	                      unsigned char pG,
	                      unsigned char pB,
	                      unsigned char pA);

	bool blitPoly2d(IND_Point *pPixel,
	                int pNumLines,
	                unsigned char pR,
	                unsigned char pG,
	                unsigned char pB,
	                unsigned char pA);

	bool blitRegularPoly(int pX,
	                     int pY,
	                     int pRadius,
	                     int pN,
	                     float pAngle,
	                     unsigned char pR,
	                     unsigned char pG,
	                     unsigned char pB,
	                     unsigned char pA);

	void setTransform2d(int pX,
	                    int  pY,
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
	                    IND_Matrix *pMatrix);

	void setTransform2d(IND_Matrix &pTransformMatrix);

	void setIdentityTransform2d ();

	void setTransform3d(float pX,
	                    float  pY,
	                    float  pZ,
	                    float pAngleX,
	                    float pAngleY,
	                    float pAngleZ,
	                    float pScaleX,
	                    float pScaleY,
	                    float pScaleZ,
	                    IND_Matrix *pMatrix);

	void setRainbow2d(IND_Type pType,
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
	                  IND_BlendingType pDs);

	void setRainbow3d(bool pCull,
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
	                  IND_BlendingType pDs);

	void lookAt(float pEyeX, float pEyeY, float pEyeZ,
	            float pLookAtX, float pLookAtY, float pLookAtZ,
	            float pUpX, float pUpY, float pUpZ);

	void perspectiveFov(float pFov, float pAspect, float pNearClippingPlane, float pFarClippingPlane);

	void perspectiveOrtho(float pWidth, float pHeight, float pNearClippingPlane, float pFarClippingPlane);

	// ----- Render Object 2d -----

	void blitSurface(IND_Surface *pSu);

	void blitGrid(IND_Surface *pSu, unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA);

	void blitRegionSurface(IND_Surface *pSu,
	                       int pX,
	                       int pY,
	                       int pWidth,
	                       int pHeight);

	bool blitWrapSurface(IND_Surface *pSu,
	                     int pWidth,
	                     int pHeight,
	                     float pUDisplace,
	                     float pVDisplace);

	int blitAnimation(IND_Animation *pAn,
	                  unsigned int pSequence,
	                  int pX, int pY,
	                  int pWidth, int pHeight,
	                  bool pToggleWrap,
	                  float pUDisplace,
	                  float pVDisplace);

	// ------ Render Text 2d -----

	void blitText(IND_Font *pFo,
	              char *pText,
	              int pX,
	              int pY,
	              int pOffset,
	              int pLineSpacing,
	              float pScaleX,
	              float pScaleY,
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
	              IND_Align pAlign);


	void blit3dMesh(IND_3dMesh *p3dMesh);
	void set3dMeshSequence(IND_3dMesh *p3dMesh, unsigned int pIndex);	
	
	// ----- Atributtes -----

	//This function returns the x position of the actual viewport
	int getViewPortX()      {
		return _info._viewPortX;
	}
	//This function returns the y position of the actual viewport
	int getViewPortY()      {
		return _info._viewPortY;
	}
	//This function returns the width position of the actual viewport
	int getViewPortWidth()      {
		return _info._viewPortWidth;
	}
	// This function returns the width position of the actual viewport
	int getViewPortHeight()      {
		return _info._viewPortHeight;
	}
	//This function returns the actual version of Direct3d that is using IndieLib.
	char *getVersion()      {
		return _info._version;
	}
	//This function returns the name of the graphic card vendor.
	char *getVendor()      {
		return _info._vendor;
	}
	//This function returns the name of the graphic card.
	char *getRenderer()      {
		return _info._renderer;
	}
	// This function returns the maximum texture size allowed by the graphic card.
	int  getMaxTextureSize()      {
		return _info._maxTextureSize;
	}

	//This function returns a pointer to the IND_Window object where the render has been created
	IND_Window *getWindow()      {
		return _window;
	}

	void getNumrenderedObjectsString(char* pBuffer);

	void getNumDiscardedObjectsString(char* pBuffer);

	int getNumrenderedObjectsInt()      {
		return _numrenderedObjects;
	};
	int getNumDiscardedObjectsInt()      {
		return _numDiscardedObjects;
	};

	void resetNumrenderedObject()      {
		_numrenderedObjects = 0;
	}
	void resetNumDiscardedObjects()      {
		_numDiscardedObjects = 0;
	}

private:

	// ----- Private methods -----

	void initVars();
	void freeVars();

	void getInfo();
	void writeInfo();

	bool initializeOpenGLRender();   //Render init
	bool checkGLExtensions();        //GL extensions managing

	//Blitting helpers
	void fillPixel(PIXEL *pPixel, float pX, float pY,  float pR, float pG, float pB, float pA);
	void fillVertex2d(CUSTOMVERTEX2D *pVertex2d, float pX, float pY, float pU, float pV);
	void setForPrimitive(unsigned char pA, bool pResetTransform);

	void blitGridQuad    (int pAx, int pAy,
                          int pBx, int pBy,
                          int pCx, int pCy,
                          int pDx, int pDy,
						  unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA);

	void blitGridLine (int pPosX1, int pPosY1, int pPosX2, int pPosY2,  unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA);

	//Text rendering helpers
	int getLongInPixels(IND_Font *pFo, char *pText, int pPos, int pOffset);
    
    void blitCharsMudFont(IND_Font *pFo, char *pText, IND_Align pAlign, int pOffset, float pScaleX, float pScaleY, int pLineSpacing);
    
    void blitCharsAngelCodeFont(IND_Font *pFo, char *pText, IND_Align pAlign, int pOffset, float pScaleX, float pScaleY, int pLineSpacing);

	//Setup helper
	bool resetViewport(int pWitdh, int pHeight);
    
    //GL state helpers
    void setDefaultGLState();
    
    void setGLClientStateToPrimitive();
    void setGLClientStateToTexturing();

    void setGLBoundTextureParams();
    
	// ----- Collisions -----

	void blitCollisionCircle(int pPosX, int pPosY, int pRadius, float pScale, unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA, IND_Matrix pWorldMatrix);
	void blitCollisionLine(int pPosX1, int pPosY1, int pPosX2, int pPosY2,  unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA, IND_Matrix pIndWorldMatrix);

	// ---- Culling helpers ----

	void reCalculateFrustrumPlanes();
	void transformVerticesToWorld(float pX1, float pY1,
											float pX2, float pY2,
											float pX3, float pY3,
											float pX4, float pY4,
											IND_Vector3 *mP1Res,
											IND_Vector3 *mP2Res,
											IND_Vector3 *mP3Res,
											IND_Vector3 *mP4Res);
	// ----- Objects -----

	IND_Math _math;
	IND_Window *_window;
	OSOpenGLManager *_osOpenGLMgr;

	FRUSTRUMPLANES _frustrumPlanes;

	// ----- Vars -----
	bool _ok;

	int _numrenderedObjects;
	int _numDiscardedObjects;

	bool _doubleBuffer;

	
	struct InfoStruct _info;
    
    struct TextureSamplerState _tex2dState;

	//Current 'model-to-world' matrix
	IND_Matrix _modelToWorld;

    //Current 'camera' matrix
    IND_Matrix _cameraMatrix;
    
	// ----- Primitives vertices -----

	// Temporal buffer of pixels for drawing primitives
	PIXEL _pixels [MAX_PIXELS];

	// ----- Vertex array -----

	// Temporal buffer of vertices for drawing regions of an IND_Surface
	CUSTOMVERTEX2D _vertices2d [MAX_PIXELS];

	// ----- Primitives vertices -----

	// ----- Vertex array -----

	// ----- Info -----

	// ----- Friends ------
	friend class OpenGLTextureBuilder;
	friend class IND_Render;
};
/** @endcond */

#endif // _OPENGLRENDER_H_
