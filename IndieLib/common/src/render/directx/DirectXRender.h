/*****************************************************************************************
 * File: DirectXRender.h
 * Desc: Render class using Direct3D
 *
 * Divided in:
 * - DirectXRender.cpp
 * - RenderTransform2dDirectX.cpp
 * - RenderTransform3dDirectX.cpp
 * - RenderTransformCommonDirectX.cpp
 * - RenderObject2dDirectX.cpp
 * - RenderObject3dDirectX.cpp
 * - RenderPrimitive2dDirectX.cpp
 * - RenderText2dDirectX.cpp
 * - RenderCollision2dDirectX.cpp
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
THIS FILE IS AN ADDITIONAL FILE ADDED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com), BUT HAS THE
SAME LICENSE AS THE WHOLE LIBRARY TO RESPECT ORIGINAL AUTHOR OF LIBRARY

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


#ifndef _DIRECTXRENDER_H_
#define _DIRECTXRENDER_H_

/** @cond DOCUMENT_PRIVATEAPI */

// ----- Includes -----

#include "Defines.h"
#include "IND_Render.h"
#include "IND_Vector3.h"
#include "IND_Math.h"

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
class IND_Math;

// ----- Libs -----

#include <d3d9.h>
#include <d3dx9.h>

// ----- Defines -----

#define MAX_PIXELS 2048
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))


// --------------------------------------------------------------------------------
//									  DirectXRender
// --------------------------------------------------------------------------------

class DirectXRender {
public:

	// ----- Init/End -----

	DirectXRender(): _ok(false),_window(NULL),_math(NULL)  { }
	~DirectXRender()              {
		end();
	}


	IND_Window*  initRenderAndWindow(IND_WindowProperties& props);
	bool    init(LPDIRECT3D9 pDirect3d, IDirect3DDevice9 *pD3dDevice);
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


	void   clearViewPort(BYTE pR,
	                     BYTE pG,
	                     BYTE pB);

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
	               BYTE pR,
	               BYTE pG,
	               BYTE pB,
	               BYTE pA);

	void blitLine(int pX1,
	              int pY1,
	              int pX2,
	              int pY2,
	              BYTE pR,
	              BYTE pG,
	              BYTE pB,
	              BYTE pA);

	void blitRectangle(int pX1,
	                   int pY1,
	                   int pX2,
	                   int pY2,
	                   BYTE pR,
	                   BYTE pG,
	                   BYTE pB,
	                   BYTE pA);

	void blitFillRectangle(int pX1,
	                       int pY1,
	                       int pX2,
	                       int pY2,
	                       BYTE pR,
	                       BYTE pG,
	                       BYTE pB,
	                       BYTE pA);

	void blitColoredTriangle(int pX1,
	                         int pY1,
	                         int pX2,
	                         int pY2,
	                         int pX3,
	                         int pY3,
	                         BYTE pR1, BYTE pG1, BYTE pB1,
	                         BYTE pR2, BYTE pG2, BYTE pB2,
	                         BYTE pR3, BYTE pG3, BYTE pB3,
	                         BYTE pA);

	void blitTriangleList(IND_Point *pTrianglePoints,
	                      int pNumPoints,
	                      BYTE pR,
	                      BYTE pG,
	                      BYTE pB,
	                      BYTE pA);

	bool blitPoly2d(IND_Point *pPixel,
	                int pNumLines,
	                BYTE pR,
	                BYTE pG,
	                BYTE pB,
	                BYTE pA);

	bool blitRegularPoly(int pX,
	                     int pY,
	                     int pRadius,
	                     int pN,
	                     float pAngle,
	                     BYTE pR,
	                     BYTE pG,
	                     BYTE pB,
	                     BYTE pA);

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
	                  BYTE pR,
	                  BYTE pG,
	                  BYTE pB,
	                  BYTE pA,
	                  BYTE pFadeR,
	                  BYTE pFadeG,
	                  BYTE pFadeB,
	                  BYTE pFadeA,
	                  IND_BlendingType pSo,
	                  IND_BlendingType pDs);

	void setRainbow3d(bool pCull,
	                  bool pFlipNormals,
	                  IND_Filter pFilter,
	                  BYTE pR,
	                  BYTE pG,
	                  BYTE pB,
	                  BYTE pA,
	                  BYTE pFadeR,
	                  BYTE pFadeG,
	                  BYTE pFadeB,
	                  BYTE pFadeA,
	                  IND_BlendingType pSo,
	                  IND_BlendingType pDs);

	void lookAt(float pEyeX, float pEyeY, float pEyeZ,
	            float pLookAtX, float pLookAtY, float pLookAtZ,
	            float pUpX, float pUpY, float pUpZ);

	void perspectiveFov(float pFov, float pAspect, float pNearClippingPlane, float pFarClippingPlane);

	void perspectiveOrtho(float pWidth, float pHeight, float pNearClippingPlane, float pFarClippingPlane);

	// ----- Render Object 2d -----

	void blitSurface(IND_Surface *pSu);

	void blitGrid(IND_Surface *pSu, BYTE pR, BYTE pG, BYTE pB, BYTE pA);

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
	                  int pSequence,
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
	              BYTE pR,
	              BYTE pG,
	              BYTE pB,
	              BYTE pA,
	              BYTE pFadeR,
	              BYTE pFadeG,
	              BYTE pFadeB,
	              BYTE pFadeA,
	              IND_Filter pLinearFilter,
	              IND_BlendingType pSo,
	              IND_BlendingType pDs,
	              IND_Align pAlign);


	void blit3dMesh(IND_3dMesh *p3dMesh);
	void set3dMeshSequence(IND_3dMesh *p3dMesh, unsigned int pIndex);


	// ----- Rendering steps -----
	void reCalculateFrustrumPlanes();

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

	//This function returns the pointer to Direct3d.
	LPDIRECT3D9 GetDirect3d()      {
		return _info._direct3d;
	}
	//This function returns the pointer to the Direct3d device.
	IDirect3DDevice9 *GetDevice()      {
		return _info._device;
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

	bool _ok;

	// ----- Objects -----

	IND_Window *_window;
	IND_Math *_math;
	HWND _wnd;

	// ----- Vars -----

	int _numrenderedObjects;
	int _numDiscardedObjects;

	FRUSTRUMPLANES _frustrumPlanes;

	D3DDISPLAYMODE mDisplayMode;                    // Display mode
	D3DPRESENT_PARAMETERS mPresentParameters;       // Presentation parameters


	// ----- Primitives vertices -----

	// Temporal buffer of pixels for drawing primitives
	PIXEL _pixels [MAX_PIXELS];

	// ----- Vertex array -----

	// Temporal buffer of vertices for drawing regions of an IND_Surface
	CUSTOMVERTEX2D _vertices2d [MAX_PIXELS];

	// ----- Info -----

	struct infoStruct {
		int _fbWidth;
		int _fbHeight;
		int _viewPortX;
		int _viewPortY;
		int _viewPortWidth;
		int _viewPortHeight;
		bool _antialiasing;
		char _version [1024] ;
		char _vendor [1024];
		char _renderer [1024];
		int _maxTextureSize;
		int _textureUnits;
        float _pointPixelScale;
		DWORD _vertexShaderVersion;
		DWORD _pixelShaderVersion;
		bool _softwareVertexProcessing;
		LPDIRECT3D9 _direct3d;
		IDirect3DDevice9 *_device;
        
		infoStruct() : _fbWidth(0),
					   _fbHeight(0),
					   _viewPortX(0),
					   _viewPortY(0),
					   _viewPortWidth(0),
					   _viewPortHeight(0),
					   _antialiasing(false),
					   _maxTextureSize(0),
					   _textureUnits(0),
					   _pointPixelScale(1.0f),
					   _vertexShaderVersion(0),
					   _pixelShaderVersion(0),
					   _softwareVertexProcessing(false),
					   _direct3d(NULL),
					   _device(NULL) {
						   strcpy(_version,"");
						   strcpy(_vendor,"");
						   strcpy(_renderer,"");
		}
	};
	struct infoStruct _info;

	// ----- Private methods -----

	bool Direct3Dinit(int pWidth, int pHeight, int pBpp, bool pVsync, bool pFullscreen);
	bool Direct3dReset(int pWidth, int pHeight, int pBpp, bool pVsync, bool pFullscreen);

	bool createRender(IND_Window *pWindow);

	void getInfo();
	void writeInfo();

	D3DBLEND GetD3DBlendingType(IND_BlendingType pBlendingType);
	D3DTEXTUREFILTERTYPE                   GetD3DFilter(IND_Filter pFilter);

	void   fillVertex2d(CUSTOMVERTEX2D *pVertex2d,
	                    float pX,
	                    float pY,
	                    float pU,
	                    float pV);

	// ----- Render Object 3d -----

	void FrameMove(IND_3dMesh *p3dMesh, float elapsedTime, const D3DXMATRIX *matWorld);
	void UpdateFrameMatrices(const D3DXFRAME *frameBase, const D3DXMATRIX *parentMatrix);
	void DrawFrame(LPD3DXFRAME frame);
	void DrawMeshContainer(LPD3DXMESHCONTAINER meshContainerBase, LPD3DXFRAME frameBase);

	// ----- Primitives -----

	bool   IsBlockRightOfViewPort(D3DXVECTOR4 pP1, D3DXVECTOR4 pP2, D3DXVECTOR4 pP3, D3DXVECTOR4 pP4);
	bool   IsBlockLeftOfViewPort(D3DXVECTOR4 pP1, D3DXVECTOR4 pP2, D3DXVECTOR4 pP3, D3DXVECTOR4 pP4);
	bool   IsBlockDownOfViewPort(D3DXVECTOR4 pP1, D3DXVECTOR4 pP2, D3DXVECTOR4 pP3, D3DXVECTOR4 pP4);
	bool   IsBlockUpOfViewPort(D3DXVECTOR4 pP1, D3DXVECTOR4 pP2, D3DXVECTOR4 pP3, D3DXVECTOR4 pP4);

	void fillPixel(PIXEL *pPixel,
	               int pX,
	               int pY,
	               BYTE pR,
	               BYTE pG,
	               BYTE pB);

	void fillPixel(PIXEL *pPixel,
	               int pX,
	               int pY,
	               int pZ,
	               BYTE pR,
	               BYTE pG,
	               BYTE pB);

	void setForPrimitive(BYTE pA);

	// ------ Fonts -----

	int getLongInPixels(IND_Font *pFo, char *pText, int pPos, int pOffset);

	void SetTranslation(int pX, int pY, D3DXMATRIX *pMatWorld, D3DXMATRIX *pMatTraslation);

	// ----- Grid -----

	void BlitGridLine(int pPosX1, int pPosY1, int pPosX2, int pPosY2,  BYTE pR, BYTE pG, BYTE pB, BYTE pA, D3DXMATRIX pWorldMatrix);
	void BlitGridQuad(int pAx, int pAy,
	                  int pBx, int pBy,
	                  int pCx, int pCy,
	                  int pDx, int pDy,
	                  BYTE pR, BYTE pG, BYTE pB, BYTE pA,
	                  D3DXMATRIX pWorldMatrix);
	
	// ----- Collisions  -----
	void blitCollisionCircle(int pPosX, int pPosY, int pRadius, float pScale, BYTE pR, BYTE pG, BYTE pB, BYTE pA, IND_Matrix pWorldMatrix);
	void blitCollisionLine(int pPosX1, int pPosY1, int pPosX2, int pPosY2,  BYTE pR, BYTE pG, BYTE pB, BYTE pA, IND_Matrix pIndWorldMatrix);

	// ----- Culling -----
	void Transform4Vertices(float pX1, float pY1,
	                        float pX2, float pY2,
	                        float pX3, float pY3,
	                        float pX4, float pY4,
	                        D3DXVECTOR4 *mP1Res,
	                        D3DXVECTOR4 *mP2Res,
	                        D3DXVECTOR4 *mP3Res,
	                        D3DXVECTOR4 *mP4Res);
	// ----- Init / End -----
	int fillPresentParameters(int pWidth,
                                 int pHeight,
                                 int pBpp,
                                 bool pVsync,
                                 bool pFullscreen);
	void DestroyD3DWindow();
	void InitAttribRender();
	void initVars();
	void freeVars();

	// ----- Friends ------
	friend class DirectXTextureBuilder;
	friend class IND_Render;
};

/** @endcond */

#endif // _DIRECTXRENDER_H_
