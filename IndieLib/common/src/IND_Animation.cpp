/*****************************************************************************************
 * File: IND_Animation.h
 * Desc: Animation object
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


// ----- Includes -----

#include "Global.h"
#include "IND_Animation.h"
#include "IND_Frame.h"
#include "IND_Sequence.h"

// --------------------------------------------------------------------------------
//										Public methods
// --------------------------------------------------------------------------------

/**
 *  Factory method to build instances of this class.
 *
 *  You should never allocate instances by calling new or allocating objects in the stack.
 *  All memory related to this instance is managed by Indielib internally, and you must add it or remove it from
 *  a manager.
 *
 *  @return A correctly initialized instance
 */
IND_Animation* IND_Animation::newAnimation() {
    return new IND_Animation();
}

void IND_Animation::destroy() {
    delete this;
}


/**
 * Get the image of a frame in the animation.
 * @param pFrame			The frame number of a frame in the animation.
 */
IND_Image *IND_Animation::getImage(unsigned int pFrame) {
	IND_Image* img = NULL;
	vector<IND_Frame*> *frames = getVectorFrames();
	if (frames && frames->size() > pFrame) {
		img = (*frames) [pFrame]->getImage();
	}
	return img;
}

/**
 * Get the surface of a frame in the animation.
 * @param pFrame			The frame number of a frame in the animation.
 */
IND_Surface *IND_Animation::getSurface(unsigned int pFrame) {
	IND_Surface* surf = NULL;
	vector<IND_Frame*> *frames = getVectorFrames();
	if (frames && frames->size() > pFrame) {
		 surf = (*frames) [pFrame]->getSurface();
	}
	return surf;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
unsigned int IND_Animation::getHighWidth(unsigned int pSequence) {
	unsigned int hw = 0;
	vector <IND_Sequence *> *sequences = getListSequences();
	if (sequences && sequences->size() > pSequence) {
		hw = (*sequences) [pSequence]->getHighWidth();
	}
	return hw;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
unsigned int IND_Animation::getHighHeight(unsigned int pSequence) {
	unsigned int hh = 0;
	vector <IND_Sequence *> *sequences = getListSequences();
	if (sequences && sequences->size() > pSequence) {
		hh = (*sequences) [pSequence]->getHighHeight();
	}
	return hh;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
unsigned int IND_Animation::getNumFrames(unsigned int pSequence) {
	unsigned int numFrames = 0;
	vector <IND_Sequence *> *sequences = getListSequences();
	if (sequences && sequences->size() > pSequence) {
		numFrames = (*getListSequences()) [pSequence]->getNumFrames();
	}
	return numFrames;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
const char* const IND_Animation::getName(unsigned int pSequence) {
	vector <IND_Sequence *> *sequences = getListSequences();
	if (sequences && sequences->size() > pSequence) {
        return ((*sequences) [pSequence]->getName());
    } else {
        return NULL;
    }
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
unsigned int IND_Animation::getActualFramePos(unsigned int pSequence) {
	unsigned int framePos = 0;
	vector <IND_Sequence *> *sequences = getListSequences();
	if (sequences && sequences->size() > pSequence) {
		framePos = (*sequences) [pSequence]->getActualFramePos();
	}
	return framePos;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
unsigned int IND_Animation::getActualFrameTime(unsigned int pSequence) {
	unsigned int frameTime = 0;
	vector <IND_Sequence *> *sequences = getListSequences();
	if (sequences && sequences->size() > pSequence) {
		frameTime = (*sequences) [pSequence]->getActualFrameTime();
	}
	return frameTime;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
unsigned int IND_Animation::getActualOffsetX(unsigned int pSequence) {
	unsigned int offset = 0;
	if (getVectorFrames() && getListSequences()) {
		offset = (*getVectorFrames()) [(*getListSequences()) [pSequence]->getActualFramePosInVec()]->GetOffsetX();
	}
	return offset;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
unsigned int IND_Animation::getActualOffsetY(unsigned int pSequence) {
	unsigned int offset = 0;
	vector<IND_Frame*> *frames = getVectorFrames();
	if (frames && frames->size() > pSequence && getListSequences()) {
		offset = (*frames) [(*getListSequences()) [pSequence]->getActualFramePosInVec()]->GetOffsetY();
	}
	return offset;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
IND_Surface *IND_Animation::getActualSurface(unsigned int pSequence) {
	IND_Surface* actualSurf = NULL;
	vector<IND_Frame*> *frames = getVectorFrames();
	vector <IND_Sequence *> *sequences = getListSequences();
	if (frames && frames->size() > pSequence && sequences && sequences->size() > pSequence) {
		actualSurf = (*getVectorFrames()) [(*getListSequences()) [pSequence]->getActualFramePosInVec()]->getSurface();
	}

	return actualSurf;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 * @param pPos                          TODO describtion.
 */
void IND_Animation::setActualFramePos(unsigned int pSequence, unsigned int pPos) {
	vector <IND_Sequence *> *sequences = getListSequences();
	if (sequences && sequences->size() > pSequence) {
		(*sequences) [pSequence]->setActualFramePos(pPos);
	}
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
IND_Timer *IND_Animation::getSequenceTimer(unsigned int pSequence) {
	IND_Timer* timer = NULL;
	vector <IND_Sequence *> *sequences = getListSequences();
	if (sequences && sequences->size() > pSequence) {
		timer = (*sequences) [pSequence]->getSequenceTimer();
	}
	return timer;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
bool IND_Animation::getIsActive(unsigned int pSequence) {
	bool active = false;
	vector <IND_Sequence *> *sequences = getListSequences();
	if (sequences && sequences->size() > pSequence) {
		active = (*sequences) [pSequence]->getIsActive();
	}
	return active;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 * @param pAct                          TODO describtion.
 */
void IND_Animation::setIsActive(unsigned int pSequence, bool pAct) {
	vector <IND_Sequence *> *sequences = getListSequences();
	if (sequences && sequences->size() > pSequence) {
		(*sequences) [pSequence]->setIsActive(pAct);
	}
}

/**
 * TODO describtion.
 * @param pFrame			The frame number of a frame in the animation.
 * @param pNewImage                     The image that is to be set on the frame.
 */
void IND_Animation::setImage(unsigned int pFrame, IND_Image *pNewImage) {
	vector<IND_Frame*> *frames = getVectorFrames();
	if (frames && frames->size() > pFrame) {
		(*getVectorFrames()) [pFrame]->setImage(pNewImage);
	}
}

/**
 * TODO describtion.
 * @param pFrame			The frame number of a frame in the animation.
 * @param pNewSurface                   The surface that is to be set on the frame.
 */
void IND_Animation::setSurface(unsigned int pFrame, IND_Surface *pNewSurface) {
	vector<IND_Frame*> *frames = getVectorFrames();
	if (frames && frames->size() > pFrame) {
		(*getVectorFrames()) [pFrame]->setSurface(pNewSurface);
	}
}
