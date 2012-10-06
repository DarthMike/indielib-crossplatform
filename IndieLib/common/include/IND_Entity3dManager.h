/*****************************************************************************************
 * File: IND_Entity3dManager.h
 * Desc: Manager de entidades 3d. Una entidad contiene el una malla 3d y gestiona
 * sus atributos. //TODO: translate this, - sorry my spanish is a little rusty
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


#ifndef _IND_ENTITY3DMANAGER_
#define _IND_ENTITY3DMANAGER_

// ----- Includes -----

#include <vector>

// ----- Forward declarations -----
class IND_Render;
class IND_Entity3d;
class IND_3dMeshManager;

// --------------------------------------------------------------------------------
//									IND_Entity3dManager
// --------------------------------------------------------------------------------

/**
@defgroup IND_Entity3dManager IND_Entity3dManager
@ingroup EntityManagers
Manager of ::IND_Entity3d objects. Click in ::IND_Entity3dManager to see all the methods of this class.
*/
/*@{*/

/**
Manager of ::IND_Entity3d objects. Used for storing these type of objects.

Other uses of this class:
- Function for drawing all the entities stored
*/
class LIB_EXP IND_Entity3dManager {
public:

	// ----- Init/End -----

	IND_Entity3dManager(): _ok(false)  { }
	~IND_Entity3dManager()              {
		end();
	}

	bool    init(IND_Render *pRender);
	void    end();
	bool    isOK()           const {
		return _ok;
	}

	// ----- Métodos -----

	bool        add(IND_Entity3d *pNewEntity3d);
	bool        remove(IND_Entity3d *pEn);

	void        renderEntities3d();

private:

	// ----- Métodos privados -----

	bool _ok;

	IND_Render *_render;

	// ----- Contenedores -----

	// Lista de imágenes
	vector <IND_Entity3d *> *_listEntities3d;

	// ----- Operaciones -----

	void addToList(IND_Entity3d *pNewEntity3d);

	void writeMessage();
	void initVars();
	void freeVars();
};
/*@}*/

#endif // _IND_ENTITY3DMANAGER_
