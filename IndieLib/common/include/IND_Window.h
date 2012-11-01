/*****************************************************************************************
 * File: IND_Window.h
 * Desc: Manages the window (uses SDL for creating the window)
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
MODIFIED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)

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


#ifndef _IND_WINDOW_H_
#define _IND_WINDOW_H_

//Dependencies
struct SDL_SysWMinfo;
struct SDL_Window;

// --------------------------------------------------------------------------------
//									  IND_Window
// --------------------------------------------------------------------------------

/**
@defgroup IND_Window IND_Window
@ingroup Main
Class for creating the window, click in ::IND_Window to see all the methods of this class.
*/

/**@{*/

/**
This structure defines definition parameters when creating a window or modyfing it's properties
*/
typedef struct IND_WindowProperties {

    //! Default constructor
	IND_WindowProperties(const char* title, int width, int height, int bpp, bool vsync, bool fs, bool dBuffer) :
                         _title(title),
						 _width(width),
						 _height(height),
						 _bpp(bpp),
						 _vsync(vsync),
						 _fullscreen(fs),
                         _doubleBuffer(dBuffer){}
	const char *_title;			///< A string containing the window title, when windowed
	int _width; 				///< Width in pixels
	int _height;				///< Heigth in pixels
	int _bpp;					///< Bit depth of the window display buffer
	bool _vsync;				///< Wether vSync is enabled (true)
	bool _fullscreen;			///< Wether window is full screen or windowed
    bool _doubleBuffer;         ///< Wether double buffering should be used. Recommended always to use it.
}IND_WindowProperties;


/**
This class manages the window where the render will be initialized.
*/
class LIB_EXP IND_Window {
public:

	// ----- Init/End -----

	IND_Window(): _ok(false)  { }
	~IND_Window()              {
		end();
	}

	//! IsOk
	/**
	This function returns 1 (true) if the manager is correctly initialized.
	*/
	bool    isOK()           const {
		return _ok;
	}

	// ----- Atributes -----

	//! This function returns the title of the window.
	char  *getTitle()          {
		return _attributes._title;
	}
	//! This function returns the width of the window.
	int getWidth()      {
		return _attributes._width;
	}
	//! This function returns the height of the window.
	int getHeight()      {
		return _attributes._height;
	}
	//! This function returns the window color quality. 32 or 16 bits.
	int getBpp()      {
		return _attributes._bits;
	}
	//! This function returns 1 (true) if the window has activated vsync.
	bool isVsync()      {
		return _attributes._vsync;
	}
	//! This function returns 1 (true) if the window is set to full screen.
	bool isFullScreen()      {
		return _attributes._fullscreen;
	}

	//! This function the SDL window information structure data
	const SDL_SysWMinfo& getWindowInfo() {
		return *_attributes._windowInfo;
	}

	//! This function the SDL window
	SDL_Window *getSDLWindow() {
		return _attributes._sdlWindow;
	}
	// ----- Public methods -----
	/**
	@brief Sets the title shown only for non-fullscreen windows
	@param pTitle The string to represent title
	*/
	void setTitle(const char *pTitle);
	void cursor(bool pEnable);

private:
	/** @cond DOCUMENT_PRIVATEAPI */
	// ----- Private -----

	bool _ok;

	// Attributes
    struct structAttributes {
        int _width;
        int _height;
        int _bits;
        bool _vsync;
        bool _fullscreen;
        char *_title;
        SDL_SysWMinfo* _windowInfo;
        SDL_Window *_sdlWindow;
        
        
        structAttributes() :
        _width(0),
        _height(0),
        _bits(0),
        _vsync(false),
        _fullscreen(false),
        _title(NULL),
        _windowInfo(NULL),
        _sdlWindow(NULL){
        }
    };
	struct structAttributes _attributes;

	// ----- Private methods -----
	void    end();

	bool create(IND_WindowProperties& props);
	void destroyIndieLibWindow();
	void initAttribWindow();
	bool reset(IND_WindowProperties& props);
	bool toggleFullScreen();

	void initVars();
	void freeVars();

	//Creation of window by renderer
	friend class OpenGLRender;
	friend class DirectXRender;
    /** @endcond */
    
};
/**@}*/

#endif // _IND_WINDOW_H_
