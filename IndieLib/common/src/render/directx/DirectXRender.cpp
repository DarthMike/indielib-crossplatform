/*****************************************************************************************
 * File: DirectXRender.cpp
 * Desc: Initialization / Destruction using D3D
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

/** @cond DOCUMENT_PRIVATEAPI */

#include "Defines.h"

#ifdef INDIERENDER_DIRECTX

// ----- Includes -----
#include "dependencies/SDL-2.0/include/SDL_syswm.h"

#include "Global.h"
#include "IND_Math.h"
#include "IND_SurfaceManager.h"
#include "DirectXRender.h"
#include "IND_Window.h"
#include "IND_FontManager.h"
#include "IND_SurfaceManager.h"
#include "IND_3dMeshManager.h"
#include "IND_AnimationManager.h"
#include "IND_FontManager.h"
#include "IND_Timer.h"
#include "IND_Font.h"
#include "IND_3dMesh.h"
#include "IND_Animation.h"
#include "IND_Camera2d.h"
#include "IND_Camera3d.h"

// ----- Defines -----

#define VENDORID_NVIDIA 0x000010de
#define VENDORID_ATI    0x00001002


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

IND_Window* DirectXRender::initRenderAndWindow(IND_WindowProperties& props) {
	if(props._bpp <= 0 || props._height <= 0 || props._width <= 0) {
		g_debug->header("Error resetting window: Invalid parameters provided", 2);
		return 0;
	}

	end();
	initVars();

	// Window creation
	_window = new IND_Window();
	if (!_window) {
		return NULL;
	}
	if(!_window->create(props)) {
		freeVars();
		return NULL;
	}

	if (!createRender(_window))
		_ok = false;
	else
		_ok = true;

	_math = new IND_Math();

	return _window;
}

bool DirectXRender::init(LPDIRECT3D9 pDirect3d, IDirect3DDevice9 *pD3dDevice) {
	g_debug->header("Initializing Direct3D", 5);

	// Fill Info
	_info._direct3d = pDirect3d;
	_info._device   = pD3dDevice;

	// Witdh and Height of the backbuffer
	D3DSURFACE_DESC mSurfaceBackBuffer;
	LPDIRECT3DSURFACE9 mBackBuffer;
	_info._device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &mBackBuffer);
	mBackBuffer->GetDesc(&mSurfaceBackBuffer);
	mBackBuffer->Release();
	_info._fbWidth  = mSurfaceBackBuffer.Width;
	_info._fbHeight = mSurfaceBackBuffer.Height;

	// System info
	getInfo();

	// Hardware info
	writeInfo();

	_ok = true;

	// ViewPort
	setViewPort2d(0, 0, _info._fbWidth, _info._fbHeight);
	//IND_Camera2d mCamera2d(_info._viewPortWidth / 2, _info._viewPortHeight / 2);
	//setCamera2d(&mCamera2d);
	clearViewPort(0, 0, 0);

	g_debug->header("Direct3D OK", 6);

	return _ok;
}

bool DirectXRender::reset(IND_WindowProperties& props) {
	if(props._bpp <= 0 || props._height <= 0 || props._width <= 0) {
		g_debug->header("Error resetting window: Invalid parameters provided", 2);
		return 0;
	}

	// Create Window
	if (!_window->reset(props)) return 0;

	// Reset device
	if (!Direct3dReset(props._width, props._height, props._bpp, props._vsync, props._fullscreen)) return 0;

	return 1;
}

bool DirectXRender::toggleFullScreen() {
	bool wasFullScreen = _window->isFullScreen();
	
	if (!_window->toggleFullScreen()) return 0;

	if (!Direct3dReset(_window->getWidth(), _window->getHeight(), _window->getBpp(), _window->isVsync(), !wasFullScreen)) return 0;

	return 1;
}

void DirectXRender::beginScene() {
	if (!_ok)
		return;

	// ----- Recovering device (after ALT+TAB) -----

	if (GetDevice()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		// Reset device
		if (GetDevice()->Reset(&mPresentParameters) != D3D_OK) exit(0);

		// Reset view
		setViewPort2d(0, 0, _info._viewPortWidth, _info._viewPortHeight);
		IND_Camera2d mCamera2d(static_cast<float>(_info._viewPortWidth / 2),
							   static_cast<float>(_info._viewPortHeight / 2));
		setCamera2d(&mCamera2d);

		//TODO: Reset timer for good FPS showing
		//ResetTimer    ();
	}

	_info._device->BeginScene();
}

void DirectXRender::endScene() {
	if (!_ok)
		return;

	_info._device->EndScene();
	_info._device->Present(NULL, NULL, NULL, NULL);
}

void DirectXRender::showFpsInWindowTitle(char *pFPSString) {
	if (!_ok)   return;

	_window->setTitle(pFPSString);
}

void DirectXRender::setPointPixelScale (float pNewScale) {
	_info._pointPixelScale = pNewScale;
}

void DirectXRender::getNumrenderedObjectsString(char* pBuffer)      {
		IND_Math::itoa(_numrenderedObjects, pBuffer);
}

void DirectXRender::getNumDiscardedObjectsString(char* pBuffer)      {
		IND_Math::itoa(_numDiscardedObjects, pBuffer);
}

void DirectXRender::end() {
	if (_ok) {
		g_debug->header("Finalizing Direct3D", 5);
		freeVars();
		g_debug->header("Direct3D finalized ", 6);
		_ok = false;
	}
}

// --------------------------------------------------------------------------------
//							        Private methods
// --------------------------------------------------------------------------------

/*
==================
Init Direct3D
==================
*/
bool DirectXRender::Direct3Dinit(int pWidth,
                                 int pHeight,
                                 int pBpp,
                                 bool pVsync,
                                 bool pFullscreen) {
	// Direct3D creation
	LPDIRECT3D9 direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!direct3d) {
		g_debug->header("Error creating D3D object", 2);
		return 0;
	} else {
		g_debug->header("Creating D3D object", 1);
		_info._direct3d = direct3d;
	}

	if (!fillPresentParameters(pWidth,pHeight,pBpp,pVsync,pFullscreen)) return 0;

	// We try to create the device using hardware vertex processing, if it is not possible
	// we use software vertex processing
	g_debug->header("Creating the device (D3DCREATE_HARDWARE_VERTEXPROCESSING)", 1);

	if ((_info._direct3d->CreateDevice(D3DADAPTER_DEFAULT,
	                                   D3DDEVTYPE_HAL,
	                                   _wnd,
	                                   D3DCREATE_HARDWARE_VERTEXPROCESSING,
	                                   &mPresentParameters,
	                                   &_info._device)) != D3D_OK) {
		g_debug->header("Not possible to create the device (D3DCREATE_HARDWARE_VERTEXPROCESSING)", 1);
		g_debug->header("Creating the device (D3DCREATE_SOFTWARE_VERTEXPROCESSING) instead", 1);

		if ((_info._direct3d->CreateDevice(D3DADAPTER_DEFAULT,
		                                   D3DDEVTYPE_HAL,
		                                   _wnd,
		                                   D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		                                   &mPresentParameters,
		                                   &_info._device)) != D3D_OK) {
			g_debug->header("Error creating the Device (D3DCREATE_SOFTWARE_VERTEXPROCESSING)", 2);
			return 0;
		} else {
			_info._softwareVertexProcessing = 1;
		}
	} else {
		_info._softwareVertexProcessing = 0;
	}

	// System info
	getInfo();

	_info._fbWidth  = pWidth;
	_info._fbHeight = pHeight;

	// Video mode
	g_debug->header("Video mode:", 3);
	g_debug->dataInt(pWidth, 0);
	g_debug->dataChar("x", 0);
	g_debug->dataInt(pHeight, 0);
	g_debug->dataChar("x", 0);
	g_debug->dataInt(pBpp, 1);

	setViewPort2d(0, 0, pWidth, pHeight);
	IND_Camera2d mCamera2d(static_cast<float>(_info._viewPortWidth / 2), 
						   static_cast<float>(_info._viewPortHeight / 2));
	setCamera2d(&mCamera2d);
	clearViewPort(0, 0, 0);

	return 1;
}


/*
==================
Reset Direct3D
==================
*/
bool DirectXRender::Direct3dReset(int pWidth,
                                  int pHeight,
                                  int pBpp,
                                  bool pVsync,
                                  bool pFullscreen) {
	g_debug->header("Reseting Direct3D", 5);

	if (!fillPresentParameters(pWidth,pHeight,pBpp,pVsync,pFullscreen)) return 0;

	// Reset Device
	HRESULT mResult = GetDevice()->Reset(&mPresentParameters);
	if (mResult != D3D_OK) {		
		switch (mResult)
		{
		    case D3DERR_DEVICELOST: printf("\nD3DERR_DEVICELOST"); break;
		    case D3DERR_DRIVERINTERNALERROR: printf("\nD3DERR_DRIVERINTERNALERROR"); break;
		    case D3DERR_INVALIDCALL: printf("\nD3DERR_INVALIDCALL"); break;
		    case D3DERR_OUTOFVIDEOMEMORY: printf("\nD3DERR_OUTOFVIDEOMEMORY"); break;
		    case E_OUTOFMEMORY: printf("\nE_OUTOFMEMORY"); break;
		    default: printf("\nUnknown"); break;
		}

		return 0;
	} 
	

	// System info
	getInfo();

	_info._fbWidth  = pWidth;
	_info._fbHeight = pHeight;

	// ViewPort
	setViewPort2d(0, 0, _info._fbWidth, _info._fbHeight);
	IND_Camera2d mCamera2d(static_cast<float>(_info._viewPortWidth / 2), 
						   static_cast<float>(_info._viewPortHeight / 2));
	setCamera2d(&mCamera2d);
	clearViewPort(0, 0, 0);

	// Video mode
	g_debug->header("Video mode:", 3);
	g_debug->dataInt(pWidth, 0);
	g_debug->dataChar("x", 0);
	g_debug->dataInt(pHeight, 0);
	g_debug->dataChar("x", 0);
	g_debug->dataInt(pBpp, 1);

	g_debug->header("Direct3D reseted", 6);

	// Reset timer
	//FIXME: CHECK HOW TO BE CONSISTENT WITH FPS COUNT (NOT TOO IMPORTANT)
	//resetTimer();

	return 1;
}


/*
==================
Creates the render
==================
*/
bool DirectXRender::createRender(IND_Window* pWindow) {
	g_debug->header("Initializing Direct3D", 5);

	// If the window is correctly initialized
	if (pWindow->isOK()) {
		//Window information
		const SDL_SysWMinfo& sdlWindowInfo = pWindow->getWindowInfo();
		_wnd                = sdlWindowInfo.info.win.window;
		//assert(_wnd);  //Just be sure here that we have a windows window handle
		int mWidthWin       = pWindow->getWidth();
		int mHeightWin      = pWindow->getHeight();
		int mBitsWin        = pWindow->getBpp();
		bool mVsync         = pWindow->isVsync();
		bool mFullscreen    = pWindow->isFullScreen();

		// Direct3d initialization
		if (!Direct3Dinit(mWidthWin, mHeightWin, mBitsWin, mVsync, mFullscreen)) {
			g_debug->header("Finalizing D3D", 5);
			freeVars();
			g_debug->header("D3D finalized", 6);
			return 0;
		}

		g_debug->header("Direct3D OK", 6);

		// Hardware information
		writeInfo();

		return 1;
	}

	// Window error
	g_debug->header("This operation can not be done:", 3);
	g_debug->dataChar("", 1);
	g_debug->header("Invalid Id or IND_Window not correctly initialized.", 2);

	return 0;
}


/*
==================
Graphics card information
==================
*/
void DirectXRender::getInfo() {
	// Adapter
	D3DADAPTER_IDENTIFIER9 *mAdapter = new D3DADAPTER_IDENTIFIER9;
	_info._direct3d->GetAdapterIdentifier(0, 0, mAdapter);

	// ----- d3d Version -----

	strcpy(_info._version, "DXSDK November 2007");

	// ----- Vendor -----

	unsigned long mVId = mAdapter->VendorId;

	switch (mVId) {
	case VENDORID_NVIDIA: {
		strcpy(_info._vendor, "Nvidia");
		break;
	}
	case VENDORID_ATI: {
		strcpy(_info._vendor, "ATI Technologies Inc");
		break;
	}
	default: {
		strcpy(_info._vendor, "?");
		break;
	}
	}

	// ----- Renderer -----

	// Chip
	strcpy(_info._renderer, mAdapter->Description);

	// Free D3DADAPTER_IDENTIFIER9 adapter
	DISPOSE(mAdapter);

	// Caps
	D3DCAPS9 mD3dcap;
	_info._device->GetDeviceCaps(&mD3dcap);

	// Antialiasing D3DRS_ANTIALIASEDLINEENABLE
	if (mD3dcap.LineCaps & D3DLINECAPS_ANTIALIAS) _info._antialiasing = 1;

	// Max texture size
	_info._maxTextureSize = mD3dcap.MaxTextureWidth;

	// Texture units
	_info._textureUnits = mD3dcap.MaxTextureBlendStages;

	// Vertex Shader version
	_info._vertexShaderVersion = mD3dcap.VertexShaderVersion;

	// Pixel Shader version
	_info._pixelShaderVersion = mD3dcap.PixelShaderVersion;
}


/*
==================
Writes the information into the debug field
==================
*/
void DirectXRender::writeInfo() {
	g_debug->header("Hardware information" , 5);

	// ----- D3D version -----

	g_debug->header("Direct3D version:" , 3);
	g_debug->dataChar(_info._version, 1);

	// ----- Vendor -----

	g_debug->header("Mark:" , 3);
	g_debug->dataChar(_info._vendor, 1);

	// ----- Renderer -----

	g_debug->header("Chip:" , 3);
	g_debug->dataChar(_info._renderer, 1);

	// ----- Antialiasing -----

	g_debug->header("Primitive antialising (D3DRS_ANTIALIASEDLINEENABLE):", 3);
	if (_info._antialiasing)
		g_debug->dataChar("Yes", 1);
	else
		g_debug->dataChar("No", 1);

	// ----- Max texture size -----

	g_debug->header("Maximum texture size:" , 3);
	g_debug->dataInt(_info._maxTextureSize, 0);
	g_debug->dataChar("x", 0);
	g_debug->dataInt(_info._maxTextureSize, 1);

	// ----- Vertex Shader version  -----

	g_debug->header("Vertex Shader:" , 3);
	g_debug->dataInt(D3DSHADER_VERSION_MAJOR(_info._vertexShaderVersion), 0);
	g_debug->dataChar(".", 0);
	g_debug->dataInt(D3DSHADER_VERSION_MINOR(_info._vertexShaderVersion), 0);

	if (_info._softwareVertexProcessing)
		g_debug->dataChar("(Software)", 1);
	else
		g_debug->dataChar("", 1);

	// ----- Pixel Shader version -----

	g_debug->header("Pixel Shader:" , 3);
	g_debug->dataInt(D3DSHADER_VERSION_MAJOR(_info._pixelShaderVersion), 0);
	g_debug->dataChar(".", 0);
	g_debug->dataInt(D3DSHADER_VERSION_MINOR(_info._pixelShaderVersion), 1);

	g_debug->header("Hardware Ok" , 6);
}


/*
==================
Fills device presentation parameters
==================
*/
int DirectXRender::fillPresentParameters(int pWidth,
                                  int pHeight,
                                  int pBpp,
                                  bool pVsync,
                                  bool pFullscreen) {
	// Windowed
	if (!pFullscreen) {
		if ((_info._direct3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mDisplayMode)) != D3D_OK) {
			g_debug->header("Error obtaining the adapter", 2);
			return 0;
		} else
			g_debug->header("Obtaining the adapter", 1);
	} else
		// Full screen
	{
		mDisplayMode.Width = pWidth;
		mDisplayMode.Height = pHeight;
		mDisplayMode.RefreshRate = 0;

		if (pBpp == 32)
			mDisplayMode.Format = D3DFMT_A8R8G8B8;
		else
			mDisplayMode.Format = D3DFMT_R5G6B5;

		g_debug->header("Obtaining the adapter", 1);
	}

	ZeroMemory(&mPresentParameters, sizeof(mPresentParameters));

	// Windowed
	if (!pFullscreen) {
		mPresentParameters.Windowed = 1;
	}
	// Full screen
	else {
		mPresentParameters.Windowed   = 0;
		mPresentParameters.BackBufferWidth  = mDisplayMode.Width;
		mPresentParameters.BackBufferHeight = mDisplayMode.Height;
	}

	mPresentParameters.BackBufferCount  = 1;
	mPresentParameters.SwapEffect       = D3DSWAPEFFECT_DISCARD;
	mPresentParameters.BackBufferFormat = mDisplayMode.Format;


	// Depth buffer
	mPresentParameters.EnableAutoDepthStencil = true;                   //own depth and stencil format
	mPresentParameters.AutoDepthStencilFormat = D3DFMT_D16;             //depth stencil format

	/*
	// Antialiasing
	DWORD mQualityLevels;

	D3DCAPS9 pCaps;
	_info._direct3d->GetDeviceCaps (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &pCaps);

	if  (SUCCEEDED (_info._direct3d->CheckDeviceMultiSampleType (pCaps.AdapterOrdinal,
	                                                            pCaps.DeviceType,
	                                                            mDisplayMode.Format,
	                                                            mPresentParameters.Windowed,
	                                                            D3DMULTISAMPLE_2_SAMPLES,
	                                                            &mQualityLevels)))
	{
	    g_debug->Header ("Iniciando Antialiasing", 1);
	    mPresentParameters.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
	    mPresentParameters.MultiSampleQuality = mQualityLevels;
	}
	*/

	// Vsync
	if (!pVsync)
		mPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	return 1;
}
/*
==================
Destroys the renderer
==================
*/
void DirectXRender::DestroyD3DWindow() {
	// Free device
	if (_info._device)
		_info._device->Release();
	_info._device = NULL;
	g_debug ->header("Finalizing the Device", 1);

	// Free D3D object
	if (_info._direct3d)
		_info._direct3d->Release();
	_info._direct3d = NULL;
	g_debug ->header("Finalizing D3D object", 1);
}

/*
==================
Init render attributes
==================
*/
void DirectXRender::InitAttribRender() {
	_info._version [0]      = 0;
	_info._vendor [0]       = 0;
	_info._renderer [0]     = 0;
	_info._maxTextureSize   = 0;
}


/*
==================
Init vars
==================
*/
void DirectXRender::initVars() {
	InitAttribRender();
	_window = NULL;
}


/*
==================
Free memory
==================
*/
void DirectXRender::freeVars() {
	// Destroy the render
	DestroyD3DWindow();
	DISPOSE(_window);
	DISPOSE(_math);
}


/** @endcond */
#endif //INDIERENDER_DIRECTX
