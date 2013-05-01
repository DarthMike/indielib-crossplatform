/*****************************************************************************************
 * File: IND_AnimationManager.cpp
 * Desc: This class is used for managing animations.
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (javierlopezpro@gmail.com)

This library is free software; you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
Suite 330, Boston, MA 02111-1307 USA
*/

// ----- Includes -----

#include "Global.h"
#include "Defines.h"
#include "dependencies/tinyxml/tinyxml.h"
#include "IND_AnimationManager.h"
#include "IND_Animation.h"
#include "IND_ImageManager.h"
#include "IND_Image.h"
#include "IND_Frame.h"
#include "IND_Sequence.h"
#include "IND_SurfaceManager.h"
#include "IND_Surface.h"
#include "IND_Timer.h"
#include "CollisionParser.h"


// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 * Init the manager. returns 1 (true) if successfully initialized.
 * Must be called before using any method.
 * @param pImageManager				Pointer to the manager IND_ImageManager correctly initialized
 * @param pSurfaceManager			Pointer to the manager IND_SurfaceManager correctly initialized
 */
bool IND_AnimationManager::init(IND_ImageManager *pImageManager, IND_SurfaceManager *pSurfaceManager) {
	end();
	initVars();

	g_debug->header("Initializing AnimationManager", DebugApi::LogHeaderBegin);

	if (pSurfaceManager->isOK()) {
		g_debug->header("SurfaceManager Ok", DebugApi::LogHeaderOk);
		_surfaceManager = pSurfaceManager;

		g_debug->header("ImageManager Ok", DebugApi::LogHeaderOk);
		_imageManager = pImageManager;

		_collisionParser = CollisionParser::instance();

		_ok = true;

		g_debug->header("AnimationManager OK", DebugApi::LogHeaderEnd);
	} else {
		g_debug->header("SurfaceManager is not correctly initialized", DebugApi::LogHeaderError);
		_ok = false;
	}

	return _ok;
}


/**
 * Frees the manager and all the objects that it contains.
 */
void IND_AnimationManager::end() {
	if (_ok) {
		g_debug->header("Finalizing AnimationManager", DebugApi::LogHeaderBegin);
		g_debug->header("Freeing animations" , DebugApi::LogHeaderBegin);
		freeVars();
		g_debug->header("Animations freed", DebugApi::LogHeaderEnd);
		g_debug->header("AnimationManager finalized", DebugApi::LogHeaderEnd);

		_ok = false;
	}
}

/** 
 Returns state of initialization.
 @return  Will give true if object initialized correctly, false otherwise
 */
bool  IND_AnimationManager::isOK() {
    return _ok;
}


// --------------------------------------------------------------------------------
//									Public methods
// --------------------------------------------------------------------------------

/**
 * Returns 1 (true) if the animation file exists and is added successfully
 * to the manager loading the frames as IND_Surface objects. So, we will be able to draw
 * this animation directly.
 * @param pNewAnimation				Pointer to a new animation object.
 * @param pAnimation				Name of the animation file.
 * @param pType					Surface type (see ::IND_Type)
 * @param pQuality				Surface quality (see ::IND_Quality)
 */
bool IND_AnimationManager::addToSurface(IND_Animation *pNewAnimation,
                                        const char *pAnimation,
                                        IND_Type pType,
                                        IND_Quality pQuality) {
	if (!addToImage(pNewAnimation, pAnimation))
		return 0;

	for (unsigned int i = 0; i < pNewAnimation->getNumTotalFrames(); i++) {
		// Pointer to the image
		IND_Image *ActualImage = pNewAnimation->getImage(i);

		// Creation of the surface
		IND_Surface *mNewSurface = IND_Surface::newSurface();
		_surfaceManager->add(mNewSurface, ActualImage, pType, pQuality);
		pNewAnimation->setSurface(i, mNewSurface);

		// Free the image
		_imageManager->remove(ActualImage);
		pNewAnimation->setImage(i, 0);
	}

	return 1;
}


/**
 * Returns 1 (true) if the animation file exists and is added successfully
 * to the manager, loading the frames with IND_Surface objects. So, we will be able to draw
 * this animation directly. Furthermore, it uses the color passed as a parameter to apply
 * a colorkey to the image. So, the zones coloured with this colour will be
 * transparent.
 * @param pNewAnimation				Pointer to a new animation object.
 * @param pAnimation				Name of the animation XML script.
 * @param pType						Surface type (see ::IND_Type)
 * @param pQuality                	Surface quality (see ::IND_Quality)
 * @param pR, pG, pB				Color where the colorkey will be applied, this area will become transparent.
 */
bool IND_AnimationManager::addToSurface(IND_Animation *pNewAnimation,
                                        const char *pAnimation,
                                        IND_Type pType,
                                        IND_Quality pQuality,
                                        BYTE pR,
                                        BYTE pG,
                                        BYTE pB) {
	//TODO:Modify API, as pType is unnecessary (must be IND_ALPHA), if we want to
	//use alpha blending and assign color key.
	assert(IND_ALPHA == pType);
	
	if (!addToImage(pNewAnimation, pAnimation))
		return 0;

	if (IND_ALPHA != pType) 
		return 0;

	for (unsigned int i = 0; i < pNewAnimation->getNumTotalFrames(); i++) {
		// Pointer to the image
		IND_Image *mCurrentImage = pNewAnimation->getImage(i);

		// Color key
		mCurrentImage->setAlpha(pR,pG,pB);
		
		// Creation of the surface
		IND_Surface *mNewSurface = IND_Surface::newSurface();
		_surfaceManager->add(mNewSurface, mCurrentImage, pType, pQuality);
		pNewAnimation->setSurface(i, mNewSurface);

		// Free the image
		_imageManager->remove(mCurrentImage);
		pNewAnimation->setImage(i, 0);
	}

	return 1;
}


/**
 * Returns 1 (true) if the animation file exists and is added successfully
 * to the manager loading the frames as IND_Surface objects. So, we will be able to draw
 * this animation directly.
 * @param pNewAnimation				Pointer to a new animation object.
 * @param pAnimation				Name of the animation file.
 * @param pBlockSize				Width of the blocks
 * @param pType					Surface type (see ::IND_Type)
 * @param pQuality				Surface quality (see ::IND_Quality)
 */
bool IND_AnimationManager:: addToSurface(IND_Animation *pNewAnimation,
        const char *pAnimation,
        int pBlockSize,
        IND_Type pType,
        IND_Quality pQuality) {
	if (!addToImage(pNewAnimation, pAnimation))
		return 0;

	for (unsigned int i = 0; i < pNewAnimation->getNumTotalFrames(); i++) {
		// Pointer to the image
		IND_Image *ActualImage = pNewAnimation->getImage(i);

		// Creation of the surface
		IND_Surface *mNewSurface = IND_Surface::newSurface();
		_surfaceManager->add(mNewSurface, ActualImage, pBlockSize,  pType, pQuality);
		pNewAnimation->setSurface(i, mNewSurface);

		// Free the image
		_imageManager->remove(ActualImage);
		pNewAnimation->setImage(i, 0);
	}

	return 1;
}


/**
 * Returns 1 (true) if the animation file exists and is added successfully
 * to the manager, loading the frames with IND_Surface objects. So, we will be able to draw
 * this animation directly. Furthermore, it uses the color passed as a parameter to apply
 * a colorkey to the image. So, the areas coloured with this color will become
 * transparent.
 * @param pNewAnimation				Pointer to a new animation object.
 * @param pAnimation				Name of the animation XML script.
 * @param pBlockSize				Width of the blocks
 * @param pType					Surface type (see ::IND_Type)
 * @param pQuality				Surface quality (see ::IND_Quality)
 * @param pR, pG, pB				Color where the colorkey will be applied, this areas will become transparent.
 */
bool IND_AnimationManager::addToSurface(IND_Animation *pNewAnimation,
                                        const char *pAnimation,
                                        int pBlockSize,
                                        IND_Type pType,
                                        IND_Quality pQuality,
                                        BYTE pR,
                                        BYTE pG,
                                        BYTE pB) {
	if (!addToImage(pNewAnimation, pAnimation))
		return 0;


	for (unsigned int i = 0; i < pNewAnimation->getNumTotalFrames(); i++) {
		// Pointer to the image
		IND_Image *mCurrentImage = pNewAnimation->getImage(i);

		// Color key
		mCurrentImage->setAlpha(pR, pG, pB);

		// Creation of the surface
		IND_Surface *mNewSurface = IND_Surface::newSurface();
		_surfaceManager->add(mNewSurface, mCurrentImage, pBlockSize, pType, pQuality);
		pNewAnimation->setSurface(i, mNewSurface);

		// Free the image
		_imageManager->remove(mCurrentImage);
		pNewAnimation->setImage(i, 0);
	}

	return 1;
}


/**
 * Returns 1 (true) if the animation file exists and is added successfully
 * to the manager, loading the frames as IND_Image objects. So this animation won't be able
 * to be drawn to the screen directly, but you will be able to modify the IND_Image objects
 * (for applying a filter, for example) accessing them trough the ::IND_Animation::getImage(int pFrame) function.
 * 
 * Once they have been modified, they can be converted to ::IND_Surface and applied to the animation
 * using ::IND_Animation::setSurface(int pFrame, IND_Surface *pNewSurface)  and then the animation will be able to be drawn correctly
 *
 * @param pNewAnimation				Pointer to a new animation object.
 * @param pAnimation				Name of the animation XML script.
 */
bool IND_AnimationManager::addToImage(IND_Animation *pNewAnimation, const char *pAnimation) {
	g_debug->header("Parsing and loading animation", DebugApi::LogHeaderBegin);
	g_debug->header("File name:", DebugApi::LogHeaderInfo);
	g_debug->dataChar(pAnimation, 1);

	if (!_ok) {
		writeMessage();
		return 0;
	}
	
	// ----- Animation file parsing -----

	if (!parseAnimation(pNewAnimation, pAnimation)) {
		g_debug->header("Fatal error, cannot load the animation xml file", DebugApi::LogHeaderError);
		return 0;
	}

	strcpy(pNewAnimation->_animation._name,pAnimation);


	// ----- Put the object into the manager  -----

	addToList(pNewAnimation);

	// ----- g_debug -----

	g_debug->header("Animation parsed and loaded", DebugApi::LogHeaderEnd);

	return 1;
}


/**
 * Returns 1 (true) if the animation object passed as a parameter
 * exists and it is successfully eliminated.
 * @param pAn				Pointer to an animation object.
 */
bool IND_AnimationManager::remove(IND_Animation *pAn) {
	if (remove(pAn, 0))
		return 1;

	return 0;
}


// --------------------------------------------------------------------------------
//									Private methods
// --------------------------------------------------------------------------------
/** @cond DOCUMENT_PRIVATEAPI */
/**
 * Returns 1 (true) if the animation object passed as a parameter exists
 * and returns in pAxisCalX and pAxisCalY two values which will be necessary for the hotspot
 * specification when we have to call to the function IND_Render::SetTransform() (before
 * drawing the surface).
 * 
 * So this function is used for changing the reference point of the image.
 * When we talk about reference point or hotspot, we are refering to an imaginary point
 * from where the translation, rotation and scaling transformation will be applied. In this way,
 * we can choose that an animation rotates over its center, over one of its vertex or over
 * any middle position.
 * 
 * For establishing the hotspot, we use pAxisX and pAxisY, which in a range from 0
 * until 1, make reference to horizontal and vertical surface position respectivily
 * In this way (0.5f,0.5f) would be the image centre, (1.0f,0.0f) would be the upper right
 * corner, etc...
 * 
 * The resulting values (pAxisCalX y pAxisCalY) must be saved and passed as parameter to
 * the function IND_Render::SetTransform(),so , when we draw the sequence the specified
 * hotspot will be used.
 * 
 * If this method is not used and we pass to IND_Render::SetTransform() two null values
 * as the parameters pAxisCalX and pAxisCalY, the transformations will be applied by default
 * from the left upper corner of the object.
 * 
 * One way of using this function is through ::IND_Entity2dManager, and can be used for
 * managing all the IndieLib objects and its attributes in a very efficient way.
 * Using this function separately for each animation can be very uncomfortble.
 * @param pAn					TODO describtion.
 * @param pSequence				TODO describtion.
 * @param pAxisX				TODO describtion.
 * @param pAxisY				TODO describtion.
 * @param pAxisCalX				TODO describtion.
 * @param pAxisCalY				TODO describtion.
 */
bool IND_AnimationManager::calculateAxis(IND_Animation *pAn,
                                       int pSequence,
                                       float pAxisX,
                                       float pAxisY,
                                       int *pAxisCalX,
                                       int *pAxisCalY) {
	if (!_ok || !pAn) {
		return 0;
	}

	if (pAxisX < 0.0f || pAxisX > 1.0f)
		pAxisX = 0.0f;

	if (pAxisY < 0.0f || pAxisY > 1.0f)
		pAxisY = 0.0f;

	*pAxisCalX = (int)(pAxisX * pAn->getHighWidth(pSequence)  * -1);
	*pAxisCalY = (int)(pAxisY * pAn->getHighHeight(pSequence)  * -1);

	return 1;
}


/**
 * Load an image and returns the pointer.
 * @param pName					TODO describtion.
 */
IND_Image *IND_AnimationManager::loadImage(const char *pName) {
	IND_Image *mNewImage = IND_Image::newImage();
    
    bool noError = _imageManager->add(mNewImage, pName);
    
    if (!noError) {
		DISPOSEMANAGED(mNewImage);
    }
	
	return noError ? mNewImage : NULL;
}


/**
 * Parses a XML animation file using Tinyxml.
 * @param pNewAnimation				TODO describtion.
 * @param pAnimationName			TODO describtion.
 */
bool IND_AnimationManager::parseAnimation(IND_Animation *pNewAnimation, const char *pAnimationName) {
	TiXmlDocument   *mXmlDoc = new TiXmlDocument(pAnimationName);

	// Fatal error, cannot load
	if (!mXmlDoc->LoadFile()) {
        DISPOSE(mXmlDoc);
     	return 0;
    }

	// Document root
	TiXmlElement *mXAnimation = 0;
	mXAnimation = mXmlDoc->FirstChildElement("animation");

	if (!mXAnimation) {
		g_debug->header("Invalid name for document root, should be <animation>", DebugApi::LogHeaderError);
		mXmlDoc->Clear();
		delete mXmlDoc;
		return 0;
	}

	// ----------------- Parse frames and create the images -----------------

	// Frames
	TiXmlElement *mXFrames = 0;
	mXFrames = mXAnimation->FirstChildElement("frames");

	if (!mXFrames) {
		g_debug->header("Invalid name for frames child, should be <frames>", DebugApi::LogHeaderError);
		mXmlDoc->Clear();
		delete mXmlDoc;
		return 0;
	}

	TiXmlElement *mXFrame = 0;
	mXFrame = mXFrames->FirstChildElement("frame");

	if (!mXFrame) {
		g_debug->header("There are no frames to parse", DebugApi::LogHeaderError);
		mXmlDoc->Clear();
		delete mXmlDoc;
		return 0;
	}

	// Parse all the frames
	while (mXFrame) {
		IND_Frame *mNewFrame = new IND_Frame;

		// Frame name attribute
		if (mXFrame->Attribute("name")) {
			mNewFrame->setName(mXFrame->Attribute("name"));
		} else {
			g_debug->header("The frame doesn't have a \"name\" attribute", DebugApi::LogHeaderError);
			mXmlDoc->Clear();
			delete mNewFrame;
			delete mXmlDoc;
			return 0;
		}

		// Frame image file attribute
		if (mXFrame->Attribute("file")) {
			// Loading image
			mNewFrame->setImage(loadImage((char *) mXFrame->Attribute("file")));
			if (mNewFrame->getImage() == 0)
				return 0;
		} else {
			g_debug->header("The frame doesn't have a \"file\" attribute", DebugApi::LogHeaderError);
			mXmlDoc->Clear();
			delete mXmlDoc;
			return 0;
		}

		// Frame offset_x attribute (optional)
		if (mXFrame->Attribute("offset_x")) {
			mNewFrame->SetOffsetX(atoi(mXFrame->Attribute("offset_x")));
		}

		// Frame offset_y attribute (optional)
		if (mXFrame->Attribute("offset_y")) {
			mNewFrame->SetOffsetY(atoi(mXFrame->Attribute("offset_y")));
		}

		// Path to a collision file (optional)
		if (mXFrame->Attribute("collision")) {
			// ----- Parsing collision file -----

			g_debug->header("Parsing collision file", DebugApi::LogHeaderBegin);
			g_debug->header("File name:", DebugApi::LogHeaderInfo);
			g_debug->dataChar((char *) mXFrame->Attribute("collision"), 1);

			if (!_collisionParser->parseCollision(mNewFrame->_frame._listBoundingCollision, (char *) mXFrame->Attribute("collision"))) {
				g_debug->header("Fatal error, cannot load the collision xml file", DebugApi::LogHeaderError);
				return 0;
			}

			// ----- g_debug -----

			g_debug->header("Collision file loaded", DebugApi::LogHeaderEnd);
		}

		// Push the frame into the frames vector
		pNewAnimation->_vectorFrames->push_back(mNewFrame);  //peta

		// Move to the next frame declaration
		mXFrame = mXFrame->NextSiblingElement("frame");
	}

	// ----------------- Parse sequences -----------------

	// Sequences
	TiXmlElement *mXSequences = 0;
	mXSequences = mXFrames->NextSiblingElement("sequences");

	if (!mXSequences) {
		g_debug->header("Invalid name for sequences child, should be <sequences>", DebugApi::LogHeaderError);
		mXmlDoc->Clear();
		delete mXmlDoc;
		return 0;
	}

	TiXmlElement *mXSequence = 0;
	mXSequence = mXSequences->FirstChildElement("sequence");

	if (!mXSequence) {
		g_debug->header("There are no sequences to parse", DebugApi::LogHeaderError);
		mXmlDoc->Clear();
		delete mXmlDoc;
		return 0;
	}

	// Parse all the sequences
	while (mXSequence) {
		IND_Sequence *mNewSequence;

		if (mXSequence->Attribute("name")) {
			mNewSequence = new IND_Sequence();
			mNewSequence->setName(mXSequence->Attribute("name"));
            mNewSequence->getSequenceTimer()->start();
		} else {
			g_debug->header("The sequence doesn't have a \"name\" attribute", DebugApi::LogHeaderError);
			mXmlDoc->Clear();
			delete mXmlDoc;
			return 0;
		}

		TiXmlElement *mXSequenceFrame = 0;
		mXSequenceFrame = mXSequence->FirstChildElement("frame");

		if (!mXSequenceFrame) {
			g_debug->header("There are no frames in the sequence to parse", DebugApi::LogHeaderError);
			mXmlDoc->Clear();
			delete mXmlDoc;
			return 0;
		}

		if (mXSequenceFrame->Attribute("name")) {
			while (mXSequenceFrame) {
				// We have to check in the name of this frame has been already declared in
				// frame declaration section
				int _pos = 0;
				if (isDeclaredFrame((char *) mXSequenceFrame->Attribute("name"), pNewAnimation, &_pos)) {
					IND_Sequence::FRAME_TIME *mNewFrameTime = new IND_Sequence::FRAME_TIME;

					// Frame time points to the frame vector
					mNewFrameTime->_pos = _pos;

					int _width  = (*pNewAnimation->getVectorFrames()) [_pos]->getImage()->getWidth();
					int _height = (*pNewAnimation->getVectorFrames()) [_pos]->getImage()->getHeight();

					if (_width > mNewSequence->getHighWidth())
						mNewSequence->setHighWidth(_width);

					if (_height > mNewSequence->getHighHeight())
						mNewSequence->setHighHeight(_height);

					// Frame time (optional)
					if (mXSequenceFrame->Attribute("time")) {
						mNewFrameTime->_time = atoi(mXSequenceFrame->Attribute("time"));
					}

					// Push the frame reference into the sequence
					mNewSequence->_sequence._listFrames->push_back(mNewFrameTime);
					int _numFrames = mNewSequence->getNumFrames();
					mNewSequence->setNumFrames(_numFrames + 1);

					// Move to the next frame reference
					mXSequenceFrame = mXSequenceFrame->NextSiblingElement("frame");
				} else {
					g_debug->header("Unknown frame in sequences", DebugApi::LogHeaderError);
					mXmlDoc->Clear();
					delete mXmlDoc;
					return 0;
				}
			}

			// Push the sequence into the animation
			pNewAnimation->_animation._listSequences->push_back(mNewSequence);
			pNewAnimation->_animation._sumSequences++;

			// Move to the next sequence
			mXSequence = mXSequence->NextSiblingElement("sequence");
		} else {
			g_debug->header("The frame sequence doesn't have a \"name\" attribute", DebugApi::LogHeaderError);
			mXmlDoc->Clear();
			delete mXmlDoc;
			return 0;
		}
	}

	// Delete our allocated document and return success ;)
	mXmlDoc->Clear();
	delete mXmlDoc;

	return 1;
}


/**
 * Deletes an animation.
 * @param pAn					TODO describtion.
 * @param pType					TODO describtion.
 */
bool IND_AnimationManager::remove(IND_Animation *pAn, bool pType) {
	g_debug->header("Freeing animation", DebugApi::LogHeaderBegin);

	if (!_ok) {
		writeMessage();
		return 0;
	}

	// Search object
	bool mIs = 0;
	list <IND_Animation *>::iterator mAnimationListIter;
	for (mAnimationListIter  = _listAnimations->begin();
	        mAnimationListIter != _listAnimations->end();
	        mAnimationListIter++) {
		if ((*mAnimationListIter) == pAn) {
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

	g_debug->header("Name:", DebugApi::LogHeaderInfo);
	g_debug->dataChar(pAn->_animation._name, 1);

	// ------ Free all the surfaces and animations -----

	vector <IND_Frame *>::iterator mVectorFramesIter;
	for (mVectorFramesIter  = pAn->_vectorFrames->begin();
	        mVectorFramesIter  != pAn->_vectorFrames->end();
	        mVectorFramesIter++) {
		// Free image
		if ((*mVectorFramesIter)->getImage())
			_imageManager->remove((*mVectorFramesIter)->getImage());

		// Free surface
		if ((*mVectorFramesIter)->getSurface())
			_surfaceManager->remove((*mVectorFramesIter)->getSurface());

		// Free frame
		DISPOSE(*mVectorFramesIter);
	}

    
	if (pType) return 1;

	// Quit from list
	delFromlist(pAn);

	g_debug->header("Ok", DebugApi::LogHeaderEnd);

	return 1;
}


/**
 * Checks if a frame name has been already declared or not in the frames section.
 * @param pFrameName				TODO describtion.
 * @param pNewAnimation				TODO describtion.
 * @param pPos					TODO describtion.
 */
bool IND_AnimationManager::isDeclaredFrame(const char *pFrameName, IND_Animation *pNewAnimation, int *pPos) {
	int _pos = 0;

	vector <IND_Frame *>::iterator mVectorFrameIter;

	for (mVectorFrameIter  = pNewAnimation->_vectorFrames->begin();
	        mVectorFrameIter  != pNewAnimation->_vectorFrames->end();
	        mVectorFrameIter++) {
		if (!strcmp(pFrameName, (*mVectorFrameIter)->getName())) {
			*pPos = _pos;
			return 1;
		}
		_pos++;
	}

	return 0;
}


/**
 * Inserts an animation into the manager.
 * @param pNewAnimation				The animation that is to be inserted into manager.
 */
void IND_AnimationManager::addToList(IND_Animation *pNewAnimation) {
	_listAnimations->push_back(pNewAnimation);
}


/**
 * Removes an animation from the manager.
 * @param pAn					The animation that is to be removed from the manager.
 */
void IND_AnimationManager::delFromlist(IND_Animation *pAn) {
	_listAnimations->remove(pAn);
    DISPOSEMANAGED(pAn);
}


/**
 * Initialization error message.
 */
void IND_AnimationManager::writeMessage() {
	g_debug->header("This operation can not be done", DebugApi::LogHeaderInfo);
	g_debug->dataChar("", 1);
	g_debug->header("Invalid Id or not correctly initialized AnimationManager", DebugApi::LogHeaderError);
}


/**
 * Init manager variables.
 */
void IND_AnimationManager::initVars() {
	_listAnimations = new list <IND_Animation *>;
}


/**
 * Free manager variables.
 */
void IND_AnimationManager::freeVars() {
	// Releases everything in the anim
	list <IND_Animation *>::iterator mAnimationListIter;
	for (mAnimationListIter  = _listAnimations->begin();
	        mAnimationListIter != _listAnimations->end();
	        mAnimationListIter++) {
		
        
		remove((*mAnimationListIter), 1);

		g_debug->header("Ok", DebugApi::LogHeaderEnd);
	}

	// Clear list
	_listAnimations->clear();

	// Free list
	DISPOSE(_listAnimations);
}

/** @endcond */
