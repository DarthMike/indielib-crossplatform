/*****************************************************************************************
 * File: IND_Entity2dManager.cpp
 * Desc: Manager of 2D entities. Contains the graphic objects and manages their attributes.
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

#include "Defines.h"
#include "Global.h"
#include "dependencies/tinyxml/tinyxml.h"
#include "IND_Entity2dManager.h"
#include "IND_Surface.h"
#include "IND_Animation.h"
#include "IND_Frame.h"
#include "IND_SurfaceManager.h"
#include "IND_AnimationManager.h"
#include "IND_Render.h"
#include "CollisionParser.h"
#include "IND_Entity2d.h"
#include "IND_Math.h"

/** @cond DOCUMENT_PRIVATEAPI */

/**
 * For sorting the vector
 */
bool zIsLess(IND_Entity2d *pLhs, IND_Entity2d *pRhs) {
	if (pLhs->getPosZ() < pRhs->getPosZ())
		return 1;
	else
		return 0;
}

unsigned int IND_Entity2dManager::_idTrack = 0;

/** @endcond */

// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 * Returns 1(true) if the manager is initialized.
 * It must be called before using any method.
 * @param pRender				Pointer to the manager if IND_Render is initialized correctly.
 */
bool IND_Entity2dManager::init(IND_Render *pRender) {
	end();
	initVars();

	g_debug->header("Initializing Entity2dManager", DebugApi::LogHeaderBegin);
	g_debug->header("Preparing Entity2dManager", DebugApi::LogHeaderOk);

	_ok = true;

	// Checking IND_Render
	if (pRender->isOK()) {
		g_debug->header(" Checking IND_Render", DebugApi::LogHeaderOk);
		_render = pRender;
	} else {
		g_debug->header("IND_Render is not correctly initialized", DebugApi::LogHeaderError);
		_ok = false;
		return _ok;
	}

	_math = new IND_Math();
	_math->init();
	g_debug->header("Entity2dManager OK", DebugApi::LogHeaderEnd);

	return _ok;
}


/**
 * Frees the manager and all the objects that it contains.
 */
void IND_Entity2dManager::end() {
	if (_ok) {
		g_debug->header("Finalizing Entity2dManager", DebugApi::LogHeaderBegin);
		DISPOSE (_math);
		g_debug->header("Freeing 2d entities" , DebugApi::LogHeaderBegin);
		freeVars();
		g_debug->header("Entities freed", DebugApi::LogHeaderEnd);
		g_debug->header("IND_Entity2dManager finalized", DebugApi::LogHeaderEnd);

		_ok = false;
	}
}

/**
 Returns state of initialization.
 @return  Will give true if object initialized correctly, false otherwise
 */
bool IND_Entity2dManager::isOK()           const {
    return _ok;
}


// --------------------------------------------------------------------------------
//										Public methods
// --------------------------------------------------------------------------------

/**
 * Returns 1 (true) if the entity object passed as parameter exists
 * and is added to the manager successfully.
 * This method is equivalent to adding the entity to layer number 0.
 * @param pNewEntity2d				Pointer to an entity object.
 */
bool IND_Entity2dManager::add(IND_Entity2d *pNewEntity2d) {
	g_debug->header("Adding 2d entity", DebugApi::LogHeaderBegin);
	g_debug->header("Name:", DebugApi::LogHeaderInfo);
    pNewEntity2d->_id = _idTrack++;
	g_debug->dataInt(pNewEntity2d->getId(), 1);

	if (!_ok) {
		writeMessage();
		return 0;
	}

	// ----- Puts the entity into the manager -----

	addToList(0, pNewEntity2d);

	pNewEntity2d->setLayer(0);
	// ----- g_debug -----

	g_debug->header("2d entity added", DebugApi::LogHeaderEnd);

	return 1;
}

/**
 * Returns 1 (true) if the entity object passed as parameter exists
 * and is added to the manager successfully in a concrete layer.
 *
 * Layers are useful for having graphical objects in different levels. For example, you can have a terrain with buildings,
 * trees, etc; all in the same layer, and to have the score and gui buttons in a different layer that will be blitted always
 * upon the terrain. In this case, the terrain an all the objects on it could be in Layer[0]. The buttons and score could be
 * in an higher layer, for example Layer[1].
 *
 * The z values of the objects (see setPosition()) will be taken in count only between objects of the same layer.
 *
 * There are 64 different layers allowed (0 - 63).
 * @param pLayer				Number of layers (0 - 63 layers allowed).
 * @param pNewEntity2d                		Pointer to an entity object.
 */
bool IND_Entity2dManager::add(int pLayer, IND_Entity2d *pNewEntity2d) {
	g_debug->header("Adding 2d entity", DebugApi::LogHeaderBegin);
	g_debug->header("Name:", DebugApi::LogHeaderInfo);
    pNewEntity2d->_id = _idTrack++;
	g_debug->dataInt(pNewEntity2d->getId(), 1);
	g_debug->header("Layer:", DebugApi::LogHeaderInfo);
	g_debug->dataInt(pLayer, 1);

	if (!_ok) {
		writeMessage();
		return 0;
	}

	// Only allow NUM_LAYERS
	if (pLayer < 0 || pLayer > NUM_LAYERS - 1) return 0;

	// ----- Puts the entity into the manager -----

	addToList(pLayer, pNewEntity2d);

	pNewEntity2d->setLayer(pLayer);
	// ----- g_debug -----

	g_debug->header("2d entity added", DebugApi::LogHeaderEnd);

	return 1;
}

/**
 * Returns 1(true) if the entity object passed as parameter exists
 * and is deleted from the manager successfully.
 * @param pEn				Pointer to an entity object.
 */
bool IND_Entity2dManager::remove(IND_Entity2d *pEn) {
	g_debug->header("Freeing 2d entity", DebugApi::LogHeaderBegin);
	g_debug->header("Name:", DebugApi::LogHeaderInfo);
	g_debug->dataInt(pEn->getId(), 1);

	if (!_ok || !pEn) {
		writeMessage();
		return 0;
	}

	// Search object in all the layers
	for (int i = 0; i < NUM_LAYERS; i++) {
		bool mIs = 0;
		vector <IND_Entity2d *>::iterator _listIter;
		for (_listIter  = _listEntities2d[i]->begin();
		        _listIter  != _listEntities2d[i]->end();
		        _listIter++) {
			if ((*_listIter) == pEn) {
				mIs = 1;
				break;
			}
		}

		// Found
		if (mIs) {
			// ----- Delete object from list -----

			// Quit from list
			_listEntities2d[i]->erase(_listIter);

			g_debug->header("Ok", DebugApi::LogHeaderEnd);

			return 1;
		}
	}

	g_debug->header("Entity not found", DebugApi::LogHeaderEnd);
	return 0;
}

/**
 * Renders (draws on the screen) all the entities of the manager of a concrete layer.
 */
void IND_Entity2dManager::renderEntities2d(int pLayer) {
	if (!_ok || _listEntities2d[pLayer]->empty()) return;

	// Sort the list by z value ONLY if the z value of an entity has changed
	// TODO: How to know if an entity has changed z-value from here int order to avoid sorting?
	sort(_listEntities2d[pLayer]->begin(), _listEntities2d[pLayer]->end(), zIsLess);

	//Set cull region
	_render->reCalculateFrustrumPlanes();

	// Iterates the list
	vector <IND_Entity2d *>::iterator mIter;
	for (mIter  = _listEntities2d[pLayer]->begin();
	        mIter != _listEntities2d[pLayer]->end();
	        mIter++) {
		// Only render it if "show" flag is true
		if ((*mIter)->_show) {
			// If it has an animation or a surface assigned
			if ((*mIter)->_su || (*mIter)->_an) {
				// Set transformations ONLY if the entity space attributes has been modified
				if ((*mIter)->_updateTransFlag) {
					(*mIter)->_updateTransFlag = 0;

					int mWidthTemp = 0;
					int mHeightTemp = 0;

					// ---- We obtain the width and height of the animation or the surface -----

					if ((*mIter)->_su) {
						mWidthTemp  = (*mIter)->_su->getWidth();
						mHeightTemp = (*mIter)->_su->getHeight();
					} else {
						if ((*mIter)->_an->getActualSurface((*mIter)->getSequence())) {
							mWidthTemp  = (*mIter)->_an->getActualSurface((*mIter)->getSequence())->getWidth();
							mHeightTemp = (*mIter)->_an->getActualSurface((*mIter)->getSequence())->getHeight();
						}
					}

					// ----- Transformations -----

					_render->setTransform2d((int)(*mIter)->_x,
					                        (int)(*mIter)->_y,
					                        (*mIter)->_angleX,
					                        (*mIter)->_angleY,
					                        (*mIter)->_angleZ,
					                        (*mIter)->_scaleX,
					                        (*mIter)->_scaleY,
					                        (*mIter)->_axisCalX,
					                        (*mIter)->_axisCalY,
					                        (*mIter)->_mirrorX,
					                        (*mIter)->_mirrorY,
					                        mWidthTemp,
					                        mHeightTemp,
					                        &(*mIter)->_mat);
				} else {
					_render->setTransform2d((*mIter)->_mat);
				}

				// ----- Color attributes -----

				_render->setRainbow2d((*mIter)->getType(),
				                      (*mIter)->_cull,
				                      (*mIter)->_mirrorX,
				                      (*mIter)->_mirrorY,
				                      (*mIter)->_filter,
				                      (*mIter)->_r,
				                      (*mIter)->_g,
				                      (*mIter)->_b,
				                      (*mIter)->_a,
				                      (*mIter)->_fadeR,
				                      (*mIter)->_fadeG,
				                      (*mIter)->_fadeB,
				                      (*mIter)->_fadeA,
				                      (*mIter)->_so,
				                      (*mIter)->_ds);

				// ----- Surface blitting -----

				if ((*mIter)->_su) {
					// Surface region specified
					if (((*mIter)->_regionWidth > 0) && ((*mIter)->_regionHeight > 0)) {
						// X or Y wrapping
						if ((*mIter)->_wrap) {
							_render->blitWrapSurface((*mIter)->_su,
							                         (*mIter)->_regionWidth,
							                         (*mIter)->_regionHeight,
							                         (*mIter)->_uDisplace,
							                         (*mIter)->_vDisplace);
						}
						// No wrapping
						else {
							_render->blitRegionSurface((*mIter)->_su,
							                           (*mIter)->_offX,
							                           (*mIter)->_offY,
							                           (*mIter)->_regionWidth,
							                           (*mIter)->_regionHeight);
						}
					}
					// Blits all the surface
					else
						_render->blitSurface((*mIter)->_su);
				}

				// ----- Animation blitting -----

				else {
					// Animation is looping
					if ((*mIter)->_numReplays == -1) {
						// Blits the animation, returns -1 when finishes
						if (_render->blitAnimation((*mIter)->_an,
						                           (*mIter)->_sequence,
						                           (*mIter)->_offX,
						                           (*mIter)->_offY,
						                           (*mIter)->_regionWidth,
						                           (*mIter)->_regionHeight,
						                           (*mIter)->_wrap,
						                           (*mIter)->_uDisplace,
						                           (*mIter)->_vDisplace) == -1) {
							// Reset the animation
							(*mIter)->_an->setActualFramePos((*mIter)->_sequence, 0);
						}
					} else
						// If there is a stablished number or replays
					{
						// Blits the animation, returns -1 when finishes
						if (_render->blitAnimation((*mIter)->_an,
						                           (*mIter)->_sequence,
						                           (*mIter)->_offX,
						                           (*mIter)->_offY,
						                           (*mIter)->_regionWidth,
						                           (*mIter)->_regionHeight,
						                           (*mIter)->_wrap,
						                           (*mIter)->_uDisplace,
						                           (*mIter)->_vDisplace) == -1) {
							// There are replays
							if ((*mIter)->_numReplays > 0) {
								// Reset animation
								(*mIter)->_an->setActualFramePos((*mIter)->_sequence, 0);

								// Decrease the number of replays
								(*mIter)->_numReplays--;
							}
							// There are no replays
							else {
								// Blits the last frame
								(*mIter)->_an->setActualFramePos((*mIter)->_sequence, (*mIter)->_an->getNumFrames((*mIter)->_sequence) - 1);
							}
						}
					}
				}
			} else
				// If it has a 2d primitive assigned
				if ((*mIter)->_pri2d) {
					switch ((*mIter)->_pri2d) {
						// Pixel
					case IND_PIXEL: {

						_render->blitPixel((int)(*mIter)->_x,
						                   (int)(*mIter)->_y,
						                   (*mIter)->_r,
						                   (*mIter)->_g,
						                   (*mIter)->_b,
						                   (*mIter)->_a);
						break;
					}

					// Regular polygon
					case IND_REGULAR_POLY: {

						_render->blitRegularPoly((int)(*mIter)->_x,
						                         (int)(*mIter)->_y,
						                         (*mIter)->_radius,
						                         (*mIter)->_numSides,
						                         (*mIter)->_polyAngle,
						                         (*mIter)->_r,
						                         (*mIter)->_g,
						                         (*mIter)->_b,
						                         (*mIter)->_a);
						break;
					}

					// Rectangle
					case IND_RECTANGLE: {

						_render->blitRectangle((*mIter)->_x1,
						                       (*mIter)->_y1,
						                       (*mIter)->_x2,
						                       (*mIter)->_y2,
						                       (*mIter)->_r,
						                       (*mIter)->_g,
						                       (*mIter)->_b,
						                       (*mIter)->_a);
						break;
					}

					// Fill rectangle
					case IND_FILL_RECTANGLE: {
						_render->blitFillRectangle((*mIter)->_x1,
						                           (*mIter)->_y1,
						                           (*mIter)->_x2,
						                           (*mIter)->_y2,
						                           (*mIter)->_r,
						                           (*mIter)->_g,
						                           (*mIter)->_b,
						                           (*mIter)->_a);
						break;
					}

					// Poly2d
					case IND_POLY2D: {

						_render->blitPoly2d((*mIter)->_polyPoints,
						                    (*mIter)->_numLines,
						                    (*mIter)->_r,
						                    (*mIter)->_g,
						                    (*mIter)->_b,
						                    (*mIter)->_a);
						break;
					}

					// Line
					case IND_LINE: {

						_render->blitLine((*mIter)->_x1,
						                  (*mIter)->_y1,
						                  (*mIter)->_x2,
						                  (*mIter)->_y2,
						                  (*mIter)->_r,
						                  (*mIter)->_g,
						                  (*mIter)->_b,
						                  (*mIter)->_a);
						break;
					}
					}
				} else
					// If it has a font assigned
					if ((*mIter)->_font) {

						_render->blitText((*mIter)->_font,
						                  (*mIter)->_text,
						                  (int)(*mIter)->_x,
						                  (int)(*mIter)->_y,
						                  (*mIter)->_charSpacing,
						                  (*mIter)->_lineSpacing,
						                  (*mIter)->_scaleX,
						                  (*mIter)->_scaleY,
						                  (*mIter)->_r,
						                  (*mIter)->_g,
						                  (*mIter)->_b,
						                  (*mIter)->_a,
						                  (*mIter)->_fadeR,
						                  (*mIter)->_fadeG,
						                  (*mIter)->_fadeB,
						                  (*mIter)->_fadeA,
						                  (*mIter)->_filter,
						                  (*mIter)->_so,
						                  (*mIter)->_ds,
						                  (*mIter)->_align);
					}
		}
	}
}

/**
 * Renders (blits on the screen) all the collision areas of the entities. It's good to use this method
 * in order to check that our collision areas are accurate.
 * This method is equivalent to rendering the collision areas of layer number 0.
 */
void IND_Entity2dManager::renderCollisionAreas(unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA) {
	renderCollisionAreas(0, pR, pG, pB, pA);
}


/**
 * This function renders (blits on the screen) all the collision areas of the entities of a certain layer. It's good to use
 * this method in order to check that our collision areas are accurate.
 */
void IND_Entity2dManager::renderCollisionAreas(int pLayer, unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA) {
	if (!_ok || _listEntities2d[pLayer]->empty()) return;

	// Iterate the list
	vector <IND_Entity2d *>::iterator mIter;
	for (mIter  = _listEntities2d[pLayer]->begin();
	        mIter != _listEntities2d[pLayer]->end();
	        mIter++) {
		// If it has an animation or a surface assigned
		if ((*mIter)->_su || (*mIter)->_an) {
			if ((*mIter)->isShowCollisionAreas()) {
				list <BOUNDING_COLLISION *> *mBoundingListToRender(NULL);

				// Bounding list of a surface
				if ((*mIter)->_su) {
					mBoundingListToRender = (*mIter)->_listBoundingCollision;
				}

				// Bounding list of a frame in an animation
				if ((*mIter)->_an) {

					vector <IND_Frame*> *frames = (*mIter)->_an->getVectorFrames();
					unsigned int framePos = (*mIter)->_an->getActualFramePos((*mIter)->getSequence());
					if (frames && frames->size() > framePos) {
						mBoundingListToRender = (*frames) [framePos]->GetListBoundingCollision();
					}
				}

				if (mBoundingListToRender) {
					list <BOUNDING_COLLISION *>::iterator _listIter;
					for (_listIter  = mBoundingListToRender->begin();
					        _listIter != mBoundingListToRender->end();
					        _listIter++) {
						switch ((*_listIter)->getType()) {
							// Triangle
						case 0: {
							_render->blitCollisionLine((*_listIter)->_ax, (*_listIter)->_ay, (*_listIter)->_bx, (*_listIter)->_by, pR, pG, pB, pA, (*mIter)->_mat);
							_render->blitCollisionLine((*_listIter)->_bx, (*_listIter)->_by, (*_listIter)->_cx, (*_listIter)->_cy, pR, pG, pB, pA, (*mIter)->_mat);
							_render->blitCollisionLine((*_listIter)->_cx, (*_listIter)->_cy, (*_listIter)->_ax, (*_listIter)->_ay, pR, pG, pB, pA, (*mIter)->_mat);
							break;
						}

						// Circle
						case 1: {
							_render->blitCollisionCircle((*_listIter)->_posX, (*_listIter)->_posY, (*_listIter)->_radius, (*mIter)->_scaleX, pR, pG, pB, pA, (*mIter)->_mat);
							break;
						}
						}
					}
				}
			}
		}
	}
}



/**
 * Renders (blits on the screen) all the grid areas of the entities of a certain layer.
 */
void IND_Entity2dManager::renderGridAreas(int pLayer, unsigned char pR, unsigned char pG, unsigned char pB, unsigned char pA) {
	if (!_ok || _listEntities2d[pLayer]->empty()) return;

	// Iterate the list
	vector <IND_Entity2d *>::iterator mIter;
	for (mIter  = _listEntities2d[pLayer]->begin();
	        mIter != _listEntities2d[pLayer]->end();
	        mIter++) {
		// If it has an animation or a surface assigned
		if ((*mIter)->_su || (*mIter)->_an) {
			if ((*mIter)->isShowGridAreas()) {
				IND_Surface *surface(NULL);

				// Surface
				if ((*mIter)->_su) {
					surface = (*mIter)->_su;
				}

				// Surface of current frame
				if ((*mIter)->_an) {
					surface = (*mIter)->_an->getActualSurface((*mIter)->getSequence());
				}

				if (surface) {
					int mWidthTemp  = surface->getWidth();
					int mHeightTemp = surface->getHeight();

					_render->setTransform2d((int)(*mIter)->_x,
											(int)(*mIter)->_y,
											(*mIter)->_angleX,
											(*mIter)->_angleY,
											(*mIter)->_angleZ,
											(*mIter)->_scaleX,
											(*mIter)->_scaleY,
											(*mIter)->_axisCalX,
											(*mIter)->_axisCalY,
											(*mIter)->_mirrorX,
											(*mIter)->_mirrorY,
											mWidthTemp,
											mHeightTemp,
											&(*mIter)->_mat);

					_render->blitGrid(surface, pR, pG, pB, pA);
				}
			}
		}
	}
}

/**
 * Checks if two entities have collided in any of their bounding areas.
 * Returns 1 if there is a collision. The id is a string of characters that identifies a collision group.
 * For creating the bounding areas and defining collision groups you can use the following methods:
 * - IND_Entity2d::setBoundingAreas()
 * - IND_Entity2d::setBoundingCircle()
 * - IND_Entity2d::setBoundingTriangle()
 * - IND_Entity2d::setBoundingRectangle()
 *
 * Important: This function only works with surfaces or animation entities.
 *
 * Rotations allowed of the object: only in the z axis.
 * Scaling allowed of the object: yes.
 *
 * Use IND_Entity2d::showCollisionAreas() method in order to show or hide the collision areas of an entity.
 * In order to render all the collision areas you should use the
 * IND_Entity2dManager::renderCollisionAreas() method.
 * @param pEn1						Pointer to an entity object.
 * @param pId1						Id of a group of collison areas. Use "*" for checking all the groups.
 * @param pEn2						Pointer to an entity object.
 * @param pId2						Id of a group of collison areas. Use "*" for checking all the groups.
 */
bool IND_Entity2dManager::isCollision(IND_Entity2d *pEn1, const char *pId1, IND_Entity2d *pEn2, const char *pId2) {
	if (!pEn1->_su && !pEn1->_an) return 0;
	if (!pEn2->_su && !pEn2->_an) return 0;

	list <BOUNDING_COLLISION *> *mBoundingList1;
	list <BOUNDING_COLLISION *> *mBoundingList2;

	// Is a surface
	if (pEn1->_su) {
		mBoundingList1 = pEn1->_listBoundingCollision;
	}
	// Is an animation
	else {
		mBoundingList1 = (*(pEn1->_an->getVectorFrames())) [pEn1->_an->getActualFramePos(pEn1->getSequence())]->GetListBoundingCollision();
	}

	// Is a surface
	if (pEn2->_su) {
		mBoundingList2 = pEn2->_listBoundingCollision;
	}
	// Is an animation
	else {
		mBoundingList2 = (*(pEn2->_an->getVectorFrames())) [pEn2->_an->getActualFramePos(pEn2->getSequence())]->GetListBoundingCollision();
	}

	if (isCollision(mBoundingList1, mBoundingList2,
	                pId1, pId2,
	                pEn1->_mat, pEn2->_mat,
	                pEn1->_scaleX, pEn2->_scaleX))
		return 1;

	return 0;
}


// --------------------------------------------------------------------------------
//									 Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
==================
Check the collision between bounding areas
==================
*/
inline bool IND_Entity2dManager::isCollision(list <BOUNDING_COLLISION *> *pBoundingList1, list <BOUNDING_COLLISION *> *pBoundingList2,
        const char *pId1, const char *pId2,
        IND_Matrix pMat1, IND_Matrix pMat2,
        float pScale1, float pScale2) {
	list <BOUNDING_COLLISION *>::iterator i;
	list <BOUNDING_COLLISION *>::iterator j;

	bool mCollision = 0;

	// Check all the collisions
	for (i  = pBoundingList1->begin();
	        i != pBoundingList1->end();
	        i++) {
		for (j  = pBoundingList2->begin();
		        j != pBoundingList2->end();
		        j++) {
			// Check only if the group is correct
            const char* const id1 = (*i)->getId();
            const char* const id2 = (*j)->getId();
			if (    (!strcmp(pId1, id1)  && !strcmp(pId2, id2))   ||      // "group1", "group2"
			        (!strcmp(pId1, "*"      )  && !strcmp(id2, pId2))   ||      // "*", "group"
			        (!strcmp(id1, pId1)  && !strcmp(pId2 , "*")     )   ||      // "group", "*"
			        (!strcmp(pId1, "*"      )  && !strcmp(pId2 , "*")     )     ) {   // "*", "*"
				
				// Triangle to triangle
				if ((*i)->getType() == 0 && (*j)->getType() == 0) {
					if (_math->isTriangleToTriangleCollision((*i), pMat1, (*j), pMat2))
						mCollision = 1;
				}

				// Circle to triangle
				if ((*i)->getType() == 1 && (*j)->getType() == 0) {
					if (_math->isCircleToTriangleCollision((*i), pMat1, pScale1, (*j), pMat2))
						mCollision = 1;
				}

				// Triangle to circle
				if ((*i)->getType() == 0 && (*j)->getType() == 1) {
					if (_math->isCircleToTriangleCollision((*j), pMat2, pScale2, (*i), pMat1))
						mCollision = 1;
				}

				// Circle to circle
				if ((*i)->getType() == 1 && (*j)->getType() == 1) {
					if (_math->isCircleToCircleCollision((*i), pMat1, pScale1, (*j), pMat2, pScale2))
						mCollision = 1;
				}
			}
		}
	}

	return mCollision;
}


/*
==================
Checks if the matrix has all its member equal to zero
==================
*/
bool IND_Entity2dManager::isNullMatrix(IND_Matrix pMat) {
	if (!pMat._11 && !pMat._12  && !pMat._13 && !pMat._14 &&
	        !pMat._21 && !pMat._22  && !pMat._23 && !pMat._24 &&
	        !pMat._31 && !pMat._32  && !pMat._33 && !pMat._34 &&
	        !pMat._41 && !pMat._42  && !pMat._43 && !pMat._44)
		return 1;

	return 0;
}


/*
==================
Inserts object into the manager in a certain layer
==================
*/
void IND_Entity2dManager::addToList(int pLayer, IND_Entity2d *pNewEntity2d) {
	_listEntities2d[pLayer]->push_back(pNewEntity2d);
}


/*
==================
Initialization error message
==================
*/
void IND_Entity2dManager::writeMessage() {
	g_debug->header("This operation can not be done", DebugApi::LogHeaderInfo);
	g_debug->dataChar("", 1);
	g_debug->header("Invalid Id or IND_Entity2dManager not correctly initialized", DebugApi::LogHeaderError);
}


/*
==================
Init manager vars
==================
*/
void IND_Entity2dManager::initVars() {
	for (int i = 0; i < NUM_LAYERS; i++)
		_listEntities2d [i] = new vector <IND_Entity2d *>;
}


/*
==================
Free manager variables
==================
*/
void IND_Entity2dManager::freeVars() {
	// Deletes all the manager entities
	vector <IND_Entity2d *>::iterator mEntityListIter;

	for (int i = 0; i < NUM_LAYERS; i++) {
		for (mEntityListIter  = _listEntities2d[i]->begin();
		        mEntityListIter != _listEntities2d[i]->end();
		        mEntityListIter++) {
			g_debug->header("Freeing 2d entity:", DebugApi::LogHeaderInfo);

			// Delete all the bounding areas
			(*mEntityListIter)->deleteBoundingAreas((char*)"*");
            g_debug->dataInt((*mEntityListIter)->getId(), 1);
            DISPOSEMANAGED((*mEntityListIter));
		}

		// Clear list
		_listEntities2d[i]->clear();

		// Free list
		DISPOSE(_listEntities2d[i]);
	}
}

/** @endcond */





