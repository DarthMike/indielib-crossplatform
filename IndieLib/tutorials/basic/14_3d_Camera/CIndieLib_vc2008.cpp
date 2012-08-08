/*****************************************************************************************
/* Desc: IndieLib singleton initialization class
/*****************************************************************************************/

#include "CIndieLib_vc2008.h"

/* === Windows === */
#include "TCHAR.h"

/*
==================
Init singleton
==================
*/
CIndieLib* CIndieLib::pinstance = 0;// initialize pointer
CIndieLib* CIndieLib::Instance () 
{
	if (pinstance == 0)  // is it the first call?
	{  
		pinstance = new CIndieLib; // create sole instance
	}
	return pinstance; // address of sole instance
}

/*
==================
Sets working path to the directory of exe file (Windows)
==================
*/
void CIndieLib::ResetCurrentDirectory_W( void )
{
	TCHAR app_path[MAX_PATH] = _T("");
	DWORD size_in_tchars = sizeof(app_path)/sizeof(TCHAR);
	// get full app path with the exe filename
	GetModuleFileName( 0, app_path, size_in_tchars-1 );
	// making app_path to end on dir char ('\\')
	// _tcsrchr - search for char from the string's end
	TCHAR* app_dir = _tcsrchr(app_path, _T('\\'));
	if (app_dir) {
		app_dir += 1;
		if (app_dir) { 
			*app_dir = 0; 
			SetCurrentDirectory(app_path);
			return;
		}
	}
	// TODO. inform somehow that func is failed.
}


/*
==================
Init IndieLib
==================
*/
bool CIndieLib::Init()
{
	ResetCurrentDirectory_W();

	// IndieLib Initialization, a debug.log file will be created.
	IndieLib::Init (IND_DEBUG_MODE);
		
	Input				=   new		IND_Input;
	Window				=	new		IND_Window;
	Render				=	new		IND_Render;
	LightManager		=	new		IND_LightManager;
	ImageManager		=	new		IND_ImageManager;
	SurfaceManager		=	new		IND_SurfaceManager;
	MeshManager			=	new		IND_3dMeshManager;
	AnimationManager	=	new		IND_AnimationManager;
	FontManager			=	new		IND_FontManager;
	Entity2dManager		=	new		IND_Entity2dManager;
	Entity3dManager		=	new		IND_Entity3dManager;
	Math				=	new		IND_Math;

	if (!Window				->Init ("", 800, 600, 32, 0, 0))					return 0;
	if (!Render				->Init (Window))									return 0;
	if (!LightManager		->Init (Render))									return 0;
	if (!ImageManager		->Init ())											return 0;
	if (!SurfaceManager		->Init (ImageManager, Render))						return 0;
	if (!AnimationManager	->Init (ImageManager, SurfaceManager))				return 0;
	if (!FontManager		->Init (ImageManager, SurfaceManager))				return 0;	
	if (!Entity2dManager	->Init (Render))									return 0;
	if (!Entity3dManager	->Init (Render))									return 0;
	if (!MeshManager		->Init (Render))									return 0;
	if (!Input				->Init (Render))									return 0;
	if (!Math				->Init ())											return 0;

	return 1;
}


/*
==================
Free Indielib managers
==================
*/
void CIndieLib::End()
{
	// ----- Freeing objects -----

	Math				->End();
	MeshManager			->End();
	Input				->End();
	Entity2dManager		->End();
	Entity3dManager		->End();
	FontManager			->End();
	AnimationManager	->End();
	SurfaceManager		->End();
	ImageManager		->End();
	LightManager		->End();
	Render				->End();
	Window				->End();

	IndieLib::End ();
}
