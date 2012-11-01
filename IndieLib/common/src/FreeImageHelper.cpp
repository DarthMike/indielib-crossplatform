/****************************************************************************************************************
 *   File: FreeImageHelper.cpp
 *   Desc: Helper class containing methods to calculate several parameters from/to freeImage library
 ****************************************************************************************************************/

/*
     Copyright: Miguel Angel Quiñones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)
     Comments : You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
                Nevertheless it would be nice if you tell me you are using something I made, just for curiosity
*/

#include "FreeImageHelper.h"
#include "Global.h"

/** @cond DOCUMENT_PRIVATEAPI */

/**
 * Freeimage format to Indielib format.
 * @param pHandle		the FIBITMAP that is used for calculation
 */
int FreeImageHelper::calculateINDFormat(FIBITMAP* pHandle) {

	FREE_IMAGE_TYPE imgType = FreeImage_GetImageType(pHandle);

	//Bitmap type (most common)
	if (FIT_BITMAP == imgType) {
		//Depending on the freeimage color type analysis, we determine our IND_ColorFormat
		FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(pHandle);

		
			
		if (FIC_MINISBLACK == colorType || FIC_MINISWHITE == colorType) {
			return IND_LUMINANCE;
		} else if (FIC_PALETTE == colorType) {
			return IND_COLOUR_INDEX;
		} else if (FIC_RGB == colorType) {
			//HACK: This is because when converting to 32 bits, even though there is alpha channel,
			//the color analysis function returns FIC_RGB, as alpha is opaque. We rely on that to know
			//if we have alpha channel so it's not good.
			if (FreeImage_GetBPP(pHandle) == 32) {
				return IND_RGBA;
			} else {
				return IND_RGB;
			}
		} else if (FIC_RGBALPHA == colorType) {
			return IND_RGBA;
		}
	}

	//TODO: OTHER IMAGE TYPES

	//Failure
	return IND_UNKNOWN;
}

/**
 * Indielib format to FreeImage Color Type.
 * @param pIndFormat		the Indielib format that is used for calculation
 */
FREE_IMAGE_COLOR_TYPE FreeImageHelper::calculateFIColorType(int pIndFormat) {
	switch (pIndFormat) {
		case (IND_LUMINANCE):
			return FIC_MINISBLACK;
			break;
		case (IND_COLOUR_INDEX):
			return FIC_PALETTE;
			break;
		case (IND_RGB):
			return FIC_RGB;
			break;
		case (IND_RGBA):
			return FIC_RGBALPHA;
			break;
		default:
			return FIC_RGB;
	}
}

/**
 * What kind of indian processor are we running on. (TODO: This method should probably be placed another central class,
 * and should be runned one time at startup and the result written to a class variable with a getter to it).
 */
bool FreeImageHelper::isLittleIndian() {
	return FreeImage_IsLittleEndian() != 0;
}

/**
 * Returns a boolean of wheather the supplied FREE_IMAGE_FORMAT is supported by Indielib or not.
 * @param pIndFormat		FREE_IMAGE_FORMAT as defined by the Freeimage library
 */
bool FreeImageHelper::isImageFormatSupported(FREE_IMAGE_FORMAT format){
		//if	  (format == FIF_UNKNOWN)	{	return false;	}	// Unknown format (returned value only, never use it as input value)
		if		(format == FIF_BMP)			{	return true;	}	// Windows or OS/2 Bitmap File (*.BMP)
		//else if (format == FIF_ICO)		{	return false;	}	// Windows Icon (*.ICO)
		else if (format == FIF_JPEG)		{	return true;	}	// Independent JPEG Group (*.JPG, *.JIF, *.JPEG, *.JPE)
		//else if (format == FIF_JNG)		{	return false;	}	// JPEG Network Graphics (*.JNG)
		//else if (format == FIF_KOALA)		{	return false;	}	// Commodore 64 Koala format (*.KOA)
		//else if (format == FIF_LBM)		{	return false;	}	// Amiga IFF (*.IFF, *.LBM)
		//else if (format == FIF_IFF)		{	return false;	}	// Amiga IFF (*.IFF, *.LBM)
		//else if (format == FIF_MNG)		{	return false;	}	// Multiple Network Graphics (*.MNG)
		//else if (format == FIF_PBM)		{	return false;	}	// Portable Bitmap (ASCII) (*.PBM)
		//else if (format == FIF_PBMRAW)	{	return false;	}	// Portable Bitmap (BINARY) (*.PBM)
		//else if (format == FIF_PCD)		{	return false;	}	// Kodak PhotoCD (*.PCD)
		else if (format == FIF_PCX)			{	return true;	}	// Zsoft Paintbrush PCX bitmap format (*.PCX)
		//else if (format == FIF_PGM)		{	return false;	}	// Portable Graymap (ASCII) (*.PGM)
		//else if (format == FIF_PGMRAW)	{	return false;	}	// Portable Graymap (BINARY) (*.PGM)
		else if (format == FIF_PNG)			{	return true;	}	// Portable Network Graphics (*.PNG)
		//else if (format == FIF_PPM)		{	return false;	}	// Portable Pixelmap (ASCII) (*.PPM)
		//else if (format == FIF_PPMRAW)	{	return false;	}	// Portable Pixelmap (BINARY) (*.PPM)
		//else if (format == FIF_RAS)		{	return false;	}	// Sun Rasterfile (*.RAS)
		else if (format == FIF_TARGA)		{   return true;	}	// truevision Targa files (*.TGA, *.TARGA)
		//else if (format == FIF_TIFF)		{	return false;	}	// Tagged Image File Format (*.TIF, *.TIFF)
		//else if (format == FIF_WBMP)		{	return false;	}	// Wireless Bitmap (*.WBMP)
		//else if (format == FIF_PSD)		{	return false;	}	// Adobe Photoshop (*.PSD)
		//else if (format == FIF_CUT)		{	return false;	}	// Dr. Halo (*.CUT)
		//else if (format == FIF_XBM)		{	return false;	}	// X11 Bitmap Format (*.XBM)
		//else if (format == FIF_XPM)		{	return false;	}	// X11 Pixmap Format (*.XPM)
		//else if (format == FIF_DDS)		{	return false;	}	// DirectDraw Surface (*.DDS)
		//else if (format == FIF_GIF)		{	return false;	}	// Graphics Interchange Format (*.GIF)
		//else if (format == FIF_HDR)		{	return false;	}	// High Dynamic Range (*.HDR)
		//else if (format == FIF_FAXG3)		{	return false;	}	// Raw Fax format CCITT G3 (*.G3)
		//else if (format == FIF_SGI)		{	return false;	}	// Silicon Graphics SGI image format (*.SGI)
		//else if (format == FIF_EXR)		{	return false;	}	// OpenEXR format (*.EXR)
		//else if (format == FIF_J2K)		{	return false;	}	// JPEG-2000 format (*.J2K, *.J2C)
		//else if (format == FIF_JP2)		{	return false;	}	// JPEG-2000 format (*.JP2)
		//else if (format == FIF_PFM)		{	return false;	}	// Portable FloatMap (*.PFM)
		//else if (format == FIF_PICT)		{	return false;	}	// Macintosh PICT (*.PICT)
		//else if (format == FIF_RAW)		{	return false;	}	// RAW camera image (*.*) 
		else								{   return false;	}
}

/**
 * Returns the extension name of the FREE_IMAGE_FORMAT in the supplied chararray. 
 * @param format		FREE_IMAGE_FORMAT as defined by the Freeimage library.
 * @param pExtImage[in,out]		image extention that is going to hold the extension of the format.If the FREE_IMAGE_FORMAT is not supported by Indielib or is not known by Freeimage, "" is returned.
 */
void FreeImageHelper::getImageFormatName(FREE_IMAGE_FORMAT format, char* pExtImage){
		//if	  (format == FIF_UNKNOWN)	{	strcpy (pExtImage, "");		}	// Unknown format (returned value only, never use it as input value)
		if		(format == FIF_BMP)			{	strcpy (pExtImage, "bmp");	}	// Windows or OS/2 Bitmap File (*.BMP)
		//else if (format == FIF_ICO)		{	strcpy (pExtImage, "");		}	// Windows Icon (*.ICO)
		else if	(format == FIF_JPEG)		{	strcpy (pExtImage, "jpg");	}	// Independent JPEG Group (*.JPG, *.JIF, *.JPEG, *.JPE)
		//else if (format == FIF_JNG)		{	strcpy (pExtImage, "");		}	// JPEG Network Graphics (*.JNG)
		//else if (format == FIF_KOALA)		{	strcpy (pExtImage, "");		}	// Commodore 64 Koala format (*.KOA)
		//else if (format == FIF_LBM)		{	strcpy (pExtImage, "");		}	// Amiga IFF (*.IFF, *.LBM)
		//else if (format == FIF_IFF)		{	strcpy (pExtImage, "");		}	// Amiga IFF (*.IFF, *.LBM)
		//else if (format == FIF_MNG)		{	strcpy (pExtImage, "");		}	// Multiple Network Graphics (*.MNG)
		//else if (format == FIF_PBM)		{	strcpy (pExtImage, "");		}	// Portable Bitmap (ASCII) (*.PBM)
		//else if (format == FIF_PBMRAW)	{	strcpy (pExtImage, "");		}	// Portable Bitmap (BINARY) (*.PBM)
		//else if (format == FIF_PCD)		{	strcpy (pExtImage, "");		}	// Kodak PhotoCD (*.PCD)
		else if	(format == FIF_PCX)			{	strcpy (pExtImage, "pcx");	}	// Zsoft Paintbrush PCX bitmap format (*.PCX)
		//else if (format == FIF_PGM)		{	strcpy (pExtImage, "");		}	// Portable Graymap (ASCII) (*.PGM)
		//else if (format == FIF_PGMRAW)	{	strcpy (pExtImage, "");		}	// Portable Graymap (BINARY) (*.PGM)
		else if	(format == FIF_PNG)			{	strcpy (pExtImage, "png");	}	// Portable Network Graphics (*.PNG)
		//else if (format == FIF_PPM)		{	strcpy (pExtImage, "");		}	// Portable Pixelmap (ASCII) (*.PPM)
		//else if (format == FIF_PPMRAW)	{	strcpy (pExtImage, "");		}	// Portable Pixelmap (BINARY) (*.PPM)
		//else if (format == FIF_RAS)		{	strcpy (pExtImage, "");		}	// Sun Rasterfile (*.RAS)
		else if	(format == FIF_TARGA)		{   strcpy (pExtImage, "tga");	}	// truevision Targa files (*.TGA, *.TARGA)
		//else if (format == FIF_TIFF)		{	strcpy (pExtImage, "");		}	// Tagged Image File Format (*.TIF, *.TIFF)
		//else if (format == FIF_WBMP)		{	strcpy (pExtImage, "");		}	// Wireless Bitmap (*.WBMP)
		//else if (format == FIF_PSD)		{	strcpy (pExtImage, "");		}	// Adobe Photoshop (*.PSD)
		//else if (format == FIF_CUT)		{	strcpy (pExtImage, "");		}	// Dr. Halo (*.CUT)
		//else if (format == FIF_XBM)		{	strcpy (pExtImage, "");		}	// X11 Bitmap Format (*.XBM)
		//else if (format == FIF_XPM)		{	strcpy (pExtImage, "");		}	// X11 Pixmap Format (*.XPM)
		//else if (format == FIF_DDS)		{	strcpy (pExtImage, "");		}	// DirectDraw Surface (*.DDS)
		//else if (format == FIF_GIF)		{	strcpy (pExtImage, "");		}	// Graphics Interchange Format (*.GIF)
		//else if (format == FIF_HDR)		{	strcpy (pExtImage, "");		}	// High Dynamic Range (*.HDR)
		//else if (format == FIF_FAXG3)		{	strcpy (pExtImage, "");		}	// Raw Fax format CCITT G3 (*.G3)
		//else if (format == FIF_SGI)		{	strcpy (pExtImage, "");		}	// Silicon Graphics SGI image format (*.SGI)
		//else if (format == FIF_EXR)		{	strcpy (pExtImage, "");		}	// OpenEXR format (*.EXR)
		//else if (format == FIF_J2K)		{	strcpy (pExtImage, "");		}	// JPEG-2000 format (*.J2K, *.J2C)
		//else if (format == FIF_JP2)		{	strcpy (pExtImage, "");		}	// JPEG-2000 format (*.JP2)
		//else if (format == FIF_PFM)		{	strcpy (pExtImage, "");		}	// Portable FloatMap (*.PFM)
		//else if (format == FIF_PICT)		{	strcpy (pExtImage, "");		}	// Macintosh PICT (*.PICT)
		//else if (format == FIF_RAW)		{	strcpy (pExtImage, "");		}	// RAW camera image (*.*) 
		else								{	strcpy (pExtImage, "");		}
}


/**
 * Helper to encapsulate Bpp conversion using FreeImage, taking into consideration color type. Creates a new bitmap if needed. Can return the same one.
 * @param pHandle		FIBITMAP as defined by the Freeimage library.
 * @param pNewBpp		the new bpp that the supplied FIBITMAP is to be coverted to.
 */
FIBITMAP* FreeImageHelper::convertBpp(FIBITMAP* pHandle, int pNewBpp) {	
	//Convert the bit depth
	FIBITMAP *converted = NULL;
	
	//Convert Bpp only if needed
	if (pNewBpp != static_cast<int>(FreeImage_GetBPP(pHandle))) {
		//Transform bpp to the pixel format
		switch (pNewBpp) {
			case (4):
				converted = FreeImage_ConvertTo4Bits(pHandle);
				break;
			case (8):
				converted = FreeImage_ConvertTo8Bits(pHandle);
				break;
			case(16):
				converted = FreeImage_ConvertTo16Bits565(pHandle);  //There's option of 16Bits555. But leaves unused bits. Chosen 565 because it's most common
				break;
			case(24):
			   converted = FreeImage_ConvertTo24Bits(pHandle);    
			    break;
			case(32): {
			    converted = FreeImage_ConvertTo32Bits (pHandle); 
				}
			    break;
			default:
				converted = NULL;
				break;
		}
	} else {
		//No change, just set 'converted' handle to current one
		converted = pHandle;
	}

	return converted;
}
/**
 * Helper to encapsulate color conversion using FreeImage, taking into consideration bpp and original color type.
 * @param pHandle		FIBITMAP as defined by the Freeimage library.
 * @param pNewFormat		IndieLib colorformat to convert to.
 */
FIBITMAP* FreeImageHelper::convertColorFormat(FIBITMAP* pHandle, int pNewFormat) {
	int colorFormat = FreeImageHelper::calculateINDFormat(pHandle);
	if (colorFormat == pNewFormat) return pHandle;

	FIBITMAP* converted (NULL);

	//TODO: Simplified version only accepts pNewFormat as IND_RGB or IND_RGBA
	//Expand it to convert to any format needed
	if (pNewFormat != IND_RGB && pNewFormat != IND_RGBA) return NULL;
		
	//Check target format
	switch (pNewFormat) {
		case IND_LUMINANCE:
			//TODO
			break;

		case IND_COLOUR_INDEX: 			
			break;
		case IND_RGB:
			if (IND_COLOUR_INDEX == colorFormat) {
				converted = FreeImage_ConvertTo16Bits565(pHandle);
			} else {
				converted = FreeImage_ConvertTo24Bits(pHandle);
			}

			break;

		case IND_RGBA:
			if (IND_COLOUR_INDEX == colorFormat) {
				converted = FreeImage_ConvertTo24Bits(pHandle);
			} else {
				converted = FreeImage_ConvertTo32Bits(pHandle);
			}
			
			break;

		default:
			g_debug->header("INCORRECT TEXTURE COLOR FORMAT SPECIFIED TO CONVERT TO", 2);
			break;
	}

	return converted;
}

/** @endcond */
