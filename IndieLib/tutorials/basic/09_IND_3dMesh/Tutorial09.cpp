/*****************************************************************************************
/* Desc: Tutorials a) 09 3d Mesh
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

	// Loading Background
	IND_Surface mSurfaceBack;
	if (!mI->SurfaceManager->Add (&mSurfaceBack, "..\\resources\\twist.jpg", IND_OPAQUE, IND_32)) return 0;

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

	// ----- 3d Mesh loading -----

	// Bones loading
	IND_3dMesh mMeshBones;
	if (!mI->MeshManager->Add (&mMeshBones, "..\\resources\\skeleton\\skeleton.x", "..\\resources\\skeleton")) return 0;

	// ----- Set the surfaces into 2d entities -----

	// Creating 2d entity for the background
	IND_Entity2d mBack;					
	mI->Entity2dManager->Add (&mBack);						// Entity adding
	mBack.SetSurface (&mSurfaceBack);						// Set the surface into the entity

	// ----- Set the mesh into 3d entities -----

	// Creating 3d entity
	IND_Entity3d mBones;					
	mI->Entity3dManager->Add (&mBones);						// Entity adding
	mBones.Set3dMesh (&mMeshBones);							// Set the 3d mesh into the entity
	
	// ----- Changing the attributes of the 2d entities -----

	mBack.SetHotSpot (0.5f, 0.5f);
	mBack.SetPosition (400, 300, 0);
	mBack.SetScale (1.7f, 1.7f);

	// ----- Changing the attributes of the 3d entities -----

	mBones.SetPosition (0, 0, 0);
	mBones.SetScale (1.5f, 1.5f, 1.5f);

	// ----- Cameras ------

	IND_Camera2d mCamera2d (mI->Window->GetWidth () / 2, mI->Window->GetHeight() / 2);
	IND_Camera3d mCamera3d (0.0f, 1.4f, -4.0f);
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
	int		mSequence = 0;
	char	mText [2048]; mText [0] = 0;
	int		mSpeed = 25;
	float	mDelta;

	while (!mI->Input->OnKeyPress (IND_ESCAPE) && !mI->Input->Quit())
	{
		// ----- Input update ----

		mI->Input->Update ();

		// ----- Text -----

		strcpy (mText, "Use up and down arrow keys to toggle the animation\n");
		mTextSmallWhite.SetText	 (mText);

		// ----- Input -----

		mDelta = mI->Render->GetFrameTime() / 1000.0f;

		// Change animation sequence
		if (mI->Input->OnKeyPress (IND_KEYUP))
		{
			mSequence++;
			if (mSequence == mMeshBones.GetNumSequences()) mSequence = 0;
			mBones.SetSequence (mSequence);
		}

		// Change animation sequence
		if (mI->Input->OnKeyPress (IND_KEYDOWN))
		{
			mSequence--;
			if (mSequence < 0) mSequence = mMeshBones.GetNumSequences() - 1;
			mBones.SetSequence (mSequence);
		}

		mAngle += mDelta * mSpeed;

		// ----- Updating entities attributes  -----

		// Back
		mBack.SetAngleXYZ (0, 0, mAngle);	

		// Skeleton
		mBones.SetAngleXYZ (0, mAngle, 0);

		// ----- Render  -----

		mI->Render->BeginScene ();
		mI->Render->SetViewPort2d (0, 0, 800, 600);
		mI->Render->SetCamera2d (&mCamera2d);
		mI->Entity2dManager->RenderEntities2d ();

		mI->Render->SetViewPort3d (0, 0, 800, 600);
		mI->Render->SetCamera3d (&mCamera3d);
		mI->LightManager->Update ();
		mI->Entity3dManager->RenderEntities3d ();

		mI->Render->EndScene ();	
	}

	// ----- Free -----

	mI->End ();

	return 0;
}

