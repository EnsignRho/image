//////////
//
// image.cpp
//
//////
//
//


#include "image.h"


//////////
// Global variables
//////
	union
	{
		SBitmap*	bmp;
		s32			_bmp;
	} u, u1;

	union
	{
		SBitmap*	bmp;
		s32			_bmp;
	} u2;


//////////
// Forward declarations
//////
	IMAGE_API s32		bmp_new							(s32 tnWidth, s32 tnHeight, s32 tnBitCount);
	IMAGE_API s32		bmp_open_file					(s8* tcPathname);
	IMAGE_API s32		bmp_crop_to_content				(s32 tnHandle, int tnRgb, int tnType, f32 tfThreshold);
	IMAGE_API s32		bmp_get_attributes				(s32 tnHandle, s8* tcWidthOutInches8, s8* tcHeightOutInches8, s8* tcBitsPerPixel8, s8* tcPixelsPerInchX8, s8* tcPixelsPerInchY8);
	IMAGE_API s32		bmp_resize						(s32 tnHandle, float tfScaleFactor);
	IMAGE_API s32		bmp_save_file					(s32 tnHandle, s8* tcNewBmpPathname);
	IMAGE_API s32		bmp_save_as_monochrome			(s32 tnHandle, s8* tcMonoBmpPathname);
	IMAGE_API s32		bmp_close						(s32 tnHandle);

	IMAGE_API s32		bmp_flip_horizontal				(s32 tnHandle);
	IMAGE_API s32		bmp_flip_vertical				(s32 tnHandle);
	IMAGE_API s32		bmp_rotate_90_degrees_cw		(s32 tnHandle);

	IMAGE_API s32		bmp_grayscale					(s32 tnHandle, float tfAlpha);
	IMAGE_API s32		bmp_set_pixel					(s32 tnHandle, s32 tnX, s32 tnY, s32 tnRgb);
	IMAGE_API s32		bmp_get_pixel					(s32 tnHandle, s32 tnX, s32 tnY);
	IMAGE_API s32		bmp_colorize					(s32 tnHandle, s32 tnULX, s32 tnULY, s32 tnLRX, s32 tnLRY, s32 templateRgb, f32 tfAlpha);
	IMAGE_API s32		bmp_swap_colors					(s32 tnHandle, s32 colorOldRgb, s32 colorNewRgb);
	IMAGE_API s32		bmp_wavy_line					(s32 tnHandle, s32 tnULX, s32 tnULY, s32 tnLRX, s32 tnLRY, s32 colorLine, f32 tfAlpha);
	IMAGE_API s32		bmp_dapple						(s32 tnHandle, s32 tnHandleDapple, f32 tfInfluence);
	IMAGE_API s32		bmp_draw_font					(s32 tnHandle, s8* tcFontName, s32 tnPointSize, bool tlBold, bool tlItalic, bool tlUnderline, bool tlStrikethru, s8* tcText, s32 tnX, s32 tnY, s32 textColor);
	IMAGE_API s32		bmp_draw_font_fixed_point		(s32 tnHandle, s32 fontPixelsX, s32 fontPixelsY, s8* tcText, s32 tnTextLength, s32 tnX, s32 tnY, s32 foreColor, s32 backColor);
	IMAGE_API s32		bmp_draw_bullet					(s32 tnHandle, s32 tnX, s32 tnY, s32 colorBullet);
	IMAGE_API s32		bmp_fill_rect					(s32 tnHandle, s32 tnULX, s32 tnULY, s32 tnLRX, s32 tnLRY, s32 tnColorNW, s32 tnColorNE, s32 tnColorSW, s32 tnColorSE, bool tlUseGradient);
	IMAGE_API s32		bmp_frame_rect					(s32 tnHandle, s32 tnULX, s32 tnULY, s32 tnLRX, s32 tnLRY, s32 tnColorNW, s32 tnColorNE, s32 tnColorSW, s32 tnColorSE, bool tlUseGradient);
	IMAGE_API s32		bmp_colorize_rect				(s32 tnHandle, s32 tnULX, s32 tnULY, s32 tnLRX, s32 tnLRY, s32 tnColorNW, s32 tnColorNE, s32 tnColorSW, s32 tnColorSE, bool tlUseGradient, f32 tfAlpha);
	IMAGE_API s32		bmp_frame_in_nine_parts			(s32 tnHandle, s32 tnULX, s32 tnULY, s32 tnLRX, s32 tnLRY, s32 tnHandleSrc);
	IMAGE_API s32		bmp_draw_line					(s32 tnHandle, s32 tnX1, s32 tnY1, s32 tnX2, s32 tnY2, s32 tnColor);
	IMAGE_API s32		bmp_draw_quad					(s32 tnHandle, s32 tnX1, s32 tnY1, s32 tnX2, s32 tnY2, s32 tnWidth, bool tlDrawEnds, s32 tnColor);
	IMAGE_API s32		bmp_colorized_line_gradient		(s32 tnHandle, s32 tnX1, s32 tnY1, s32 tnX2, s32 tnY2, s32 tnColorBeg, s32 tnColorEnd, f32 tfAlpha);
	



//////////
//
// Main DLL load point
//
//////
	BOOL APIENTRY DllMain( HANDLE hModule, 
						DWORD  ul_reason_for_call, 
						LPVOID lpReserved
						)
	{
		switch (ul_reason_for_call)
		{
			case DLL_PROCESS_ATTACH:
			case DLL_THREAD_ATTACH:
			case DLL_THREAD_DETACH:
			case DLL_PROCESS_DETACH:
				break;
		}
		return TRUE;
	}




//////////
//
// Called to create a new bitmap of the indicated size
//
//////
	IMAGE_API s32 bmp_new(s32 tnWidth, s32 tnHeight, s32 tnBitCount)
	{
		union
		{
			SBitmap*	bmpNew;
			s32			_bmpNew;
		};


		// Verify our parameters
		if (tnWidth < 0 || tnHeight < 0 || !(tnBitCount == 24 || tnBitCount == 32))
			return -1;

		// Create our bitmap
		if (!(bmpNew = iBmp_allocate()))
			return -2;

		// Create it to size
		iBmp_createBySize(bmpNew, tnWidth, tnHeight, tnBitCount);
		if (!bmpNew->bd)
			return -3;

		// Signify
		return _bmpNew;
	}




//////////
//
// Called to open the indicated image file
//
//////
	IMAGE_API s32 bmp_open_file(s8* tcPathname)
	{
		if (!tcPathname)
			return -1;

		// Try to open the file
		u.bmp = iBmp_rawLoadImageFile((cu8*)tcPathname, false, true);
		if (!u.bmp)
			return -2;

		// Signify
		return u._bmp;
	}




//////////
//
// Will crop the image to the content which is populated based on color
//
// tnType values:		0	white
//						1	black
//						2	nRgb
//						3	auto-detect
//
//////
	IMAGE_API s32 bmp_crop_to_content(s32 tnHandle, int tnRgb, int tnType, f32 tfThreshold)
	{
		union
		{
			SBitmap*	bmpCropped;
			s32			_bmpCropped;
		};


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		//iBmp_saveToDisk(u.bmp, "c:\\temp\\test.bmp");

		// Crop it down to its non-border color content
		bmpCropped = iBmp_cropToContent(u.bmp, tnRgb, tnType, tfThreshold);

		// Signify
		return _bmpCropped;
	}




//////////
//
// Retrieve image attributes
//
//////
	IMAGE_API s32 bmp_get_attributes(s32 tnHandle, s8* tcWidthOutInches8, s8* tcHeightOutInches8, s8* tcBitsPerPixel8, s8* tcPixelsPerInchX8, s8* tcPixelsPerInchY8)
	{
		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Write out the info
		sprintf(tcWidthOutInches8,  "%.3f", iBmp_getWidthInches(u.bmp));
		sprintf(tcHeightOutInches8, "%.3f", iBmp_getHeightInches(u.bmp));
		sprintf(tcBitsPerPixel8,    "%d",   u.bmp->bi.biBitCount);
		sprintf(tcPixelsPerInchX8,  "%d",   (s32)iBmp_getPixelsPerInchX(u.bmp));
		sprintf(tcPixelsPerInchY8,  "%d",   (s32)iBmp_getPixelsPerInchY(u.bmp));
		return 0;
	}




//////////
//
// Resize the image based on the scale factor
//
//////
	IMAGE_API s32 bmp_resize(s32 tnHandle, float tfScaleFactor)
	{
		union
		{
			SBitmap*	bmpScaled;
			s32			_bmpScaled;
		};


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Calculate the ratios
		bmpScaled = iBmp_scale(u.bmp, tfScaleFactor);
		
		// Signify
		return _bmpScaled;
	}
;;




//////////
//
// Called to save the bitmap to the indicated file type.
// We currently only recognize PNG and JPG.
//
//////
	IMAGE_API s32 bmp_save_file(s32 tnHandle, s8* tcNewBmpPathname)
	{
		s8		fileExtension[_MAX_PATH];


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Get the file extension
		if (iDisk_getFileExtension((cs8*)tcNewBmpPathname, fileExtension) <= 0)
			return -2;

		// Save it based on file type
		if (_memicmp(fileExtension, cgcPng, sizeof(cgcPng)) == 0)
			iBmp_rawSave(u.bmp, tcNewBmpPathname, PNG);	// PNG file
		else if (_memicmp(fileExtension, cgcJpg, sizeof(cgcJpg)) == 0)
			iBmp_rawSave(u.bmp, tcNewBmpPathname, JPG);	// JPG file
		else if (_memicmp(fileExtension, cgcGif, sizeof(cgcGif)) == 0)
			iBmp_rawSave(u.bmp, tcNewBmpPathname, GIF);	// GIF file
		else if (_memicmp(fileExtension, cgcTif, sizeof(cgcTif)) == 0)
			iBmp_rawSave(u.bmp, tcNewBmpPathname, TIF);	// TIF file
		else
			iBmp_rawSave(u.bmp, tcNewBmpPathname, BMP);	// Assume bitmap file

		// Signify
		return 0;
	}

	IMAGE_API s32 bmp_save_as_monochrome(s32 tnHandle, s8* tcMonoBmpPathname)
	{
		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Write out as monochrome
		return iBmp_saveToDisk_asMonochromeBitmap(u.bmp, tcMonoBmpPathname);
	}




//////////
//
// Called to close a bitmap, or release its image data
//
//////
	IMAGE_API s32 bmp_close(s32 tnHandle)
	{
		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Delete image
		iBmp_delete(&u.bmp, true, true);

		// Signify
		return 0;
	}




//////////
//
// Flips the image pixel by pixel horizontally.
// Creates a new bitmap for return.
//
//////
	IMAGE_API s32 bmp_flip_horizontal(s32 tnHandle)
	{
		union
		{
			SBitmap*	bmpNew;
			s32			_bmpNew;
		};


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Calculate the ratios
		bmpNew = iBmp_flipHorizontal(u.bmp);

		// Signify
		return _bmpNew;
	}




//////////
//
// Flips the image pixel by pixel vertically.
// Creates a new bitmap for return.
//
//////
	IMAGE_API s32 bmp_flip_vertical(s32 tnHandle)
	{
		union
		{
			SBitmap*	bmpNew;
			s32			_bmpNew;
		};


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Calculate the ratios
		bmpNew = iBmp_flipVertical(u.bmp);

		// Signify
		return _bmpNew;
	}




//////////
//
// Rotates the image counter-clockwise 90 degrees.
// Creates a new bitmap for return.
//
//////
	IMAGE_API s32 bmp_rotate_90_degrees_ccw(s32 tnHandle)
	{
		union
		{
			SBitmap*	bmpNew;
			s32			_bmpNew;
		};


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Calculate the ratios
		bmpNew = iBmp_rotate90DegreesCCW(u.bmp);

		// Signify
		return _bmpNew;
	}




//////////
//
// Apply a grayscaling to the image
//
//////
	IMAGE_API s32 bmp_grayscale(s32 tnHandle, float tfAlpha)
	{
		RECT	lrc;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Set our rectangle
		SetRect(&lrc, 0, 0, u.bmp->bi.biWidth, u.bmp->bi.biHeight);

		// Grayscale
		return iBmp_grayscale(u.bmp, &lrc, tfAlpha);
	}




//////////
//
// Sets a pixel in the image
//
//////
	IMAGE_API s32 bmp_set_pixel(s32 tnHandle, s32 tnX, s32 tnY, s32 tnRgb)
	{
		SBgra	color;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Set our color
		color.red = red_rgb(tnRgb);
		color.grn = grn_rgb(tnRgb);
		color.blu = blu_rgb(tnRgb);
		color.alp = 255;

		// Grayscale
		return iiBmp_setPixel(u.bmp, tnX, tnY, color);
	}




//////////
//
// Grabs a pixel in the image
//
//////
	IMAGE_API s32 bmp_get_pixel(s32 tnHandle, s32 tnX, s32 tnY)
	{
		u8		lnRed, lnGrn, lnBlu;
		SBgra	color;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Grab the pixel color at that location
		color = iBmp_extractColorAtPoint(u.bmp, tnX, tnY);

		// Set our color
		lnRed = color.red;
		lnGrn = color.grn;
		lnBlu = color.blu;

		// Signify
		return RGB(lnRed, lnGrn, lnBlu);
	}




//////////
//
// Colorizes an image
//
//////
	IMAGE_API s32 bmp_colorize(s32 tnHandle, s32 tnULX, s32 tnULY, s32 tnLRX, s32 tnLRY, s32 templateRgb, f32 tfAlpha)
	{
		RECT	lrc;
		SBgra	colorTemplate;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Set our rectangle
		SetRect(&lrc, tnULX, tnULY, tnLRX, tnLRY);

		// Set the colors
		colorTemplate.red = red_rgb(templateRgb);
		colorTemplate.grn = grn_rgb(templateRgb);
		colorTemplate.blu = blu_rgb(templateRgb);
		colorTemplate.alp = 255;

		// Colorize
		return iBmp_colorize(u.bmp, &lrc, colorTemplate, tfAlpha);
	}




//////////
//
// Swaps a color in an image with another color
//
//////
	IMAGE_API s32 bmp_swap_colors(s32 tnHandle, s32 colorOldRgb, s32 colorNewRgb)
	{
		SBgra	colorOld, colorNew;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Set the colors
		colorOld.red = red_rgb(colorOldRgb);
		colorOld.grn = grn_rgb(colorOldRgb);
		colorOld.blu = blu_rgb(colorOldRgb);
		colorOld.alp = 255;

		colorNew.red = red_rgb(colorNewRgb);
		colorNew.grn = grn_rgb(colorNewRgb);
		colorNew.blu = blu_rgb(colorNewRgb);
		colorNew.alp = 255;

		// Swap out the color
		return iBmp_swapColors(u.bmp, colorOld, colorNew);
	}




//////////
//
// Draws a wavy line like how a misspelled word is highlighted
//
//////
	IMAGE_API s32 bmp_wavy_line(s32 tnHandle, s32 tnULX, s32 tnULY, s32 tnLRX, s32 tnLRY, s32 colorLine, f32 tfAlpha)
	{
		SBgra	color;
		RECT	lrc;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Set our rectangle
		SetRect(&lrc, tnULX, tnULY, tnLRX, tnLRY);

		// Set the line color
		color.red = red_rgb(colorLine);
		color.grn = grn_rgb(colorLine);
		color.blu = blu_rgb(colorLine);
		color.alp = 255;

		// Colorize
		return iBmp_wavyLine(u.bmp, &lrc, color, tfAlpha);
	}




//////////
//
// Dapples an image with another image by the influence
//
//////
	IMAGE_API s32 bmp_dapple(s32 tnHandle, s32 tnHandleDapple, f32 tfInfluence)
	{
		// Grab our bitmaps
		if (!(u._bmp = tnHandle))			return -1;
		if (!(u2._bmp = tnHandleDapple))	return -1;

		// Dapple
		iBmp_dapple(u.bmp, u2.bmp, NULL, 1.0f, tfInfluence);

		// Signify
		return 0;
	}




//////////
//
// Draws some text in the indicated font
//
//////
	IMAGE_API s32 bmp_draw_font(s32 tnHandle, s8* tcFontName, s32 tnPointSize, bool tlBold, bool tlItalic, bool tlUnderline, bool tlStrikethru, s8* tcText, s32 tnX, s32 tnY, s32 textColor)
	{
		SBgra	color;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Verify the parameters
		if (!tcText)			return -2;
		if (!tcFontName)		return -3;
		if (tnPointSize < 0)	return -4;

		// Set the text color
		color.red = red_rgb(textColor);
		color.grn = grn_rgb(textColor);
		color.blu = blu_rgb(textColor);
		color.alp = 255;

		// Draw the text
		return iBmp_drawFontTT(	u.bmp,
								tcFontName, tnPointSize, tlBold, tlItalic, tlUnderline, tlStrikethru,
								tcText, tnX, tnY, color);
	}




//////////
//
// Draws some text in a fixed point font, 8x6, 8x8, 8x14, 8x16, or 16x32
//
//////
	IMAGE_API s32 bmp_draw_font_fixed_point(s32 tnHandle, s32 fontPixelsX, s32 fontPixelsY, s8* tcText, s32 tnTextLength, s32 tnX, s32 tnY, s32 foreColor, s32 backColor)
	{
		SBgra		colorFore, colorBack;
		RECT		lrc;


		//////////
		// Validate parameters
		//////
			// Grab our bitmap
			if (!(u._bmp = tnHandle))
				return -1;

			// Make sure we have text
			if (!tcText)
				return -2;


		//////////
		// Validate the fontPixelsX x fontPixelsY
		//////
			switch (fontPixelsX)
			{
				case 8:
					// Only allow 8x6, 8x8, 8x14, and 8x16
					switch (fontPixelsY)
					{
						case 6:
						case 8:
						case 12:
						case 14:
						case 16:
							break;
						default:
							return -3;	// Invalid height
					}
					break;

				case 16:
					// Only allow font 16x32
					if (fontPixelsY != 32)
						return -4;	// Invalid height
					break;

				default:
					// Invalid width
					return -5;
			}


		//////////
		// Build our colors
		//////
			colorFore.red = red_rgb(foreColor);
			colorFore.grn = grn_rgb(foreColor);
			colorFore.blu = blu_rgb(foreColor);
			colorFore.alp = 255;

			colorBack.red = red_rgb(backColor);
			colorBack.grn = grn_rgb(backColor);
			colorBack.blu = blu_rgb(backColor);
			colorBack.alp = 255;


		//////////
		// Physically render
		//////
			SetRect(&lrc, tnX, tnY, tnX + (tnTextLength * fontPixelsX), (tnY + fontPixelsY));
			iiBmp_drawFontBitmap(u.bmp, fontPixelsX, fontPixelsY, tcText, tnTextLength, &lrc, colorFore, colorBack);

			// Signify
			return tnTextLength;
	}




//////////
//
// Draws a bullet at the indicated location
//
//////
	IMAGE_API s32 bmp_draw_bullet(s32 tnHandle, s32 tnX, s32 tnY, s32 colorBullet, f32 tfAlpha)
	{
		SBgra	color;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Set the text color
		color.red = red_rgb(colorBullet);
		color.grn = grn_rgb(colorBullet);
		color.blu = blu_rgb(colorBullet);
		color.alp = 255;

		// Draw the bullet
		iBmp_drawBullet(u.bmp, tnX, tnY, color, tfAlpha);

		// Signify
		return 0;
	}




//////////
//
// Fills the rectangle with a color, or an optional gradient
//
//////
	IMAGE_API s32 bmp_fill_rect(s32 tnHandle, s32 tnULX, s32 tnULY, s32 tnLRX, s32 tnLRY, s32 tnColorNW, s32 tnColorNE, s32 tnColorSW, s32 tnColorSE, bool tlUseGradient)
	{
		SBgra		colorNW, colorNE, colorSW, colorSE;
		RECT		lrc;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Extract the colors
		colorNW.red = red_rgb(tnColorNW);
		colorNW.grn = grn_rgb(tnColorNW);
		colorNW.blu = blu_rgb(tnColorNW);
		colorNW.alp = 255;

		colorNE.red = red_rgb(tnColorNE);
		colorNE.grn = grn_rgb(tnColorNE);
		colorNE.blu = blu_rgb(tnColorNE);
		colorNE.alp = 255;

		colorSW.red = red_rgb(tnColorSW);
		colorSW.grn = grn_rgb(tnColorSW);
		colorSW.blu = blu_rgb(tnColorSW);
		colorSW.alp = 255;

		colorSE.red = red_rgb(tnColorSE);
		colorSE.grn = grn_rgb(tnColorSE);
		colorSE.blu = blu_rgb(tnColorSE);
		colorSE.alp = 255;

		// Fill the rectangle
		SetRect(&lrc, tnULX, tnULY, tnLRX, tnLRY);
		iBmp_fillRect(u.bmp, &lrc, colorNW, colorNE, colorSW, colorSE, tlUseGradient);

		// Signify
		return 0;
	}




//////////
//
// Frames the rectangle with a color, or an optional gradient
//
//////
	IMAGE_API s32 bmp_frame_rect(s32 tnHandle, s32 tnULX, s32 tnULY, s32 tnLRX, s32 tnLRY, s32 tnColorNW, s32 tnColorNE, s32 tnColorSW, s32 tnColorSE, bool tlUseGradient)
	{
		SBgra		colorNW, colorNE, colorSW, colorSE;
		RECT		lrc;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Extract the colors
		colorNW.red = red_rgb(tnColorNW);
		colorNW.grn = grn_rgb(tnColorNW);
		colorNW.blu = blu_rgb(tnColorNW);
		colorNW.alp = 255;

		colorNE.red = red_rgb(tnColorNE);
		colorNE.grn = grn_rgb(tnColorNE);
		colorNE.blu = blu_rgb(tnColorNE);
		colorNE.alp = 255;

		colorSW.red = red_rgb(tnColorSW);
		colorSW.grn = grn_rgb(tnColorSW);
		colorSW.blu = blu_rgb(tnColorSW);
		colorSW.alp = 255;

		colorSE.red = red_rgb(tnColorSE);
		colorSE.grn = grn_rgb(tnColorSE);
		colorSE.blu = blu_rgb(tnColorSE);
		colorSE.alp = 255;

		// Frame the rectangle
		SetRect(&lrc, tnULX, tnULY, tnLRX, tnLRY);
		iBmp_frameRect(u.bmp, &lrc, colorNW, colorNE, colorSW, colorSE, tlUseGradient);

		// Signify
		return 0;
	}




//////////
//
// Colorize the indicated rectangle (changes everything there to a grayscale, then makes it the color indicated)
//
//////
	IMAGE_API s32 bmp_colorize_rect(s32 tnHandle, s32 tnULX, s32 tnULY, s32 tnLRX, s32 tnLRY, s32 tnColorNW, s32 tnColorNE, s32 tnColorSW, s32 tnColorSE, bool tlUseGradient, f32 tfAlpha)
	{
		SBgra		colorNW, colorNE, colorSW, colorSE;
		RECT		lrc;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Extract the colors
		colorNW.red = red_rgb(tnColorNW);
		colorNW.grn = grn_rgb(tnColorNW);
		colorNW.blu = blu_rgb(tnColorNW);
		colorNW.alp = 255;

		colorNE.red = red_rgb(tnColorNE);
		colorNE.grn = grn_rgb(tnColorNE);
		colorNE.blu = blu_rgb(tnColorNE);
		colorNE.alp = 255;

		colorSW.red = red_rgb(tnColorSW);
		colorSW.grn = grn_rgb(tnColorSW);
		colorSW.blu = blu_rgb(tnColorSW);
		colorSW.alp = 255;

		colorSE.red = red_rgb(tnColorSE);
		colorSE.grn = grn_rgb(tnColorSE);
		colorSE.blu = blu_rgb(tnColorSE);
		colorSE.alp = 255;

		// Colorize the rectangle
		SetRect(&lrc, tnULX, tnULY, tnLRX, tnLRY);
		iBmp_colorizeRect(u.bmp, &lrc, colorNW, colorNE, colorSW, colorSE, tlUseGradient, NULL, false, tfAlpha);

		// Signify
		return 0;
	}




//////////
//
// Used for drawing things like buttons, will draw the reference image in nine parts, which are
// the upper-left, upper-middle, upper-right, midle-left, middle-middle, middle-right, lower-left,
// lower-middle, and lower-right.  It allows a small image to frame a much larger or arbitrary
// size using the outlining shape.
//
//////
	IMAGE_API s32 bmp_frame_in_nine_parts(s32 tnHandle, s32 tnULX, s32 tnULY, s32 tnLRX, s32 tnLRY, s32 tnHandleSrc)
	{
		RECT	lrc;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))			return -1;
		if (!(u2._bmp = tnHandleSrc))		return -1;

		// Frame the image
		SetRect(&lrc, tnULX, tnULY, tnLRX, tnLRY);
		iiBmp_frameInNineParts(u.bmp, &lrc, u2.bmp);

		// Signify
		return 0;
	}




//////////
//
// Draws a line in the indicated color
//
//////
	IMAGE_API s32 bmp_draw_line(s32 tnHandle, s32 tnX1, s32 tnY1, s32 tnX2, s32 tnY2, s32 tnColor)
	{
		SBgra	color;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Set the color
		color.red = red_rgb(tnColor);
		color.grn = grn_rgb(tnColor);
		color.blu = blu_rgb(tnColor);
		color.alp = 255;

		// Draw the line
		iBmp_drawArbitraryLine(u.bmp, tnX1, tnY1, tnX2, tnY2, color);

		// Signify
		return 0;
	}




//////////
//
// Draws a quad, which is different than frame_rect because it allows for a width, and no gradient
//
//////
	IMAGE_API s32 bmp_draw_quad(s32 tnHandle, s32 tnX1, s32 tnY1, s32 tnX2, s32 tnY2, s32 tnWidth, bool tlDrawEnds, s32 tnColor)
	{
		SBgra	color;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Set the color
		color.red = red_rgb(tnColor);
		color.grn = grn_rgb(tnColor);
		color.blu = blu_rgb(tnColor);
		color.alp = 255;

		// Draw the line
		iBmp_drawArbitraryQuad(u.bmp, tnX1, tnY1, tnX2, tnY2, tnWidth, tlDrawEnds, color);

		// Signify
		return 0;
	}




//////////
//
// Colorizes a line (draws a line)
//
//////
	IMAGE_API s32 bmp_colorized_line_gradient(s32 tnHandle, s32 tnX1, s32 tnY1, s32 tnX2, s32 tnY2, s32 tnColorBeg, s32 tnColorEnd, f32 tfAlpha)
	{
		SBgra	colorBeg, colorEnd;


		// Grab our bitmap
		if (!(u._bmp = tnHandle))
			return -1;

		// Set the color
		colorBeg.red = red_rgb(tnColorBeg);
		colorBeg.grn = grn_rgb(tnColorBeg);
		colorBeg.blu = blu_rgb(tnColorBeg);
		colorBeg.alp = 255;

		colorEnd.red = red_rgb(tnColorEnd);
		colorEnd.grn = grn_rgb(tnColorEnd);
		colorEnd.blu = blu_rgb(tnColorEnd);
		colorEnd.alp = 255;

		// Draw the line
		iBmp_drawColorizedLineGradient(u.bmp, tnX1, tnY1, tnX2, tnY2, colorBeg, colorEnd, tfAlpha);

		// Signify
		return 0;
	}
