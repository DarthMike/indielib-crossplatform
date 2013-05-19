/*****************************************************************************************
 * File: IND_SpriterManager.cpp
 * Desc: This class is used for managing Spriter animations.
 *****************************************************************************************/

/*
Something about license goes here ;)
*/

// ----- Includes -----

#include "Global.h"
#include "Defines.h"
#include "dependencies/tinyxml/tinyxml.h"
#include "IND_SpriterManager.h"
#include "IND_SpriterEntity.h"
#include "IND_Image.h"
#include "IND_ImageManager.h"
//#ifdef linux
#include <string>
//#endif


// ----- Defines -----

#define MAX_TOKEN 1024


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 * Init the manager. returns 1 (true) if successfully initialized.
 * Must be called before using any method.
 */
bool IND_SpriterManager::init() {
	end();
	initVars();

	g_debug->header("Initializing SpriterManager", 5);

	_ok = true;

	g_debug->header("SpriterManager OK", 6);

	return _ok;
}


/**
 * Frees the manager and all the objects that it contains.
 */
void IND_SpriterManager::end() {
	if (_ok) {
		g_debug->header("Finalizing SpriterManager", 5);
		freeVars();
		g_debug->header("SpriterManager finalized", 6);

		_ok = false;
	}
}


// --------------------------------------------------------------------------------
//									Public methods
// --------------------------------------------------------------------------------



/**
 * Returns 1 (true) if the Spriter Entity object passed as a parameter
 * exists and it is successfully eliminated.
 * @param pEn				Pointer to a Spriter Entity object.
 */
bool IND_SpriterManager::remove(IND_SpriterEntity *pEn) {
//	if (remove(pAn, 0))
//		return 1;
// TODO : search for entity and remove from vector



	return 0;
}

bool IND_SpriterManager::addSpriterFile(list<IND_SpriterEntity*> *pNewSpriterEntityList, const char *pSCMLFileName){
	if (parseSpriterData(pNewSpriterEntityList,pSCMLFileName)){
		return 1;
	}
	
	return 0;
}



// --------------------------------------------------------------------------------
//									Private methods
// --------------------------------------------------------------------------------



/**
 * Parses a Spriter SCML animations file using Tinyxml.
 * @param pNewSpriterAnimations		TODO describtion.
 * @param pSCMLFileName				TODO describtion.
 */
bool IND_SpriterManager::parseSpriterData(list<IND_SpriterEntity*> *pNewSpriterEntityList,const char *pSCMLFileName) {
	
    g_debug->header("Start parsing Spriter SGML file", 5);
    
    TiXmlDocument *eXmlDoc = new TiXmlDocument(pSCMLFileName);

	// Fatal error, cannot load
	if (!eXmlDoc->LoadFile()){
		g_debug->header("Not able to load the Spriter SGML file", 2);
		return 0;
	}


	string spriterTopPath;
    string s = string(pSCMLFileName);

	unsigned int lastPosTemp = s.find_last_of("\\/");

	if(lastPosTemp == string::npos){
		spriterTopPath = "./";
    }
	else{
    	spriterTopPath = s.substr(0, lastPosTemp + 1);
	}
    
	g_debug->header("Top directory : ", 3);
    g_debug->dataChar(spriterTopPath.c_str(), true);

	// Document root
	TiXmlElement *eSpriter_data = 0;
	eSpriter_data = eXmlDoc->FirstChildElement("spriter_data");

	if (!eSpriter_data) {
		g_debug->header("Invalid name for Spriter document root, should be <spriter_data>", 2);
		eXmlDoc->Clear();
		delete eXmlDoc;
		return 0;
	}

	// ----------------- Parse folders and create the images -----------------
    
      
    IND_SpriterEntity sEnt = *IND_SpriterEntity::newSpriterEntity();
    

	TiXmlElement *eFolder = 0;
	eFolder = eSpriter_data->FirstChildElement("folder");

	if (!eFolder) {
		g_debug->header("There are no Spriter resourcefolders to parse", 3);
		eXmlDoc->Clear();
		delete eXmlDoc;
		return 0;
	}

	while (eFolder) {

		//printf("Folder: %s\n", eFolder->Attribute("name"));
		TiXmlElement *eFile = 0;
		eFile = eFolder->FirstChildElement("file");

		while (eFile){
			//printf("  id    : %s\n", eFile->Attribute("id"));
			//printf("  name  : %s\n", eFile->Attribute("name"));
			//printf("  width : %s\n", eFile->Attribute("width"));
			//printf("  height: %s\n", eFile->Attribute("height"));

			string result = spriterTopPath + string(eFile->Attribute("name"));
			
			IND_Image *mImageTemp = IND_Image::newImage();
			if (!_imageManager->add(mImageTemp, result.c_str())){
				g_debug->header("Unable to add Spriter image", 2);
                eXmlDoc->Clear();
				delete eXmlDoc;
				return 0;
			}	

			eFile = eFile->NextSiblingElement("file");
		}

		eFolder = eFolder->NextSiblingElement("folder");
	}

	// ----------------- Parse entities -----------------

	TiXmlElement *eEntity = 0;
	eEntity = eSpriter_data->FirstChildElement("entity");

	if (!eEntity) {
		g_debug->header("There are no entities to parse", 2);
		eXmlDoc->Clear();
		delete eXmlDoc;
		return 0;
	}
    
    sEnt._id = eEntity->Attribute("id");
    sEnt._name = eEntity->Attribute("name");

	while (eEntity) {

		//IND_SpriterEntity *entityTemp = new IND_SpriterEntity(); TODO: use....

		//printf("Entity id: %s\n", eEntity->Attribute("id"));

		TiXmlElement *eAnimation = 0;
		eAnimation = eEntity->FirstChildElement("animation");

		while(eAnimation){
			//printf("  id     : %s\n", eAnimation->Attribute("id"));
			//printf("  name   : %s\n", eAnimation->Attribute("name"));
			//printf("  length : %s\n", eAnimation->Attribute("length"));
			//printf("  looping: %s\n", eAnimation->Attribute("looping"));
			
			TiXmlElement *eMainline = 0;
			eMainline = eAnimation->FirstChildElement("mainline");
			
			if (!eMainline) {
				g_debug->header("Animation is missing mainline", 2);
				eXmlDoc->Clear();
				delete eXmlDoc;
				return 0;
			}
			
			TiXmlElement *eMKey = 0;
			eMKey = eMainline->FirstChildElement("key");

			while (eMKey){
				//printf(" id: %s", eMKey->Attribute("id"));
				//printf(" time: %s\n", eMKey->Attribute("time"));

				TiXmlElement *eObject_ref = 0;
				eObject_ref = eMKey->FirstChildElement("object_ref");
				
				while (eObject_ref){
					//printf(" id: %s",        eObject_ref->Attribute("id"));
					//printf(" timeline: %s",  eObject_ref->Attribute("timeline"));
					//printf(" key: %s",       eObject_ref->Attribute("key"));
					//printf(" z_index: %s\n", eObject_ref->Attribute("z_index"));
				
					eObject_ref = eObject_ref->NextSiblingElement("object_ref");
				}

				eMKey = eMKey->NextSiblingElement("key");
			}

			TiXmlElement *eTimeline = 0;
			eTimeline = eAnimation->FirstChildElement("timeline");

			while (eTimeline){
				//printf("timeline id: %s\n", eTimeline->Attribute("id"));
				
				TiXmlElement *eTKey = 0;
				eTKey = eTimeline->FirstChildElement("key");
				
				while (eTKey){
					//printf(" id: %s", eTKey->Attribute("id"));
				    //printf(" spin: %s\n", eTKey->Attribute("spin"));

					//TiXmlElement *eObject = 0;
					//eObject = eTKey->FirstChildElement("object"); // asumption: there is only one "object" element under the key....					
					
					//printf(" folder: %s", eObject->Attribute("folder"));
					//printf(" file: %s", eObject->Attribute("file"));
					//printf(" x: %s", eObject->Attribute("x"));
					//printf(" y: %s", eObject->Attribute("y"));
					//printf(" pivot_x: %s", eObject->Attribute("pivot_x"));
					//printf(" pivot_y: %s", eObject->Attribute("pivot_y"));
					//printf(" angle: %s\n", eObject->Attribute("angle"));

					eTKey = eTimeline->NextSiblingElement("key");

				}
				

				eTimeline = eTimeline->NextSiblingElement("timeline");
			}



			eAnimation = eAnimation->NextSiblingElement("animation");
		}




		eEntity = eEntity->NextSiblingElement("entity");


	}


	// Delete our allocated document and return success.
	eXmlDoc->Clear();
	delete eXmlDoc;

    g_debug->header("End parsing Spriter SGML file", 6);
    
	return 1;
}


/**
 * Inserts an animation into the manager.
 * @param pNewAnimation				The animation that is to be inserted into manager.
 */
void IND_SpriterManager::addToList(IND_SpriterEntity *pNewEntity) {
	_listSpriterEntity->push_back(pNewEntity);
}


/**
 * Removes a Spriter animation from the manager.
 * @param pAn					The animation that is to be removed from the manager.
 */
void IND_SpriterManager::delFromlist(IND_SpriterEntity *pEn) {
	_listSpriterEntity->remove(pEn);
	pEn = NULL;
}


/**
 * Initialization error message.
 */
void IND_SpriterManager::writeMessage() {
	g_debug->header("This operation can not be done", 3);
	g_debug->dataChar("", 1);
	g_debug->header("Invalid Id or not correctly initialized SpriterAnimationManager", 2);
}


/**
 * Init manager variables.
 */
void IND_SpriterManager::initVars() {
	_listSpriterEntity = new list <IND_SpriterEntity *>;
    _imageManager = new IND_ImageManager();
	_imageManager->init();
}


/**
 * Free manager variables.
 */

void IND_SpriterManager::freeVars() {
	// Releases everything in the anim
/*	list <IND_Animation *>::iterator mAnimationListIter;
	for (mAnimationListIter  = _listAnimations->begin();
	        mAnimationListIter != _listAnimations->end();
	        mAnimationListIter++) {
		// We released the animation(indicates no release pointer (Google Translate)
		remove((*mAnimationListIter), 1);

		g_debug->header("Ok", 6);
	}

	// Clear list
	_listAnimations->clear();

	// Free list
	DISPOSE(_listAnimations);
*/
}

