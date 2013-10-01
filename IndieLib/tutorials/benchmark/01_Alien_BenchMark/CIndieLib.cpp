/*****************************************************************************************
 * Desc: IndieLib singleton initialization class
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


#include "CIndieLib.h"


/*
==================
Init singleton
==================
*/
CIndieLib *CIndieLib::_pinstance = 0;// initialize pointer
CIndieLib *CIndieLib::instance() {
	if (_pinstance == 0) { // is it the first call?
		_pinstance = new CIndieLib; // create sole instance
	}
	return _pinstance; // address of sole instance
}



/*
==================
Init IndieLib
==================
*/
bool CIndieLib::init() {
	//resetCurrentDirectory_W();

	// IndieLib Initialization, a debug.log file will be created.
	IndieLib::init(IND_DEBUG_MODE);

	_input               =   new     IND_Input;
	_render              =   new     IND_Render;
	//_lightManager      =   new     IND_LightManager;
	_imageManager        =   new     IND_ImageManager;
	_surfaceManager      =   new     IND_SurfaceManager;
	//_meshManager           =   new     IND_3dMeshManager;
	_animationManager    =   new     IND_AnimationManager;
	_fontManager         =   new     IND_FontManager;
	_entity2dManager     =   new     IND_Entity2dManager;
	//_entity3dManager       =   new     IND_Entity3dManager;
	_math                =   new     IND_Math;

	IND_WindowProperties props ("IndieLib", 800, 600, 32, 0, 0, 1);
	
	
	_window = _render        ->initRenderAndWindow(props);                                 
	if(!_window)
		return 0;

	//if (!_lightManager     ->init (_render))                                    return 0;
	if (!_imageManager       ->init())                                           return 0;
	if (!_surfaceManager     ->init(_imageManager, _render))                     return 0;
	if (!_animationManager   ->init(_imageManager, _surfaceManager))             return 0;
	if (!_fontManager        ->init(_imageManager, _surfaceManager))             return 0;
	if (!_entity2dManager    ->init(_render))                                    return 0;
	//if (!_entity3dManager  ->init (_render))                                    return 0;
	//if (!_meshManager      ->init (_render))                                    return 0;
	if (!_input              ->init(_render))                                    return 0;
	if (!_math               ->init())                                           return 0;

	return 1;
}


/*
==================
Free Indielib managers
==================
*/
void CIndieLib::end() {
	
    // ----- Freeing objects -----

	_math                ->end();
	//_meshManager           ->end();
	_input               ->end();
	_entity2dManager     ->end();
	//_entity3dManager       ->end();
	_fontManager         ->end();
	_animationManager    ->end();
	_surfaceManager      ->end();
	_imageManager        ->end();
	//_lightManager      ->end();
	_render              ->end();

	DISPOSE(_math);
	//DISPOSE(_meshManager);
	DISPOSE(_input);
	DISPOSE(_entity2dManager);
	//DISPOSE(_entity3dManager);
	DISPOSE(_fontManager);
	DISPOSE(_animationManager);
	DISPOSE(_surfaceManager);
    DISPOSE(_imageManager);
	//DISPOSE(_lightManager);
	DISPOSE(_render);

	IndieLib::end();

    DISPOSE(_pinstance);
}
