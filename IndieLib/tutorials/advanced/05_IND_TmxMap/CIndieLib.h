/*****************************************************************************************
 * Desc: IndieLib singleton initialization class
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
	IND_TmxMapManager       *_tmxMapManager;

protected:

	CIndieLib()  {}
	CIndieLib(const CIndieLib &);
	CIndieLib &operator = (const CIndieLib &);

private:
	static CIndieLib *_pinstance;

	//void resetCurrentDirectory_W(void);

};


#endif // _CINDIELIB_
