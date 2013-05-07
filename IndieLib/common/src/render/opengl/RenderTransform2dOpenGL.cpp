/*****************************************************************************************
 * File: RenderTransform2dOpenGL.cpp
 * Desc: Transformations applied before blitting a 2d object usind OpenGL
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
#include "Defines.h"

#ifdef INDIERENDER_OPENGL
// ----- Includes -----

#include "Global.h"
#include "Defines.h"
#include "OpenGLRender.h"
#include "IND_Camera2d.h"
#include "IND_Window.h"

/** @cond DOCUMENT_PRIVATEAPI */

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------

bool OpenGLRender::setViewPort2d(int pX,
                                 int pY,
                                 int pWidth,
                                 int pHeight) {
	// ----- If the region is outside the framebuffer return error -----

	if (pX +  pWidth > _info._fbWidth)  return 0;
	if (pX < 0)                         return 0;
	if (pY + pHeight > _info._fbHeight) return 0;
	if (pY < 0)                         return 0;

	// ----- Viewport characteristics -----

	_info._viewPortX      = pX;
	_info._viewPortY      = pY;
	_info._viewPortWidth  = pWidth;
	_info._viewPortHeight = pHeight;
    _info._viewPortApectRatio = static_cast<float>(pWidth/pHeight);

	//Clear projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Define the viewport
	glViewport(static_cast<GLint>(pX),
	           static_cast<GLint>(pY),
	           static_cast<GLsizei>(pWidth),
	           static_cast<GLsizei>(pHeight));
	glMatrixMode(GL_MODELVIEW);

	setDefaultGLState();
	return true;
}


void OpenGLRender::setCamera2d(IND_Camera2d *pCamera2d) {
	// ----- Lookat matrix -----
	//Rotate that axes in Z by the camera angle
	//Roll is rotation around the z axis (_look)
	IND_Matrix rollmatrix;
	float rotAngle (pCamera2d->_angle - pCamera2d->_prevAngle);
	_math.matrix4DSetRotationAroundAxis(rollmatrix, rotAngle , pCamera2d->_look);
	_math.transformVector3DbyMatrix4D(pCamera2d->_right, rollmatrix);
	_math.transformVector3DbyMatrix4D(pCamera2d->_up, rollmatrix);
	//Reset camera angle difference
	pCamera2d->setAngle(pCamera2d->_angle);
	
	//Build the view matrix from the transformed camera axis
	IND_Matrix lookatmatrix; 
	_math.matrix4DLookAtMatrixLH(pCamera2d->_right,
	                           pCamera2d->_up,
							   pCamera2d->_look,
							   pCamera2d->_pos,
	                           lookatmatrix);
#ifdef _DEBUG
	int mmode;
	glGetIntegerv(GL_MATRIX_MODE,&mmode);
	assert( mmode == GL_MODELVIEW);
#endif

	glLoadIdentity();
    //------ Zooming -----
	if (pCamera2d->_zoom != 1.0f) {
        //Zoom global scale (around where camera points - screen center)
        glScalef(pCamera2d->_zoom, pCamera2d->_zoom,0);
	} 

	//------ Lookat transform -----
	glMultMatrixf(reinterpret_cast<GLfloat *>(&lookatmatrix));
    
	//------ Global point to pixel ratio -----
    glScalef(_info._pointPixelScale, _info._pointPixelScale, 1.0f);

    //Store result from GL matrix back to our local matrix
    float cam[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, cam);
    _cameraMatrix.readFromArray(cam);
    
	// ----- Projection Matrix -----
	//Setup a 2d projection (orthogonal)
	perspectiveOrtho(static_cast<float>(_info._viewPortWidth), static_cast<float>(_info._viewPortHeight), 2048.0f, -2048.0f);

	/*float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	std::cout << "\nMATRIX:\n";
	for (size_t i = 0; i < 4; ++i) {    
		for (size_t j = 0; j < 4; ++j) {        
			std::cout << m[i + j * 4] << " ";    
		}    
		std::cout << std::endl;
	}*/
}


void OpenGLRender::setTransform2d(int pX,
                                  int pY,
                                  float pAngleX,
                                  float pAngleY,
                                  float pAngleZ,
                                  float pScaleX,
                                  float pScaleY,
                                  int pAxisCalX,
                                  int pAxisCalY,
                                  bool pMirrorX,
                                  bool pMirrorY,
                                  int pWidth,
                                  int pHeight,
                                  IND_Matrix *pMatrix) {

	//Temporal holders for all accumulated transforms
	IND_Matrix totalTrans;
	_math.matrix4DSetIdentity(totalTrans);
	IND_Matrix temp;
	_math.matrix4DSetIdentity(temp);

	//Initialize to identity given matrix, if exists
	if (pMatrix) {
		_math.matrix4DSetIdentity(*pMatrix);
	}
    
	// Translations
	if (pX != 0 || pY != 0) {
		IND_Matrix trans;
		_math.matrix4DSetTranslation(trans,static_cast<float>(pX),static_cast<float>(pY),0.0f);
		_math.matrix4DMultiply(totalTrans,trans,temp);
		totalTrans = temp;
	}

	// Scaling
	if (pScaleX != 1.0f || pScaleY != 1.0f) {
		IND_Matrix scale;
		_math.matrix4DSetScale(scale,pScaleX,pScaleY,0.0f);
		_math.matrix4DMultiply(totalTrans,scale,temp);
		totalTrans = temp;
	}

	// Rotations
	if (pAngleX != 0.0f) {
		IND_Matrix angleX;
		_math.matrix4DSetRotationAroundAxis(angleX,pAngleX,IND_Vector3(1.0f,0.0f,0.0f));
		_math.matrix4DMultiply(totalTrans,angleX,temp);
		totalTrans = temp;
	}

	if (pAngleY != 0.0f) {
		IND_Matrix angleY;
		_math.matrix4DSetRotationAroundAxis(angleY,pAngleY,IND_Vector3(0.0f,1.0f,0.0f));
		_math.matrix4DMultiply(totalTrans,angleY,temp);
		totalTrans = temp;
	}

	if (pAngleZ != 0.0f) {
		IND_Matrix angleZ;
		_math.matrix4DSetRotationAroundAxis(angleZ,pAngleZ,IND_Vector3(0.0f,0.0f,1.0f));
		_math.matrix4DMultiply(totalTrans,angleZ,temp);
		totalTrans = temp;
	}

	// Hotspot - Add hotspot to make all transforms to be affected by it
	if (pAxisCalX != 0 || pAxisCalY != 0) {
		IND_Matrix hotspot;
		_math.matrix4DSetTranslation(hotspot,static_cast<float>(pAxisCalX),static_cast<float>(pAxisCalY),0.0f);
		_math.matrix4DMultiply(totalTrans,hotspot,temp);
		totalTrans = temp;
	}

    // Mirroring (180º rotations) and translation
	if (pMirrorX || pMirrorY) {
		//A mirror is a rotation in desired axis (the actual mirror) and a repositioning because rotation
		//also moves 'out of place' the entity translation-wise
		if (pMirrorX) {
			IND_Matrix mirrorX;
            //After rotation around origin, move back texture to correct place
            _math.matrix4DSetTranslation(mirrorX,
                                         static_cast<float>(pWidth),
                                         0.0f,
                                         0.0f);
			_math.matrix4DMultiply(totalTrans,mirrorX,temp);
			totalTrans = temp;
            
            //Rotate in y, to invert texture
			_math.matrix4DSetRotationAroundAxis(mirrorX,180.0f,IND_Vector3(0.0f,1.0f,0.0f));
			_math.matrix4DMultiply(totalTrans,mirrorX,temp);
			totalTrans = temp;
		}
        
		//A mirror is a rotation in desired axis (the actual mirror) and a repositioning because rotation
		//also moves 'out of place' the entity translation-wise
		if (pMirrorY) {
			IND_Matrix mirrorY;
            //After rotation around origin, move back texture to correct place
            _math.matrix4DSetTranslation(mirrorY,
                                         0.0f,
                                         static_cast<float>(pHeight),
                                         0.0f);
			_math.matrix4DMultiply(totalTrans,mirrorY,temp);
			totalTrans = temp;
            
            //Rotate in x, to invert texture
			_math.matrix4DSetRotationAroundAxis(mirrorY,180.0f,IND_Vector3(1.0f,0.0f,0.0f));
			_math.matrix4DMultiply(totalTrans,mirrorY,temp);
			totalTrans = temp;
		}
	}
	//Cache the change
	_modelToWorld = totalTrans;

	//Apply the changes to the GL matrix stack (model view)
    //Camera transform
    float camMatrixArray [16];
    _cameraMatrix.arrayRepresentation(camMatrixArray);
    glLoadMatrixf(camMatrixArray);
    
    //Actual object transform
    float matrixArray [16];
    totalTrans.arrayRepresentation(matrixArray);
	glMultMatrixf(matrixArray);

	// ----- Return World Matrix (in IndieLib format) ----
	//Transformations have been applied where needed
	if (pMatrix) {
		*pMatrix = totalTrans;
	}
}

void OpenGLRender::setTransform2d(IND_Matrix &pMatrix) {
	// ----- Applies the transformation -----
    float camMatrixArray [16];
    _cameraMatrix.arrayRepresentation(camMatrixArray);
    glLoadMatrixf(camMatrixArray);
    
    //Object transform
    float matrixArray [16];
    pMatrix.arrayRepresentation(matrixArray);
	glMultMatrixf(matrixArray);

	//Finally cache the change
	_modelToWorld = pMatrix;
}

void OpenGLRender::setIdentityTransform2d ()  {
	// ----- Applies the transformation -----
	float camMatrixArray [16];
    _cameraMatrix.arrayRepresentation(camMatrixArray);
    glLoadMatrixf(camMatrixArray);

	//Finally cache the change
	_math.matrix4DSetIdentity(_modelToWorld);
}

void OpenGLRender::setRainbow2d(IND_Type pType,
                                bool pCull,
                                bool pMirrorX,
                                bool pMirrorY,
                                IND_Filter pFilter,
                                BYTE pR,
                                BYTE pG,
                                BYTE pB,
                                BYTE pA,
                                BYTE pFadeR,
                                BYTE pFadeG,
                                BYTE pFadeB,
                                BYTE pFadeA,
                                IND_BlendingType pSo,
                                IND_BlendingType pDs) {
	//Parameters error correction:
	if (pA > 255) {
		pA = 255;
	}

	//Setup neutral 'blend' for texture stage
	float blendR, blendG, blendB, blendA;
	blendR = blendG = blendB = blendA = 1.0f;

	// ----- Filters -----
    // In GL, texture filtering is applied to the bound texture. From this method we don't know which is the
    // bound texture, so we cache the requested state, so before actually rendering, we could set the state
    // to the bound texture
	int filterType = GL_NEAREST;
	if (IND_FILTER_LINEAR == pFilter) {
		filterType = GL_LINEAR;
	}

    _tex2dState.magFilter = filterType;
    _tex2dState.minFilter = filterType;

	// ----- Back face culling -----
	if (pCull) {
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);    
	} else {
		glDisable(GL_CULL_FACE);
	}

	// ----- Back face culling -----
	// Mirroring (180º rotations)
	if (pMirrorX || pMirrorY) {
		if (pMirrorX && !pMirrorY) {
			// Back face culling
			if (pCull) {
				glEnable(GL_CULL_FACE);
				glFrontFace(GL_CCW);    
			} else {
				glDisable(GL_CULL_FACE);
			}
		}

		if (!pMirrorX && pMirrorY) {
			if (pCull) {
				glEnable(GL_CULL_FACE);
				glFrontFace(GL_CCW);    
			} else {
				glDisable(GL_CULL_FACE);
			}
		}
	}

	// ----- Blending -----
	switch (pType) {
        case IND_OPAQUE: {
            // Alphablending and alpha test = OFF
            glDisable(GL_ALPHA_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ZERO);
            
            // Tinting
            if (pR != 255 || pG != 255 || pB != 255) {
				blendR = static_cast<float>(pR) / 255.0f;
				blendG = static_cast<float>(pG) / 255.0f;
				blendB = static_cast<float>(pB) / 255.0f;
                glColor4f(blendR, blendG, blendB, blendA);
            }
            
            // Alpha
            if (pA != 255) {
				glDisable(GL_ALPHA_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				blendA = static_cast<float>(pA) / 255.0f;
                glColor4f(blendR, blendG, blendB, blendA);
            }
            
            // Fade to color
            if (pFadeA != 255) {
                glDisable(GL_ALPHA_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				blendA = static_cast<float>(pFadeA) / 255.0f;
                blendR = static_cast<float>(pFadeR) / 255.0f;
                blendG = static_cast<float>(pFadeG) / 255.0f;
                blendB = static_cast<float>(pFadeB) / 255.0f;
                glColor4f(blendR, blendG, blendB, blendA);
            }
            
            if (pSo && pDs) {
                //Alpha blending
            }
        }
            break;
            
        case IND_ALPHA: {
            // Alpha test = OFF
            glDisable(GL_ALPHA_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(blendR, blendG, blendB, blendA);
            
            // Tinting
            if (pR != 255 || pG != 255 || pB != 255) {
				blendR = static_cast<float>(pR) / 255.0f;
				blendG = static_cast<float>(pG) / 255.0f;
				blendB = static_cast<float>(pB) / 255.0f;
                glColor4f(blendR, blendG, blendB, blendA);
            }
            
            // Alpha
            if (pA != 255) {
				glDisable(GL_ALPHA_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				blendA = static_cast<float>(pA) / 255.0f;
                glColor4f(blendR, blendG, blendB, blendA);
            }
            
            // Fade to color
            if (pFadeA != 255) {
                blendA = static_cast<float>(pFadeA) / 255.0f;
                blendR = static_cast<float>(pFadeR) / 255.0f;
                blendG = static_cast<float>(pFadeG) / 255.0f;
                blendB = static_cast<float>(pFadeB) / 255.0f;
                glColor4f(blendR, blendG, blendB, blendA);
            }
            
            if (!pSo || !pDs) {
                //Alpha blending
            } else {
                
            }

	}
	break;
	default: {
	}
	}


}

void OpenGLRender::setDefaultGLState() {
    // ----- 2d GLState -----
	//Many defaults are GL_FALSE, but for the sake of explicitly safe operations (and code clearness)
	//I include glDisable explicits
	glDisable(GL_LIGHTING); //We don't want lighting (may change in successive versions! for modern 2d..)
	glDisable(GL_DEPTH_TEST); //No depth testing
	glDisable(GL_NORMALIZE); //Don't normalize normal vectors after submitting them
	glShadeModel(GL_SMOOTH); //Default shading mode (will change it where it is necessary)
    
	// ----- Texturing settings  -----
	// the texture wraps over at the edges (repeat)
    glEnable(GL_TEXTURE_2D);
	//Generally we work with byte-aligned textures.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
    
    setGLClientStateToTexturing();
}

void OpenGLRender::setGLClientStateToPrimitive() {
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void OpenGLRender::setGLClientStateToTexturing() {
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void OpenGLRender::setGLBoundTextureParams() {
    //Texture wrap mode
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,_tex2dState.wrapS);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,_tex2dState.wrapT);
    //By default select fastest texture filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _tex2dState.magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _tex2dState.minFilter);
}

/** @endcond */

#endif //INDIERENDER_OPENGL
