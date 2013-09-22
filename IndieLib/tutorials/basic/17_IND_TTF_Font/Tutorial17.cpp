/*****************************************************************************************
 * Desc: Tutorials a) 17 IND_TTFFont
 *****************************************************************************************/

#include "CIndieLib.h"

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
	// Mouse pointer
	IND_Surface mMouseSurface;
	IND_Entity2d mMouseEntity;
	// Load the mouse pointer, it is loaded to and IndieLib surface
	if (!mI->SurfaceManager->Add (&mMouseSurface, "..\\resources\\cursor.png", IND_ALPHA, IND_32)) 
		return 0;
	
	// Add the Mouse entity to the IndieLib Entity Manager
	mI->Entity2dManager->Add (10, &mMouseEntity);
	mMouseEntity.SetSurface (&mMouseSurface);

	// Create a collision bounding rectangle for the mouse. It will be used in order to be able to select 
	// the editor elements (by checking if there is a collision
	mMouseEntity.SetBoundingRectangle ("cursor", 0, 0, 20, 20);
	mMouseEntity.SetHotSpot (0.0f, 0.0f);

		
	// Loading Background
	IND_Surface mSurfaceBack;
	if (!mI->SurfaceManager->Add (&mSurfaceBack, "..\\resources\\blue_background.jpg", IND_OPAQUE, IND_32)) return 0;

	// ----- Animations loading -----

	// Characters animations, we apply a color key of (0, 48, 152)
	IND_Animation mAnimationCharacter1;
	if (!mI->AnimationManager->AddToSurface (&mAnimationCharacter1, "..\\resources\\animations\\character1.xml", IND_ALPHA, IND_32, 0, 48, 152)) return 0;

	// Dust animation, we apply a color key of (255, 0, 255)
	IND_Animation mAnimationDust;
	if (!mI->AnimationManager->AddToSurface (&mAnimationDust, "..\\resources\\animations\\dust.xml", IND_ALPHA, IND_16, 255, 0, 255)) return 0;

	// ----- Set the surface and animations into 2d entities -----

	// Creating 2d entity for the background
	IND_Entity2d mBack;					
	mI->Entity2dManager->Add (&mBack);							// Entity adding
	mBack.SetSurface (&mSurfaceBack);							// Set the surface into the entity

	// Character 1
	IND_Entity2d mPlayer1;
	mI->Entity2dManager->Add (&mPlayer1);						// Entity adding
	mPlayer1.SetAnimation (&mAnimationCharacter1);				// Set the animation into the entity

	// Dust explosion
	IND_Entity2d mDust;
	mI->Entity2dManager->Add (&mDust);							// Entity adding
	mDust.SetAnimation (&mAnimationDust);						// Set the animation into the entity

	// ----- Changing the attributes of the 2d entities -----

	// Player 1
	mPlayer1.SetSequence (0);									// Choose sequence
	mPlayer1.SetPosition (140, 200, 0);
	mPlayer1.SetMirrorX (1);									// Horizontal mirroring
		
	// Dust explosion
	mDust.SetPosition (360, 250, 0);
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	//TTF font testing
	//simsun is a Chinese ttf font
	mI->TTFFontManager->AddFont("SimSun", "c:\\windows\\fonts\\simsun.ttc", 19, true);	
//	mI->TTFFontManager->SetFontColor("SimSun", RGBCOLOR(255,255,255));

	mI->TTFFontManager->AddFont("Arial", "C:\\WINDOWS\\Fonts\\arial.TTF", 16);
//	mI->TTFFontManager->SetFontColor("Arial", RGBCOLOR(255,255,255));
	mI->TTFFontManager->CacheFontString("Arial", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");	

	mI->TTFFontManager->AddFont("ArialBold", "C:\\WINDOWS\\Fonts\\arial.TTF", 16, true);
//	mI->TTFFontManager->SetFontColor("ArialBold", RGBCOLOR(255,255,255));
	mI->TTFFontManager->CacheFontString("ArialBold", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");	

	mI->TTFFontManager->AddFont("ArialItalic", "C:\\WINDOWS\\Fonts\\arial.TTF", 16, false, true);
//	mI->TTFFontManager->SetFontColor("ArialItalic", RGBCOLOR(255,255,255));
	mI->TTFFontManager->CacheFontString("ArialItalic", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");	

	mI->TTFFontManager->AddFont("ArialBoldItalic", "C:\\WINDOWS\\Fonts\\arial.TTF", 16, true, true);
//	mI->TTFFontManager->SetFontColor("ArialBoldItalic", RGBCOLOR(255,255,255));
	mI->TTFFontManager->CacheFontString("ArialBoldItalic", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");	
		
	mI->TTFFontManager->AddFont("Comic", "c:\\windows\\fonts\\comic.ttf", 30);	
//	mI->TTFFontManager->SetFontColor("Comic", RGBCOLOR(0,0,255));

	//Chinese text test
	std::wstring str = L"";
	str += L"李清照\n\n";
	str += L"寻寻觅觅 冷冷清清\n凄凄惨惨戚戚\n\n乍暖还寒时候 最难将息\n\n" ;
	str += L"三杯两盏淡酒 怎敌他\n晚来风急\n\n雁过也 正伤心\n却是旧时相识\n\n";
	str += L"满地黄花堆积 憔悴损\n如今有谁堪摘\n\n守著窗儿 独自怎生得黑\n\n";
	str += L"梧桐更兼细雨 到黄昏\n点点滴滴 这次第\n怎一个 愁字了得";

	// transparency
	mI->TTFFontManager->DrawText(1, "Comic", L"Blue Font 50% Transparency",200, 260, 
										RGBCOLOR(0,0,255),false, false,0, 128);
			
	// transparency & flipX
	mI->TTFFontManager->DrawText(2, "Comic", L"Mirrored MS Comic",300, 340, 
									RGBCOLOR(0,0,255),true, false, 0, 128);

	// rotated
	mI->TTFFontManager->DrawText(3, "ArialBold", L"Hey,don't piss me off (rotated text :-)",260, 400,
										RGBCOLOR(255,255,255),
										false, false, -15);



	mI->TTFFontManager->DrawTextEx(4, "SimSun", str,300, 20, 780, 260, 
								DT_EX_VERTICAL | DT_EX_LINEWRAP | DT_EX_RIGHT | DT_EX_TOP | DT_EX_BORDER | DT_EX_RTOLREADING| DT_EX_BACKCOLOR, 
								RGBCOLOR(0,255,0), 0, RGBCOLOR(255,255,255), 55, 55);

	
	std::wstring str1 = L"(Underline wraped text centered in both H & V)\nIndieLib is a c++ 2.5d engine for game development and fast game prototyping in a really easy way. Internally it uses Direct3d for hardware acceleration, but doesn't use DirectDraw or ID3DXSprite, it directly draws textures on polygons.";
	mI->TTFFontManager->DrawTextEx(5, "Arial", str1,
								20, 430, 600, 570, 
								DT_EX_LINEWRAP | DT_EX_CENTER | DT_EX_VCENTER | DT_EX_BORDER/* | DT_EX_RTOLREADING| DT_EX_BACKCOLOR*/, 
								RGBCOLOR(255,0,0), 0, RGBCOLOR(0,255,0), 55, 55, 
								false, false, 0, 255, true,true);

	std::wstring str2 = L"The quick brown fox\njumps over\nthe lazy dog";
	
	// Create and start the timer;
	IND_Timer mTimer;
	mTimer.Start();

	int iBlk;
	float xPos = 0.f;
	float yPos = 550.f;
	float mSpeed = 30.f;
	float mSpeed2 = 40.f;
	float mDelta;

	while (!mI->Input->OnKeyPress (IND_ESCAPE) && !mI->Input->Quit())
	{
		// Get the elapsed time of the last frame
		mDelta =  (float) mI->Render->GetFrameTime() / 1000.f;
		
		// ----- Input update ----

		mI->Input->Update ();

		mMouseEntity.SetPosition((float)mI->Input->GetMouseX(), (float)mI->Input->GetMouseY(), 1);

		// ----- Render  -----

		mI->Render->BeginScene ();
		mI->Render->ClearViewPort (60, 60, 60);
		mI->Entity2dManager->RenderEntities2d ();

		iBlk = (int)(mTimer.GetTicks() / 100.0f)  % 15;

		//iClk = (int)(mTimer.GetTicks() / 1000.0f);
		xPos += mSpeed * mDelta;

		if(xPos >= mI->Window->GetWidth())
			mSpeed = -30;
		else if (xPos <= 0)
			mSpeed = 30;
			
		//draw string		
		mI->TTFFontManager->DrawText(6, "Arial", xPos, 0, RGBCOLOR(0,iBlk * 255 / 15,iBlk * 255 / 15), false, false, 0, 255, false, false, L"FPS:%d", mI->Render->GetFpsInt());
		mI->TTFFontManager->DrawText(7, "ArialBold", 0, 20, RGBCOLOR(255,128,255),false, false, 0, 255 * iBlk /14, false, false,L"Bold FPS:%d", mI->Render->GetFpsInt());
		mI->TTFFontManager->DrawText(8, "ArialItalic", 0, 40, RGBCOLOR(128,255,128),false, false, 0, 255, false, false,L"Italic FPS:%d", mI->Render->GetFpsInt());
		mI->TTFFontManager->DrawText(9, "ArialBoldItalic", 0, 60, RGBCOLOR(255,255,128),false, false, 0, 255, false, true,L"Bold Italic FPS (Underline):%d", mI->Render->GetFpsInt());

		yPos -= mSpeed2 * mDelta;
		if(yPos < 300)
			yPos = 555;

		mI->TTFFontManager->DrawText(10, "ArialBold", str2, 610, (int)yPos, RGBCOLOR(255,255,255), false, false, 0, yPos - 300, false, false);


		//Added by Joel Gao
		//Render all texts
		mI->TTFFontManager->RenderAllTexts();


		mI->Entity2dManager->RenderEntities2d (10);
		mI->Render->EndScene ();	
	}

	// ----- Free -----

	mI->End ();

	return 0;
}
