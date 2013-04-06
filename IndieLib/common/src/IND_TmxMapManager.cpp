/*****************************************************************************************
 * File: IND_TmxMapManager.cpp
 * Desc: TmxMap manager
 *****************************************************************************************/

/*
 
 Here goes something about license ......

*/

// ----- Includes -----

#include "Global.h"
#include "dependencies/TmxParser/Tmx.h"
#include "dependencies/FreeImage/Dist/FreeImage.h"
#include "IND_TmxMapManager.h"
#include "IND_TmxMap.h"

#ifdef PLATFORM_LINUX
#include <string.h>
#endif


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
This function returns 1 (true) if the adminstrator is successfully initialized.
Must be called before using any method.
*/
bool IND_TmxMapManager::init() {
	end();
	initVars();

	g_debug->header("Initializing TmxMapManager", 5);
	g_debug->header("Preparing TmxMapManager", 1);
	_ok = true;

	g_debug->header("TmxMapManager OK", 6);
	
	//TODO: REGISTER ERROR HANDLERS FOR FREEIMAGE

	return _ok;
}


/**
@b Operation:

This function frees the manager and all the objects that it contains.
*/
void IND_TmxMapManager::end() {
	if (_ok) {
		g_debug->header("Finalizing TmxMapManager", 5);
		g_debug->header("Freeing TmxMaps" , 5);
		freeVars();
		g_debug->header("TmxMaps freed", 6);
		g_debug->header("TmxMapManager finalized", 6);

		_ok = false;
	}
}


// --------------------------------------------------------------------------------
//									Public methods
// --------------------------------------------------------------------------------

/**
@b Parameters:

@arg @b pNewTmxMap            Pointer no a new IND_TmxMap object.
@arg @b pName                 TmxMap filename

@b Operation:

This function returns 1 (true) if the image object passed as a parameter
exists and is added successfully to the manager.

It supports the following file formats:
tmx.

*/
bool IND_TmxMapManager::add(IND_TmxMap *pNewTmxMap,const char *pName) {
	g_debug->header("Loading TmxMap", 5);
	
	if(!pName) {
		g_debug->header("Invalid File name provided (null)",2);
		return 0;
	}

	g_debug->header("File name:", 3);
	g_debug->dataChar(pName, 1);

	if (!_ok) {
		writeMessage();
		return 0;
	}

	// ----- Obtaining and checking file extension -----

	char ext [128];
	getExtensionFromName(pName,ext);
	if (!checkExtImage(ext)){
		g_debug->header("Unknown extension", 2);
		return 0;
	}
	

	// ----- Load TmxMap -----

	Tmx::Map *map = new Tmx::Map();
	map->ParseFile(pName);
    
	if (map->HasError()) {
		g_debug->header("Error code:", 2);
		//g_debug->dataChar(map->GetErrorCode(), 1); //TODO
		g_debug->header("Error text:", 2);
		g_debug->dataChar(map->GetErrorText().c_str(), 1);
		
        DISPOSE(map);
		
        return 0;
	}

    
   	// ----- Load TmxMap tilesetimagesheet -----
    
    
    string tmxPath;
    string imagePath;
    string s = string(pName);
    
    unsigned int lastPosTemp = s.find_last_of("\\/");
    
    if(lastPosTemp == string::npos){
        tmxPath = "./";
    }
    else{
        tmxPath = s.substr(0, lastPosTemp + 1);
    }
    
    
    imagePath = tmxPath.append(map->GetTileset(0)->GetImage()->GetSource());  // FIXME : this is very wrong we need to store an array of images instead.... i.e NO '0'
    
    
    // ----- Load image -----

	FREE_IMAGE_FORMAT imgFormat =  FreeImage_GetFileType(imagePath.c_str(), 0);
	if (FIF_UNKNOWN == imgFormat) {
		g_debug->header("Image not found", 2);
        DISPOSE(map);
        return 0;
    }
	FIBITMAP* image = FreeImage_Load(imgFormat, imagePath.c_str(), 0);
	if (!image) {
		g_debug->header("Image could not be loaded", 2);
        DISPOSE(map);
		return 0;
	}

	
	 // ----- Attributes -----
	
    pNewTmxMap->setTmxMapHandle(map);
	pNewTmxMap->setName(pName);
    pNewTmxMap->setImage(image);                    // FIXME should be added to an array
    pNewTmxMap->setImagePath(imagePath.c_str());    // FIXME should be added to an array
    
	// ----- Puts the object into the manager -----
    
    addToList(pNewTmxMap);

	// ----- g_debug -----
    
    g_debug->header("TmxMap loaded", 6);

    
	return 1;
}



/**
@b Parameters:

@arg @b pMap                 Pointer to an IND_TmxMap object.

@b Operation:

This function returns 1(true) if the IND_TmxMap object passed as parameter exists and it is
deleted from the manager successfully.
*/
bool IND_TmxMapManager::remove(IND_TmxMap *pMap) {
	g_debug->header("Freeing Map", 5);

	if (!_ok || !pMap) {
		writeMessage();
		return 0;
	}

	// Search object
	bool mIs = 0;
	list <IND_TmxMap *>::iterator mMapListIter;
	for (mMapListIter  = _listMaps->begin();
	        mMapListIter != _listMaps->end();
	        mMapListIter++) {
		if ((*mMapListIter) == pMap) {
			mIs = 1;
			break;
		}
	}

	// Not found
	if (!mIs) {
		writeMessage();
		return 0;
	}

	// ----- Free object -----

//FIXME	g_debug->header("File name:", 3);
//FIXME	g_debug->dataChar(pIm->getName(), 1);

	// Quit from list
	delFromlist(pMap);

	// Free Map
//FIXME	FreeImage_Unload(pIm->getFreeImageHandle());	

	g_debug->header("Ok", 6);

	return 1;
}


/**
@b Parameters:

@arg @b pNewTmxMap            Pointer to a new IND_TmxMap object
@arg @b pOldTmxMap            Pointer to the IND_TmxMAp we want to clone

@b Operation:

This function returns 1 (true) if the image object passed as a parameter
exists and is added successfully to the manager creating a new image cloning from a previous one.
*/
bool IND_TmxMapManager::clone(IND_TmxMap *pNewTmxMap, IND_TmxMap *pOldTmxMap) { // FIXME: is a clone method needed or should it be removed
//	g_debug->header("Cloning Image", 5);

//	if (!_ok || !pNewImage || !pOldImage || !pOldImage->getFreeImageHandle()) {
//		writeMessage();

//		return 0;
//	}

//	// ----- Image creation using FreeImage -----

//	// Clone the old image

//	FIBITMAP* image = FreeImage_Clone(pOldImage->getFreeImageHandle());
//	if (!image) {
//		g_debug->header("Image could not be cloned", 2);
//		return 0;
//	}
	
//	// Attributes

//	pNewImage->setWidth(pOldImage->getWidth());
//	pNewImage->setHeight(pOldImage->getHeight());
//	pNewImage->setBpp(pOldImage->getBpp());
//	pNewImage->setBytespp(pOldImage->getBytespp());
//	pNewImage->setFormatInt(pOldImage->getFormatInt());
//	char ext [128];
//	getExtensionFromName(pOldImage->getName(),ext);
//	pNewImage->setExtension(ext);
//	pNewImage->setPointer(FreeImage_GetBits(image));
//	pNewImage->setName("Image cloned");
//	pNewImage->setFreeImageHandle(image);

//	// ----- Put the object into the manager -----

//	addToList(pNewImage);

	// ----- g_debug -----

//	g_debug->header("File name:", 3);
//	g_debug->dataChar(pOldImage->getName(), 1);
//	g_debug->header("Image cloned", 6);

	return 1;
}






// --------------------------------------------------------------------------------
//									Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
==================
Obtain file extension
Return "" if the file hasn't got an extension
==================
*/
void IND_TmxMapManager::getExtensionFromName(const char *pName, char* pExtImage) {
	
	int i;
	// The search starts at the end of the name
	for (i = strlen(pName); i > -1; i--)
		if (pName [i] == '.') 
			break;

	// Image without extension
	if (pName [i] != '.')
		strcpy (pExtImage, "");

	// Copy the extension
	strcpy(pExtImage, &pName [i + 1]);
}


/*
==================
Check if it is a known extension
==================
*/
bool IND_TmxMapManager::checkExtImage(const char *pExtImage) {
	for (int i = 0; i < MAX_EXT_TMXMAP; i++) {
		if (!strcmp(pExtImage, _supportedExt [i]))
			return 1;
	}

	return 0;
}


/*
==================
Inserts object into the manager
==================
*/
void IND_TmxMapManager::addToList(IND_TmxMap *pNewMap) {
	_listMaps->push_back(pNewMap);
}


/*
==================
Deletes object from the manager
==================
*/
void IND_TmxMapManager::delFromlist(IND_TmxMap *pMap) {
	_listMaps->remove(pMap);
	DISPOSE(pMap);
}


/*
==================
Initialization error message
==================
*/
void IND_TmxMapManager::writeMessage() {
	g_debug->header("This operation can not be done", 3);
	g_debug->dataChar("", 1);
	g_debug->header("Invalid Id or IND_TmxMapManager not correctly initialized", 2);
}


/*
==================
Init manager vars
==================
*/
void IND_TmxMapManager::initVars() {
	_listMaps = new list <IND_TmxMap *>;

	// Supported extensions
	_supportedExt [0]  = (char*) "tmx";
}


/*
==================
Free manager memory
==================
*/
void IND_TmxMapManager::freeVars() {
// FIXME: free chararray for name and free the TMXMap itself fo each of the IND_TmxMap
//	// Deletes all the manager entities
//	list <IND_Image *>::iterator mImageListIter;
//	for (mImageListIter  = _listImages->begin();
//	        mImageListIter != _listImages->end();
//	        mImageListIter++) {
//			FIBITMAP* handle = (*mImageListIter)->getFreeImageHandle();
//		if (handle) {
//			g_debug->header("Freeing image:", 3);
//			g_debug->dataChar((*mImageListIter)->getName(), 1);
//
//			// Free image
//			FreeImage_Unload(handle);
//
//			// Free the string of the extension
//			DISPOSEARRAY((*mImageListIter)->_image._ext);
//		}
//	}
//
	// Clear list
	_listMaps->clear();

	// Free list
	DISPOSE(_listMaps);
}

/** @endcond */





