/*****************************************************************************************
 * File: IND_Input.h
 * Desc: Input class (wrapping SDL input)
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (javierlopezpro@gmail.com)

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

#ifndef _IND_INPUT_
#define _IND_INPUT_

// ----- Includes -----

#include "IND_Timer.h"

// ----- Forward declarations -----
class IND_Render;

// --------------------------------------------------------------------------------
//									      CKey
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */
// This class stores information about a keyboard key
class CKey {
public:

	// ----- Init/End -----

	CKey() :
		_released(false),
		_pressed(false),
		_key(0),
		_keyState(IND_KEY_NOT_PRESSED){
		_timer.start();
	}

	~CKey()  {}

	// ----- Public methods -----

	// Sets the key state
	void setState(IND_KeyState pKeyState) {
		// If the key was pressed and is not pressed anymore we set the flag "released"
		if (_keyState == IND_KEY_PRESSED && pKeyState == IND_KEY_NOT_PRESSED) {
			_released = 1;
			// Set the new state
			_keyState = IND_KEY_NOT_PRESSED;
		}

		// If the key was not pressed and now is pressed we set the flag "pressed"
		if (_keyState == IND_KEY_NOT_PRESSED && pKeyState == IND_KEY_PRESSED) {
			_pressed = 1;
		// Set the new state
			_keyState = IND_KEY_PRESSED;
		}
		
		// If the key is pressed, we reset the timer
		if (pKeyState == IND_KEY_PRESSED) {
			_timer.start();
		}
	}

	bool _released;
	bool _pressed;
	IND_Key _key;
	IND_KeyState _keyState;
	IND_Timer _timer;
};


// --------------------------------------------------------------------------------
//									      CMouse
// --------------------------------------------------------------------------------

// This class stores information about a mouse button
class CMouseButton {
public:

	// ----- Init/End -----

	CMouseButton() : 
		_pressed(false),
		_released(false),
		_button(IND_MBUTTON_LEFT),
		_buttonState(IND_MBUTTON_NOT_PRESSED){
		_timer.start();
	}

	~CMouseButton()  {}

	// ----- Methods -----

	//Sets the button state.
	void setState(IND_MouseButtonState pButtonState) {
		// If the button was pressed and is not being pressed anymore we set the flag "released"
		if (_buttonState == IND_MBUTTON_PRESSED && pButtonState == IND_MBUTTON_NOT_PRESSED)
			_released = 1;

		// If the button was not pressed and is now being pressed we set the flag "pressed"
		if (_buttonState == IND_MBUTTON_NOT_PRESSED && pButtonState == IND_MBUTTON_PRESSED)
			_pressed = 1;

		// Sets the new state
		_buttonState = pButtonState;

		// If the button is pressed, we reset the timer
		if (pButtonState == IND_MBUTTON_PRESSED) {
			_timer.start();
		}
	}

	bool _pressed;
	bool _released;
	IND_MouseButton _button;
	IND_MouseButtonState _buttonState;
	IND_Timer _timer;
};




// Class that stores information about the mouse
class CMouse {
public:

	// ----- Init/End -----

	CMouse() : 
	    _mouseX(0),
		_mouseY(0),
		_mouseMotion(false),
		_mouseScrollX(0),
		_mouseScrollY(0),
		_mouseScroll(false)
		 {}
	~CMouse()  {}

	int _mouseX, _mouseY;
	bool _mouseMotion;
	
	int _mouseScrollX, _mouseScrollY;
	bool _mouseScroll;

	CMouseButton _mouseButtons [3];
};

/** @endcond */


// --------------------------------------------------------------------------------
//									 IND_Input
// --------------------------------------------------------------------------------

/**
@defgroup IND_Input IND_Input
@ingroup Input
IND_Input class for having input from keyboard and mouse. Click in IND_Input to see all the methods of this class.
*/
/**@{*/

/**
@b IND_Input is a wrapper class of SDL input functions for giving IndieLib the possibility of
keyboard and mouse Input.
*/
class LIB_EXP IND_Input {
public:

	// ----- Init/End -----

	IND_Input(): _ok(false)                      { }
	~IND_Input()                                  {
		end();
	}

	bool    init(IND_Render *pRender);
	void    end();
	bool    isOK();

	// ----- Public methods -----

	/** @name Input common
	*
	*/
	/**@{*/
	void update();
	bool quit();
	/**@}*/

	/** @name Keyboard
	*
	*/
	/**@{*/
	bool onKeyPress(IND_Key pKey);
	bool onKeyRelease(IND_Key pKey);
	bool isKeyPressed(IND_Key pKey);
	bool isKeyPressed(IND_Key pKey, unsigned long pTime);
	/**@}*/

	/** @name Mouse
	*
	*/
	/**@{*/
	bool isMouseMotion();
	int getMouseX();
	int getMouseY();
	bool isMouseScroll();
	int getMouseScrollX();
	int getMouseScrollY();

	bool onMouseButtonPress(IND_MouseButton pMouseButton);
	bool onMouseButtonRelease(IND_MouseButton pMouseButton);
	bool isMouseButtonPressed(IND_MouseButton pMouseButton);
	bool isMouseButtonPressed(IND_MouseButton pMouseButton, unsigned long pTime);
	/**@}*/

private:
	/** @cond DOCUMENT_PRIVATEAPI */
	// ----- Private -----

	IND_Render *_render;
	bool _ok;
	bool _quit;

	// ----- Objects -----

	CKey _keys [132];
	CMouse _mouse;

	// ----- Private methods -----

	void                initFlags();
	void                initVars();
	void                freeVars();
    /** @endcond */
};
/**@}*/

#endif // _IND_INPUT_
