/*****************************************************************************************
/* Desc: Tutorial b) 03 Blitting 3d directly
/*****************************************************************************************/

#include "CIndieLib_vc2008.h"

/*
==================
Main
==================
*/
int IndieLib()
{
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::Instance();
	if (!mI->Init ()) return 0;			

	// ----- 3d Mesh loading -----

	// Bones loading
	IND_3dMesh mMeshBones;
	if (!mI->MeshManager->Add (&mMeshBones, "../resources/skeleton/skeleton.x", "../resources/skeleton")) return 0;

	// ----- Cameras ------

	IND_Camera3d mCamera3d (0.0f, 0.9f, -3.0f);
	mCamera3d.SetAspect ((float) mI->Window->GetWidth () / mI->Window->GetHeight());

	// ----- Light -----

	IND_Light mLight;
	mI->LightManager->Add (&mLight, IND_DIRECTIONAL_LIGHT);
	mLight.SetPosition (0, 0, 0);
	mLight.SetColor (1.0f, 1.0f, 1.0f, 1.0f);
	mLight.SetDirection (0.0f, -0.3f, 0.5f);
	mLight.SetRange (1000.0f);

	// ----- Main Loop -----

	int	mSequence = 0;

	while (!mI->Input->OnKeyPress (IND_ESCAPE) && !mI->Input->Quit())
	{
		// ----- Input update ----

		mI->Input->Update ();

		// ----- Input -----

		// Change sequence when pressing UP key
		if (mI->Input->OnKeyPress (IND_KEYUP))
		{
			mSequence++;
			if (mSequence == mMeshBones.GetNumSequences()) mSequence = 0;
			mI->Render->Set3dMeshSequence (&mMeshBones, mSequence);
		}

		// Change sequence when pressing KEY up
		if (mI->Input->OnKeyPress (IND_KEYDOWN))
		{
			mSequence--;
			if (mSequence < 0) mSequence = mMeshBones.GetNumSequences() - 1;
			mI->Render->Set3dMeshSequence (&mMeshBones, mSequence);
		}

		// ----- Render  -----

		mI->Render->BeginScene ();
		mI->Render->ClearViewPort (60, 60, 60);
		mI->Render->SetViewPort3d (0, 0, mI->Window->GetWidth(), mI->Window->GetHeight());
		mI->Render->SetCamera3d (&mCamera3d);
		mI->LightManager->Update ();

		// 1) We apply the world space transformations (translation, rotation, scaling).
		// If you want to recieve the transformation in a single matrix you can pass
		// and IND_Matrix object by reference.
		IND_Matrix mMatrix;
		mI->Render->SetTransform3d	(0,					// x pos
									0,					// y pos
									0,					// z pos
									0,					// Angle x	
									0,					// Angle y
									0,					// Angle z
									1,					// Scale x
									1,					// Scale y
									1,					// Scale z		
									&mMatrix);			// Matrix in wich the transformation will be applied (optional)
 
		// 2) We apply the color, blending and culling transformations.
		mI->Render->SetRainbow3d	(1,					// Back face culling 0/1 => off / on
									0,					// Flip normals 0/1 => off / on
									IND_FILTER_LINEAR,	// IND_Filter
									255,				// R Component	for tinting
									255,				// G Component	for tinting
									255,				// B Component	for tinting			
									255,				// A Component	for tinting
									0,					// R Component	for fading to a color		
									0,					// G Component	for fading to a color		
									0,					// B Component	for fading to a color			
									255,				// Amount of fading	
									IND_SRCALPHA,		// IND_BlendingType (source)
									IND_INVSRCALPHA);	// IND_BlendingType (destination)

		// 3) Blit the 3d Mesh
		mI->Render->Blit3dMesh (&mMeshBones);
	
		mI->Render->EndScene ();	
	}

	// ----- Free -----

	mI->End ();

	return 0;
}
