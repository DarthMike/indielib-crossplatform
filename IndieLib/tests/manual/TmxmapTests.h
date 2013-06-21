/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */

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

#endif
