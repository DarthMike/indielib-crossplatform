/*****************************************************************************************
/* Desc: Tutorial a) 14 3d Camera
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

	// Bones loading
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

	IND_Camera2d mCamera2d (mI->Window->GetWidth () / 2, mI->Window->GetHeight () / 2);
	IND_Camera3d mCamera3d (0.0f, 0.0f, -2.0f);
	mCamera3d.SetAspect ((float) mI->Window->GetWidth () / mI->Window->GetHeight());

	// ----- Light -----

	IND_Light mLight;
	mI->LightManager->Add (&mLight, IND_DIRECTIONAL_LIGHT);
	mLight.SetPosition (0, 0, 0);
	mLight.SetColor (1.0f, 1.0f, 1.0f, 1.0f);
	mLight.SetDirection (0.0f, -0.3f, 0.5f);
	mLight.SetRange (1000.0f);

	// ----- Main Loop -----

	float	mAngle = 0;
	float	mYaw = 0;
	char	mText [2048]; mText [0] = 0;
	float	mSpeedRotation = 50;
	float	mDelta;

	while (!mI->Input->OnKeyPress (IND_ESCAPE) && !mI->Input->Quit())
	{
		// ----- Input update ----

		mI->Input->Update ();

		// ----- Text -----

		strcpy (mText, "Use arrow keys to move\n");
		strcat (mText, "Use Z / X keys to strafe\n");
		mTextSmallWhite.SetText	 (mText);

		// ----- Input -----

		mDelta = mI->Render->GetFrameTime() / 1000.0f;

		if (mI->Input->IsKeyPressed (IND_KEYLEFT))	mYaw -= mSpeedRotation * mDelta;
		if (mI->Input->IsKeyPressed (IND_KEYRIGHT))	mYaw += mSpeedRotation * mDelta;
		if (mI->Input->IsKeyPressed (IND_KEYUP))	mCamera3d.MoveWalk (0.01f);
		if (mI->Input->IsKeyPressed (IND_KEYDOWN))	mCamera3d.MoveWalk (-0.01f);
		if (mI->Input->IsKeyPressed (IND_Z))		mCamera3d.MoveStrafe (-0.01f);
		if (mI->Input->IsKeyPressed (IND_X))		mCamera3d.MoveStrafe (0.01f);

		// ----- Updating entities attributes  -----

		mCamera3d.SetYaw (mYaw);

		// ----- Render  -----

		mI->Render->ClearViewPort (60, 60, 60);
		mI->Render->BeginScene ();

		mI->Render->SetViewPort3d (0, 0, mI->Window->GetWidth(), mI->Window->GetHeight());
		mI->Render->SetCamera3d (&mCamera3d);
		mI->LightManager->Update ();
		mI->Entity3dManager->RenderEntities3d ();

		mI->Render->SetViewPort2d (0, 0, mI->Window->GetWidth(), mI->Window->GetHeight());
		mI->Render->SetCamera2d (&mCamera2d);
		mI->Entity2dManager->RenderEntities2d ();

		mI->Render->EndScene ();	
	}

	// ----- Free -----

	mI->End ();

	return 0;
}
