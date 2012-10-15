/*****************************************************************************************
 * File: TextureDefinitions.h
 * Desc: Data definitions regarding textures (render-specific) 
 *****************************************************************************************/

/*
IndieLib 2d library Copyright (C) 2005 Javier López López (info@pixelartgames.com)
THIS FILE IS AN ADDITIONAL FILE ADDED BY Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com), BUT HAS THE
SAME LICENSE AS THE WHOLE LIBRARY TO RESPECT ORIGINAL AUTHOR OF LIBRARY

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

#ifndef _TEXTUREDEFINITIONS_
#define _TEXTUREDEFINITIONS_

// ----- Includes -----

#include "Defines.h"

/** @cond DOCUMENT_PRIVATEAPI */

#ifdef INDIERENDER_DIRECTX
    #include "render/directx/DirectXRender.h"
	//Texture handle
	struct TEXTURE {
		IDirect3DTexture9 *_texture;            // Texture
	};
#endif

#ifdef INDIRENDER_GLES_IOS
	typedef GLuint TEXTURE;
#endif

#ifdef INDIERENDER_OPENGL
    #include "render/opengl/OpenGLRender.h"
    //Texture handle
	typedef GLuint TEXTURE;   //A texture is just a pointer to the unsigned int id
#endif


// Attributes
struct structAttributes {
	structAttributes () :
		_type(IND_OPAQUE),
		_quality(IND_32),
		_numTextures(0), 
		_width(0), 
		_height(0), 
		_blocksX(0),
		_blocksY(0),
		_spareX(0),
		_spareY(0),
		_numBlocks(0),
		_widthBlock(0),
		_heightBlock(0),
		_isHaveSurface(false),
		_isHaveGrid(false){}

    IND_Type    _type;                      // Surface type
    IND_Quality _quality;                   // Color quality
    int         _numTextures;               // Number of textures
    int         _width;                     // Image width
    int         _height;                    // Image height
    int         _blocksX;                   // Number of blocks width
    int         _blocksY;                   // Number of blocks height
    int         _spareX;                    // Right spare area
    int         _spareY;                    // Upper spare area
    int         _numBlocks;                 // Total number of blocks
    int         _widthBlock;                // Block width
    int         _heightBlock;               // Block height
    bool        _isHaveSurface;             // Surface loaded or not
    bool        _isHaveGrid;
};
typedef struct structAttributes ATTRIBUTES;

// TYPE
struct SURFACE {
    SURFACE() : _vertexArray(NULL), _texturesArray(NULL){}
    ~SURFACE(){
        // Free textures handles
        DISPOSEARRAY(_texturesArray);
	    // Free vertex buffer
	    DISPOSEARRAY(_vertexArray);
    }
	CUSTOMVERTEX2D *_vertexArray;       // Vertex array (store the blocks (quads) of the IND_Surface
	TEXTURE *_texturesArray;            // Texture array (one texture per block)
	ATTRIBUTES _attributes;             // Attributes
};

/** @endcond */

#endif //_TEXTUREDEFINITIONS_
