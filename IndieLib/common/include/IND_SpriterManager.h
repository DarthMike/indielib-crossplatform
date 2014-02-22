/*****************************************************************************************
 * File: IND_SpriterManager.h
 * Desc: Spriter manager
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


#ifndef _IND_SPRITERMANAGER_
#define _IND_SPRITERMANAGER_

// ----- Includes -----

#include "../dependencies/SpriterParser/Animation.h"
#include "../dependencies/SpriterParser/Mainline.h"
#include "../dependencies/SpriterParser/Timeline.h"

#include <list>
#include <vector>

// ----- Forward declarations -----

class IND_SurfaceManager;
class IND_Image;
class IND_Timer;
class CollisionParser;
class IND_SpriterEntity;
class IND_Render;
class IND_Surface;
class IND_Timer;


// --------------------------------------------------------------------------------
//									IND_SpriterManager
// --------------------------------------------------------------------------------

/**
@defgroup IND_SpriterManager IND_SpriterManager
@ingroup Managers
Manager of IND_SpriterEntity objects. Click in ::IND_SpriterManager to see all the methods of this class.
*/
/**@{*/

/**
TODO: Describtion.
*/

class LIB_EXP IND_SpriterManager {
public:

	// ----- Init/End -----

	IND_SpriterManager(): _ok(false)  { }
	~IND_SpriterManager()              {
		end();
	}

	bool    init(IND_SurfaceManager *pSurfaceManager, IND_Render *pRender);
	void    end();
    //! True if object initialized correctly, false otherwise
	bool    isOK() {
		return _ok;
	}

	// ----- Public methods -----
    
	bool addSpriterFile(const char *pSCMLFileName);
	bool remove(IND_SpriterEntity *pSen);

    //! Get the list of managed entities
    vector <IND_SpriterEntity *>* getEntities() {
        return _listSpriterEntity;
    }

    void renderEntities();
    

private:
    /** @cond DOCUMENT_PRIVATEAPI */
	
    // ----- Private -----

    bool _ok;
    double _deltaTime;
    double _lastTime;

	// ----- Enums -----

	// ----- Objects -----
    
    IND_Render * _render;
	IND_SurfaceManager *_surfaceManager;
    IND_Timer * _timer;

	// ----- Containers -----

	vector <IND_SpriterEntity *> *_listSpriterEntity;

	// ----- Private methods -----

	void        addToList(IND_SpriterEntity *pNewEntity);
	void        delFromlist(IND_SpriterEntity *pEn);
	IND_Image*  loadImage(char *pName);
	bool        remove(IND_SpriterEntity *pEn, bool pType);

    // ----- Render methods -----

    void        draw(IND_SpriterEntity *ent);
    void        drawTransientObject(IND_SpriterEntity *ent, MainlineObjectref *mObjectref);
    void        drawPersistentObject(IND_SpriterEntity *ent, MainlineObjectref *mObjectref);
    void        drawBone(IND_SpriterEntity *ent, MainlineObjectref *mObjectref);
    
    void        updateCurrentTime(IND_SpriterEntity *ent, double deltaTime);
    void        updateCurrentKey(IND_SpriterEntity *ent);
    TimelineObject* getTimelineObject(IND_SpriterEntity *ent,int timelineId, int keyId);
    IND_Surface*    getSurface(IND_SpriterEntity *ent, int folderId, int fileId);
       
    // ----- Parser methods -----

	bool        parseSpriterData(const char *pSCMLFileName);
    int         toInt(const char* input);
    float       toFloat(const char* input);
	
    void        writeMessage();
	void        initVars();
	void        freeVars();

	/** @endcond */
};
/**@}*/

#endif // _IND_SPRITERMANAGER_
