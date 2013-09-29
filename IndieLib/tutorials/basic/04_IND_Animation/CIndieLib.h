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


#ifndef _CINDIELIB_
#define _CINDIELIB_

#include "Indie.h"

class CIndieLib {
public:

	static CIndieLib *instance();

	bool init();
	void end();

	// ----- IndieLib objects -----

	IND_3dMeshManager       *_meshManager;
	IND_Input               *_input;
	IND_Window              *_window;
	IND_Render              *_render;
	IND_LightManager        *_lightManager;
	IND_ImageManager        *_imageManager;
	IND_SurfaceManager      *_surfaceManager;
	IND_AnimationManager    *_animationManager;
	IND_FontManager         *_fontManager;
	IND_Entity2dManager     *_entity2dManager;
	IND_Entity3dManager     *_entity3dManager;
	IND_Math                *_math;

protected:

	CIndieLib()  {}
	CIndieLib(const CIndieLib &);
	CIndieLib &operator = (const CIndieLib &);

private:
	static CIndieLib *_pinstance;

	//void resetCurrentDirectory_W(void);

};


#endif // _CINDIELIB_
