/*****************************************************************************************
 * File: TextureDefinitions.h
 * Desc: Data definitions regarding textures (render-specific) 
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
    SURFACE(int pNumBlocks, int numVertices) : _vertexArray(NULL), _texturesArray(NULL) {
        // This buffer will be used for drawing the IND_Surface using DrawPrimitiveUp
        _vertexArray = new CUSTOMVERTEX2D[numVertices];
        // Each block, needs a texture. We use an array of textures in order to store them.
        _texturesArray = new TEXTURE [pNumBlocks];
    }
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
