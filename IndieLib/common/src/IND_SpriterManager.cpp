/*****************************************************************************************
 * File: IND_SpriterAnimationManager.cpp
 * Desc: This class is used for managing Spriter animations.
 *****************************************************************************************/

/*
Something about license goes here ;)
*/

// ----- Includes -----

#include "Global.h"
#include "Defines.h"
#include "dependencies/tinyxml/tinyxml.h"
#include "IND_AnimationManager.h"


// ----- Defines -----

#define MAX_TOKEN 1024


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 * Init the manager. returns 1 (true) if successfully initialized.
 * Must be called before using any method.
 */
bool IND_AnimationManager::init() {
	end();
	initVars();

	g_debug->header("Initializing SpriterAnimationManager", 5);

	_ok = true;

	g_debug->header("SpriterAnimationManager OK", 6);

	return _ok;
}


/**
 * Frees the manager and all the objects that it contains.
 */
void IND_AnimationManager::end() {
	if (_ok) {
		g_debug->header("Finalizing SpriterAnimationManager", 5);
		freeVars();
		g_debug->header("SpriterAnimationManager finalized", 6);

		_ok = false;
	}
}


// --------------------------------------------------------------------------------
//									Public methods
// --------------------------------------------------------------------------------



/**
 * Returns 1 (true) if the Spriter animation object passed as a parameter
 * exists and it is successfully eliminated.
 * @param pAn				Pointer to a Spriter animation object.
 */
bool IND_SpriterAnimationManager::remove(IND_SPriterAnimation *pAn) {
	if (remove(pAn, 0))
		return 1;

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
bool IND_AnimationManager::parseSpriterAnimations(list<IND_Animation*> *pNewSpriterAnimationList,char *pSCMLFileName) {
	TiXmlDocument *eXmlDoc = new TiXmlDocument(pSCMLFileName);

	// Fatal error, cannot load
	if (!eXmlDoc->LoadFile()) return 0;

	// Document root
	TiXmlElement *eSpriter_data = 0;
	eSpriter_data = eXmlDoc->FirstChildElement("spriter_data");

	if (!eSpriter_data) {
		g_debug->header("Invalid name for document root, should be <spriter_data>", 2);
		eXmlDoc->Clear();
		delete eXmlDoc;
		return 0;
	}

	// ----------------- Parse folders and create the images -----------------

	// Folders
	TiXmlElement *eFolder = 0;
	eFolder = eSpriter_data->FirstChildElement("folder");

	if (!eFolder) {
		g_debug->header("There are no folders to parse", 2);
		mXmlDoc->Clear();
		delete mXmlDoc;
		return 0;
	}

/*
vector< vector<int> > vec;

for (int i = 0; i < 10; i++) {
    vec.push_back(vector<int>()); // Add an empty row
}

for (int j = 0; j < 20; j++) {
    for (int i = 0; i < vec.size(); i++) {
        vec[i].push_back(i * j); // Add column to all rows
    }
}
*/

	// Parse all the folders
	while (eFolder) {

		// TODO loop over files, and parse each

		eFolder = eFolder->NextSiblingElement("folder");
	}

	// ----------------- Parse entities -----------------

	// Entities
	TiXmlElement *eEntity = 0;
	eEntity = eSpriter_data->FirstChildElement("entity");

	if (!eEntity) {
		g_debug->header("There are no entities to parse", 2);
		mXmlDoc->Clear();
		delete mXmlDoc;
		return 0;
	}


	// Parse all the entities
	while (eFolder) {

		// TODO loop over animation etc. and parse each

		eFolder = eFolder->NextSiblingElement("entity");
	}


	// Delete our allocated document and return success.
	mXmlDoc->Clear();
	delete mXmlDoc;

	return 1;
}


/**
 * Inserts an animation into the manager.
 * @param pNewAnimation				The animation that is to be inserted into manager.
 */
void IND_SpriterAnimationManager::addToList(IND_SpriterAnimation *pNewSpriterAnimation) {
	_listSpriterAnimations->push_back(pNewSpriterAnimation);
}


/**
 * Removes a Spriter animation from the manager.
 * @param pAn					The animation that is to be removed from the manager.
 */
void IND_SpriterAnimationManager::delFromlist(IND_SpriterAnimation *pAn) {
	_listSpriterAnimations->remove(pAn);
	pAn = NULL;
}


/**
 * Initialization error message.
 */
void IND_SpriterAnimationManager::writeMessage() {
	g_debug->header("This operation can not be done", 3);
	g_debug->dataChar("", 1);
	g_debug->header("Invalid Id or not correctly initialized SpriterAnimationManager", 2);
}


/**
 * Init manager variables.
 */
void IND_SpriterAnimationManager::initVars() {
//	_listAnimations = new list <IND_Animation *>;
}


/**
 * Free manager variables.
 */
/*
void IND_AnimationManager::freeVars() {
	// Releases everything in the anim
	list <IND_Animation *>::iterator mAnimationListIter;
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
}
*/
