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
//#include "../dependencies/SpriterParser/MainlineKey.h"
//#include "../dependencies/SpriterParser/TimelineKey.h"


#include <list>
#include <vector>

// ----- Forward declarations -----
class IND_SurfaceManager;
class IND_ImageManager;
class IND_Image;
class IND_Timer;
class CollisionParser;
class IND_SpriterEntity;


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

	bool    init();
	void    end();
	bool    isOK() {
		return _ok;
	}

	// ----- Public methods -----
    
	bool addSpriterFile(vector<IND_SpriterEntity*> *pSpriterEntityList,const char *pSCMLFileName);
	bool remove(IND_SpriterEntity *pSen);


private:

	// ----- Private -----

	bool _ok;

	// ----- Enums -----


	// ----- Objects -----

	IND_ImageManager *_imageManager;

	// ----- Containers -----

	vector <IND_SpriterEntity *> *_listSpriterEntity;

	// ----- Private methods -----


	void        addToList(IND_SpriterEntity *pNewEntity);
	void        delFromlist(IND_SpriterEntity *pEn);
	IND_Image   *loadImage(char *pName);
	bool        remove(IND_SpriterEntity *pEn, bool pType);

	bool        parseSpriterData(vector<IND_SpriterEntity*> *pNewSpriterEntityList,const char *pSCMLFileName);

    int         toInt(const char* input);
    float       toFloat(const char* input);
	void        writeMessage();
	void        initVars();
	void        freeVars();
};

#endif // _IND_SPRITERMANAGER_
