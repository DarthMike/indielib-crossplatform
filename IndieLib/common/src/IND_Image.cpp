/*****************************************************************************************
 * File: IND_Image.cpp
 * Desc: Image object
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

// ----- Includes -----

#include <string.h>
#include "Global.h"
#include "IND_Image.h"
#include "FreeImageHelper.h"

// ----- Dependencies -----
#include "dependencies/FreeImage/Dist/FreeImage.h"

// --------------------------------------------------------------------------------
//										Public methods
// --------------------------------------------------------------------------------

/**
 * Clears the image to the chosen color. Returns 0 if there is no image loaded.
 * For ::IND_LUMINANCE image type, the 'color' value is taken from the pA parameter. Other parameters are ignored
 * @param pR						Byte R (Red).
 * @param pG						Byte G (Green).
 * @param pB						Byte B (Blue).
 * @param pA						Byte A (Transparency).
 */
bool IND_Image::clear(BYTE pR, BYTE pG, BYTE pB, BYTE pA) {
	// No image loaded
	if (!isImageLoaded() || !FreeImage_HasPixels(getFreeImageHandle())) 
		return false;

	FIBITMAP* dib = getFreeImageHandle();
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);
	int bpp (getBpp());
	int bytespp = bpp/8;
	int colorFormat =  getFormatInt();

	//Interprets differently depending on image type
	switch(image_type) {
		case FIT_BITMAP:
			//LOOP - Y coords
			for(unsigned y = 0; y < FreeImage_GetHeight(dib); y++) {
				BYTE *bits = FreeImage_GetScanLine(dib, y);
				//LOOP - X coords
				for(unsigned x = 0; x < FreeImage_GetWidth(dib); x++) {
					// Set pixel color to total transparency, if color matches given colorkey
					
					//RGBx color format
					if(IND_COLOUR_INDEX != colorFormat && IND_LUMINANCE != colorFormat) {
						bits[FI_RGBA_RED] = pR;
						bits[FI_RGBA_GREEN] = pG;
						bits[FI_RGBA_BLUE] = pB;
					} else if (IND_COLOUR_INDEX == colorFormat) {
						//TODO
					} else if (IND_LUMINANCE == colorFormat) {
						assert (8 == bpp); //The bpp for IND_LUMINANCE should be 8 for this image type
						*bits = pA;
		}
					 
					if (IND_RGBA == colorFormat) {
						bits[FI_RGBA_ALPHA] = pA;
	}

					// jump to next pixel
					bits += bytespp;
				}//LOOP END
			}//LOOP END
		break;

		//TODO: OTHER IMAGE TYPES
		default:
		break;
	}
	return true;
}

/**
 * Returns by reference the colour of the specified pixel (in RGBA). Returns 0 if there is no image loaded.
 * If the image format (see ::IND_ColorFormat) doesn't have alpha value (pA) this parameter can be omitted.
 * In the special case of ::IND_LUMINANCE type images, the pixel colour will be returned in pA.
 * @param pX						X coordinate.
 * @param pY						Y coordinate.
 * @param pR						Byte R (Red).
 * @param pG						Byte G (Green).
 * @param pB						Byte B (Blue).
 * @param pA						Byte A (Transparency).
 */
bool IND_Image::getPixel(int pX, int pY, BYTE *pR, BYTE *pG, BYTE *pB, BYTE *pA) {
	// No image loaded
	if (!isImageLoaded() || !FreeImage_HasPixels(getFreeImageHandle()))  return false;

	// Out of range
	if (pX >= getWidth())     return false;
	if (pY >= getHeight())     return false;
	if (pX < 0)                 return false;
	if (pY < 0)                 return false;

	FIBITMAP* dib = getFreeImageHandle();
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);
	int bpp (getBpp());
	int bytespp = bpp/8;
	int colorFormat =  getFormatInt();

	//Interprets differently depending on image type
	switch(image_type) {
		case FIT_BITMAP: {
			//Point to requested y coord
			BYTE *bits = FreeImage_GetScanLine(dib, pY);
			//Point to requested x coord
			bits += (bytespp * pX);
					
			//RGBx color format
			if(IND_COLOUR_INDEX != colorFormat && IND_LUMINANCE != colorFormat) {
				*pR = bits[FI_RGBA_RED];
				*pG = bits[FI_RGBA_GREEN];
				*pB = bits[FI_RGBA_BLUE];
			} else if (IND_COLOUR_INDEX == colorFormat) {
				//TODO
			} else if (IND_LUMINANCE == colorFormat) {
				assert (8 == bpp); //The bpp for IND_LUMINANCE should be 8 for this image type
				*pA = *bits;
	}
					 
			if (IND_RGBA == colorFormat) {
				*pA = bits[FI_RGBA_ALPHA];
	}
		break;
	}
		//TODO: OTHER IMAGE TYPES
		default:
		break;
	}

	return true;
}

/**
 * Puts a pixel in the position and color specified as parameters (in RGBA). Returns 0 if there is no image loaded.
 * If the image format (see ::IND_ColorFormat) doesn't have alpha value (pA) this parameter can be omitted.
 * In the special case of ::IND_LUMINANCE type images, the pixel color must be specified in pA, and other params (pR, pG, pB) are ignored.
 * @param pX						X coordinate.
 * @param pY						Y coordinate.
 * @param pR						Byte R (Red).
 * @param pG						Byte G (Green).
 * @param pB						Byte B (Blue).
 * @param pA						Byte A (Transparency).
 */
bool IND_Image::putPixel(int pX, int pY, BYTE pR, BYTE pG, BYTE pB, BYTE pA) {
	// No image loaded
	if (!isImageLoaded() || !FreeImage_HasPixels(getFreeImageHandle()))  return false;

	// Out of range
	if (pX >= getWidth())	return false;
	if (pY >= getHeight())	return false;
	if (pX < 0)		return false;
	if (pY < 0)		return false;

	FIBITMAP* dib = getFreeImageHandle();
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);
	int bpp (getBpp());
	int bytespp = bpp/8;
	int colorFormat =  getFormatInt();

	//Interprets differently depending on image type
	switch(image_type) {
		case FIT_BITMAP: {
			//Point to requested y coord
			BYTE *bits = FreeImage_GetScanLine(dib, pY);
			//Point to requested x coord
			bits += (bytespp * pX);
					
			//RGBx color format
			if(IND_COLOUR_INDEX != colorFormat && IND_LUMINANCE != colorFormat) {
				bits[FI_RGBA_RED] = pR;
				bits[FI_RGBA_GREEN] = pG;
				bits[FI_RGBA_BLUE] = pB;
			} else if (IND_COLOUR_INDEX == colorFormat) {
				//TODO
			} else if (IND_LUMINANCE == colorFormat) {
				assert (8 == bpp); //The bpp for IND_LUMINANCE should be 8 for this image type
				*bits = pA;
	}
					 
			if (IND_RGBA == colorFormat) {
				bits[FI_RGBA_ALPHA] = pA;
	}
		break;
	}
		//TODO: OTHER IMAGE TYPES
		default:
			break;	
	}

	return true;
}

/**
 * Horizontal inversion. Returns 0 if there is no image loaded.
 */
bool IND_Image::flipHorizontal() {
	// No image loaded
	if (!isImageLoaded()) return false;

	return FreeImage_FlipHorizontal(getFreeImageHandle()) != 0;
}

/**
 * Vertical inversion. Returns 0 if there is no image loaded.
 */
bool IND_Image::flipVertical() {
	// No image loaded
	if (!isImageLoaded()) return false;

	return FreeImage_FlipVertical(getFreeImageHandle()) != 0;
}

/**
 * Converts the image to the specified format. 
 * First convert to requested different color format, then convert to the requested bit depth.
 * Returns false if there is no image loaded or conversion is not possible.
 * @param pColorFormat				Format of the image. See ::IND_ColorFormat.
 * @param pBpp						New bits per pixel of the image.
 */
bool IND_Image::convert(IND_ColorFormat pColorFormat, int pBpp) {
	// No image loaded
	if (!isImageLoaded()) return false;

	//First convert based in target color format (if needed)
	FIBITMAP* converted;
	converted = FreeImageHelper::convertColorFormat(getFreeImageHandle(),pColorFormat);
	if (converted &&converted != getFreeImageHandle()) {
		FreeImage_Unload(getFreeImageHandle());
		setFreeImageHandle(converted);
    }
    
	//Convert based in bpp (if needed)
	converted = FreeImageHelper::convertBpp(getFreeImageHandle(),pBpp);
	if (converted && converted != getFreeImageHandle()) {
		FreeImage_Unload(getFreeImageHandle());
		setFreeImageHandle(converted);
	}

	//Reset image parameters to new freeimage modified one
	if (converted) {
		setFreeImageHandle(converted);
		IND_ColorFormat indFormat = FreeImageHelper::calculateINDFormat(converted);
		setFormatInt(indFormat);
		setBpp(FreeImage_GetBPP(converted));
		setBytespp(FreeImage_GetBPP(converted)/8);
		setPointer(FreeImage_GetBits(converted));
	}
	return converted != NULL;
}

/**
 * Sets the alpha value of all pixels of the specified color to transparent. This is know as "color key". Returns 0 if
 * there is no image loaded.
 * If you call this method on an image not having 32 bpp and ::IND_RGBA format, it will be converted first to that format.
 * It is recommended that you use this method with IND_RGBA color format images and 32 bpp.
 * The method returns true if image could be converted and alpha channel set correctly, false otherwise.
 * @param pR						Byte R (Red).
 * @param pG						Byte G (Green).
 * @param pB						Byte B (Blue).
 */
bool IND_Image::setAlpha(BYTE pR, BYTE pG, BYTE pB) {
	// No image loaded
	if (!isImageLoaded() || !FreeImage_HasPixels(getFreeImageHandle()))  return false;
	
	bool success = true;

	// We add alpha channel if the image hasn't.
	if (getFormatInt() != IND_RGBA) {
		int targetbpp (32);
		if (IND_RGB == getFormatInt()) {
			targetbpp = (getBpp()* 4 )/ 3;
	}

		success = convert(IND_RGBA,targetbpp);
	} 

	if (success) {
	setAlphaChannel(pR, pG, pB);
	}

	return success;
}

/**
 * Alpha blend or combine a sub part image with the current dib image.
 * For images of type FITBITMAP only: The bit depth of the dst bitmap must be greater than or
 * equal to the bit depth of the src. Upper promotion of src is done internally, without modifying
 * src. Supported dst bit depth equals to 1, 4, 8, 16, 24 or 32.
 *
 * For any other image type: The image type of the dst bitmap must be equal to the image type
 * of the src. The alpha parameter is always ignored and the source image is combined to the
 * destination image.
 *
 * The function returns TRUE if successful, FALSE otherwise (including if the one or both images is not loaded).
 * @param pIm               Pointer to an image object, that is going to be pasted (Alpha blended and/or combined with the current image)
 * @param pX                X Coordenate
 * @param pY                Y Coordenate
 * @param pAlpha            Alpha value on the destination image
 */
bool IND_Image::pasteImage(IND_Image *pIm, int pX, int pY, int pAlpha) {
	// No image loaded
	if (!isImageLoaded() || !pIm->isImageLoaded()) return false;

	return FreeImage_Paste(getFreeImageHandle(), pIm->getFreeImageHandle(), pX, pY, pAlpha) != 0;
}

/**
 * Applies a gaussian filter which intensity depends on the pIter parameter. Returns 0 if there
 * is no image loaded.
 * @param pIter						Iterations
 */
bool IND_Image::gaussianBlur(int) {
	// No image loaded
	if (!isImageLoaded()) return false;

	//TODO: Implement this on later release 
	//It was previously functionality offered by DevilLib.

	return true;
}

/**
 * Adjusts the contrast of a 8-, 24- or 32-bit image by a certain amount. This amount is given by
 * the percentage parameter, where percentage is a value between [-100..100]. A value 0
 * means no change, less than 0 will decrease the contrast and greater than 0 will increase the
 * contrast of the image.
 * The function returns TRUE on success, FALSE otherwise (e.g. when the bitdepth of the
 * source dib cannot be handled, or if no image is loaded).
 * @param pPercentage					Contrast.
 */
bool IND_Image::contrast(double pPercentage) {
	// No image loaded
	if (!isImageLoaded()) return false;

	return FreeImage_AdjustContrast(getFreeImageHandle(), pPercentage) != 0;
}

/**
 * Applies a equalizator filter. Returns 0 if there is no image loaded.
 */
bool IND_Image::equalize() {
	// No image loaded
	if (!isImageLoaded()) return false;

	//TODO: Implement this on later release 
	//It was previously functionality offered by DevilLib.

	return true;
}

/**
 * A value of 1.0 leaves the image alone, less than one darkens it, and greater than one lightens it.
 * The function returns TRUE on success. It returns FALSE when no image is loaded or gamma is less than or equal
 * to zero or when the bitdepth of the source dib cannot be handled.
 * @param pGamma					Gamma quality.
 */
bool IND_Image::setGamma(double pGamma) {
	// No image loaded
	if (!isImageLoaded()) return false;

	return FreeImage_AdjustGamma(getFreeImageHandle(), pGamma) != 0;
}

/**
 * Applies a negative filter. Returns 0 if there is no image loaded.
 */
bool IND_Image::negative() {
	// No image loaded
	if (!isImageLoaded()) return false;

	return FreeImage_Invert(getFreeImageHandle()) != 0;
}

/**
 * Adjusts an image's brightness, contrast and gamma as well as it may optionally
 * invert the image within a single operation. If more than one of these image display properties
 * need to be adjusted, using this function should be preferred over calling each adjustment
 * function separately. That's particularly true for huge images or if performance is an issue.
 * return false if no image is loaded or if the image manipulation otherwise fails.
 * @param pBrightness					Brightness.
 * @param pContrast					Contrast.
 * @param pGamma					Gamma quality.
 * @param pNegative					Negative filter on / off.
 */
bool IND_Image::adjustColors(double pBrightness, double pContrast, double pGamma, bool pNegative) {
	// No image loaded
	if (!isImageLoaded()) return false;

	return FreeImage_AdjustColors(getFreeImageHandle(), pBrightness, pContrast, pGamma, pNegative) != 0;
}

/**
 * Applies a noise filter which intensity depends on the pNoise parameter. The greater this value is,
 * the more noise is applied to the image. Returns 0 if there is no image loaded.
 * @param pNoise					Noise quantity.
 */
bool IND_Image::noisify(float) {
	// No image loaded
	if (!isImageLoaded()) return false;

	//TODO: Implement this on later release 
	//It was previously functionality offered by DevilLib.

	return true;
}

/**
 * Applies a pixelation filter. The size of each pixel is specified by the parameter
 * pPixSize. Returns 0 if there is no image loaded.
 * @param pPixSize					Size of each pixel.
 */
bool IND_Image::pixelize(int) {
	// No image loaded
	if (!isImageLoaded()) return false;

	//TODO: Implement this on later release 
	//It was previously functionality offered by DevilLib.

	return true;
}

/**
 * Applies a definition filter that depends of the parameters pFactor (the greater this
 * parameter, the more "defined" the image will be) and the pIter parameter that indicates the number of times that
 * the filter is applied consecutivity. This filter can improve the quality in images that
 * have been deteriorated. Returns 0 if there is no image loaded.
 * @param pFactor					"Definition" factor.
 * @param pIter						Numbers of iterations.
 */
bool IND_Image::sharpen(float, int) {
	// No image loaded
	if (!isImageLoaded()) return false;

	//TODO: Implement this on later release 
	//It was previously functionality offered by DevilLib.

	return true;
}

/**
 * Applies a scale to the height and width specified as paramteres. Returns 0 if there is no image loaded, or if the scaling fails (wrong bitdepth or to large image memorywise).
 *
 *
 *	TODO: It could be nice to add an extra parameter so that a user could choose to use our "default" filter choice, or to specify what exact filter there is to be used.
 *
 *	possible filter choices by FreeImage:
 *
 *	Filter flag, Description
 *	FILTER_BOX Box, pulse, Fourier window, 1st order (constant) B-Spline
 *	FILTER_BILINEAR Bilinear filter
 *	FILTER_BSPLINE 4th order (cubic) B-Spline
 *	FILTER_BICUBIC Mitchell and Netravali's two-param cubic filter
 *	FILTER_CATMULLROM Catmull-Rom spline, Overhauser spline
 *	FILTER_LANCZOS3 Lanczos-windowed sinc filter
 *
 * @param pWidth					Width in píxels.
 * @param pHeight					Height in píxels.
 */
bool IND_Image::scale(int pWidth, int pHeight) {
	// No image loaded
	if (!isImageLoaded()) return false;

	FIBITMAP *scaled = NULL;

	// Scaling filter (when minimizing => Bilinear | when maximizing => Catmull-Rom)
	if (pWidth < getWidth() || pHeight < getHeight())
		scaled = FreeImage_Rescale(getFreeImageHandle(), pWidth, pHeight, FILTER_BILINEAR);

	else
		scaled = FreeImage_Rescale(getFreeImageHandle(), pWidth, pHeight, FILTER_CATMULLROM);

	// scaling can in some rare circumstances return a NULL value (NULL value is returned when the bitdepth cannot be handled or when there is not enough memory (very large images)).
	if (scaled == NULL) return false;

	//Reset image parameters to new freeimage modified one
	FreeImage_Unload(getFreeImageHandle());
	setFreeImageHandle(scaled);
	setPointer(FreeImage_GetBits(scaled));
	setWidth(FreeImage_GetWidth(getFreeImageHandle()));
	setHeight(FreeImage_GetHeight(getFreeImageHandle()));

	return true;
}

/**
 * Applies a filter of border detection (type 1). Returns 0 if there is no image loaded.
 */
bool IND_Image::edgeDetect1() {
	// No image loaded
	if (!isImageLoaded()) return false;

	//TODO: Implement this on later release 
	//It was previously functionality offered by DevilLib.

	return true;
}

/**
 * Applies a filter of border detection (type 2). Returns 0 if there is no image loaded.
 */
bool IND_Image::edgeDetect2() {
	// No image loaded
	if (!isImageLoaded()) return false;

	//TODO: Implement this on later release 
	//It was previously functionality offered by DevilLib.

	return true;
}

/**
 * Applies a emboss filter. Returns 0 if there is no image loaded.
 */
bool IND_Image::emboss() {
	// No image loaded
	if (!isImageLoaded()) return false;

	//TODO: Implement this on later release 
	//It was previously functionality offered by DevilLib.

	return true;
}

/**
 * Applies a rotation in an angle which is specified as the pAngle parameter. Returns 0 if there is no image loaded or if the rotation fails.
 * Rotation occurs around the center of the image area. Rotated image
 * retains size and aspect ratio of source image (destination image size is usually bigger), so
 * that this function should be used when rotating an image by 90°, 180° or 270°.
 * @param pAngle					Rotation angle.
 */
bool IND_Image::rotate(double pAngle) {
	//TODO: MAY NEED ACCESS TO FILLCOLOR WHEN ANGLE IS NOT MULTIPLE OF 90 (BIGGER IMAGE AND BACKGROUND FILLED WITH BLACK COLOR, OPAQUE, BY DEFAULT)
	
	// No image loaded
	if (!isImageLoaded()) return false;

	FIBITMAP *rotated = FreeImage_Rotate(getFreeImageHandle(), pAngle);

	// rotation can in some rare circumstances return a NULL value (1-bit images, rotation is limited to angles whose value is an integer multiple of 90°)
	if (rotated == NULL) return false;
	
	//Reset image parameters to new freeimage modified one
	FreeImage_Unload(getFreeImageHandle());
	setFreeImageHandle(rotated);
	setPointer(FreeImage_GetBits(rotated));
	setWidth(FreeImage_GetWidth(rotated));
	setHeight(FreeImage_GetHeight(rotated));
	
	return true;
}

/**
 * Inverts the alpha values (transparency) of the image. Returns 0 if there is no image loaded.
 * Note!: if image type is ::IND_LUMINANCE, it does nothing. Use ::IND_Image::negative() instead.
 */
bool IND_Image::invertAlpha() {
	// No image loaded
	if (!isImageLoaded() || !FreeImage_HasPixels(getFreeImageHandle())) 
		return false;

	int bpp (getBpp());
	int bytespp = bpp/8;
	FIBITMAP* dib = getFreeImageHandle();
	FREE_IMAGE_TYPE imgType = FreeImage_GetImageType(dib);
	
	switch (imgType) {
		case FIT_BITMAP: {
			//TODO: IND_COLOUR_INDEX
			if(getFormatInt() == IND_RGBA) {
			//LOOP - Y coords
			for(unsigned y = 0; y < FreeImage_GetHeight(dib); y++) {
				BYTE *bits = FreeImage_GetScanLine(dib, y);
				//LOOP - X coords
				for(unsigned x = 0; x < FreeImage_GetWidth(dib); x++) {
					// Invert the alpha value in a per-pixel basis
					BYTE alpha = bits[FI_RGBA_ALPHA];
					BYTE max = 255;
					bits[FI_RGBA_ALPHA] = max - alpha;
					// jump to next pixel
					bits += bytespp;
				}//LOOP END
			}//LOOP END
			}
			break;
		}
		//TODO: OTHER IMAGE TYPES
		default:
			break;
	}

	return true;
}

/**
 * Applies a saturation filter which depends of the paramenter pSaturation. The greater
 * this value is, the more saturated the image will be. Returns 0 if there is no image loaded.
 * @param pSaturation					Saturation quantity of the image.
 */
bool IND_Image::saturation(float) {
	// No image loaded
	if (!isImageLoaded()) return false;

	//TODO: Implement this on later release 
	//It was previously functionality offered by DevilLib.

	return true;
}

// --------------------------------------------------------------------------------
//									Private methods
// --------------------------------------------------------------------------------

/** @cond DOCUMENT_PRIVATEAPI */

/*
==================
ColorKey
==================
*/
void IND_Image::setAlphaChannel(BYTE pR, BYTE pG, BYTE pB) {
	if (!isImageLoaded()) return;
	if (IND_RGBA != getFormatInt()) return;

	int bpp (getBpp());
	int bytespp = bpp/8;

	//FIXME: this will only work with 32bpp and IND_RGBA formats!

	FIBITMAP* dib = getFreeImageHandle();
	FREE_IMAGE_TYPE imgType = FreeImage_GetImageType(dib);
	switch (imgType) {
		case FIT_BITMAP:
			if(	32 == bpp) {
				//LOOP - Y coords
				for(unsigned y = 0; y < FreeImage_GetHeight(dib); y++) {
					BYTE *bits = FreeImage_GetScanLine(dib, y);
					//LOOP - X coords
					for(unsigned x = 0; x < FreeImage_GetWidth(dib); x++) {
						// Set pixel color to total transparency, if color matches given colorkey
						if (pR == bits[FI_RGBA_RED] && pG == bits[FI_RGBA_GREEN] && pB == bits[FI_RGBA_BLUE]) {
							bits[FI_RGBA_ALPHA] = 0x00;
						} else {
							bits[FI_RGBA_ALPHA] = 0xFF;
		}
						// jump to next pixel
						bits += bytespp;
					}//LOOP END
				}//LOOP END
			} else {
				g_debug->header("Intent to set alpha channel via colorkey to non 32-bit RGBA image!",DebugApi::LogHeaderError);
	}
		break;
		
		default:
			g_debug->header("Intent to set alpha channel via colorkey to not supported image!",DebugApi::LogHeaderError);
			break;
	}
}

/*
==================
Id (integer) format to string
==================
*/
string IND_Image::formatToString(IND_ColorFormat pColorFormat) {
	switch (pColorFormat) {
	case IND_COLOUR_INDEX:
		return "IND_COLOUR_INDEX";
	case IND_RGB:
		return "IND_RGB";
	case IND_RGBA:
		return "IND_RGBA";
	case IND_LUMINANCE:
		return "IND_LUMINANCE";
	case IND_UNKNOWN:
	default:
		return "FORMAT_NOT_IDENTIFIED";
	}
}

/** @endcond */
