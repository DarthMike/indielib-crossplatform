/*****************************************************************************************
 * File: IND_SpriterEntity.h
 * Desc: Object that holds a spriter entity object.
 *****************************************************************************************/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */

#ifndef _IND_SPRITERENTITY_
#define _IND_SPRITERENTITY_

// ----- Includes -----

#include "IND_object.h"
#include "IND_Surface.h"
#include "IND_Render.h"
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



//typedef std::pair<unsigned int, unsigned int> Fileref;	// Filref consist of a (folder, file) combo.



// --------------------------------------------------------------------------------
//									IND_SpriterEntity
// --------------------------------------------------------------------------------

class LIB_EXP IND_SpriterEntity : public IND_Object {
public:

	// ----- Init -----

    static IND_SpriterEntity* newSpriterEntity();

    void destroy();


	// ----- Public methods ------
    
    void playAnimation(int animation, IND_Render *render); // TODO maybe input parameter animationname instead??
    void update(int deltaTime);
    void draw(float x, float y, float angle, float scale_x, float scale_y);
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
    int                         _currentTime;           // current time of the animation
    
    bool                        _drawBones;             // TODO: support this in a later version
    bool                        _drawObjectpositions;   // TODO: support this in a later version
    
    IND_Render                  *_render;               // render where the direct blitting is handled


	// ----- Private sets ------
	


	// ----- Private methods -----
    
    void drawTransientObject(float x, float y, float angle, float scale_x, float scale_y,  MainlineObjectref *mObjectref);
    void drawPersistentObject(float x, float y, float angle, float scale_x, float scale_y);
    void drawBone(float x, float y, float angle, float scale_x, float scale_y);             // TODO: support this in a later version
    
    TimelineObject* getTimelineObject(int timelineId, int keyId);
    IND_Surface* getSurface(int folderId, int fileId);
    
    void initAttrib();
    void addSurface(int folderId, int fileId, IND_Surface *pSurface);
    Animation* addAnimation(int id, const char* name, int length, const char* looping, int loop_to);
    
	// ----- Friends -----

	
    friend class IND_SpriterManager;

};

#endif // _IND_SPRITERENTITY_
