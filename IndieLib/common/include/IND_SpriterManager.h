/*****************************************************************************************
 * File: IND_SpriterManager.h
 * Desc: Spriter manager
 *****************************************************************************************/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */


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

class LIB_EXP IND_SpriterManager {
public:

	// ----- Init/End -----

	IND_SpriterManager(): _ok(false)  { }
	~IND_SpriterManager()              {
		end();
	}

	bool    init(IND_SurfaceManager *pSurfaceManager, IND_Render *pRender);
	void    end();
	bool    isOK() {
		return _ok;
	}

	// ----- Public methods -----
    
	bool addSpriterFile(const char *pSCMLFileName);
	bool remove(IND_SpriterEntity *pSen);
    
    vector <IND_SpriterEntity *>* getEntities() {
        return _listSpriterEntity;
    }

    void renderEntities();
    

private:

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

    // ----- render methods -----
    void        draw(IND_SpriterEntity *ent);
    void        drawTransientObject(IND_SpriterEntity *ent, MainlineObjectref *mObjectref);
    void        drawPersistentObject(IND_SpriterEntity *ent, MainlineObjectref *mObjectref);
    void        drawBone(IND_SpriterEntity *ent, MainlineObjectref *mObjectref);
    
    void        updateCurrentTime(IND_SpriterEntity *ent, double deltaTime);
    void        updateCurrentKey(IND_SpriterEntity *ent);
    TimelineObject* getTimelineObject(IND_SpriterEntity *ent,int timelineId, int keyId);
    IND_Surface*    getSurface(IND_SpriterEntity *ent, int folderId, int fileId);
    
    
    // ----- parser methods -----
	bool        parseSpriterData(const char *pSCMLFileName);
    int         toInt(const char* input);
    float       toFloat(const char* input);
	
    void        writeMessage();
	void        initVars();
	void        freeVars();
};

#endif // _IND_SPRITERMANAGER_
