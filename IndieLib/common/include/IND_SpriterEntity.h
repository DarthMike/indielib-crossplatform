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
#include <map>

// ----- Forward declarations -----


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
	

private:
	IND_SpriterEntity();
	virtual ~IND_SpriterEntity();

	// ----- Containers -----

	
	
	// ----- Structures ------

	typedef std::pair<int, int> Fileref;	// Filref consist of a (folder, file) combo.

    map<Fileref, IND_Image *> *_images;		// list of images used in animations
	const char *_id;						// Entity ID
	const char *_name;						// Entity name

    
	// ----- Private sets ------
	


	// ----- Private methods -----
    void initAttrib();
    
	// ----- Friends -----
	friend class IND_SpriterManager;

};

#endif // _IND_SPRITERENTITY_
