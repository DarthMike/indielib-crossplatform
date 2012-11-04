/*****************************************************************************************
 * Desc: Tutorials b) 05 IND_TmxMap
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"

#include <cstring>

/*
==================
Main
==================
*/
int IndieLib()			
{
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;			

	// ----- Spriter animation loading -----

	if (!mI->_spriterManager->addSpriterFile(NULL,"Example.SCML")) return 0;




	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Text -----

//		strcpy(mText, "Press space to see the grid in action. This is really cool, isn't it?");
//		mTextSmallWhite->setText(mText);

		// ----- Input ----

		// for future use ...
		if (mI->_input->onKeyPress(IND_SPACE))
		{

		}

		// ----- Updating entities attributes  -----


		// ----- Render  -----

		mI->_render->beginScene();
		mI->_render->clearViewPort(0, 0, 0);
//		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();	
	}

	// ----- Free -----

	mI->end();

	return 0;
}
