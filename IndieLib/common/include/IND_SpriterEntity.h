/*****************************************************************************************
 * File: IND_SpriterEntity.h
 * Desc: Object that holds a spriter entity object.
 *****************************************************************************************/


/*
something about license here.....
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

	const char                  *_id;           // Entity ID
	const char                  *_name;         // Entity name
    map<Fileref*, IND_Image*>   *_images;       // map of images used in animations
    std::vector <Animation *>   *_animations;    // vector of animations
    
	// ----- Private sets ------
	


	// ----- Private methods -----
    void initAttrib();
    void addImage(const char *folderId, const char *fileId, IND_Image *pImage);
    Animation* addAnimation(int id, const char* name, int length, const char* looping, int loop_to);
    
	// ----- Friends -----
	friend class IND_SpriterManager;

};

#endif // _IND_SPRITERENTITY_
