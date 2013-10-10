/*****************************************************************************************
 * File: OpenGLTextureBuilder.h
 * Desc: Builder-specific class for OpenGL textures
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


#ifndef _OPENGLTEXTUREBUILDER_H_
#define _OPENGLTEXTUREBUILDER_H_

#include "Defines.h"
#include "TextureBuilder.h"
#include "IND_Render.h"

#ifdef INDIERENDER_OPENGL
#include "dependencies/glew-1.9.0/include/GL/glew.h" //Extension loading facilites library
#endif


#ifdef INDIERENDER_GLES_IOS
#include <OpenGLES/ES2/gl.h>
// Define inverted G-B pixel types, only available in desktop
#define GL_BGR GL_RGB
#define GL_BGRA GL_RGBA
#endif
/** @cond DOCUMENT_PRIVATEAPI */

class IND_Image;
class IND_Surface;
class ImageCutter;
class IND_ImageManager;

class OpenGLTextureBuilder : public TextureBuilder {
public:
	//------CONSTRUCTOR/DESTRUCTOR------
	OpenGLTextureBuilder(IND_ImageManager *imagemgr, IND_Render *render);
	~OpenGLTextureBuilder();

	// ----- Interface implementation -----
	virtual bool createNewTexture(IND_Surface    *pNewSurface,
	                              IND_Image       *pImage,
	                              int             pBlockSizeX,
	                              int             pBlockSizeY) ;

private:
	// ----- Private Objects ------
	ImageCutter *_cutter;
	IND_Render *_render;

	// ----- Private Methods ------
	void getGLFormat (IND_Surface *pNewSurface, IND_Image* pNewImage, GLint *pGLInternalFormat, GLint *pGLFormat, GLint *pGLType);

	void pushVertex(CUSTOMVERTEX2D *pVertices,
	                               int pPosVert,
	                               int pVx,
	                               int pVy,
	                               int pVz,
	                               float pU,
	                               float pV);

	void push4Vertices(CUSTOMVERTEX2D *pVertices,
	                                  int pPosVert,
	                                  int pPosX,
	                                  int pPosY,
	                                  int pPosZ,
	                                  int pWidthBlock,
	                                  int pHeightBlock,
	                                  float pU,
	                                  float pV);

};

/** @endcond */

#endif  //_OPENGLTEXTUREBUILDER_H_
