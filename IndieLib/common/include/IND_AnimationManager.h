/*****************************************************************************************
 * File: IND_AnimationManager.h
 * Desc: Animation manager
 *****************************************************************************************/

/*********************************** The zlib License ************************************
 *
 * Copyright (c) 2013 Indielib-crossplatform Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 *****************************************************************************************/


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
	                  unsigned char pR,
	                  unsigned char pG,
	                  unsigned char pB);

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
	                  unsigned char pR,
	                  unsigned char pG,
	                  unsigned char pB);

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
