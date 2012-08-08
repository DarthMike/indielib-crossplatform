/*****************************************************************************************
 * File: IND_Animation.h
 * Desc: Animation object
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
MODIFIED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)

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
#include "IND_Animation.h"
#include "IND_Frame.h"
#include "IND_Sequence.h"

// --------------------------------------------------------------------------------
//										Public methods
// --------------------------------------------------------------------------------

/**
 * Get the image of a frame in the animation.
 * @param pFrame			The frame number of a frame in the animation.
 */
IND_Image *IND_Animation::getImage(int pFrame) {
	IND_Image* img = NULL;
	if (getVectorFrames()) {
		img = (*getVectorFrames()) [pFrame]->getImage();
	}
	return img;
}

/**
 * Get the surface of a frame in the animation.
 * @param pFrame			The frame number of a frame in the animation.
 */
IND_Surface *IND_Animation::getSurface(int pFrame) {
	IND_Surface* surf = NULL;
	if (getVectorFrames()) {
		 surf = (*getVectorFrames()) [pFrame]->getSurface();
	}
	return surf;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
int IND_Animation::getHighWidth(int pSequence) {
	int hw = 0;
	if (getListSequences()) {
		hw = (*getListSequences()) [pSequence]->getHighWidth();
	}
	return hw;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
int IND_Animation::getHighHeight(int pSequence) {
	int hh = 0;
	if (getListSequences()) {
		hh = (*getListSequences()) [pSequence]->getHighHeight();
	}
	return hh;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
int IND_Animation::getNumFrames(int pSequence) {
	int numFrames = 0;
	if (getListSequences()) {
		numFrames = (*getListSequences()) [pSequence]->getNumFrames();
	}
	return numFrames;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
char *IND_Animation::getName(int pSequence) {
	char* name = (char*) "NO_ANIM_NAME";
	if (getListSequences()) {
		name = (*getListSequences()) [pSequence]->getName();
	}
	return name;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
int IND_Animation::getActualFramePos(int pSequence) {
	int framePos = 0;
	if (getListSequences()) {
		framePos = (*getListSequences()) [pSequence]->getActualFramePos();
	}
	return framePos;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
int IND_Animation::getActualFrameTime(int pSequence) {
	int frameTime = 0;
	if (getListSequences()) {
		frameTime = (*getListSequences()) [pSequence]->getActualFrameTime();
	}
	return frameTime;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
int IND_Animation::getActualOffsetX(int pSequence) {
	int offset = 0;
	if (getVectorFrames() && getListSequences()) {
		offset = (*getVectorFrames()) [(*getListSequences()) [pSequence]->getActualFramePosInVec()]->GetOffsetX();
	}
	return offset;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
int IND_Animation::getActualOffsetY(int pSequence) {
	int offset = 0;
	if (getVectorFrames() && getListSequences()) {
		offset = (*getVectorFrames()) [(*getListSequences()) [pSequence]->getActualFramePosInVec()]->GetOffsetY();
	}
	return offset;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
IND_Surface *IND_Animation::getActualSurface(int pSequence) {
	IND_Surface* actualSurf = NULL;
	if (getVectorFrames() && getListSequences()) {
		actualSurf = (*getVectorFrames()) [(*getListSequences()) [pSequence]->getActualFramePosInVec()]->getSurface();
	}

	return actualSurf;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 * @param pPos                          TODO describtion.
 */
void IND_Animation::setActualFramePos(int pSequence, int pPos) {
	if (getListSequences()) {
		(*getListSequences()) [pSequence]->setActualFramePos(pPos);
	}
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
IND_Timer *IND_Animation::getSequenceTimer(int pSequence) {
	IND_Timer* timer = NULL;
	if (getListSequences()) {
		timer = (*getListSequences()) [pSequence]->getSequenceTimer();
	}
	return timer;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 */
bool IND_Animation::getIsActive(int pSequence) {
	bool active = false;
	if (getListSequences()) {
		active = (*getListSequences()) [pSequence]->getIsActive();
	}
	return active;
}

/**
 * TODO describtion.
 * @param pSequence			The sequence number of a sequence in the list of sequences.
 * @param pAct                          TODO describtion.
 */
void IND_Animation::setIsActive(int pSequence, bool pAct) {
	if (getListSequences()) {
		(*getListSequences()) [pSequence]->setIsActive(pAct);
	}
}

/**
 * TODO describtion.
 * @param pFrame			The frame number of a frame in the animation.
 * @param pNewImage                     The image that is to be set on the frame.
 */
void IND_Animation::setImage(int pFrame, IND_Image *pNewImage) {
	if (getVectorFrames()) {
		(*getVectorFrames()) [pFrame]->setImage(pNewImage);
	}
}

/**
 * TODO describtion.
 * @param pFrame			The frame number of a frame in the animation.
 * @param pNewSurface                   The surface that is to be set on the frame.
 */
void IND_Animation::setSurface(int pFrame, IND_Surface *pNewSurface) {
	if (getVectorFrames()) {
		(*getVectorFrames()) [pFrame]->setSurface(pNewSurface);
	}
}
