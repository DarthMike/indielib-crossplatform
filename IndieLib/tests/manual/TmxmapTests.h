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

#ifndef _TMXMAPTESTS_
#define _TMXMAPTESTS_

#include "manual/ManualTests.h"

class TmxmapTests : public ManualTests {
public:
    
    TmxmapTests() {
        init();
	}
    
	virtual ~TmxmapTests() {
		release();
	}
	void prepareTests();
	void performTests(float dt);
    
    bool isActive();
    void setActive(bool active);
    
private:
	void init();
	void release();
    
    
    int _mapCenterOffset;
    bool _showIsometric;
    char _text [2048];
    IND_Font  *_fontSmall;
    IND_Entity2d *_textSmallWhite;
    IND_TmxMap *_orthogonalMap;
    IND_TmxMap *_isometricMap;
    IND_Surface *_surfaceIsometricTiles;
    IND_Surface *_surfaceOrthogonalTiles;

};


#endif // _TMXMAPTESTS_
