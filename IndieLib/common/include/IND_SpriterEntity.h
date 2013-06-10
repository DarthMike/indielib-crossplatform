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
#include "IND_Image.h"
#include "../dependencies/SpriterParser/Animation.h"
#include "../dependencies/SpriterParser/Mainline.h"
#include "../dependencies/SpriterParser/Timeline.h"
#include <map>
#include <vector>

// ----- Forward declarations -----

typedef std::pair<unsigned int, unsigned int> Fileref;	// Filref consist of a (folder, file) combo.

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

    
	const char                  *_id;                   // Entity ID
	const char                  *_name;                 // Entity name
    map<Fileref*, IND_Image*>   *_images;               // map of images used in animations
    std::vector <Animation *>   *_animations;           // vector of animations
    
    int                         _currentAnimation;      // current animation playing
    int                         _currentKey;            // current key of animation playing
    int                         _currentTime;           // current time of the animation
    
    bool                        _drawBones;             // TODO: support this in a later version
    bool                        _drawObjectpositions;   // TODO: support this in a later version


	// ----- Private sets ------
	


	// ----- Private methods -----
    
    void drawTransientObject(float x, float y, float angle, float scale_x, float scale_y);
    void drawPersistentObject(float x, float y, float angle, float scale_x, float scale_y);
    void drawBone(float x, float y, float angle, float scale_x, float scale_y);             // TODO: support this in a later version
    
    void initAttrib();
    void addImage(const char *folderId, const char *fileId, IND_Image *pImage);
    Animation* addAnimation(int id, const char* name, int length, const char* looping, int loop_to);
    
	// ----- Friends -----

	
    friend class IND_SpriterManager;

};

#endif // _IND_SPRITERENTITY_
