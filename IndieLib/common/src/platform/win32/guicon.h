/*****************************************************************************************
 * File: guicon.h
 * Desc: Utility to redirect and create std in out to console under windows
 * Note: This is available under: http://dslweb.nwnexus.com/~ast/dload/guicon.htm
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


#ifndef __GUICON_H__
#define __GUICON_H__

#ifdef _DEBUG
#include <stdio.h>

/** @cond DOCUMENT_PRIVATEAPI */

void RedirectIOToConsole();

/** @endcond */

#endif /_DEBUG

#endif //__GUICON_H__
