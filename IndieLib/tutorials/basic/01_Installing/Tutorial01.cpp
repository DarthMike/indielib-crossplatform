/*****************************************************************************************
 * Desc: Tutorial a) 01 Installing
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"

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
	
	// ----- Main Loop -----

	while (!mI->_input->onKeyPress (IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input Update ----

		mI->_input->update();

		// -------- Render -------

		mI->_render->clearViewPort(60, 60, 60);
		mI->_render->beginScene();
		mI->_render->endScene();
		//mI->_render->showFpsInWindowTitle();
	}

	// ----- Indielib End -----

	mI->end();

	return 0;
}
