/*****************************************************************************************
 * Desc: Tutorials a) 17 IND_TTFFont
 *****************************************************************************************/

#include "CIndieLib.h"
#include "IND_Entity2d.h"
#include "IND_Surface.h"
#include "IND_Animation.h"
#include "IND_Timer.h"
#include "../../WorkingPath.h"

/*
==================
Main
==================
*/
Indielib_Main			
{
    
    //Sets the working path at the resources directory. Resources paths are relative to that directory
	if (!WorkingPathSetup::setWorkingPath(WorkingPathSetup::resourcesDirectory())) {
		std::cout<<"\nUnable to Set the working path !";
    }
        
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init ()) return 0;

	// ----- Surface loading -----

        // Mouse pointer
	IND_Surface *mMouseSurface = IND_Surface::newSurface();
	IND_Entity2d *mMouseEntity = IND_Entity2d::newEntity2d();
	// Load the mouse pointer, it is loaded to and IndieLib surface
	if (!mI->_surfaceManager->add(mMouseSurface, "cursor.png", IND_ALPHA, IND_32))
		return 0;
	
	// Add the Mouse entity to the IndieLib Entity Manager
	mI->_entity2dManager->add(10, mMouseEntity);
	mMouseEntity->setSurface(mMouseSurface);

	// Create a collision bounding rectangle for the mouse. It will be used in order to be able to select 
	// the editor elements (by checking if there is a collision
	mMouseEntity->setBoundingRectangle("cursor", 0, 0, 20, 20);
	mMouseEntity->setHotSpot(0.0f, 0.0f);

		
	// Loading Background
	IND_Surface *mSurfaceBack = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBack, "blue_background.jpg", IND_OPAQUE, IND_32)) return 0;

	// ----- Animations loading -----

	// Characters animations, we apply a color key of (0, 48, 152)
	IND_Animation *mAnimationCharacter1 = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationCharacter1, "animations/character1.xml", IND_ALPHA, IND_32, 0, 48, 152)) return 0;

	// Dust animation, we apply a color key of (255, 0, 255)
	IND_Animation *mAnimationDust = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationDust, "animations/dust.xml", IND_ALPHA, IND_16, 255, 0, 255)) return 0;

	// ----- Set the surface and animations into 2d entities -----

	// Creating 2d entity for the background
	IND_Entity2d *mBack = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mBack);							// Entity adding
	mBack->setSurface(mSurfaceBack);							// Set the surface into the entity

	// Character 1
	IND_Entity2d *mPlayer1 = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mPlayer1);						// Entity adding
	mPlayer1->setAnimation(mAnimationCharacter1);				// Set the animation into the entity

	// Dust explosion
	IND_Entity2d *mDust = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mDust);							// Entity adding
	mDust->setAnimation(mAnimationDust);						// Set the animation into the entity

	// ----- Changing the attributes of the 2d entities -----

	// Player 1
	mPlayer1->setSequence(0);									// Choose sequence
	mPlayer1->setPosition(140, 200, 0);
	mPlayer1->setMirrorX(1);									// Horizontal mirroring
		
	// Dust explosion
	mDust->setPosition(360, 250, 0);


    mI->_TTFFontManager->addFont("Tangerine", "font/ttf/Tangerine/Tangerine_Regular.ttf", 48, true, true );
    //mI->_TTFFontManager->SetFontColor("Tangerine", RGBCOLOR(255,255,255));
    mI->_TTFFontManager->CacheFontString("Tangerine", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");
    mI->_TTFFontManager->drawText(1, "Tangerine", L"Tangerine Font",250, 40, RGBCOLOR(255,255,255),false, true,0, 255);

    
    mI->_TTFFontManager->addFont("SpicyRice", "font/ttf/spicy-rice/SpicyRice-Regular.otf", 32, false, false );
    //mI->_TTFFontManager->SetFontColor("SpicyRice", RGBCOLOR(255,255,255));
    mI->_TTFFontManager->CacheFontString("SpicyRice", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");
    mI->_TTFFontManager->drawText(2, "SpicyRice", L"SpicyRice Font",250, 240, RGBCOLOR(255,255,255),false, true,0, 255);
        
        
    mI->_TTFFontManager->addFont("droid-serif", "font/ttf/droid-serif/DroidSerif.ttf", 16, false, false );
    //mI->_TTFFontManager->SetFontColor("droid-serif", RGBCOLOR(255,255,255));
    mI->_TTFFontManager->CacheFontString("droid-serif", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");
    mI->_TTFFontManager->drawText(3, "droid-serif", L"droid-serif Font\nmulti-line\ntext",50, 300, RGBCOLOR(255,255,255),false, true,0, 255);
        
    mI->_TTFFontManager->addFont("ostrich", "font/ttf/ostrich-sans/ostrich-regular.ttf", 32, false, false );
    //mI->_TTFFontManager->SetFontColor("ostrich", RGBCOLOR(255,255,255));
    mI->_TTFFontManager->CacheFontString("ostrich", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");
    mI->_TTFFontManager->drawText(4, "ostrich", L"ostrich Font\nmulti-line\ntext",500, 300, RGBCOLOR(255,255,255),false, true,0, 255);


    mI->_TTFFontManager->addFont("alex-brush", "font/ttf/alex-brush/AlexBrush-Regular.ttf", 32, false, false );
    //mI->_TTFFontManager->SetFontColor("alex-brush", RGBCOLOR(255,255,255));
    mI->_TTFFontManager->CacheFontString("alex-brush", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");
    mI->_TTFFontManager->drawText(5, "alex-brush", L"alex-brush Font\nmulti-line\ntext",300, 425, RGBCOLOR(255,255,255),false, true,0, 255);
    
        
        
	//////////////////////////////////////////////////////////////////////////////////////////////
	//TTF font testing
	//simsun is a Chinese ttf font
//	mI->_TTFFontManager->addFont("SimSun", "c:\\windows\\fonts\\simsun.ttc", 19, true);
	//mI->_TTFFontManager->SetFontColor("SimSun", RGBCOLOR(255,255,255));

//	mI->_TTFFontManager->addFont("Arial", "C:\\WINDOWS\\Fonts\\arial.TTF", 16);
	//mI->_TTFFontManager->SetFontColor("Arial", RGBCOLOR(255,255,255));
//	mI->_TTFFontManager->CacheFontString("Arial", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

//	mI->_TTFFontManager->addFont("ArialBold", "C:\\WINDOWS\\Fonts\\arial.TTF", 16, true);
	//mI->_TTFFontManager->SetFontColor("ArialBold", RGBCOLOR(255,255,255));
//	mI->_TTFFontManager->CacheFontString("ArialBold", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

//	mI->_TTFFontManager->addFont("ArialItalic", "C:\\WINDOWS\\Fonts\\arial.TTF", 16, false, true);
	//mI->_TTFFontManager->SetFontColor("ArialItalic", RGBCOLOR(255,255,255));
//	mI->_TTFFontManager->CacheFontString("ArialItalic", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

//	mI->_TTFFontManager->addFont("ArialBoldItalic", "C:\\WINDOWS\\Fonts\\arial.TTF", 16, true, true);
	//mI->_TTFFontManager->SetFontColor("ArialBoldItalic", RGBCOLOR(255,255,255));
//	mI->_TTFFontManager->CacheFontString("ArialBoldItalic", L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
		
//	mI->_TTFFontManager->addFont("Comic", "c:\\windows\\fonts\\comic.ttf", 30);
	//mI->_TTFFontManager->SetFontColor("Comic", RGBCOLOR(0,0,255));

	//Chinese text test
	std::wstring str = L"";
    str += L"this is a test";
        
    // original text I can't reproduce hehe (and it won't compile .. )
	//str += L"李清照\n\n";
	//str += L"寻寻觅觅 冷冷清清\n凄凄惨惨戚戚\n\n乍暖还寒时候 最难将息\n\n" ;
	//str += L"三杯两盏淡酒 怎敌他\n晚来风急\n\n雁过也 正伤心\n却是旧时相识\n\n";
	//str += L"满地黄花堆积 憔悴损\n如今有谁堪摘\n\n守著窗儿 独自怎生得黑\n\n";
	//str += L"梧桐更兼细雨 到黄昏\n点点滴滴 这次第\n怎一个 愁字了得";

	// transparency
//	mI->_TTFFontManager->drawText(1, "Comic", L"Blue Font 50% Transparency",200, 260,
//										RGBCOLOR(0,0,255),false, false,0, 128);
			
	// transparency & flipX
//	mI->_TTFFontManager->drawText(2, "Comic", L"Mirrored MS Comic",300, 340,
//									RGBCOLOR(0,0,255),true, false, 0, 128);

	// rotated
//	mI->_TTFFontManager->drawText(3, "ArialBold", L"Hey,don't piss me off (rotated text :-)",260, 400,
//										RGBCOLOR(255,255,255),
//										false, false, -15);



//	mI->_TTFFontManager->drawTextEx(4, "SimSun", str,300, 20, 780, 260,
//								DT_EX_VERTICAL | DT_EX_LINEWRAP | DT_EX_RIGHT | DT_EX_TOP | DT_EX_BORDER | DT_EX_RTOLREADING| DT_EX_BACKCOLOR,
//								RGBCOLOR(0,255,0), 0, RGBCOLOR(255,255,255), 55, 55);

	
	std::wstring str1 = L"(Underline wraped text centered in both H & V)\nIndieLib is a c++ 2.5d engine for game development and fast game prototyping in a really easy way. Internally it uses Direct3d for hardware acceleration, but doesn't use DirectDraw or ID3DXSprite, it directly draws textures on polygons.";
//	mI->_TTFFontManager->drawTextEx(5, "Arial", str1,
//								20, 430, 600, 570,
//								DT_EX_LINEWRAP | DT_EX_CENTER | DT_EX_VCENTER | DT_EX_BORDER/* | DT_EX_RTOLREADING| DT_EX_BACKCOLOR*/,
//								RGBCOLOR(255,0,0), 0, RGBCOLOR(0,255,0), 55, 55,
//								false, false, 0, 255, true,true);

	std::wstring str2 = L"The quick brown fox\njumps over\nthe lazy dog";
	
	// Create and start the timer;
	IND_Timer *mTimer =  new IND_Timer();
	mTimer->start();

	int iBlk;
	float xPos = 0.f;
	float yPos = 550.f;
	float mSpeed = 30.f;
	float mSpeed2 = 40.f;
	float mDelta;

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// Get the elapsed time of the last frame
		mDelta =  (float) mI->_render->getFrameTime() / 1000.f;
		
		// ----- Input update ----

		mI->_input->update();

		mMouseEntity->setPosition((float)mI->_input->getMouseX(), (float)mI->_input->getMouseY(), 1);

		// ----- Render  -----

		mI->_render->beginScene();
		mI->_render->clearViewPort(60, 60, 60);
		mI->_entity2dManager->renderEntities2d();

		iBlk = (int)(mTimer->getTicks() / 100.0f)  % 15;

		//iClk = (int)(mTimer->getTicks() / 1000.0f);
		xPos += mSpeed * mDelta;

		if(xPos >= mI->_window->getWidth())
			mSpeed = -30;
		else if (xPos <= 0)
			mSpeed = 30;
			
		//draw string		
//		mI->_TTFFontManager->drawText(6, "Arial", xPos, 0, RGBCOLOR(0,iBlk * 255 / 15,iBlk * 255 / 15), false, false, 0, 255, false, false, L"FPS:%d", mI->_render->getFpsInt());
//		mI->_TTFFontManager->drawText(7, "ArialBold", 0, 20, RGBCOLOR(255,128,255),false, false, 0, 255 * iBlk /14, false, false,L"Bold FPS:%d", mI->_render->getFpsInt());
//		mI->_TTFFontManager->drawText(8, "ArialItalic", 0, 40, RGBCOLOR(128,255,128),false, false, 0, 255, false, false,L"Italic FPS:%d", mI->_render->getFpsInt());
//		mI->_TTFFontManager->drawText(9, "ArialBoldItalic", 0, 60, RGBCOLOR(255,255,128),false, false, 0, 255, false, true,L"Bold Italic FPS (Underline):%d", mI->_render->getFpsInt());

//		yPos -= mSpeed2 * mDelta;
//		if(yPos < 300)
//			yPos = 555;

//		mI->_TTFFontManager->drawText(10, "ArialBold", str2, 610, (int)yPos, RGBCOLOR(255,255,255), false, false, 0, yPos - 300, false, false);


		//Render all texts
		mI->_TTFFontManager->renderAllTexts();


		mI->_entity2dManager->renderEntities2d(10);
		mI->_render->endScene();
	}

	// ----- Free -----

	mI->end();

	return 0;
}
