/*****************************************************************************************
 * File: IND_LightManager.h
 * Desc: Light manager
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
