/*****************************************************************************************
 * File: IND_Object.h
 * Desc: Base object class
 *****************************************************************************************/

/*
 IndieLib 2d library Copyright (C) 2005 Javier LÛpez LÛpez (info@pixelartgames.com)
 MODIFIED BY Miguel Angel QuiÒones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)
 
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

#ifndef _IND_OBJECT_
#define _IND_OBJECT_

/**
 @defgroup IND_Object IND_Object
 @ingroup Classes
 Base class for some of the user objects in the library. This includes IND_Entity2d, IND_Font, IND_Animation and others.
 
 Managers are not deriving from this class.
 */
/**@{*/


/**
 Defines base methods for all user objects. 
 
 Generally these methods are related to memory management.
 */
class LIB_EXP IND_Object {
public:
    
    /**
     Destroys this instance. 
     
     This is the same as calling delete on the instance object.
     
     You should never need call this method, as all classes derived from this base class are memory-managed by Indielib itself.
     
     If you want to delete an object created via one of the factory methods provided, remove it from the corresponding manager.
     */
    virtual void destroy() = 0;
    
    
    virtual ~IND_Object() {}
};

/**@}*/

#endif
