/*****************************************************************************************
 * Desc: Main entry point for tests. Manual tests are the 'fast' way in this project to 
 * test UI-related things and graphics output in a controlled way. The way this tests are
 * divided is the following: 
 *    Permanent tests: allways run, every run cycle. This tests generally manipulate
      library settings, or general setup of Window/renderer/camera etc which influences the 
	  way the output is displayed by other tests. 
	  Selectable tests: This tests, execute one after another, and have to be switched manually
	  using the arrow keys (controls below). This tests never execute at the same time, only
	  execute at the same time of other permanent tests.

  See the base class "ManualTests.h" which defines the main interface for any test class 
  (permanent or selectable).

  CONTROLS: Arrows (left-right): Changes the selected test running at this time. This causes
  the current test to be inactive (a flag) and next one active. On next running cycle
  this test will be executed till the current test is selected.

            ESC-Key: Exit application. Cleaning up all tests before exiting.

			Other controls: See Permanent test classes + current test.

 WORKING DIRECTORY: Important: working directory must be set to be one folder below in the 
 trunk tree i.e "IndieLib/tests/" or "IndieLib/linux/".
 All relative loading paths will take into account that premise.
 *****************************************************************************************/

#include "CIndieLib.h"
#include <vector>

//Leaks control
#ifdef PLATFORM_WIN32
	#include "platform/win32/LeaksVisualC.h"
#endif

//Working path utility
#include "WorkingPath.h"


//Tests
#include "manual/ManualTests.h"
#include "functionality/FunctionalityTests.h"
#include "primitives/PrimitiveTests.h"
#include "camera2d/Camera2dTests.h"
#include "surface/SurfaceTests.h"
#include "surface/SurfaceTests_wrap.h"
#include "surface/SurfaceTests_textBlock.h"
#include "image/INDImageTests_POTLoad.h"
#include "image/INDImageTests_nonPOTLoad.h"
#include "image/INDImageTests_Conversions.h"
#include "image/INDImageTests_Conversions_NonRGB.h"
#include "animation/AnimationTests.h"
#include "font/FontTests.h"
#include "font/FontTests_Pretty.h"
#include "collision/CollisionTests.h"
#include "entities/EntityTests_animateRotations.h"
#include "entities/EntityTests_animateScale.h"
#include "entities/EntityTests_interactions.h"
#include "surface/SurfaceTests_Filters.h"
#include "surface/SurfaceTests_layout.h"

/*
==================
Main
==================
*/


typedef std::vector<ManualTests *> testsvectortype;
typedef testsvectortype::iterator testsvectoriterator;

testsvectortype g_permanenttests;
testsvectortype g_tests;
IND_Timer g_mainTimer;
const unsigned char g_gridcolorR = 0;
const unsigned char g_gridcolorG = 0;
const unsigned char g_gridcolorB = 255;
const unsigned char g_gridcolorA = 200;
const unsigned char g_collisionColorR = 255;
const unsigned char g_collisionColorG = 0;
const unsigned char g_collisionColorB = 0;
const unsigned char g_collisionColorA = 255;

void initTests() {
    //Permanent tests
	FunctionalityTests *func = new FunctionalityTests();
    func->setActive(true);
	g_permanenttests.push_back(func);
    Camera2dTests *cam2d = new Camera2dTests();
    cam2d->setActive(true);
	g_permanenttests.push_back(cam2d);
	
    //Temporary tests
    PrimitiveTests *prim = new PrimitiveTests();
	g_tests.push_back(prim);
    SurfaceTests *surf = new SurfaceTests();
    g_tests.push_back(surf);
	SurfaceTests_wrap *surfwrap = new SurfaceTests_wrap();
	g_tests.push_back(surfwrap);
	SurfaceTests_textBlock *surfblock = new SurfaceTests_textBlock();
	g_tests.push_back(surfblock);
	INDImageTests_POTLoad *indImage = new INDImageTests_POTLoad();
	g_tests.push_back(indImage);
	INDImageTests_nonPOTLoad *indImage2 = new INDImageTests_nonPOTLoad();
	g_tests.push_back(indImage2);
    INDImageTests_Conversions *indImage3 = new INDImageTests_Conversions();
	g_tests.push_back(indImage3);
	INDImageTests_Conversions_NonRGB *indImage4 = new INDImageTests_Conversions_NonRGB();
	g_tests.push_back(indImage4);
	AnimationTests *animt1 = new AnimationTests();
	g_tests.push_back(animt1);
	FontTests *font1 = new FontTests();
	g_tests.push_back(font1);
	FontTests_Pretty *font2 = new FontTests_Pretty();
	g_tests.push_back(font2);
	CollisionTests *collision1 = new CollisionTests();
	g_tests.push_back(collision1);
	EntityTests_animateRotations * entity1 = new EntityTests_animateRotations();
	g_tests.push_back(entity1);
	EntityTests_animateScale * entity2 = new EntityTests_animateScale();
	g_tests.push_back(entity2);
    EntityTests_interactions* entity3 = new EntityTests_interactions();
    g_tests.push_back(entity3);
    SurfaceTests_Filters* filters = new SurfaceTests_Filters();
    g_tests.push_back(filters);
	SurfaceTests_layout* layout = new SurfaceTests_layout();
	g_tests.push_back(layout);

}

void releaseTests() {
	//Clear tests
    testsvectoriterator itr;
    //LOOP
	for (itr = g_tests.begin(); itr != g_tests.end(); ++itr) {
		delete (*itr);
	}//LOOP END

	g_tests.clear();

    //Clear temporary tests
    //LOOP
    for (itr = g_permanenttests.begin(); itr != g_permanenttests.end(); ++itr) {
		delete (*itr);
	}//LOOP END

    g_permanenttests.clear();
}

int IndieLib() {
#ifdef PLATFORM_WIN32
	//memory leaks for win32 creation
	FINDMEMLEAK(-1);    //When you find a mem. leak, refer to the number displayed in the console and put it here to break when it is created
#endif
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;
    
    //Sets the working path as the 'exe' directory. All resource paths are relative to this directory
	if (!WorkingPathSetup::setWorkingPathFromExe(NULL)) {
		std::cout<<"\nUnable to Set the working path !";
	}
	
    // ----- Fcn pointer tests initialization -----
	initTests();

	testsvectoriterator itr;
    //LOOP - Prepare Permanent tests
	for (itr = g_permanenttests.begin(); itr != g_permanenttests.end(); ++itr) {
		(*itr)->prepareTests();
	}//LOOP END

    //LOOP - Prepare Temporary tests
	for (itr = g_tests.begin(); itr != g_tests.end(); ++itr) {
		(*itr)->prepareTests();
	}//LOOP END

	// ----- Main Loop -----
	g_mainTimer.start();
	double ticks = g_mainTimer.getTicks();
	double pastticks = g_mainTimer.getTicks();
    testsvectoriterator currentTest = g_tests.begin();
    if (currentTest != g_tests.end()) {
        (*currentTest)->setActive(true);
    }

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit()) {
		// ----- Input Update ----

		mI->_input->update();

		// -------- Render -------
        mI->_render->clearViewPort(0,0,0);
		mI->_render->beginScene();
		//LOOP - Permanent Tests
		for (itr = g_permanenttests.begin(); itr != g_permanenttests.end(); ++itr) {
			(*itr)->performTests(static_cast<float>(ticks - pastticks));
		}//LOOP END

		//LOOP - Tests
		for (itr = g_tests.begin(); itr != g_tests.end(); ++itr) {
			(*itr)->performTests(static_cast<float>(ticks - pastticks));
		}//LOOP END

		mI->_entity2dManager->renderEntities2d();   //Entities rendering
		mI->_entity2dManager->renderGridAreas(g_gridcolorR,g_gridcolorG,g_gridcolorB,g_gridcolorA); //Grid areas rendering (on top of it) can be disabled per entity
		mI->_entity2dManager->renderCollisionAreas(g_collisionColorR,g_collisionColorG,g_collisionColorB,g_collisionColorA);  //Collision areas rendering (on top of it) can be disabled per entity
		mI->_render->endScene();
		//mI->_render->showFpsInWindowTitle();

		pastticks = ticks;
		ticks += g_mainTimer.getTicks();

        //Perform next test setup (if needed) Arrow keys change current temporary test using
        if (mI->_input->onKeyPress(IND_KEYRIGHT) && !mI->_input->onKeyPress(IND_KEYLEFT)){
            //Current test is not active
            (*currentTest)->setActive(false);
            currentTest++;
            
            //IF - Check limits
            if(currentTest == g_tests.end()){
                currentTest = g_tests.begin();
            }
            
            //Next test is active
            (*currentTest)->setActive(true);
        } else if (mI->_input->onKeyPress(IND_KEYLEFT) && !mI->_input->onKeyPress(IND_KEYRIGHT)){
            //Current test is not active
            (*currentTest)->setActive(false);
          
            //Decrement (taking into account limits)
            if(currentTest == g_tests.begin()){
                currentTest = g_tests.end();
            }
            currentTest--;
            
            //Next test is active
            (*currentTest)->setActive(true);
        }
            

	}

	// ----- Indielib End -----

    releaseTests();

    mI->end();
	return 0;
}
