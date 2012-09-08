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

// --------------------------------------------------------------------------------
//							         Public methods
// --------------------------------------------------------------------------------


/*!
\b Parameters:

\arg \b pX              Upper left X coordinate of the Viewport
\arg \b pY              Upper left Y coordinate of the Viewport
\arg \b pWidth          Viewport width
\arg \b pHeight         Viewport height

\b Operation:

This function returns 1 (true) if a 2d ViewPort is created in the specified area.

The ViewPort is the drawing region. By default, IndieLib is initialized with a Viewport
that has the same area as the window. Using this method you can define different viewports before
calling to IND_Entity2dManager::renderEntities2d().

After using this method, all the bliting methods will render the graphical objects inside the Viewport.
Objects or blocks drawn outside the viewport will be discarded.

This method returns 0 (false) if the user tries to create a Viewport outside the window area.
*/
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


	//Clear projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Define the viewport
	glViewport(static_cast<GLint>(pX),
	           static_cast<GLint>(pY),
	           static_cast<GLsizei>(pWidth),
	           static_cast<GLsizei>(pHeight));
	glMatrixMode(GL_MODELVIEW);

	// ----- 2d GLState -----
	//Many defaults are GL_FALSE, but for the sake of explicitly safe operations (and code clearness)
	//I include glDisable explicits
	glDisable(GL_LIGHTING); //We don't want lighting (may change in successive versions! for modern 2d..)
	glDisable(GL_DEPTH_TEST); //No depth testing
	glDisable(GL_NORMALIZE); //Don't normalize normal vectors after submitting them
	glShadeModel(GL_SMOOTH); //Default shading mode (will change it where it is necessary)

	// ----- Texturing settings  -----
	// the texture wraps over at the edges (repeat)
    //TODO: Verify this for iOS
    glEnable(GL_TEXTURE_2D);
	//Generally we work with byte-aligned textures.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	//Texture clamp ON by default
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);	
	return true;
}


/*!
\b Parameters:

\arg \b pCamera2d               ::IND_Camera2d object that defines a camera.

\b Operation:

This function sets a 2d camera. See the methods of ::IND_Camera2d for information on how you can manipulate the camera.
*/
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

	//------ Set the transformation -----
	glMultMatrixf(reinterpret_cast<GLfloat *>(&lookatmatrix));
    
    //Store result from GL matrix back to our local matrix
    float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
    _cameraMatrix.readFromArray(m);
    
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


/*!
\defgroup Graphical_Objects Bliting Surfaces, Animations, Fonts and setting transformations directly
\ingroup Advances
*/
/*@{*/

/*!
\b Parameters:

\arg \b pX                          Translation in the X coordinate. The (0, 0) position is the
                                    upper-left corner of the Viewport
\arg \b pY                          Translation in the Y coordinate. The (0, 0) position is the
                                    upper-left corner of the Viewport
\arg \b pAngleX                     Rotation in the angle x in degrees
\arg \b pAngleY                     Rotation in the angle y in degrees
\arg \b pAngleZ                     Rotation in the angle z in degrees
\arg \b pScaleX                     Scaling in the x coordinate. 1 for maintaining the original size
\arg \b pScaleY                     Scaling in the y coordinate. 1 for maintaining the original size
\arg \b pAxisCalX, \b pAxisCalY     Parameters that indicates the displacement that the graphical
                                    object undergoes due to the HotSpot. If the HotSpot is not specified,
                                    the value should be 0 for both of them. All the transformation
                                    will be aplied from the upper-left corner of the object.
\arg \b pMirrorX                    Horizontal mirroring. (true / false) = (activated / deactivated).
\arg \b pMirrorY                    Vertical mirroring. (true / false) = (activated / deactivated).
\arg \b pWidth                      Width of the graphical object that we are going to blit just after
                                    applying the transformation. You shoud use the getWidth() method
                                    of the object.
\arg \b pHeight                     Height of the graphical object that we are going to blit just after
                                    applying the transformation. You shoud use the getHeight() method
                                    on the object.
\arg \b pMatrix                     Pointer to a ::IND_Matrix matrix. In this parameter will be
                                    returned by reference the world matrix transformation that
                                    will be aplied to the graphical object. This matrix can be useful
                                    for advanced programmers that need the algebraic description
                                    of the transformation. It is possible to use the value 0
                                    if it not necessary to have this matrix information.
\b Operation:

This function sets the 2d transformation (translation, rotation, scaling, mirroring and hotspot)
of the following 2d graphical objects that will be rendered
by the engine. You should use this method before calling to any of the Bliting methods.

This method will apply transformations in a cummulative form to the already present transformations before calling it.
In other words, it doesn't RESET transformations but cumulates on them. If you want to completely REPLACE transformations
you should use the setTransform2d(IND_Matrix &pMatrix) method.

Remember that you can use IND_Entity2d object for applying 2d transformations to the graphical
objects without having to use this advanced method directly. This method is only useful for advanced
users with really concrete purposes.

Using this method is equivalent to using a combination of these methods:
- IND_Entity2d::setAnimation()
- IND_Entity2d::setSurface()
- IND_Entity2d::setPrimitive2d()
- IND_Entity2d::setFont()
- IND_Entity2d::setPosition()
- IND_Entity2d::setAngleXYZ()
- IND_Entity2d::setScale()
- IND_Entity2d::setBackCull()
- IND_Entity2d::setMirrorX()
- IND_Entity2d::setMirrorY()
- IND_Entity2d::setFilter()
- IND_Entity2d::setHotSpot()
- IND_Entity2d::setRegion()
- IND_Entity2d::toggleWrap()
*/
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

	// Hotspot
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
			_math.matrix4DSetRotationAroundAxis(mirrorX,180.0f,IND_Vector3(0.0f,1.0f,0.0f));
			_math.matrix4DMultiply(totalTrans,mirrorX,temp);
			totalTrans = temp;
			_math.matrix4DSetTranslation(mirrorX,
				static_cast<float>(-pWidth+pAxisCalX),//pWidth is the neeeded amount for normal mirroring, pAxisCalX is a correction for hotspot
				static_cast<float>(-pAxisCalY),  //Corrects the next translation when hotspot is on in Y
				0.0f); 
			_math.matrix4DMultiply(totalTrans,mirrorX,temp);
			totalTrans = temp;
		}

		//A mirror is a rotation in desired axis (the actual mirror) and a repositioning because rotation
		//also moves 'out of place' the entity translation-wise
		if (pMirrorY) {
			IND_Matrix mirrorY;
			_math.matrix4DSetRotationAroundAxis(mirrorY,180.0f,IND_Vector3(1.0f,0.0f,0.0f));
			_math.matrix4DMultiply(totalTrans,mirrorY,temp);
			totalTrans = temp;
			_math.matrix4DSetTranslation(mirrorY,
				static_cast<float>(-pAxisCalX), //Corrects the next translation when hotspot is on in X
				static_cast<float>(-pHeight+pAxisCalY), //pHeight is the neeeded amount for normal mirroring, pAxisCalY is a correction for hotspot
				0.0f); 
			_math.matrix4DMultiply(totalTrans,mirrorY,temp);
			totalTrans = temp;
		}
	}

	//Apply the changes to the GL matrix stack (model view)
    float camMatrixArray [16];
    _cameraMatrix.arrayRepresentation(camMatrixArray);
    glLoadMatrixf(camMatrixArray);
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
    float matrixArray [16];
    pMatrix.arrayRepresentation(matrixArray);
	glMultMatrixf(matrixArray);
}

void OpenGLRender::setIdentityTransform2d ()  {
	// ----- Applies the transformation -----
	float camMatrixArray [16];
    _cameraMatrix.arrayRepresentation(camMatrixArray);
    glLoadMatrixf(camMatrixArray);
}

/*!
\b Parameters:

\arg \b pType                                       ::IND_Type type of the following graphical object.
\arg \b pCull                                       Backface culling. (true / false) = (on / off). This feature can
                                                    be deactivated when we are going to rotate the graphical object
                                                    in the x or y coordinates. This way it is possible to draw for
                                                    example the leaf of a tree that falls down rotating and shows
                                                    both faces. In cases that you don't want to draw back faces
                                                    deactivate this for faster render times.
\arg \b pMirrorX                                    Horizontal mirroring. (true / false) = (activated / deactivated).
\arg \b pMirrorY                                    Vertical mirroring. (true / false) = (activated / deactivated).
\arg \b pFilter                                     Type of filter ::IND_Filter. There are two types of filters that
                                                    can be applied to the graphical object when it suffers a rotation
                                                    or scaling:
                                                    - ::IND_FILTER_POINT (Nearest point filter). Less quality, but
                                                    bigger performance. It is possible to use this one without
                                                    quality loss in graphical objects that don't rotate or are
                                                    affected by scaling.
                                                    - ::IND_FILTER_LINEAR (Bilinear filter). More quality, but less

                                                    performance. Used in graphical objects that be rotated or
                                                    affected by scaling.
\arg \b pR, pG, pB                                  Color bytes FadeR, FadeG, FadeB
                                                    Used for fading the image to a certain color.
                                                    For example, if we use RGB = (255, 0, 0),
                                                    the image will progressively become red.
\arg \b pA                                          The A byte indicates the level of transparency.
                                                    If a value of 0 is used, the following graphical
                                                    object will be completely transparent, as opposed
                                                    to the value 255 that will cause the object
                                                    to be drawn completely opaque. It is possible to use all the
                                                    intermediate values for different levels of
                                                    transparency.
\arg \b pFadeR, \b pFadeG, \b pFadeB, \b pFadeA     Bytes FadeR, FadeG, FadeB
                                                    Used for fading the image to a certain color.
                                                    For example, if we use RGB = (255, 0, 0),
                                                    the image will progressively become red.
\arg \b pSo                                         Indicates the blending source,
                                                    see (::IND_BlendingType).
\arg \b pDs                                         Indicates the blending destiny,
                                                    see (::IND_BlendingType).

Operation:

This functions sets the color and blending attributes of a graphical object. It should
be used before any calling to the Blit methods.

Important: It is not possible to use tinting and fading at the same time, the fading will overide the tinting.

Remember that you can use IND_Entity2d objects for applying color
transformations to the graphical objects without having to use this advanced method directly.
This method is only useful for advanced users with really concrete purposes.

Using this method is equivalent to using a combination of these methods:
- IND_Entity2d::setTransparency()
- IND_Entity2d::setFade()
- IND_Entity2d::setTint()
- IND_Entity2d::setBlendSource()
- IND_Entity2d::setBlendDest()
*/
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
	//Maybe disable texture-related features? (Filtering and so on...)

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


#endif //INDIERENDER_OPENGL
