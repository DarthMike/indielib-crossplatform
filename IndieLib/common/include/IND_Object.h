/*****************************************************************************************
 * File: IND_Object.h
 * Desc: Base object class
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
#ifndef _IND_OBJECT_
#define _IND_OBJECT_

#include "IndiePlatforms.h"

/**
 @defgroup IND_Object IND_Object
 @ingroup Classes
 Base class for some of the user objects in the library. This includes IND_Entity2d, IND_Font, IND_Animation, IND_TMXMap and others.
 
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
