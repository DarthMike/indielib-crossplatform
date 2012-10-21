/*****************************************************************************************
 * Desc: Utility to redirect and create std in out to console under windows

 This is available under:
 http://dslweb.nwnexus.com/~ast/dload/guicon.htm
 *****************************************************************************************/

#ifndef __GUICON_H__
#define __GUICON_H__

#include "IndiePlatforms.h"
#ifdef PLATFORM_WIN32
#ifdef _DEBUG
#include <stdio.h>
/** @cond DOCUMENT_PRIVATEAPI */

void RedirectIOToConsole();

/** @endcond */

#endif
#endif
#endif
