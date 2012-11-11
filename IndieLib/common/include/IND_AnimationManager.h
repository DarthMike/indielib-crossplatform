/*****************************************************************************************
 * File: IND_AnimationManager.h
 * Desc: Animation manager
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (javierlopezpro@gmail.com)

This library is free software; you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef _IND_ANIMATIONMANAGER_
#define _IND_ANIMATIONMANAGER_

// ----- Includes -----

#include <list>
#include <vector>

// ----- Forward declarations -----
class IND_SurfaceManager;
class IND_ImageManager;
class IND_Image;
class IND_Timer;
class CollisionParser;
class IND_Animation;


// --------------------------------------------------------------------------------
//									IND_AnimationManager
// --------------------------------------------------------------------------------

/**
@defgroup IND_AnimationManager IND_AnimationManager
@ingroup Managers
Manager of ::IND_Animation objects. Click in ::IND_AnimationManager to see all the methods of this class.
*/
/**@{*/

/**
Manager of ::IND_Animation objects. Used for storing these type of objects.
*/
class LIB_EXP IND_AnimationManager {
public:

	// ----- Init/End -----

	IND_AnimationManager(): _ok(false)  { }
	~IND_AnimationManager()              {
		end();
	}

	bool    init(IND_ImageManager *pImageManager, IND_SurfaceManager *pSurfaceManager);
	void    end();
    
	bool    isOK();

	// ----- Public methods -----

	bool addToImage(IND_Animation *pNewAnimation, const char *pAnimation);

	// ----- Not specifying block size -----

	bool addToSurface(IND_Animation *pNewAnimation,
	                  const char *pAnimation,
	                  IND_Type pType,
	                  IND_Quality pQuality);

	bool addToSurface(IND_Animation *pNewAnimation,
	                  const char *pAnimation,
	                  IND_Type pType,
	                  IND_Quality pQuality,
	                  BYTE pR,
	                  BYTE pG,
	                  BYTE pB);

	// ----- Specifying block size -----

	bool addToSurface(IND_Animation *pNewAnimation,
	                  const char *pAnimation,
	                  int pBlockSize,
	                  IND_Type pType,
	                  IND_Quality pQuality);

	bool addToSurface(IND_Animation *pNewAnimation,
	                  const char *pAnimation,
	                  int pBlockSize,
	                  IND_Type pType,
	                  IND_Quality pQuality,
	                  BYTE pR,
	                  BYTE pG,
	                  BYTE pB);

	bool remove(IND_Animation *pAn);


private:
	/** @cond DOCUMENT_PRIVATEAPI */
	// ----- Private -----

	bool _ok;

	// ----- Enums -----

	enum {
		FRAMES,
		FRAME_,
		PATH,
		NAME,
		FILE,
		OFFSET_X,
		OFFSET_Y,
		SEQUENCES,
		TIME,
		OBRACKET,
		CBRACKET,
		KW_UNKNOWN
	};

	// ----- Objects -----

	IND_ImageManager *_imageManager;
	IND_SurfaceManager *_surfaceManager;
	CollisionParser *_collisionParser;

	// ----- Containers -----

	list <IND_Animation *> *_listAnimations;

	// ----- Private methods -----

	bool calculateAxis(IND_Animation *pAn,
	                 int pSequence,
	                 float pAxisX,
	                 float pAxisY,
	                 int *pAxisCalX,
	                 int *pAxisCalY);

	void        addToList(IND_Animation *pNewAnimation);
	void        delFromlist(IND_Animation *pAn);
	IND_Image   *loadImage(const char *pName);
	bool        remove(IND_Animation *pAn, bool pType);

	bool        parseAnimation(IND_Animation *pNewAnimation, const char *pAnimationName);
	bool        isDeclaredFrame(const char *pFrameName, IND_Animation *pNewAnimation, int *pPos);

	void        writeMessage();
	void        initVars();
	void        freeVars();
    /** @endcond */
};
/**@}*/

#endif // _IND_ANIMATIONMANAGER_
