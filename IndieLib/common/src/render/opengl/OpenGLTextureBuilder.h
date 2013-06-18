/*****************************************************************************************
 * File: OpenGLTextureBuilder.h
 * Desc: Builder-specific class for OpenGL textures
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
