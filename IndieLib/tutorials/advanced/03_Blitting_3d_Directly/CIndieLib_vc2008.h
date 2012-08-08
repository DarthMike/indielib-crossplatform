/*****************************************************************************************
/* Desc: IndieLib singleton initialization class
/*****************************************************************************************/

#ifndef _CINDIELIB_
#define _CINDIELIB_

#pragma comment (lib, "../../common/IndieLib_vc2008.lib")
#include "../../common/LibHeaders/Indie.h"

class CIndieLib 
{
public:
	
	static CIndieLib* Instance();

	bool Init ();
	void End ();

	// ----- IndieLib objects -----

	IND_3dMeshManager		*MeshManager;
	IND_Input				*Input;
	IND_Window				*Window;
	IND_Render				*Render;
	IND_LightManager		*LightManager;
	IND_ImageManager		*ImageManager; 
	IND_SurfaceManager		*SurfaceManager;
	IND_AnimationManager	*AnimationManager;
	IND_FontManager			*FontManager;
	IND_Entity2dManager		*Entity2dManager;
	IND_Entity3dManager		*Entity3dManager;
	IND_Math				*Math;

protected:

	CIndieLib()  {}
	CIndieLib(const CIndieLib&);
	CIndieLib& operator = (const CIndieLib&);

private:
	static CIndieLib *pinstance;

	void ResetCurrentDirectory_W( void );

};


#endif // _CINDIELIB_
