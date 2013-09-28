/*****************************************************************************************
 * File: IND_Entity3dManager.cpp
 * Desc: Manager of 3D entities. Contains the graphic objects and manages their attributes.
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

#include "Defines.h"
#include "Global.h"
#include "IND_Entity3dManager.h"
#include "IND_Render.h"
#include "IND_3dMeshManager.h"
#include "IND_Entity3d.h"

// --------------------------------------------------------------------------------
//							  Initialization / Destruction
// --------------------------------------------------------------------------------

/**
 * Returns 1(true) if the manager is initialized satisfactorily.
 * It must be called before using any method.
 * @param pRender				Pointer to IND_Render.
 */
bool IND_Entity3dManager::init(IND_Render *pRender) {
	end();
	initVars();

	g_debug->header("Initializing Entity3dManager", 5);
	g_debug->header("Preparing Entity3dManager", 1);

	_ok = true;

	// Checking IND_Render
	if (pRender->isOK()) {
		g_debug->header("Checking IND_Render", 1);
		_render = pRender;
	} else {
		g_debug->header("IND_Render is not correctly itialized", 2);
		_ok = false;
		return _ok;
	}

	g_debug->header("Entity3dManager OK", 6);

	return _ok;
}

/**
 * Frees the manager and all the objects that it contains.
 */
void IND_Entity3dManager::end() {
	if (_ok) {
		g_debug->header("Finalizing Entity3dManager", 5);
		g_debug->header("Freeing 3d entities" , 5);
		freeVars();
		g_debug->header("3d entities freed", 6);
		g_debug->header("Entity3dManager finalized", 6);

		_ok = false;
	}
}

// --------------------------------------------------------------------------------
//									 Public methods
// --------------------------------------------------------------------------------

/**
 * Returns 1 (true) if the entity object passed as parameter exists
 * and is added to the manager successfully.
 * @param pNewEntity3d				Pointer to an entity object.
 */
bool IND_Entity3dManager::add(IND_Entity3d *pNewEntity3d) {
	g_debug->header("Adding 3d entity", 5);
	g_debug->header("Name:", 3);
	g_debug->dataInt((int) pNewEntity3d, 1);

	if (!_ok) {
		writeMessage();
		return 0;
	}

	// ----- Puts the entinty into the manager -----

	addToList(pNewEntity3d);

	// ----- g_debug -----

	g_debug->header("3d entity added", 6);

	return 1;
}

/**
 * Returns 1 (true) if the entity object passed as parameter exists
 * and is deleted from the manager successfully.
 * @param pNewEntity3d				Pointer to an entity object.
 */
bool IND_Entity3dManager::remove(IND_Entity3d *pEn) {
	g_debug->header("Freeing 3d entity", 5);

	if (!_ok || !pEn) {
		writeMessage();
		return 0;
	}

	// Search object
	bool mIs = 0;
	vector <IND_Entity3d *>::iterator _listIter;
	for (_listIter  = _listEntities3d->begin();
	        _listIter  != _listEntities3d->end();
	        _listIter++) {
		if ((*_listIter) == pEn) {
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

	g_debug->header("Name:", 3);
	g_debug->dataInt((int) pEn, 1);

	// Quit from list
	_listEntities3d->erase(_listIter);

	g_debug->header("Ok", 6);

	return 1;
}

/**
 * Renders (blits on the screen) all the 3d entities of the manager.
 */
void IND_Entity3dManager::renderEntities3d() {
	if (!_ok || _listEntities3d->empty()) {
		return;
	}

	// Iterates the list
	vector <IND_Entity3d *>::iterator mIter;
	for (mIter  = _listEntities3d->begin();
	        mIter != _listEntities3d->end();
	        mIter++) {
		// Only render it if "show" flag is true
		if ((*mIter)->_show) {
			// If it has a 3d Mesh
			if ((*mIter)->_me) {
				_render->setTransform3d((*mIter)->_x,
				                        (*mIter)->_y,
				                        (*mIter)->_z,
				                        (*mIter)->_angleX,
				                        (*mIter)->_angleY,
				                        (*mIter)->_angleZ,
				                        (*mIter)->_scaleX,
				                        (*mIter)->_scaleY,
				                        (*mIter)->_scaleZ,
				                        &(*mIter)->_mat);


				// ----- Color attributes -----

				_render->setRainbow3d((*mIter)->_cull,
				                      (*mIter)->_flipNormals,
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

				// ----- Blit Mesh -----

				if ((*mIter)->_changeSequenceFlag) {
					(*mIter)->_changeSequenceFlag = 0;
					_render->set3dMeshSequence((*mIter)->_me, (*mIter)->_sequence);
				}

				_render->blit3dMesh((*mIter)->_me);
			}
		}
	}
}

// --------------------------------------------------------------------------------
//									 Private methods
// --------------------------------------------------------------------------------

/*
==================
Inserts object into the manager
==================
*/
void IND_Entity3dManager::addToList(IND_Entity3d *pNewEntity3d) {
	_listEntities3d->push_back(pNewEntity3d);
}

/*
==================
Initialization error message
==================
*/
void IND_Entity3dManager::writeMessage() {
	g_debug->header("This operation can not be done", 3);
	g_debug->dataChar("", 1);
	g_debug->header("Invalid Id or IND_Entity3dManager not correctly initialized", 2);
}

/*
==================
Init manager vars
==================
*/
void IND_Entity3dManager::initVars() {
	_listEntities3d = new vector <IND_Entity3d *>;
}

/*
==================
Free manager memory
==================
*/
void IND_Entity3dManager::freeVars() {
	// Deletes all the manager entities
	vector <IND_Entity3d *>::iterator _listIter;

	for (_listIter  = _listEntities3d->begin();
	        _listIter != _listEntities3d->end();
	        _listIter++) {
		g_debug->header("Freeing 3d entity:", 3);
		g_debug->dataInt((int)(*_listIter), 1);
	}

	// Clear list
	_listEntities3d->clear();

	// Free list
	DISPOSE(_listEntities3d);
}
