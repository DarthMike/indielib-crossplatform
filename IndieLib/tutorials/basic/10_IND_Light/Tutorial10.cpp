/*****************************************************************************************
/* Desc: Tutorial a) 10 IND_Light
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

	// ----- Surface loading -----

	// 3d Dino loading
	IND_3dMesh mMeshDino;
	if (!mI->MeshManager->Add (&mMeshDino, "..\\resources\\trex dx\\dino videogame.x", "..\\resources\\trex dx")) return 0;

	// Font
	IND_Font mFontSmall;
	if (!mI->FontManager->Add (&mFontSmall, "..\\resources\\font_small.png", "..\\resources\\font_small.xml", IND_ALPHA, IND_32)) return 0;

	// ----- Font creation -----

	IND_Entity2d mTextSmallWhite;					
	mI->Entity2dManager->Add		(&mTextSmallWhite);			// Entity adding
	mTextSmallWhite.SetFont			(&mFontSmall);				// Set the font into the entity
	mTextSmallWhite.SetLineSpacing	(18);
	mTextSmallWhite.SetCharSpacing	(-8);
	mTextSmallWhite.SetPosition		(5, 5, 1);
	mTextSmallWhite.SetAlign		(IND_LEFT);

	// ----- Set the mesh into 3d entity -----

	// Creating 3d entity
	IND_Entity3d mDino;					
	mI->Entity3dManager->Add (&mDino);						// Entity adding
	mDino.Set3dMesh (&mMeshDino);							// Set the 3d mesh into the entity

	// ----- Cameras ------

	IND_Camera2d mCamera2d (mI->Window->GetWidth () / 2, mI->Window->GetHeight() / 2);
	IND_Camera3d mCamera3d (0.0f, 0.0f, -2.0f);
	mCamera3d.SetAspect ((float) mI->Window->GetWidth () / mI->Window->GetHeight());

	// ----- Light -----

	IND_Light mLight0;
	mI->LightManager->Add (&mLight0, IND_AMBIENT_LIGHT);
	mLight0.SetColor (1.0f, 1.0f, 1.0f, 1.0f);

	// Light 1 (Direction light)
	IND_Light mLight1;
	mI->LightManager->Add (&mLight1, IND_DIRECTIONAL_LIGHT);
	mLight1.SetColor (1.0f, 1.0f, 1.0f, 1.0f);
	mLight1.SetDirection (0.0f, -0.3f, 0.5f);
	mLight1.SetRange (1000.0f);

	// Light 2 (Point light)
	IND_Light mLight2;
	mI->LightManager->Add (&mLight2, IND_POINT_LIGHT);
	mLight2.SetPosition (3, 3, 3);
	mLight2.SetColor (0.4f, 1.0f, 0.4f, 1.0f);
	mLight2.SetRange (200);
	mLight2.SetAttenuation (0.5f);

	// Light 3 (Spot light)
	IND_Light mLight3;
	mI->LightManager->Add (&mLight3, IND_SPOT_LIGHT);
	mLight3.SetPosition (5, 5, 5);
	mLight3.SetColor (1.0f, 1.0f, 1.0f, 1.0f);
	mLight3.SetDirection (0.0f, -0.3f, 0.5f);
	mLight3.SetRange (1000);
	mLight3.SetAttenuation (0.2f);
	mLight3.SetFalloff (1.0f);
	mLight3.SetPhi (8.0f);
	mLight3.SetTheta (7);

	// ----- Main Loop -----

	mI->LightManager->Disable(&mLight2);
	mI->LightManager->Disable(&mLight3);
	float	mAngle = 0;
	char	mText [2048]; mText [0] = 0;
	int		mSpeed = 25;
	float	mDelta;

	while (!mI->Input->OnKeyPress (IND_ESCAPE) && !mI->Input->Quit())
	{
		// ----- Input update ----

		mI->Input->Update ();

		// ----- Text -----

		strcpy (mText, "Press 1, 2 or 3 in order to toggle between different lights\n");
		mTextSmallWhite.SetText	 (mText);	

		// ----- Input -----

		mDelta = mI->Render->GetFrameTime() / 1000.0f;

		// Activate only light 1
		if (mI->Input->IsKeyPressed (IND_1))
		{
			mI->LightManager->Disable(&mLight2);
			mI->LightManager->Disable(&mLight3);
			mI->LightManager->Enable(&mLight1);
		}

		// Activate only light 2
		if (mI->Input->IsKeyPressed (IND_2))
		{
			mI->LightManager->Disable(&mLight1);
			mI->LightManager->Disable(&mLight3);
			mI->LightManager->Enable(&mLight2);
		}

		// Activate only light 3
		if (mI->Input->IsKeyPressed (IND_3))
		{
			mI->LightManager->Disable(&mLight1);
			mI->LightManager->Disable(&mLight2);
			mI->LightManager->Enable(&mLight3);
		}

		// ----- Updating entities attributes  -----

		mAngle += mDelta * mSpeed;
		mDino.SetAngleXYZ (0, mAngle, 0);

		// ----- Render  -----

		mI->Render->ClearViewPort (60, 60, 60);
		mI->Render->BeginScene ();

		mI->Render->SetViewPort3d (0, 0, mI->Window->GetWidth (), mI->Window->GetHeight());
		mI->Render->SetCamera3d (&mCamera3d);
		mI->LightManager->Update ();
		mI->Entity3dManager->RenderEntities3d ();

		mI->Render->SetViewPort2d (0, 0, mI->Window->GetWidth (), mI->Window->GetHeight ());
		mI->Render->SetCamera2d (&mCamera2d);
		mI->Entity2dManager->RenderEntities2d ();

		mI->Render->EndScene ();	
	}

	// ----- Free -----

	mI->End ();

	return 0;
}
