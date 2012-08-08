/*****************************************************************************************
 * File: IND_Entity2dManager.cpp
 * Desc: Manager of 2D entities. Contains the graphic objects and manages their attributes.
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


/**
 * For sorting the vector
 */
bool zIsLess(IND_Entity2d *pLhs, IND_Entity2d *pRhs) { //TODO: Move this method down...
	if (pLhs->getPosZ() < pRhs->getPosZ())
		return 1;
	else
		return 0;
}


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

	g_debug->header("Initializing Entity2dManager", 5);
	g_debug->header("Preparing Entity2dManager", 1);

	_ok = true;

	// Checking IND_Render
	if (pRender->isOK()) {
		g_debug->header(" Checking IND_Render", 1);
		_render = pRender;
	} else {
		g_debug->header("IND_Render is not correctly initialized", 2);
		_ok = false;
		return _ok;
	}

	g_debug->header("Entity2dManager OK", 6);

	return _ok;
}


/**
 * Frees the manager and all the objects that it contains.
 */
void IND_Entity2dManager::end() {
	if (_ok) {
		g_debug->header("Finalizing Entity2dManager", 5);
		g_debug->header("Freeing 2d entities" , 5);
		freeVars();
		g_debug->header("Entities freed", 6);
		g_debug->header("IND_Entity2dManager finalized", 6);

		_ok = false;
	}
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
	g_debug->header("Adding 2d entity", 5);
	g_debug->header("Name:", 3);
    //FIXME: This breaks on LLVM compiler (OSX, XCode)
	//g_debug->dataInt((int) pNewEntity2d, 1);

	if (!_ok) {
		writeMessage();
		return 0;
	}

	// ----- Puts the entity into the manager -----

	addToList(0, pNewEntity2d);

	pNewEntity2d->setLayer(0);
	// ----- g_debug -----

	g_debug->header("2d entity added", 6);

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
 * The z values of the objects (see ::setPosition()) will be taken in count only between objects of the same layer.
 *
 * There are 64 different layers allowed (0 - 63).
 * @param pLayer				Number of layers (0 - 63 layers allowed).
 * @param pNewEntity2d                		Pointer to an entity object.
 */
bool IND_Entity2dManager::add(int pLayer, IND_Entity2d *pNewEntity2d) {
	g_debug->header("Adding 2d entity", 5);
	g_debug->header("Name:", 3);
    //FIXME: This breaks on LLVM compiler (OSX, XCode)
	//g_debug->dataInt((int) pNewEntity2d, 1);
	g_debug->header("Layer:", 3);
	g_debug->dataInt((int) pLayer, 1);

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

	g_debug->header("2d entity added", 6);

	return 1;
}

/**
 * Returns 1(true) if the entity object passed as parameter exists
 * and is deleted from the manager successfully.
 * @param pNewEntity2d				Pointer to an entity object.
 */
bool IND_Entity2dManager::remove(IND_Entity2d *pEn) {
	g_debug->header("Freeing 2d entity", 5);
	g_debug->header("Name:", 3);
    //FIXME: This breaks on LLVM compiler (OSX, XCode)
	//g_debug->dataInt((int) pEn, 1);

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

			g_debug->header("Ok", 6);

			return 1;
		}
	}

	g_debug->header("Entity not found", 6);
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

	// Set culling region
	_render->calculeFrustumPlanes();
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
void IND_Entity2dManager::renderCollisionAreas(BYTE pR, BYTE pG, BYTE pB, BYTE pA) {
	renderCollisionAreas(0, pR, pG, pB, pA);
}


/**
 * This function renders (blits on the screen) all the collision areas of the entities of a certain layer. It's good to use
 * this method in order to check that our collision areas are accurate.
 */
void IND_Entity2dManager::renderCollisionAreas(int pLayer, BYTE pR, BYTE pG, BYTE pB, BYTE pA) {
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
					mBoundingListToRender = (*(*mIter)->_an->getVectorFrames()) [(*mIter)->_an->getActualFramePos((*mIter)->getSequence())]->GetListBoundingCollision();
				}
                assert(mBoundingListToRender);
				if (!mBoundingListToRender->empty()) {
					list <BOUNDING_COLLISION *>::iterator _listIter;
					for (_listIter  = mBoundingListToRender->begin();
					        _listIter != mBoundingListToRender->end();
					        _listIter++) {
						switch ((*_listIter)->_type) {
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
void IND_Entity2dManager::renderGridAreas(int pLayer, BYTE pR, BYTE pG, BYTE pB, BYTE pA) {
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
bool IND_Entity2dManager::isCollision(IND_Entity2d *pEn1, char *pId1, IND_Entity2d *pEn2, char *pId2) {
	if (!pEn1->_su && !pEn1->_an) return 0;
	if (!pEn2->_su && !pEn2->_an) return 0;

	// Apply transformation if the entities dont have a world matrix
	if (pEn1->isNullMatrix() || pEn2->isNullMatrix()) {
		int mWidthTemp, mHeightTemp;
		if (pEn1->_su) {
			mWidthTemp  = pEn1->_su->getWidth();
			mHeightTemp = pEn1->_su->getHeight();
		} else {
			mWidthTemp  = pEn1->_an->getActualSurface(pEn1->getSequence())->getWidth();
			mHeightTemp = pEn1->_an->getActualSurface(pEn1->getSequence())->getHeight();
		}

		IND_Matrix mMat1;
		_render->setTransform2d((int) pEn1->_x,
		                        (int) pEn1->_y,
		                        pEn1->_angleX,
		                        pEn1->_angleY,
		                        pEn1->_angleZ,
		                        pEn1->_scaleX,
		                        pEn1->_scaleY,
		                        pEn1->_axisCalX,
		                        pEn1->_axisCalY,
		                        pEn1->_mirrorX,
		                        pEn1->_mirrorY,
		                        mWidthTemp,
		                        mHeightTemp,
		                        &mMat1);
		pEn1->_mat = mMat1;


		if (pEn2->_su) {
			mWidthTemp  = pEn2->_su->getWidth();
			mHeightTemp = pEn2->_su->getHeight();
		} else {
			mWidthTemp  = pEn2->_an->getActualSurface(pEn2->getSequence())->getWidth();
			mHeightTemp = pEn2->_an->getActualSurface(pEn2->getSequence())->getHeight();
		}

		IND_Matrix mMat2;
		_render->setTransform2d((int) pEn2->_x,
		                        (int) pEn2->_y,
		                        pEn2->_angleX,
		                        pEn2->_angleY,
		                        pEn2->_angleZ,
		                        pEn2->_scaleX,
		                        pEn2->_scaleY,
		                        pEn2->_axisCalX,
		                        pEn2->_axisCalY,
		                        pEn2->_mirrorX,
		                        pEn2->_mirrorY,
		                        mWidthTemp,
		                        mHeightTemp,
		                        &mMat2);
		pEn2->_mat = mMat2;
	}

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

/*
==================
Check the collision between bounding areas
==================
*/
inline bool IND_Entity2dManager::isCollision(list <BOUNDING_COLLISION *> *pBoundingList1, list <BOUNDING_COLLISION *> *pBoundingList2,
        char *pId1, char *pId2,
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
			if (    (!strcmp(pId1, (*i)->_id)  && !strcmp(pId2, (*j)->_id))   ||      // "group1", "group2"
			        (!strcmp(pId1, "*"      )  && !strcmp((*j)->_id, pId2))   ||      // "*", "group"
			        (!strcmp((*i)->_id, pId1)  && !strcmp(pId2 , "*")     )   ||      // "group", "*"
			        (!strcmp(pId1, "*"      )  && !strcmp(pId2 , "*")     )     ) {   // "*", "*"
				
				// Triangle to triangle
				if ((*i)->_type == 0 && (*j)->_type == 0) {
					if (_render->isTriangleToTriangleCollision((*i), pMat1, (*j), pMat2))
						mCollision = 1;
				}

				// Circle to triangle
				if ((*i)->_type == 1 && (*j)->_type == 0) {
					if (_render->isCircleToTriangleCollision((*i), pMat1, pScale1, (*j), pMat2))
						mCollision = 1;
				}

				// Triangle to circle
				if ((*i)->_type == 0 && (*j)->_type == 1) {
					if (_render->isCircleToTriangleCollision((*j), pMat2, pScale2, (*i), pMat1))
						mCollision = 1;
				}

				// Circle to circle
				if ((*i)->_type == 1 && (*j)->_type == 1) {
					if (_render->isCircleToCircleCollision((*i), pMat1, pScale1, (*j), pMat2, pScale2))
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
	g_debug->header("This operation can not be done", 3);
	g_debug->dataChar("", 1);
	g_debug->header("Invalid Id or IND_Entity2dManager not correctly initialized", 2);
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
			g_debug->header("Freeing 2d entity:", 3);

			// Delete all the bounding areas
			(*mEntityListIter)->deleteBoundingAreas((char*)"*");
			//FIXME: This breaks on LLVM compiler (OSX, XCode)
			//g_debug->dataInt((int)(*mEntityListIter), 1);
		}

		// Clear list
		_listEntities2d[i]->clear();

		// Free list
		DISPOSE(_listEntities2d[i]);
	}
}







