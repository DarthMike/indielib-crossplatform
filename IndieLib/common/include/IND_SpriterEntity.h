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



// ----- Forward declarations -----


// --------------------------------------------------------------------------------
//									IND_SpriterEntity
// --------------------------------------------------------------------------------

class LIB_EXP IND_SpriterEntity {
public:

	// ----- Public methods ------

	// ----- Public gets ------

private:


	// ----- Containers -----

	//list <IND_Image *> *_listImages;
	
	// ----- Structures ------

	struct structSpriterEntity {
		int  *_id;		
		char *_name;                                 // Entity name
		list <IND_Image *> *_listImage;            // images referenced by animations
		//vector <SpriterAnimation *> *_listAnimation; // Entity animation list
		
		structSpriterEntity() { 
			_id = 0;
			_name = new char [128];
			_listImage = new list <IND_Image *>;
			//_listAnimation(NULL) {}
		}
		
		~structSpriterEntity() {
			DISPOSEARRAY(_listImage);
		}
		
	};
	typedef struct structSpriterEntity SPRITERENTITY;

	SPRITERENTITY _spriterEntity;



	// ----- Private sets ------

	// ----- Private methods -----

	// ----- Friends -----

};

#endif // _IND_SPRITERENTITY_
