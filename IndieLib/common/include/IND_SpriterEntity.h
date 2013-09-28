/*****************************************************************************************
 * File: IND_SpriterEntity.h
 * Desc: Object that holds a spriter entity object.
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


#ifndef _IND_SPRITERENTITY_
#define _IND_SPRITERENTITY_

// ----- Includes -----

#include "IND_Object.h"
#include "IND_Surface.h"
#include "../dependencies/SpriterParser/Animation.h"
#include "../dependencies/SpriterParser/Mainline.h"
#include "../dependencies/SpriterParser/Timeline.h"
#include <map>
#include <vector>

// ----- Forward declarations -----


struct Fileref {
    unsigned int folderId;
    unsigned int fileId;
    
    Fileref(unsigned int folderid, unsigned int fileid) {
        folderId = folderid;
        fileId = fileid;
    }
    
    // Provide a "<" operator that orders keys.
    // The way it orders them doesn't matter, all that matters is that
    // it orders them consistently.
    bool operator<(Fileref const& other) const {
        if (folderId < other.folderId) return true; else
            if (folderId == other.folderId) {
                if (fileId < other.fileId) return true; else
                    if (fileId == other.fileId) {
                        // We should never reach this point
                        
                    }
            }
        
        return false;
    }
};



// --------------------------------------------------------------------------------
//									IND_SpriterEntity
// --------------------------------------------------------------------------------

class LIB_EXP IND_SpriterEntity : public IND_Object {
public:

	// ----- Init -----

    static IND_SpriterEntity* newSpriterEntity();

    void destroy();


	// ----- Public methods ------
    
    void playAnimation(int animation); // TODO maybe input parameter animationname instead??
    void stopAnimation();

	// ----- Public gets ------

	
    const char* getId() {
        return _id;
    }
	
    const char* getName() {
        return _name;
    }
    
     std::vector <Animation *>* getAnimations() {
         return _animations;
     }
	

private:
	
    IND_SpriterEntity();
	virtual ~IND_SpriterEntity();

	// ----- Containers -----

	
	
	// ----- Structures ------

    typedef map <Fileref, IND_Surface*> SurfaceToFileMap;
    
	const char                  *_id;                   // Entity ID
	const char                  *_name;                 // Entity name
    SurfaceToFileMap            *_surfaces;             // map of surfaces used in animations
    std::vector <Animation *>   *_animations;           // vector of animations
    
    int                         _currentAnimation;      // current animation playing
    int                         _currentKey;            // current key of animation playing
    double                         _currentTime;           // current time of the animation
    
    bool                        _drawBones;             // TODO: support this in a later version
    bool                        _drawObjectpositions;   // TODO: support this in a later version


	// ----- Private sets ------
	


	// ----- Private methods -----
    
    
    TimelineObject* getTimelineObject(int timelineId, int keyId);
    IND_Surface* getSurface(int folderId, int fileId);
    
    void initAttrib();
    void addSurface(int folderId, int fileId, IND_Surface *pSurface);
    Animation* addAnimation(int id, const char* name, int length, const char* looping, int loop_to);
    
	// ----- Friends -----

	
    friend class IND_SpriterManager;

};

#endif // _IND_SPRITERENTITY_
