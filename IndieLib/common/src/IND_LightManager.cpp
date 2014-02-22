/*****************************************************************************************
 * File: IND_LightManager.cpp
 * Desc: Light manager
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
#include "IND_LightManager.h"
#include "IND_Render.h"


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 * Returns 1 (true) if the adminstrator is successfully initialized.
 * Must be called before using any method.
 * @param pRender					Pointer to IND_Render correctly initialized.
 */
bool IND_LightManager::init(IND_Render *pRender) {
	end();
	initVars();

	g_debug->header("Initializing LightManager", 5);
	g_debug->header("Preparing LightManager", 1);

	// Checking IND_Render
	if (pRender->isOK()) {
		g_debug->header("Checking IND_Render", 1);
		_render = pRender;
	} else {
		g_debug->header("IND_Render is not correctly initialized", 2);
		_ok = false;
		return _ok;
	}

	_ok = true;

	g_debug->header("LightManager OK", 6);

	return _ok;
}


/**
 * Frees the manager and all the objects that it contains.
 */
void IND_LightManager::end() {
	if (_ok) {
		g_debug->header("Finalizing LightManager", 5);
		g_debug->header("Freeing lights" , 5);
		freeVars();
		g_debug->header("Lights freed", 6);
		g_debug->header("LightManager finalized", 6);

		_ok = false;
	}
}

// --------------------------------------------------------------------------------
//									  Public methods
// --------------------------------------------------------------------------------

/**
 * Returns 1 (true) if the light object passed as a parameter exists and is added successfully to the manager.
 * See ::IND_LightType to see the different type of lights you can use.
 *
 * Using this method, IndieLib will try to enable the light automatically. You can only have 8 ligths enabled at the same time
 * (not including ambient light), but you can add to the manager as many
 * as you want.
 *
 * If you add a light to the manager and there are already 8 lights enabled, it will be automatically disabled.
 * You can disable other lights in the scene and then you will be able to enable this light.
 *
 * Use the ::IND_Light methods in order to change the attributes of the light.
 * @param pNewLight					Pointer to a new IND_Light object.
 * @param pLightType					Type of the light. See ::IND_LightType.
 */
bool IND_LightManager::add(IND_Light *pNewLight, IND_LightType pLightType) {
	g_debug->header("Adding Light", 5);
	g_debug->header("Light name:", 3);
	g_debug->dataInt((int) &pNewLight->_light, 1);

	if (!_ok) {
		writeMessage();
		return 0;
	}

	// ----- Light creation -----

	//TODO: SUPPORT LIGHTS
	/*
	switch (pLightType)
	{
	    case IND_AMBIENT_LIGHT:
	    {
	        pNewLight->mLight->mLightType = IND_AMBIENT_LIGHT;
	        pNewLight->mLight->_attributes.mIsEnabled = 1;
	        //_render->GetDevice()->SetRenderState (D3DRS_AMBIENT, D3DCOLOR_XRGB (255, 255, 255));
	        break;
	    }

	    case IND_DIRECTIONAL_LIGHT:
	    {
	        CreateLight (pNewLight, D3DLIGHT_DIRECTIONAL);

	        break;
	    }

	    case IND_POINT_LIGHT:
	    {
	        CreateLight (pNewLight, D3DLIGHT_POINT);

	        break;
	    }

	    case IND_SPOT_LIGHT:
	    {
	        CreateLight (pNewLight, D3DLIGHT_SPOT);

	        break;
	    }
	}


	// ----- Puts the object into the manager -----

	AddToList (pNewLight);

	g_debug->header("Light added", 6);
	*/
	return 1;
}

/**
 * Returns 1(true) if the light object passed as parameter exists and it is
 * deleted from the manager successfully.
 * @param pLight					Pointer to a IND_Light object.
 */
bool IND_LightManager::remove(IND_Light *pLight) {
	g_debug->header("Freeing light", 5);

	if (!_ok || !pLight) {
		writeMessage();
		return 0;
	}
	//TODO: SUPPORT LIGHTS
	/*
	// Search object
	bool mIs = 0;
	list <IND_Light*>::iterator _listIter;
	for (_listIter  = _listLights->begin();
	     _listIter != _listLights->end();
	     _listIter++)
	{
	    if ((*_listIter) == pLight)
	    {
	        mIs = 1;
	        break;
	    }
	}

	// Not found
	if (!mIs)
	{
	    writeMessage ();
	    return 0;
	}

	// ----- Free object -----

	g_debug->Header ("Light name:", 3);
	g_debug->DataInt ((int) &pLight->mLight, 1);

	// Quit from list
	delFromlist (pLight);

	g_debug->Header ("Ok", 6);
	*/
	return 1;
}

/**
 * This method must be called always before the IND_Entity3dManager::renderEntities3d() method. It will
 * update all the lights attributes.
 */
void IND_LightManager::update() {
	//TODO: SUPPORT LIGHTS
	/*
	list <IND_Light*>::iterator _listIter;
	for (_listIter  = _listLights->begin();
	     _listIter  != _listLights->end();
	     _listIter++)
	{
	    // Ambient light
	    if ((*_listIter)->mLight.mLightType == IND_AMBIENT_LIGHT)
	    {
	        //TODO: MOVE THIS CODE TO OTHER CLASS
	        //_render->GetDevice()->SetRenderState (D3DRS_AMBIENT, D3DCOLOR_XRGB    (   (BYTE) (*_listIter)->mLight._attributes._r * 255,
	        //                                                                      (BYTE) (*_listIter)->mLight._attributes._g * 255,
	        //                                                                      (BYTE) (*_listIter)->mLight._attributes._b * 255) );
	    }
	    else
	    // Update lights that has been modified
	    if ((*_listIter)->mLight.mSetLightFlag)
	    {
	        // Rest of lights
	        (*_listIter)->mLight.mSetLightFlag = 0;
	        //TODO: SOLVE DEPENDANCE IN DIRECT3D ONLY BY MOVING CODE SOMEWHERE ELSE?
	        //_render->GetDevice()->SetLight ((*_listIter)->mLight._lightIndex, &(*_listIter)->mLight.mD3DLight);
	    }
	}
	*/
}

/**
 * This function returns 1 (true) if the light object passed as a parameter exists and the light is enabled.
 * You can only have 8 ligths enabled at the same time (not including ambient light). This method will return 0 if there are
 * already 8 lights enabled.
 * Use the IND_LightManager::GetNumLightsEnabled() method in order to know how many lights are enabled.
 * @param pLight					Pointer to a IND_Light object.
 */
bool IND_LightManager::enable(IND_Light *pLight) {
	if (!_ok) {
		writeMessage();
		return 0;
	}

	if (!pLight) return 0;
	//TODO: SUPPORT LIGHTS
	/*
	// Check that the light is disabled
	if (!pLight->IsEnable())
	{
	    // Ambient light
	    if (pLight->mLight.mLightType == IND_AMBIENT_LIGHT)
	    {
	        pLight->mLight._attributes._r = pLight->mLight._attributes.mLastR;
	        pLight->mLight._attributes._g = pLight->mLight._attributes.mLastG;
	        pLight->mLight._attributes._b = pLight->mLight._attributes.mLastB;

	        pLight->mLight._attributes.mIsEnabled = 1;
	    }
	    // Rest of lights

	    // Check how many lights are enabled already and that the light is not already enabled
	    else
	    if (GetNumLightsEnabled() < MAX_NUM_LIGHTS && !pLight->IsEnable())
	    {
	        //TODO: SOLVE DEPENDANCE IN DIRECT3D ONLY BY MOVING CODE SOMEWHERE ELSE?
	        //_render->GetDevice()->LightEnable (pLight->mLight._lightIndex, 1);    // Enable light
	        _numLightsEnabled++;
	        pLight->mLight._attributes.mIsEnabled = 1;
	        return 1;
	    }
	}
	*/
	return 0;
}

/**
 * Returns 1 (true) if the light object passed as a parameter exists and the light is disabled.
 * Use the IND_LightManager::GetNumLightsEnabled() method in order to know how many lights are enabled.
 * @param pLight					Pointer to a IND_Light object.
 */
bool IND_LightManager::disable(IND_Light *pLight) {
	if (!_ok) {
		writeMessage();
		return 0;
	}

	if (!pLight) return 0;
	//TODO: SUPPORT LIGHTS
	/*
	// Check that the light is enable
	if (pLight->IsEnable())
	{
	    // Ambient light
	    if (pLight->mLight.mLightType == IND_AMBIENT_LIGHT)
	    {
	        pLight->mLight._attributes.mLastR = pLight->mLight._attributes._r;
	        pLight->mLight._attributes.mLastG = pLight->mLight._attributes._g;
	        pLight->mLight._attributes.mLastB = pLight->mLight._attributes._b;

	        pLight->SetColor (0, 0, 0, 0);

	        pLight->mLight._attributes.mIsEnabled = 0;
	    }
	    // Rest of lights
	    else
	    {
	        //TODO: SOLVE DEPENDANCE IN DIRECT3D ONLY BY MOVING CODE SOMEWHERE ELSE?
	        //_render->GetDevice()->LightEnable (pLight->mLight._lightIndex, 0);    // Disable light
	        _numLightsEnabled--;
	        pLight->mLight._attributes.mIsEnabled = 0;
	    }

	    return 1;
	}
	*/
	return 0;
}

// --------------------------------------------------------------------------------
//										Private methods
// --------------------------------------------------------------------------------

/*
==================
Initializes a light
==================
*/
void IND_LightManager::createLight(IND_Light *pNewLight) {
	//TODO: SUPPORT LIGHTS
	/*
	// Set light
	pNewLight->mLight.mD3DLight.Type = pLightType;
	pNewLight->mLight._lightIndex = _lightIndex;
	//TODO: SOLVE DEPENDANCE IN DIRECT3D ONLY BY MOVING CODE SOMEWHERE ELSE?
	//_render->GetDevice()->SetLight (_lightIndex, &pNewLight->mLight.mD3DLight);

	// Enable light if possible
	if (GetNumLightsEnabled() < MAX_NUM_LIGHTS)
	{
	    //TODO: SOLVE DEPENDANCE IN DIRECT3D ONLY BY MOVING CODE SOMEWHERE ELSE?
	    //_render->GetDevice()->LightEnable (_lightIndex, 1);
	    pNewLight->mLight._attributes.mIsEnabled = 1;
	    _numLightsEnabled++;
	}
	// Too much lights, create the light but disable it
	else
	{
	    pNewLight->mLight._attributes.mIsEnabled = 0;
	}

	// Increase light index
	_lightIndex++;
	*/
}

/*
==================
Inserts object into the manager
==================
*/
void IND_LightManager::addToList(IND_Light *) {
//void IND_LightManager::addToList(IND_Light *pLi) {
	//TODO: SUPPORT LIGHTS
	/*
	_listLights->push_back (pLi);
	*/
}

/*
==================
Deletes object from the manager
==================
*/
//void IND_LightManager::delFro_list(IND_Light *pLi) {
void IND_LightManager::delFro_list(IND_Light *) {
	//TODO: SUPPORT LIGHTS
	/*
	_listLights->remove (pLi);
	pLi = 0;
	*/
}

/*
==================
Initialization error message
==================
*/
void IND_LightManager::writeMessage() {
	g_debug->header("This operation can not be done", 3);
	g_debug->dataChar("", 1);
	g_debug->header("Invalid Id or IND_LightManager not correctly initialized", 2);
}

/*
==================
Init manager vars
==================
*/
void IND_LightManager::initVars() {
	_listLights = new list <IND_Light *>;

	_numLightsEnabled = 0;
	_lightIndex = 0;
}

/*
==================
Free memory
==================
*/
void IND_LightManager::freeVars() {
	//TODO: SUPPORT LIGHTS
	/*
	// Release all of the manager images
	list <IND_Light*>::iterator mLightListIter;
	for (mLightListIter  = _listLights->begin();
	     mLightListIter != _listLights->end();
	     mLightListIter++)
	{
	    g_debug->Header   ("Freeing light:", 3);
	    g_debug->DataInt ((int) &(*mLightListIter)->mLight, 1);
	}

	// Clear list
	_listLights->clear ();

	// Free list
	Dispose (_listLights);
	*/
}
