/*****************************************************************************************
 * File: IND_LightManager.h
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


#ifndef _IND_LIGHTMANAGER_
#define _IND_LIGHTMANAGER_


// ----- Includes -----

#include <list>
#include "IND_Light.h"

class IND_Render;

// ----- Defines -----

#define MAX_NUM_LIGHTS 8


// --------------------------------------------------------------------------------
//									IND_LightManager
// --------------------------------------------------------------------------------

/**
@defgroup IND_LightManager IND_LightManager
@ingroup Managers
Manager of IND_Light objects. Click in ::IND_LightManager to see all the methods of this class.
*/
/*@{*/

/**
This class manages the lights used in 3d scenes. These light don't affect IND_Surface objects
or other 2d graphical objects.

See all the methods of ::IND_Light in order to check which attributes you can change in each light.
The maximun number of allowed enabled lights at the same time is 8, but can can create as many as you
want and disable / enable them.

There are several types of lights, see ::IND_LightType for more details.
*/
class LIB_EXP IND_LightManager {
public:

	// ----- Init/End -----

	IND_LightManager(): _ok(false)  { }
	~IND_LightManager()                                  {
		end();
	}

	bool    init(IND_Render *pRender);
	void    end();
	bool    isOK()           const {
		return _ok;
	}

	// ----- Public methods -----

	bool add(IND_Light *pNewLight, IND_LightType pLightType);
	bool remove(IND_Light *pLight);
	bool enable(IND_Light *pLight);
	bool disable(IND_Light *pLight);
	void update();

	// ----- Public gets -----

	int getNumLightsEnabled()  {
		return _numLightsEnabled;
	}

private:

	// ----- Private -----

	bool _ok;
	int _numLightsEnabled;      // Number of lights enabled
	int _lightIndex;            // Index that will be increased each time we use SetLight() method

	// ----- Objects -----

	IND_Render *_render;

	// ----- Container -----

	list <IND_Light *> *_listLights;

	// ----- Private methods -----

	void createLight(IND_Light *pNewLight);

	void addToList(IND_Light *pLi);
	void delFro_list(IND_Light *pLi);
	void writeMessage();
	void initVars();
	void freeVars();
};
/*@}*/

#endif // _IND_LIGHTMANAGER_
