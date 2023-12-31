//////////
//
// This file is part of the Visual FreePro, Jr. project, modified for image.dll.
//
// Thank you.  And may The Lord bless you richly as you lift up your life, your
// talents, your gifts, your praise, unto Him.  In Jesus' name I pray.  Amen.
//
//


#define _EXCLUDE_iBmp_cask_createAndPopulate
#define _EXCLUDE_iBmp_colorizeAsCommonTooltipBackground


//////////
// Allocates a new structure
//////
	SBitmap* iBmp_allocate(void)
	{
		SBitmap* bmp;


		// Allocate our new structure
		bmp = (SBitmap*)malloc(sizeof(SBitmap));

		// Initialize if successful
		if (bmp)
			memset(bmp, 0, sizeof(SBitmap));

		// Indicate our success or failure
		return(bmp);
	}





//////////
//
// Called to copy a bitmap, to duplicate it completely
//
//////
	SBitmap* iBmp_copy(SBitmap* bmpSrc)
	{
		SBitmap*	bmp;
		RECT		lrc;


		// Make sure our environment is sane
		bmp = NULL;
		if (bmpSrc && iBmp_validate(bmpSrc))
		{
			// Allocate a new structure
			bmp = iBmp_allocate();
			if (bmp)
			{
				// Create a bitmap of the target size
				iBmp_createBySize(bmp, bmpSrc->bi.biWidth, bmpSrc->bi.biHeight, bmpSrc->bi.biBitCount);

				// Copy the bitmap over
				SetRect(&lrc, 0, 0, bmpSrc->bi.biWidth, bmpSrc->bi.biHeight);

				// Optimization, copy directly all allocated data rather than bit by bit
				memcpy(bmp->bd, bmpSrc->bd, bmpSrc->bi.biSizeImage);
				//iBmp_bitBlt(bmp, &lrc, bmpSrc);
			}
		}

		// Indicate our success or failure
		return(bmp);
	}




//////////
//
// Called to crop an image to valid content (non-white pixels)
//
//////
	//
	// Will crop the image to the content which is populated based on color
	//
	// tnType values:		0	white
	//						1	black
	//						2	nRgb
	//						3	auto-detect
	//
	SBitmap* iBmp_cropToContent(SBitmap* bmpSrc, int tnRgb, int tnType, f32 tfThreshold)
	{
		s32			lnX, lnY, lnNewWidth, lnNewHeight;
		s32			lnCropTop, lnCropLeft, lnCropRight, lnCropBottom;
		SRgb		maskColor, c1, c2, c3, c4;
		SRgb*		lrgb;
		SRgba*		lrgba;
		SBitmap*	bmpNew;


		// Make sure our environment is sane
		if (!bmpSrc || (bmpSrc->bi.biBitCount != 24 && bmpSrc->bi.biBitCount != 32))
			return NULL;

		// Initialize
		lnCropTop		= 0;
		lnCropLeft		= 0;
		lnCropRight		= 0;
		lnCropBottom	= 0;

		// Based on the type, determine our maskColor (the color we're looking for that is our border color)
		switch (tnType)
		{
			case 0:
			default:
				maskColor.red = 255;
				maskColor.grn = 255;
				maskColor.blu = 255;
				break;

			case 1:
				maskColor.red = 0;
				maskColor.grn = 0;
				maskColor.blu = 0;
				break;

			case 2:
				maskColor.red = red(tnRgb);
				maskColor.grn = grn(tnRgb);
				maskColor.blu = blu(tnRgb);
				break;

			case 3:
				// Auto-detect the colors
				iibmp_getBorderColors(bmpSrc, &c1, &c2, &c3, &c4);

				// They must all be the same for the auto-detect to work
				if (!iBmp_compareColors(&c1, &c2, &c3, &c4))
					return NULL;

				maskColor.red = c1.red;
				maskColor.grn = c1.grn;
				maskColor.blu = c1.blu;
				break;
		}


//////////
// 24-bit
//////
		// Process
		if (bmpSrc->bi.biBitCount == 24)
		{
			// 24-bit image

			//////////
			// Left
			//////
				// Search for the first non-white pixel
				for (lnX = 0; lnX < bmpSrc->bi.biWidth / 2; ++lnX, ++lnCropLeft)
				{
					// Get our starting offset for this row
					lrgb = (SRgb*)(bmpSrc->bd + ((bmpSrc->bi.biHeight - 1) * bmpSrc->rowWidth) + (lnX * (bmpSrc->bi.biBitCount / 8)));

					// Iterate top-down
					for (lnY = 0; lnY < bmpSrc->bi.biHeight; ++lnY, lrgb = (SRgb*)((s8*)lrgb - bmpSrc->rowWidth))
					{
						// If something's not white, we're done
						if (!iiBmp_isWithinColorThreshold(lrgb, maskColor, tfThreshold))
							goto done_left_24;
					}
				}
			done_left_24:


			//////////
			// Right
			//////
				// Search for the first non-white pixel
				for (lnX = bmpSrc->bi.biWidth - 1; lnX >= bmpSrc->bi.biWidth / 2; --lnX, ++lnCropRight)
				{
					// Get our starting offset for this row
					lrgb = (SRgb*)(bmpSrc->bd + ((bmpSrc->bi.biHeight - 1) * bmpSrc->rowWidth) + (lnX * (bmpSrc->bi.biBitCount / 8)));

					// Iterate top-down
					for (lnY = 0; lnY < bmpSrc->bi.biHeight; ++lnY, lrgb = (SRgb*)((s8*)lrgb - bmpSrc->rowWidth))
					{
						// If something's not white, we're done
						if (!iiBmp_isWithinColorThreshold(lrgb, maskColor, tfThreshold))
							goto done_right_24;
					}
				}
			done_right_24:


			//////////
			// Top
			//////
				// Search for the first non-white pixel
				for (lnY = 0; lnY < bmpSrc->bi.biHeight / 2; ++lnY, ++lnCropTop)
				{
					// Get our starting offset for this row
					lrgb = (SRgb*)(bmpSrc->bd + ((bmpSrc->bi.biHeight - lnY - 1) * bmpSrc->rowWidth));

					// Iterate top-down
					for (lnX = 0; lnX < bmpSrc->bi.biWidth; ++lnX, ++lrgb)
					{
						// If something's not white, we're done
						if (!iiBmp_isWithinColorThreshold(lrgb, maskColor, tfThreshold))
							goto done_top_24;
					}
				}
			done_top_24:


			//////////
			// Bottom
			//////
				// Search for the first non-white pixel
				for (lnY = bmpSrc->bi.biHeight - 1; lnY >= bmpSrc->bi.biHeight / 2; --lnY, ++lnCropBottom)
				{
					// Get our starting offset for this row
					lrgb = (SRgb*)(bmpSrc->bd + ((bmpSrc->bi.biHeight - lnY - 1) * bmpSrc->rowWidth) + ((bmpSrc->bi.biWidth - 1) * (bmpSrc->bi.biBitCount / 8)));

					// Iterate top-down
					for (lnX = 0; lnX < bmpSrc->bi.biWidth; ++lnX, --lrgb)
					{
						// If something's not white, we're done
						if (!iiBmp_isWithinColorThreshold(lrgb, maskColor, tfThreshold))
							goto done_bottom_24;
					}
				}
			done_bottom_24:
				nocode;


//////////
// 32-bit
//////
		} else if (bmpSrc->bi.biBitCount == 32) {
			// 32-bit image

			//////////
			// Left
			//////
				// Search for the first non-white pixel
				for (lnX = 0; lnX < bmpSrc->bi.biWidth / 2; ++lnX, ++lnCropLeft)
				{
					// Get our starting offset for this row
					lrgba = (SRgba*)(bmpSrc->bd + ((bmpSrc->bi.biHeight - 1) * bmpSrc->rowWidth) + (lnX * (bmpSrc->bi.biBitCount / 8)));

					// Iterate top-down
					for (lnY = 0; lnY < bmpSrc->bi.biHeight; ++lnY, lrgba = (SRgba*)((s8*)lrgba - bmpSrc->rowWidth))
					{
						// If something's not white, we're done
						if (!iiBmp_isWithinColorThreshold(lrgba, maskColor, tfThreshold))
							goto done_left_32;
					}
				}
			done_left_32:


			//////////
			// Right
			//////
				// Search for the first non-white pixel
				for (lnX = bmpSrc->bi.biWidth - 1; lnX >= bmpSrc->bi.biWidth / 2; --lnX, ++lnCropRight)
				{
					// Get our starting offset for this row
					lrgba = (SRgba*)(bmpSrc->bd + ((bmpSrc->bi.biHeight - 1) * bmpSrc->rowWidth) + (lnX * (bmpSrc->bi.biBitCount / 8)));

					// Iterate top-down
					for (lnY = 0; lnY < bmpSrc->bi.biHeight; ++lnY, lrgba = (SRgba*)((s8*)lrgba - bmpSrc->rowWidth))
					{
						// If something's not white, we're done
						if (!iiBmp_isWithinColorThreshold(lrgba, maskColor, tfThreshold))
							goto done_right_32;
					}
				}
			done_right_32:


			//////////
			// Top
			//////
				// Search for the first non-white pixel
				for (lnY = 0; lnY < bmpSrc->bi.biHeight / 2; ++lnY, ++lnCropTop)
				{
					// Get our starting offset for this row
					lrgba = (SRgba*)(bmpSrc->bd + ((bmpSrc->bi.biHeight - lnY - 1) * bmpSrc->rowWidth));

					// Iterate top-down
					for (lnX = 0; lnX < bmpSrc->bi.biWidth; ++lnX, ++lrgba)
					{
						// If something's not white, we're done
						if (!iiBmp_isWithinColorThreshold(lrgba, maskColor, tfThreshold))
							goto done_top_32;
					}
				}
			done_top_32:


			//////////
			// Bottom
			//////
				// Search for the first non-white pixel
				for (lnY = bmpSrc->bi.biHeight - 1; lnY >= bmpSrc->bi.biHeight / 2; --lnY, ++lnCropBottom)
				{
					// Get our starting offset for this row
					lrgba = (SRgba*)(bmpSrc->bd + ((bmpSrc->bi.biHeight - lnY - 1) * bmpSrc->rowWidth) + ((bmpSrc->bi.biWidth - 1) * (bmpSrc->bi.biBitCount / 8)));

					// Iterate top-down
					for (lnX = 0; lnX < bmpSrc->bi.biWidth; ++lnX, --lrgba)
					{
						// If something's not white, we're done
						if (!iiBmp_isWithinColorThreshold(lrgba, maskColor, tfThreshold))
							goto done_bottom_32;
					}
				}
			done_bottom_32:
				nocode;

		} else {
			// Invalid bit count
			return NULL;
		}

		// We have the crop coordinates
		// Calculate our new dimensions
		lnNewWidth  = bmpSrc->bi.biWidth  - (lnCropLeft + lnCropRight);
		lnNewHeight = bmpSrc->bi.biHeight - (lnCropTop  + lnCropBottom);

		// Make sure we're at a minimum of 1x1
		if (lnNewWidth < 1)		lnNewWidth  = 1;
		if (lnNewHeight < 1)	lnNewHeight = 1;

		// Create a new bitmap
		bmpNew = iBmp_allocate();
		iBmp_createBySize(bmpNew, lnNewWidth, lnNewHeight, bmpSrc->bi.biBitCount);

		// Copy over the portion
		BitBlt(bmpNew->hdc, 0, 0, lnNewWidth, lnNewHeight, bmpSrc->hdc, lnCropLeft, lnCropTop, SRCCOPY);

		iBmp_saveToDisk(bmpNew, "z:\\Pictures\\image.bmp");

		// Signify
		return bmpNew;
	}

	bool iiBmp_isWithinColorThreshold(SRgb* rgb, SRgb maskColor, f32 tfThreshold)
	{
		// What is the threshold?
		if (tfThreshold == 0.0f)
		{
			// An exact color match
			if (rgb->red != maskColor.red || rgb->grn != maskColor.grn || rgb->blu != maskColor.blu)
				return false;

			// Signify
			return true;
		}

		// Compute a common threshold
		return iiBmp_isWithinColorThreshold(rgb->red, rgb->grn, rgb->blu, maskColor, tfThreshold);
	}

	bool iiBmp_isWithinColorThreshold(SRgba* rgba, SRgb maskColor, f32 tfThreshold)
	{
		// What is the threshold?
		if (tfThreshold == 0.0f)
		{
			// An exact color match
			if (rgba->red != maskColor.red || rgba->grn != maskColor.grn || rgba->blu != maskColor.blu)
				return false;

			// Signify
			return true;
		}

		// Compute a common threshold
		return iiBmp_isWithinColorThreshold(rgba->red, rgba->grn, rgba->blu, maskColor, tfThreshold);
	}

	bool iiBmp_isWithinColorThreshold(u8 red, u8 grn, u8 blu, SRgb maskColor, f32 tfThreshold)
	{
		// Do the colors
		if (!iiBmp_isColorChannelWithinThreshold((f32)red, (f32)maskColor.red, tfThreshold))	return false;
		if (!iiBmp_isColorChannelWithinThreshold((f32)grn, (f32)maskColor.grn, tfThreshold))	return false;
		if (!iiBmp_isColorChannelWithinThreshold((f32)blu, (f32)maskColor.blu, tfThreshold))	return false;

		// Signify
		return true;
	}

	bool iiBmp_isColorChannelWithinThreshold(f32 c, f32 cRef, f32 tfThreshold)
	{
		f32		cDelta, cMin, cMax;


		// Calculate the delta
		cDelta = (tfThreshold * 255.0f);

		// Calculate the min
		if ((cMin = cRef - cDelta) < 0.0f)
			cMin = 0.0f;

		// Calculate the max
		if ((cMax = cRef + cDelta) > 255.0f)
			cMax = 255.0f;

		// Perform the test
		return (c >= cMin && c <= cMax);
	}




//////////
//
// Called to get the border colors (across the top, right, bottom, left in that order)
//
//////
	bool iibmp_getBorderColors(SBitmap* bmp, SRgb* cTop, SRgb* cRight, SRgb* cBottom, SRgb* cLeft)
	{
		s32		lnX, lnY;
		SRgb*	lrgb;
		SRgb*	rgbLast;
		SRgba*	lrgba;
		SRgba*	rgbaLast;


		//////////
		// Top
		//////
			if (bmp->bi.biBitCount == 24)
			{
				// Grab 24-bit pointer
				lrgb = (SRgb*)(bmp->bd + ((bmp->bi.biHeight - 1) * bmp->rowWidth));

				// Iterate across the top
				rgbLast = lrgb;
				for (lnX = 0; lnX < bmp->bi.biWidth; ++lnX, ++lrgb)
				{
					// See if the color's changed
					if (lrgb->red != rgbLast->red || lrgb->grn != rgbLast->grn || lrgb->blu != rgbLast->blu)
						return  false;		// Color has changed
				}

				// When we get here, rgbaLast is the color
				cTop->red = rgbLast->red;
				cTop->grn = rgbLast->grn;
				cTop->blu = rgbLast->blu;

			} else if (bmp->bi.biBitCount == 32) {
				// Grab 32-bit pointer
				lrgba = (SRgba*)(bmp->bd + ((bmp->bi.biHeight - 1) * bmp->rowWidth));

				// Iterate across the top
				rgbaLast = lrgba;
				for (lnX = 0; lnX < bmp->bi.biWidth; ++lnX, ++lrgba)
				{
					// See if the color's changed
					if (lrgba->red != rgbaLast->red || lrgba->grn != rgbaLast->grn || lrgba->blu != rgbaLast->blu)
						return  false;		// Color has changed
				}

				// When we get here, rgbaLast is the color
				cTop->red = rgbaLast->red;
				cTop->grn = rgbaLast->grn;
				cTop->blu = rgbaLast->blu;

			} else {
				// Unworkable
				return false;
			}


		//////////
		// Right
		//////
			if (bmp->bi.biBitCount == 24)
			{
				// Grab 24-bit pointer
				lrgb = (SRgb*)(bmp->bd + ((bmp->bi.biWidth - 1) * 3));

				// Iterate across the right-side
				rgbLast = lrgb;
				for (lnY = 0; lnX < bmp->bi.biHeight; ++lnY, lrgb = (SRgb*)((s8*)lrgb + bmp->rowWidth))
				{
					// See if the color's changed
					if (lrgb->red != rgbLast->red || lrgb->grn != rgbLast->grn || lrgb->blu != rgbLast->blu)
						return  false;		// Color has changed
				}

				// When we get here, rgbaLast is the color
				cRight->red = rgbLast->red;
				cRight->grn = rgbLast->grn;
				cRight->blu = rgbLast->blu;

			} else if (bmp->bi.biBitCount == 32) {
				// Grab 32-bit pointer
				lrgba = (SRgba*)(bmp->bd + ((bmp->bi.biWidth - 1) * 4));

				// Iterate across the right-side
				rgbaLast = lrgba;
				for (lnY = 0; lnX < bmp->bi.biHeight; ++lnY, lrgba = (SRgba*)((s8*)lrgba + bmp->rowWidth))
				{
					// See if the color's changed
					if (lrgba->red != rgbaLast->red || lrgba->grn != rgbaLast->grn || lrgba->blu != rgbaLast->blu)
						return  false;		// Color has changed
				}

				// When we get here, rgbaLast is the color
				cRight->red = rgbaLast->red;
				cRight->grn = rgbaLast->grn;
				cRight->blu = rgbaLast->blu;

			} else {
				// Unworkable
				return false;
			}


		//////////
		// Bottom
		//////
			if (bmp->bi.biBitCount == 24)
			{
				// Grab 24-bit pointer
				lrgb = (SRgb*)bmp->bd;

				// Iterate across the bottom
				rgbLast = lrgb;
				for (lnX = 0; lnX < bmp->bi.biWidth; ++lnX, ++lrgb)
				{
					// See if the color's changed
					if (lrgb->red != rgbLast->red || lrgb->grn != rgbLast->grn || lrgb->blu != rgbLast->blu)
						return  false;		// Color has changed
				}

				// When we get here, rgbaLast is the color
				cBottom->red = rgbLast->red;
				cBottom->grn = rgbLast->grn;
				cBottom->blu = rgbLast->blu;

			} else if (bmp->bi.biBitCount == 32) {
				// Grab 32-bit pointer
				lrgba = (SRgba*)bmp->bd;

				// Iterate across the bottom
				rgbaLast = lrgba;
				for (lnX = 0; lnX < bmp->bi.biWidth; ++lnX, ++lrgba)
				{
					// See if the color's changed
					if (lrgba->red != rgbaLast->red || lrgba->grn != rgbaLast->grn || lrgba->blu != rgbaLast->blu)
						return  false;		// Color has changed
				}

				// When we get here, rgbaLast is the color
				cBottom->red = rgbaLast->red;
				cBottom->grn = rgbaLast->grn;
				cBottom->blu = rgbaLast->blu;

			} else {
				// Unworkable
				return false;
			}


		//////////
		// Left
		//////
			if (bmp->bi.biBitCount == 24)
			{
				// Grab 24-bit pointer
				lrgb = (SRgb*)bmp->bd;

				// Iterate across the left-side
				rgbLast = lrgb;
				for (lnY = 0; lnX < bmp->bi.biHeight; ++lnY, lrgb = (SRgb*)((s8*)lrgb + bmp->rowWidth))
				{
					// See if the color's changed
					if (lrgb->red != rgbLast->red || lrgb->grn != rgbLast->grn || lrgb->blu != rgbLast->blu)
						return  false;		// Color has changed
				}

				// When we get here, rgbaLast is the color
				cLeft->red = rgbLast->red;
				cLeft->grn = rgbLast->grn;
				cLeft->blu = rgbLast->blu;

			} else if (bmp->bi.biBitCount == 32) {
				// Grab 32-bit pointer
				lrgba = (SRgba*)(bmp->bd + ((bmp->bi.biWidth - 1) * 4));

				// Iterate across the left-side
				rgbaLast = lrgba;
				for (lnY = 0; lnX < bmp->bi.biHeight; ++lnY, ++lnX, ++lrgba)
				{
					// See if the color's changed
					if (lrgba->red != rgbaLast->red || lrgba->grn != rgbaLast->grn || lrgba->blu != rgbaLast->blu)
						return  false;		// Color has changed
				}

				// When we get here, rgbaLast is the color
				cLeft->red = rgbaLast->red;
				cLeft->grn = rgbaLast->grn;
				cLeft->blu = rgbaLast->blu;

			} else {
				// Unworkable
				return false;
			}


		//////////
		// Once we get here, we have our colors, and we're good
		//////
			return true;

	}

	bool iBmp_compareColors(SRgb* cRef, ...)
	{
		SRgb*		cThis;
		va_list		colors;


		// Get our list
		va_start(colors, cRef);

		// Iterate through each pointer
		while ((cThis = (SRgb*)va_arg(colors, SRgb*)))
		{
			// Verify the color is the same
			if (cRef->red != cThis->red || cRef->grn != cThis->grn || cRef->blu != cThis->blu)
				return false;
		}

		// All done
		va_end(colors);

		// When we get here, the color(s) match
		return true;
	}




//////////
//
// Called to copy and colorize the indicated bitmap
//
//////
	SBitmap* iBmp_copyColorize(SBitmap* bmpSrc, SBgra colorTemplate, f32 tfAlpha)
	{
		RECT		lrc;
		SBitmap*	bmp;


		// Create a copy
		bmp = iBmp_copy(bmpSrc);
		if (bmp)
		{
			// Colorize
			SetRect(&lrc, 0, 0, bmp->bi.biWidth, bmp->bi.biHeight);
			iBmp_colorize(bmp, &lrc, colorTemplate, tfAlpha);
		}

		// Indicate success or failure
		return(bmp);
	}




//////////
//
// Called to make sure there is a copy, and if not it creates it, and if so
// then makes sure they are the same size, and if not then deletes the existing
// one and creates a new copy of the same size.
//
// Note:  It does not bitblt into the copy, but only creates a bmp the same size.
//
//////
	SBitmap* iBmp_verifyCopyIsSameSize(SBitmap* bmpCopy, SBitmap* bmp)
	{
		SBitmap* bmpNew;


		// Make sure we have something that makes sense
		if (bmp)
		{
			if (bmpCopy)
			{
				// Make sure it's the same size
				if (bmp->bi.biWidth == bmpCopy->bi.biWidth && bmp->bi.biHeight == bmpCopy->bi.biHeight)
					return(bmpCopy);		// They're the same already

				// If we get here, we need to delete the copy
				iBmp_delete(&bmpCopy, true, true);
				// Note:  From now on we use bmpNew, and return that
			}

			// When we get here, we need to create a new one
			bmpNew = iBmp_allocate();
			if (bmpNew)
				iBmp_createBySize(bmpNew, bmp->bi.biWidth, bmp->bi.biHeight, bmp->bi.biBitCount);

			// Indicate our success or failure
			return(bmpNew);

		} else {
			// Unknown what the size is, so we just leave it the way it is
			return(bmpCopy);
		}
	}




//////////
//
// Called to verify the bitmap size is correct, and if not then it will create one of
// the correct size and scale this one into it.
//
//////
	SBitmap* iBmp_verifySizeOrResize(SBitmap* bmp, u32 tnWidth, u32 tnHeight, u32 tnBitCount)
	{
		SBitmap* bmpNew;


		// Make sure our environment is sane
		if (!bmp || bmp->bi.biWidth != (s32)tnWidth || bmp->bi.biHeight != (s32)tnHeight)
		{
			// Something has changed
			bmpNew = iBmp_allocate();
			iBmp_createBySize(bmpNew, tnWidth, tnHeight, ((bmp) ? bmp->bi.biBitCount : ((tnBitCount == 32 || tnBitCount == 24) ? tnBitCount : 24)));

			// Delete the old version
			if (bmp)
				iBmp_delete(&bmp, true, true);

			// All done!

		} else {
			// They're the same
			bmpNew = bmp;
		}

		// Indicate our success
		return(bmpNew);
	}




//////////
//
// Called to load a bitmap file that was loaded from disk, or simulated loaded from disk.
//
//////
	SBitmap* iBmp_rawLoad(cu8* bmpRawFileData, bool tlLeaveAs32Bits)
	{
		BITMAPFILEHEADER*	bh;
		BITMAPINFOHEADER*	bi;
		SBitmap				bmpLoad;
		SBitmap*			bmp;
		RECT				lrc;


		//////////
		// Grab the headers
		//////
			bh = (BITMAPFILEHEADER*)bmpRawFileData;
			bi = (BITMAPINFOHEADER*)(bh + 1);


		//////////
		// Initialize the bitmap, and populate
		//////
			bmp = iBmp_allocate();
			if (bmp)
			{
				// Prepare bmpLoad
				memcpy(&bmpLoad.bh, bh, sizeof(bmp->bh));
				memcpy(&bmpLoad.bi, bi, sizeof(bmp->bi));
				bmpLoad.bd			= (s8*)(bmpRawFileData + bh->bfOffBits);
				bmpLoad.rowWidth	= iBmp_computeRowWidth(&bmpLoad);

				// Create the bitmap
				iBmp_createBySize(bmp, bmpLoad.bi.biWidth, bmpLoad.bi.biHeight, bmpLoad.bi.biBitCount);

				// Copy it over
				SetRect(&lrc, 0, 0, bmpLoad.bi.biWidth, bmpLoad.bi.biHeight);
				memcpy(bmp->bd, bmpLoad.bd, bmp->bi.biSizeImage);

				// Convert to 24-bit if need be
				if (bmp->bi.biBitCount == 32 && !tlLeaveAs32Bits)
					iBmp_convertTo24Bits(bmp);
			}

		//////////
		// Indicate our success or failure
		//////
			return(bmp);
	}




//////////
//
// Called to load the indicated file from disk, which allows it to load more than .BMP files
//
//////
	bool				glBmp_gdiPlus_initialized = false;
	GdiplusStartupInput	gsGdiplusStartup;
	ULONG_PTR			gnGdiplusToken;

	// Loads a png, jpg, etc., file by its extension
	SBitmap* iBmp_rawLoadImageFile(cu8* tcPathname, bool tlLeaveAs32Bits, bool tlFlipOnLoad)
	{
		s32			fileDataLength, lnNumRead;
		cu8*		fileData;
		FILE*		fh;
		SBitmap*	bmp;
		SBitmap*	bmp32;
		s8			fileExtension[_MAX_PATH];


		// Try to open the file
		fh = fopen((cs8*)tcPathname, "rb+");
		if (fh == (FILE*)-1)
			return NULL;

		fseek(fh, 0, SEEK_END);
		fileDataLength = ftell(fh);
		fseek(fh, 0, SEEK_SET);

		fileData = (cu8*)malloc(fileDataLength);
		if (!fileData)
		{
exit_politely:
			if (fileData)
				free((void*)fileData);
			fclose(fh);
			return NULL;
		}

		lnNumRead = (s32)fread((void*)fileData, 1, fileDataLength, fh);
		if (lnNumRead != fileDataLength)
			goto exit_politely;

		// Determine the file extension
		if (!iDisk_getFileExtension((cs8*)tcPathname, fileExtension))
			goto exit_politely;		// Unknown file extension

		// Close the file
		fclose(fh);

		// Process
		bmp = iBmp_rawLoad_fromFile(fileData, fileDataLength, (cu8*)&fileExtension[0], tlFlipOnLoad);
		if (bmp && tlLeaveAs32Bits && bmp->bi.biBitCount != 32)
		{
			// Convert to 32-bits
			bmp32 = iBmp_allocate();
			iBmp_createBySize(bmp32, bmp->bi.biWidth, bmp->bi.biHeight, 32);
			BitBlt(bmp32->hdc, 0, 0, bmp->bi.biWidth, bmp->bi.biHeight, bmp->hdc, 0, 0, SRCCOPY);
			iBmp_delete(&bmp, true, true, true);
			bmp = bmp32;
		}

		// Signify
		return bmp;
	}

	SBitmap* iBmp_rawLoad_fromFile(cu8* rawFileData, u32 tnRawFileDataLength, cu8* fileExtension, bool tlReverseOnLoad)
	{
		DWORD		error;
		s32			lnFile;
		u32			lnX, lnY;
		BitmapData	bitmapdata;
		Rect*		r;
		SRgb		*rgbSrc, *rgbDst;
		s8			*rgbSrcBase, *rgbDstBase;
		SBitmap*	bmp;
		Bitmap*		bmpObj;
		s8			tempFilename[_MAX_PATH];
		wchar_t		tempFilenameW[_MAX_PATH];


		//////////
		// Initialize
		//////
			bmp = NULL;
			if (!glBmp_gdiPlus_initialized)
			{
				glBmp_gdiPlus_initialized = true;
				GdiplusStartup(&gnGdiplusToken, &gsGdiplusStartup, NULL);
			}


		//////////
		// Create a temporary file
		//////
			iDisk_getTemporaryPathname(tempFilename, NULL, -1, NULL, (cs8*)fileExtension);
			lnFile = iDisk_open(tempFilename, _O_RDWR | _O_BINARY, _SH_DENYRW, true);
			if (lnFile)
			{
				if (iDisk_write(lnFile, 0, (void*)rawFileData, tnRawFileDataLength) == tnRawFileDataLength)
				{
					// Close the file
					iDisk_close(lnFile);

					// Load it as an image
					memset(&tempFilenameW, 0, sizeof(tempFilenameW));
					bmpObj = Bitmap::FromFile(c2w(tempFilename, -1, tempFilenameW));
					if (bmpObj)
					{
						// Lock the bits
						r = new Rect(0, 0, bmpObj->GetWidth(), bmpObj->GetHeight());
						if (bmpObj->LockBits(r, ImageLockModeRead, PixelFormat24bppRGB, &bitmapdata) == Ok)
						{
							// Create a bmpObj
							bmp = iBmp_allocate();
							iBmp_createBySize(bmp, bmpObj->GetWidth(), bmpObj->GetHeight(), 24);

							// Set the starting places
							rgbSrcBase = (s8*)bitmapdata.Scan0;
							if (tlReverseOnLoad)		rgbDstBase = bmp->bd + ((bmp->bi.biHeight - 1) * bmp->rowWidth);
							else						rgbDstBase = bmp->bd;

							// Copy the bits over
							for (lnY = 0; lnY < bmpObj->GetHeight(); ++lnY)
							{
								// Assign for the start of the row
								rgbSrc = (SRgb*)rgbSrcBase;
								rgbDst = (SRgb*)rgbDstBase;

								// Copy row
								for (lnX = 0; lnX < bmpObj->GetWidth(); ++lnX, ++rgbSrc, ++rgbDst)
								{
									// Copy pixels
									rgbDst->red = rgbSrc->red;
									rgbDst->grn = rgbSrc->grn;
									rgbDst->blu = rgbSrc->blu;
								}

								// Move for next row
								rgbSrcBase += bitmapdata.Stride;
								if (tlReverseOnLoad)	rgbDstBase -= bmp->rowWidth;
								else					rgbDstBase += bmp->rowWidth;
							}

							// All done
							bmpObj->UnlockBits(&bitmapdata);
							// Right now, our bmp contains the bits
// For debugging:
// iBmp_saveToDisk(bmp, "c:\\temp\\png.bmp");
						}

						// Clean house
						delete r;
						delete bmpObj;

					} else {
						error = GetLastError();
					}
				}
			}


		//////////
		// Delete the file
		//////
			iDisk_deleteFile(tempFilename);


		//////////
		// Indicate success or file
		//////
			return(bmp);

	}

	// Returns:  0=okay, others=error
	s32 iBmp_rawSave(SBitmap* bmp, cs8* tcFilename, EImageFormat format)
	{
		bool				llFound;
		wchar_t*			lcwExt;
		Bitmap*				bmpObj;
		ImageCodecInfo		codec					= { };
		wchar_t				filenameW[_MAX_PATH]	= { };


		//////////
		// Make sure our environment is sane
		//////
			if (!bmp || !bmp->bd || !tcFilename)
				return -1;


		//////////
		// Initialize
		//////
			if (!glBmp_gdiPlus_initialized)
			{
				glBmp_gdiPlus_initialized = true;
				GdiplusStartup(&gnGdiplusToken, &gsGdiplusStartup, NULL);
			}


		//////////
		// Physically write out
		//////
			llFound = true;
			switch (format)
			{
				case PNG:	lcwExt = cgwPng;	break;
				case JPG:	lcwExt = cgwJpg;	break;
				case GIF:	lcwExt = cgwGif;	break;
				case TIF:	lcwExt = cgwTif;	break;
				case BMP:	lcwExt = cgwBmp;	break;
				default:
					// Should never happen
					llFound = false;
					break;
			}

			// Was it found?
			if (!llFound || !iiBmp_findCodecByExtension(&codec, lcwExt))
				return -2;	// Could not find the codec


		//////////
		// Physically save out
		//////
			// Allocate a bitmap
			if (!(bmpObj = Bitmap::FromBITMAPINFO((BITMAPINFO*)&bmp->bi, bmp->bd)))
				return -3;
			
			// Save out
			bmpObj->Save(c2w((s8*)tcFilename, -1, filenameW), &codec.Clsid);
			delete bmpObj;


		//////////
		// Signify
		//////
			return 0;

	}




//////////
//
// Called to search the installed system codecs for the indicated one by file extension
//
//////
	bool iiBmp_findCodecByExtension(ImageCodecInfo* codec, wchar_t* twFilenameExtension)
	{
		bool				llFound;
		s32					lnLengthSrc, lnLengthThis;
		u32					lnI, lnTotalArraySize, lnEncoderCount;
		ImageCodecInfo*		imageCodecs;


		// How many encoders do we have on the system?
		GetImageEncodersSize(&lnEncoderCount, &lnTotalArraySize);
		if (!lnTotalArraySize || !lnEncoderCount || !(imageCodecs = (ImageCodecInfo*)malloc(lnTotalArraySize)))
			return false;

		// Grab the encoders
		GetImageEncoders(lnEncoderCount, lnTotalArraySize, imageCodecs);

		// Search for the one they're looking for
		lnLengthSrc = (s32)wcslen(twFilenameExtension);
		for (lnI = 0, llFound = false; lnI < lnEncoderCount; ++lnI)
		{
			// Is this it?
			lnLengthThis = (s32)wcslen(imageCodecs[lnI].FilenameExtension);
			if (iIsNeedleInHaystack(twFilenameExtension, lnLengthSrc, (wchar_t*)imageCodecs[lnI].FilenameExtension, lnLengthThis))
			{
				llFound = true;
				memcpy(codec, &imageCodecs[lnI], sizeof(ImageCodecInfo));
				break;
			}
		}

		// Clean house
		free(imageCodecs);

		// Signify
		return llFound;
	}



/////////
//
// Allows a bitmap to be cached, with validation on up to eight data points
// to determine on subsequent usage if the cache is still valid.
//
//////
	SBitmap* iBmp_isValidCache(SBmpCache** bmpCache, u32 data1, u32 data2, u32 data3, u32 data4, u32 data5, u32 data6, u32 data7, u32 data8, u32 data9, u32 data10, u32 data11, u32 data12, u32 data13, u32 data14)
	{
		SBmpCache* bc;


		// Make sure our environment is sane
		if (bmpCache)
		{
			// Grab the pointer
			bc = *bmpCache;

			// Is there a cache entry?
			if (!bc || !bc->bmpCached)
				return(NULL);

			// Validate the data points
			if (data1 != bc->data1)		return(NULL);
			if (data2 != bc->data2)		return(NULL);
			if (data3 != bc->data3)		return(NULL);
			if (data4 != bc->data4)		return(NULL);
			if (data5 != bc->data5)		return(NULL);
			if (data6 != bc->data6)		return(NULL);
			if (data7 != bc->data7)		return(NULL);
			if (data8 != bc->data8)		return(NULL);
			if (data9 != bc->data9)		return(NULL);
			if (data10 != bc->data10)	return(NULL);
			if (data11 != bc->data11)	return(NULL);
			if (data12 != bc->data12)	return(NULL);
			if (data13 != bc->data13)	return(NULL);
			if (data14 != bc->data14)	return(NULL);

			// If we get here, we're good
			return(bc->bmpCached);
		}

		// If we get here, invalid
		return(NULL);
	}




//////////
//
// Create a bitmap cache.
//
//////
	SBitmap* iBmp_createCache(SBmpCache** bmpCache, SBitmap* bmp, u32 data1, u32 data2, u32 data3, u32 data4, u32 data5, u32 data6, u32 data7, u32 data8, u32 data9, u32 data10, u32 data11, u32 data12, u32 data13, u32 data14, bool tlCopyBitmap)
	{
		SBmpCache* bc;


		// Make sure the environment is sane
		if (bmpCache && bmp)
		{
			// Create or get the existing copy
			if (!*bmpCache)
			{
				// Create a new entry
				bc = (SBmpCache*)malloc(sizeof(SBmpCache));
				if (!bc)
					return(NULL);

				// Store the cache
				*bmpCache = bc;

				// Initialize it
				memset(bc, 0, sizeof(SBmpCache));

			} else {
				// Grab the existing cache
				bc = *bmpCache;

				// Delete the previous bitmap
				if (bc->bmpCached)
					iBmp_delete(&bc->bmpCached, true, true);
			}

			// Store the data points
			bc->data1	= data1;
			bc->data2	= data2;
			bc->data3	= data3;
			bc->data4	= data4;
			bc->data5	= data5;
			bc->data6	= data6;
			bc->data7	= data7;
			bc->data8	= data8;
			bc->data9	= data9;
			bc->data10	= data10;
			bc->data11	= data11;
			bc->data12	= data12;
			bc->data13	= data13;
			bc->data14	= data14;

			// Store or copy the bitmap
			if (tlCopyBitmap)		bc->bmpCached = iBmp_copy(bmp);
			else					bc->bmpCached = bmp;
		}

		// If we get here, invalid
		return(NULL);
	}




//////////
//
// Delete a previously created cache.
//
//////
	SBitmap* iBmp_deleteCache(SBmpCache** bmpCache)
	{
		SBmpCache* bc;


		// Make sure the environment is sane
		if (bmpCache && *bmpCache)
		{
			// Grab the cache
			bc = *bmpCache;

			// Reset the root
			*bmpCache = NULL;

			// Delete the previous bitmap
			if (bc->bmpCached)
				iBmp_delete(&bc->bmpCached, true, true);

			// Delete self
			free(bc);
		}

		// If we get here, invalid
		return(NULL);
	}




//////////
//
// Called to save the indicated bitmap to a disk file.
//
//////
	void iBmp_saveToDisk(SBitmap* bmp, s8* tcPathname)
	{
		FILE*		lfh;
		RGBQUAD		colors[2];
		
		
		// Make sure we have a bitmap
		if (!bmp)
			return;

		// Simple disk write
		lfh = fopen(tcPathname, "wb+");
		if (lfh)
		{
			// Write file header, info header, bitmap bits
			bmp->bh.bfType		= 'MB';
			bmp->bh.bfReserved1	= 0;
			bmp->bh.bfReserved2	= 0;
			bmp->bh.bfOffBits	= sizeof(bmp->bh) + sizeof(bmp->bi) + ((bmp->bi.biBitCount == 1) ? sizeof(colors) : 0);
			bmp->bh.bfSize		= bmp->bh.bfOffBits + bmp->bi.biSizeImage;

			// Write header and info
			fwrite(&bmp->bh, 1, sizeof(bmp->bh), lfh);
			fwrite(&bmp->bi, 1, sizeof(bmp->bi), lfh);

			// Write two RGBQUAD values, with colors black and white
			if (bmp->bi.biBitCount == 1)
			{
				// Initialize colors
				colors[0].rgbRed		= 0;			// 0=Black
				colors[0].rgbGreen		= 0;
				colors[0].rgbBlue		= 0;
				colors[0].rgbReserved	= 0;
				colors[1].rgbRed		= 255;			// 1=White
				colors[1].rgbGreen		= 255;
				colors[1].rgbBlue		= 255;
				colors[1].rgbReserved	= 0;

				// Write color data
				fwrite(&colors, 1, sizeof(colors), lfh);
			}

			// Write bits
			fwrite(bmp->bd, 1, bmp->bi.biSizeImage, lfh);

			// Close
			fclose(lfh);
		}
	}

	s32 iBmp_saveToDisk_asMonochromeBitmap(SBitmap* bmp, s8* tcMonoBmpPathname)
	{
		RECT			lrc;
		SBitmap*		bmp1;


		// Create a physical copy by size
		if (!(bmp1 = iBmp_allocate()))
			return 0;

		// Physically create it
		iBmp_createBySize(bmp1, bmp->bi.biWidth, bmp->bi.biHeight, 1);

		// Copy the existing bitmap's content into it
		SetRect(&lrc, 0, 0, bmp->bi.biWidth, bmp->bi.biHeight);
		iBmp_bitBlt(bmp1, &lrc, bmp);
		//BitBlt(bmp1->hdc, 0, 0, bmp->bi.biWidth, bmp->bi.biHeight, bmp->hdc, 0, 0, SRCCOPY);

		// Write it out
		iBmp_saveToDisk(bmp1, tcMonoBmpPathname);
		//iBmp_saveToDisk(bmp, "full.bmp");

		// Delete it
		iBmp_delete(&bmp1, true, true);

		// Signify
		return 0;
	}
	// 		int				width		= bmp->bi.biWidth;
	// 		int				height		= bmp->bi.biHeight;
	// 		int				bufferSize	= ((width + 31) / 32) * 4 * height;		// Calculate the buffer size
	// 		BYTE*			buffer		= new BYTE[bufferSize];					// Allocate memory for the buffer
	// 		BITMAPINFO		bmi			= { };
	// 
	// 		// Fill the buffer with monochrome data
	// 		memset(buffer, 255, bufferSize);  // Set all bits to white
	// 
	// 		// Create a monochrome bitmap
	// 		bmi.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	// 		bmi.bmiHeader.biWidth		= width;
	// 		bmi.bmiHeader.biHeight		= height;
	// 		bmi.bmiHeader.biPlanes		= 1;
	// 		bmi.bmiHeader.biBitCount	= 1;
	// 		bmi.bmiHeader.biCompression	= BI_RGB;
	// 		bmi.bmiHeader.biSizeImage	= bufferSize;
	// 
	// 		// Create a device context (DC) for the screen
	// 		HDC screenDC		= GetDC(NULL);						// Get the screen DC
	// 		HDC memoryDC		= CreateCompatibleDC(screenDC);		// Create a compatible DC
	// 		HBITMAP hBitmap		= CreateDIBSection(memoryDC, &bmi, DIB_RGB_COLORS, (void**)&buffer, NULL, 0);
	// 		HBITMAP hBitmapOld	= (HBITMAP)SelectObject(memoryDC, hBitmap);
	// 
	// 		// Perform some drawing operations on the memory DC here...
	// 
	// 		// Cleanup
	// 		SelectObject(memoryDC, hBitmapOld);  // Restore the old bitmap
	// 		DeleteObject(hBitmap);  // Delete the DIB section
	// 		DeleteDC(memoryDC);  // Delete the memory DC
	// 		ReleaseDC(NULL, screenDC);  // Release the screen DC
	// 
	// 		delete[] buffer;  // Free the buffer memory




//////////
//
// Called to convert the indicated bitmap to 32-bits if need be
//
//////
	void iBmp_convertTo32Bits(SBitmap* bmp)
	{
		SBitmap bmp32;


		// Are we already there?
		if (bmp && bmp->bi.biBitCount != 32)
		{
			// No, but we only know how to handle 24-bit bitmaps
			if (bmp->bi.biBitCount == 24)
			{
				// We need to convert it
				// Create the 32-bit version
				memset(&bmp32, 0, sizeof(SBitmap));
				iBmp_createBySize(&bmp32, bmp->bi.biWidth, bmp->bi.biHeight, 32);

				// Copy it
				iBmp_copy24To32(&bmp32, bmp);

				// Free the (now old) bitmap
				iBmp_delete(&bmp, true, false);

				// Copy our bitmap to the destination
				memcpy(bmp, &bmp32, sizeof(SBitmap));
			}
		}
	}




//////////
//
// Called to convert the indicated bitmap to 32-bits if need be
//
//////
	void iBmp_convertTo24Bits(SBitmap* bmp)
	{
		SBitmap	bmp24;
		RECT	lrc;


		// Are we already there?
		if (bmp && bmp->bi.biBitCount != 24)
		{
			// No, but we only know how to handle 32-bit bitmaps
			if (bmp->bi.biBitCount == 32)
			{
				// We need to convert it
				// Create the 24-bit version
				memset(&bmp24, 0, sizeof(SBitmap));
				iBmp_createBySize(&bmp24, bmp->bi.biWidth, bmp->bi.biHeight, 24);

				// Copy it
				SetRect(&lrc, 0, 0, bmp->bi.biWidth, bmp->bi.biHeight);
				iBmp_bitBlt(&bmp24, &lrc, bmp);

				// Free the (now old) bitmap
				iBmp_delete(&bmp, true, false);

				// Copy our bitmap to the destination
				memcpy(bmp, &bmp24, sizeof(SBitmap));
			}
		}
	}




//////////
//
// Called to convert an argb into our SBgra format
//
//////
	void iBmp_convertFrom_argb(SBitmap* bmp)
	{
		s32		lnX, lnY;
		SBgra	pixel;
		SBgra*	lbgra;


		// Make sure our environment is sane
		if (bmp)
		{
			// Iterate through every row
			for (lnY = 0; lnY < bmp->bi.biHeight; ++lnY)
			{
				// Grab this pointer
				lbgra = (SBgra*)((s8*)bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth));

				// And across every pixel
				for (lnX = 0; lnX < bmp->bi.biWidth; ++lnX, ++lbgra)
				{
					// Load it in the format
					pixel = *lbgra;

					// Need to swap blue and alp
					lbgra->blu = pixel.alp;
					lbgra->alp = pixel.blu;
				}
			}
		}
	}




//////////
//
// Copies the 24-bit bitmap to a 32-bit bitmap.
// Note:  This function is no longer needed, but may be faster for equal sized
//        bitmaps that are known to be 24-bit and 32-bit.  The iBmp_bitBlt()
//        function now handles arbitrary 24-bit and 32-bit sources and destinations.
//
//////
	void iBmp_copy24To32(SBitmap* bmp32, SBitmap* bmp24)
	{
		s32		lnX, lnY;
		SBgr*	lbgr;
		SBgra*	lbgra;


		if (bmp32 && bmp24)
		{
			// Iterate through every row
			for (lnY = 0; lnY < bmp24->bi.biHeight; lnY++)
			{
				// Grab our pointers
				lbgr	= (SBgr*)(bmp24->bd  + ((bmp24->bi.biHeight - lnY - 1) * bmp24->rowWidth));
				lbgra	= (SBgra*)(bmp32->bd + ((bmp32->bi.biHeight - lnY - 1) * bmp32->rowWidth));

				// Iterate though every column
				for (lnX = 0; lnX < bmp24->bi.biWidth; lnX++, lbgr++, lbgra++)
				{
					// Copy the pixel
					lbgra->alp	= 255;
					lbgra->red	= lbgr->red;
					lbgra->grn	= lbgr->grn;
					lbgra->blu	= lbgr->blu;
				}
			}
		}
	}




//////////
//
// Performs basic tests on the bitmap to see if it appears to be a valid structure.
//
//////
	bool iBmp_validate(SBitmap* bmp)
	{
		// Pointer must exist
		if (!bmp)
			return(false);

		// Planes must be 1
		if (bmp->bi.biPlanes != 1)
			return(false);

		// Bits must be 24 or 32
		if (bmp->bi.biBitCount != 24 && bmp->bi.biBitCount != 32)
			return(false);

		// No compression (meaning default BMP file)
		if (bmp->bi.biCompression != 0)
			return(false);

		// Make sure the biSizeImage is accurate
		iBmp_computeRowWidth(bmp);
		if (bmp->rowWidth * bmp->bi.biHeight != bmp->bi.biSizeImage)
			return(false);

		// Make sure the pixels per meter are set
		if (bmp->bi.biXPelsPerMeter <= 0)			bmp->bi.biXPelsPerMeter = 2835;					// Default to 72 pixels per inch, a "twip" as it were
		if (bmp->bi.biYPelsPerMeter <= 0)			bmp->bi.biYPelsPerMeter = 2835;

		// We're good
		return(true);
	}

	s32 iBmp_validate_bitCount(s32 tnBitCount, s32 tnBitCount_default)
	{
		// Make sure the bit count is something we support
		switch (tnBitCount)
		{
			case 24:		// SBgr  and SRgb
			case 32:		// SBgra and SRgba
				break;

			default:
				tnBitCount = tnBitCount_default;
				break;
		}

		// Indicate our count
		return(tnBitCount);
	}




//////////
//
// Called to extract sub-rectangles from a larger bitmap.  They are blocked off
// by the indicated color as pixels one pixel beyond the border:
//
//		a         b
//		 +-------+
//		 |       |
//		 +-------+
//		c         d
//
// The scan for what to search for continues looking for rectangles which have
// pixels defined on the same row, and down the same columns as the left and
// right pixel, and then continues to extract.  The right-most pixel will then
// be used again to look for another rectangle, allowing for sequential sub-
// rectangles to be extracted.  The process completes when all rectangles are
// extracted, or the maximum count is reached.
//
//////
	s32 iBmp_extractSubRectangleBitmaps(SBitmap* bmp, SBitmapArray* bmpSubs[], s32 tnMaxCount, SBgra boundingColor)
	{
		s32		lnX, lnY, lnSubCount;
		SBgra	color;


// TODO:  This could be sped up by directly adding pointers to the bitmap and iterating
		// Make sure our environment is sane
		lnSubCount = 0;
		if (bmp && bmpSubs && tnMaxCount > 0)
		{
			// Iterate through the bitmap row by row, pixel by pixel
			for (lnY = 0; lnY < bmp->bi.biHeight && lnSubCount < tnMaxCount; lnY++)
			{
				for (lnX = 0; lnX < bmp->bi.biWidth; lnX++)
				{
					// Is this our target color
					color = iBmp_extractColorAtPoint(bmp, lnX, lnY);
					if (color.red == boundingColor.red && color.grn == boundingColor.grn && color.blu == boundingColor.blu)
					{
						// We found a pixel, extract this sub-rect if possible
						iiBmp_extractSubRectangleBitmap_scanAndExtract(bmp, lnX, lnY, lnSubCount, bmpSubs, boundingColor);
						if (lnSubCount >= tnMaxCount)
							break;
					}
				}
			}
		}

		// Indicate how many were found
		return(lnSubCount);
	}

	// Called to scan to see if a rectangle exists
	// Upon entry we've have pixel a
	s32 iiBmp_extractSubRectangleBitmap_scanAndExtract(SBitmap* bmp, s32 tnX, s32 tnY, s32 tnThisSub, SBitmapArray* bmpSubs[], SBgra boundingColor)
	{
		bool	llFound;
		s32		lnY, lnX;
		SBgra	color;


		//////////
		// Pixel b
		//////
			for (lnX = tnX + 2, llFound = false; lnX < bmp->bi.biWidth; lnX++)
			{
				// Grab the pixel at that point
				color = iBmp_extractColorAtPoint(bmp, lnX, tnY);
				if (color.red == boundingColor.red && color.grn == boundingColor.grn && color.blu == boundingColor.blu)
				{
					llFound = true;
					break;
				}
			}
			if (!llFound)
				return(0);		// No more found on this row


		//////////
		// Pixel c
		//////
			for (lnY = tnY + 2, llFound = false; lnY < bmp->bi.biHeight; lnY++)
			{
				// Grab the pixel at that point
				color = iBmp_extractColorAtPoint(bmp, lnX, tnY);
				if (color.red == boundingColor.red && color.grn == boundingColor.grn && color.blu == boundingColor.blu)
				{
					llFound = true;
					break;
				}
			}
			if (!llFound)
				return(0);		// No more found on this row


		//////////
		// Pixel d
		//////
			color = iBmp_extractColorAtPoint(bmp, lnX, lnY);
			if (color.red == boundingColor.red && color.grn == boundingColor.grn && color.blu == boundingColor.blu)
			{
				// Extract the inner rectangle
				SetRect(&bmpSubs[tnThisSub]->rc, tnX + 1, tnY + 1, lnX - 1, lnY - 1);
				bmpSubs[tnThisSub]->bmp = iBmp_createAndExtractRect(bmp, tnX + 1, tnY + 1, lnX - 1, lnY - 1);

				// Indicate an extraction
				return(1);
			}
			// If we get here, no extraction
			return(0);
	}




//////////
//
// Computes the row width of the pixels using BGR format (3 bytes per pixel) then rounded up to
// the nearest DWORD.
//
//////
	s32 iBmp_computeRowWidth(SBitmap* bmp)
	{
		s32 lnWidth;


		// Make sure our environment is sane
		if (bmp)
		{
			// See the bit counts
			if (bmp->bi.biBitCount == 24)
			{
				// 24-bit formats are rounded up to nearest DWORD
				lnWidth = bmp->bi.biWidth * 3;
				if (lnWidth % 4 == 0)
					return(lnWidth);

				// Increase the width
				lnWidth += (4 - (lnWidth % 4));
				return(lnWidth);


			} else if (bmp->bi.biBitCount == 32) {
				// 32-bit formats are also DWORD aligned, but naturally, of course. :-)
				return(bmp->bi.biWidth * 4);


			} else if (bmp->bi.biBitCount == 1) {
				// Monochrome bitmap are rounded up to nearest BYTE
				return (bmp->bi.biWidth + 7) / 8;


			} else {
				// Uh oh, spaghetti-oh!
				return(bmp->bi.biSizeImage / bmp->bi.biHeight);
			}
		}

		// Invalid
		return(0);
	}




//////////
//
// Called to create a basic bitmap by the indicated size, and initially populate it to white
//
//////
	void iBmp_createBySize(SBitmap* bmp, u32 width, u32 height, u32 tnBitCount)
	{
		HDC		hScreenDC;


		// Is there a bitmap?
		if (bmp)
		{
			// Populate the initial structure (min of 1x1, max of 3840 x 2160 (4x 1920x1080)
			iBmp_populateBitmapStructure(bmp, min(max(width, (u32)1), (u32)3840), min(max(height, (u32)1), (u32)2160), tnBitCount);

			// Create the HDC and DIB Section
			if (tnBitCount == 1)
			{
				// Monochrome DC
				hScreenDC = GetDC(NULL);
				bmp->hdc  = CreateCompatibleDC(hScreenDC);

				// Clean up
				DeleteDC(hScreenDC);

			} else {
				// Normal DC
				bmp->hdc = CreateCompatibleDC(GetDC(GetDesktopWindow()));
			}

			// Create a bitmap
			bmp->hbmp = CreateDIBSection(bmp->hdc, (BITMAPINFO*)&bmp->bi, DIB_RGB_COLORS, (void**)&bmp->bd, null0, 0);

			// It should've allocated bitmap data
			if (bmp->bd)
			{
				// Populate local variables
				SetRect(&bmp->rc, 0, 0, width - 1, height - 1);
				SelectObject(bmp->hdc, bmp->hbmp);				// Select its bitmap into its context
				memset(bmp->bd, 255, bmp->bi.biSizeImage);		// Paint it white initially (the fast/easy way)
			}
		}
	}




//////////
//
// Called to create an empty 24-bit or 32-bit bitmap
//
//////
	void iBmp_populateBitmapStructure(SBitmap* bmp, u32 tnWidth, u32 tnHeight, u32 tnBitCount)
	{
		if (bmp)
		{
			memset(&bmp->bi, 0, sizeof(bmp->bi));
			bmp->bi.biSize				= sizeof(bmp->bi);
			bmp->bi.biWidth				= tnWidth;
			bmp->bi.biHeight			= tnHeight;
			bmp->bi.biCompression		= BI_RGB;
			bmp->bi.biPlanes			= 1;

			// Jun.17.2023 RCH -- Added support for monochrome bitmap creation.  None of the algorithms will work for it, except save
			if (tnBitCount == 1)		bmp->bi.biBitCount = 1;
			else						bmp->bi.biBitCount = (u16)((tnBitCount == 24 || tnBitCount == 32) ? tnBitCount : 24);

			bmp->bi.biXPelsPerMeter		= 2835;	// Assume 72 dpi
			bmp->bi.biYPelsPerMeter		= 2835;
			bmp->rowWidth				= iBmp_computeRowWidth(bmp);
			bmp->bi.biSizeImage			= bmp->rowWidth * tnHeight;
		}
//////////
// Note:  The compression formats can be:
// 0	BI_RGB	none (most common)
// 4	BI_JPEG	data bits following need to be fed into a JPG decoder to access bit rows, to save back to disk a JPG encoder is required, lossy compression (always loses bitmap color data)
// 5	BI_PNG	data bits following need to be fed into a PNG decoder to access bit rows, to save back to disk a PNG encoder is required, lossless compression (always maintains original color data)
//////
	}




//////////
//
// Called to delete the indicated bitmap and optionally free all resources.
// Note:  The tlFreeBits flags allows the bitmap data to be copied to something else, with
//        the container SBitmap being deleted, but not the bits and related data within.
//
//////
	void iBmp_delete(SBitmap** bmpRoot, bool tlFreeBits, bool tlDeleteSelf, bool tlDeleteFont)
	{
		SBitmap* bmp;


		if (bmpRoot && *bmpRoot)
		{
			// Grab the pointer
			bmp = *bmpRoot;

			// Release the font
			if (tlDeleteFont && bmp->font)
			{
#ifdef iFont_delete
				iFont_delete(&bmp->font, true);
#endif
				bmp->font = NULL;
			}

			// Do we need to free the internals?
			if (tlFreeBits)
			{
				// Free the internal/Windows bits
				DeleteObject((HGDIOBJ)bmp->hbmp);
				DeleteDC(bmp->hdc);
			}

			// Release the bitmap
			if (tlDeleteSelf)
			{
				free(bmp);
				*bmpRoot = NULL;
			}
		}
	}




//////////
//
// Called to invert the colors in the indicated block
//
//////
	void iBmp_invert(SBitmap* bmp, RECT* rc)
	{
		iBmp_invert(bmp, rc->left, rc->top, rc->right, rc->bottom);
	}

	void iBmp_invert(SBitmap* bmp, s32 tnUlX, s32 tnUlY, s32 tnLrX, s32 tnLrY)
	{
		s32		lnX, lnY;
		SBgr*	lbgr;
		SBgra*	lbgra;


		// Iterate through every row
		for (lnY = tnUlY; lnY < tnLrY && lnY < bmp->bi.biHeight; lnY++)
		{
			if (bmp->bi.biBitCount == 24)
			{
				// Grab our pointers
				lbgr = (SBgr*)(bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth) + (tnUlX * 3));

				// Iterate though every column
				for (lnX = tnUlX; lnX < tnLrX && lnX < bmp->bi.biWidth; lnX++, lbgr++)
				{
					// Copy the pixel
					lbgr->red	= 255 - lbgr->red;
					lbgr->grn	= 255 - lbgr->grn;
					lbgr->blu	= 255 - lbgr->blu;
				}

			} else if (bmp->bi.biBitCount == 32) {
				// Grab our pointers
				lbgra	= (SBgra*)(bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth) + (tnUlX * 4));

				// Iterate through every column
				for (lnX = tnUlX; lnX < tnLrX && lnX < bmp->bi.biWidth; lnX++, lbgra++)
				{
					// Copy the pixel
					lbgra->alp	= 255;
					lbgra->red	= 255 - lbgra->red;
					lbgra->grn	= 255 - lbgra->grn;
					lbgra->blu	= 255 - lbgra->blu;
				}
			}
		}
	}




//////////
//
// Divides the color to its half color version
//
//////
	SBgra* iBmp_colorHalf(SBgra color, SBgra* colorHalf)
	{
		// Halve
		colorHalf->red = color.red / 2;
		colorHalf->grn = color.grn / 2;
		colorHalf->blu = color.blu / 2;

		// The new color
		return(colorHalf);
	}




//////////
//
// Draw the indicated object
//
//////
#ifdef SObject
	void iBmp_bitBltObject(SBitmap* bmpDst, SObject* objSrc, SBitmap* bmpSrc)
	{
		iBmp_bitBlt(bmpDst, &objSrc->rc, bmpSrc);
	}
#endif




//////////
//
// Draws all except bits with the mask color rgb(222,22,222)
//
//////
#ifdef SObject
	void iBmp_bitBltObjectMask(SBitmap* bmpDst, SObject* obj, SBitmap* bmpSrc)
	{
		iBmp_bitBltMask(bmpDst, &obj->rc, bmpSrc);
	}
#endif




//////////
//
// Physically render the bitmap atop the bitmap
//
//////
	u32 iBmp_bitBlt(SBitmap* bmpDst, RECT* trc, SBitmap* bmpSrc)
	{
		bool		llBit;
		u8			mono1, mono0;
		u32			lnPixelsRendered;
		s32			lnY, lnX, lnYDst, lnXDst;
		f64			lfAlp, lfMalp;
		u8*			monoDst;
		SBgr*		lbgrDst;
		SBgr*		lbgrSrc;
		SBgra*		lbgraDst;
		SBgra*		lbgraSrc;



		// Make sure the environment is sane
		lnPixelsRendered = 0;
		if (bmpDst && bmpSrc && trc)
		{

		//////////
		// Use the system bitblt for speed
		//////
#ifdef _MSC_VER
			BitBlt(bmpDst->hdc, trc->left, trc->top, trc->right - trc->left, trc->bottom - trc->top, bmpSrc->hdc, 0, 0, SRCCOPY);
			return(bmpSrc->bi.biSizeImage);
#endif


		//////////
		// Draw it
		//////
			for (lnY = 0, lnYDst = trc->top; lnY < bmpSrc->bi.biHeight && lnYDst < trc->bottom; lnYDst++, lnY++)
			{
				// Are we on the image?
				if (lnYDst >= 0 && lnYDst < bmpDst->bi.biHeight)
				{
					// Build the pointer
					lbgrDst		= (SBgr*)((s8*)bmpDst->bd  + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (trc->left * (bmpDst->bi.biBitCount / 8)));
					lbgrSrc		= (SBgr*)((s8*)bmpSrc->bd  + ((bmpSrc->bi.biHeight - lnY    - 1) * bmpSrc->rowWidth));
					lbgraDst	= (SBgra*)((s8*)bmpDst->bd + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (trc->left * (bmpDst->bi.biBitCount / 8)));
					lbgraSrc	= (SBgra*)((s8*)bmpSrc->bd + ((bmpSrc->bi.biHeight - lnY    - 1) * bmpSrc->rowWidth));

					// What exactly are we copying?
					if (bmpSrc->bi.biBitCount == 24)
					{
						// 24-bit source
						if (bmpDst->bi.biBitCount == 24)
						{
							// 24-bit to 24-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth)
								{
									// Copy the pixel
									lbgrDst->red	= lbgrSrc->red;
									lbgrDst->grn	= lbgrSrc->grn;
									lbgrDst->blu	= lbgrSrc->blu;
									++lnPixelsRendered;
								}

								// Move to next pixel on both
								++lbgrDst;
								++lbgrSrc;
							}

						} else if (bmpDst->bi.biBitCount == 32) {
							// 24-bit to 32-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth)
								{
									// Copy the pixel
									lbgraDst->alp	= 255;
									lbgraDst->red	= lbgrSrc->red;
									lbgraDst->grn	= lbgrSrc->grn;
									lbgraDst->blu	= lbgrSrc->blu;
									++lnPixelsRendered;
								}

								// Move to next pixel on both
								++lbgraDst;
								++lbgrSrc;
							}

						} else if (bmpDst->bi.biBitCount == 1) {
							// 24-bit to 1-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth)
								{
									// Determine the pixel offset, AND, and OR bits
									monoDst = (u8*)bmpDst->bd + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (lnXDst / 8);
									mono1   = 1 << (7 - (lnXDst % 8));
									mono0   = ~mono1;

									// Is this bit on?
									llBit   = ((((f32)lbgrSrc->red * 0.35f) + ((f32)lbgrSrc->grn * 0.54f) + ((f32)lbgrSrc->blu * 0.11f)) <= 0.5f);
									if (llBit)	*monoDst &= mono0;	// On,  set bit to 0 (black)
									else		*monoDst |= mono1;	// Off, set bit to 1 (white)

									// Increase our pixel count
									++lnPixelsRendered;
								}

								// Move to next pixel on both
								++lbgrSrc;
							}
						}

					} else if (bmpSrc->bi.biBitCount == 32) {
						// 32-bit source
						if (bmpDst->bi.biBitCount == 24)
						{
							// 32-bit to 24-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lbgraSrc->alp != 0)
								{
									// Copy the pixel
									if (lbgraSrc->alp == 255)
									{
										// Opaque
										lbgrDst->red	= lbgraSrc->red;
										lbgrDst->grn	= lbgraSrc->grn;
										lbgrDst->blu	= lbgraSrc->blu;

									} else {
										// Some degree of transparency
										lfAlp			= ((f64)lbgraSrc->alp / 255.0);
										lfMalp			= 1.0 - lfAlp;
										lbgrDst->red	= (u8)min(max(((f64)lbgrDst->red * lfMalp) + (lbgraSrc->red * lfAlp), 0.0), 255.0);
										lbgrDst->grn	= (u8)min(max(((f64)lbgrDst->grn * lfMalp) + (lbgraSrc->grn * lfAlp), 0.0), 255.0);
										lbgrDst->blu	= (u8)min(max(((f64)lbgrDst->blu * lfMalp) + (lbgraSrc->blu * lfAlp), 0.0), 255.0);
									}
									++lnPixelsRendered;
								}

								// Move to next pixel on both
								++lbgrDst;
								++lbgraSrc;
							}

						} else if (bmpDst->bi.biBitCount == 32) {
							// 32-bit to 32-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lbgraSrc->alp != 0)
								{
									// Copy the pixel
									if (lbgraSrc->alp == 255)
									{
										// Opaque
										lbgraDst->alp	= 255;
										lbgraDst->red	= lbgraSrc->red;
										lbgraDst->grn	= lbgraSrc->grn;
										lbgraDst->blu	= lbgraSrc->blu;

									} else {
										// Some degree of transparency
										lfAlp			= ((f64)lbgraSrc->alp / 255.0);
										lfMalp			= 1.0 - lfAlp;
	 									lbgraDst->alp	= 255;
										lbgraDst->red	= (u8)min(max(((f64)lbgraDst->red * lfMalp) + (lbgraSrc->red * lfAlp), 0.0), 255.0);
										lbgraDst->grn	= (u8)min(max(((f64)lbgraDst->grn * lfMalp) + (lbgraSrc->grn * lfAlp), 0.0), 255.0);
										lbgraDst->blu	= (u8)min(max(((f64)lbgraDst->blu * lfMalp) + (lbgraSrc->blu * lfAlp), 0.0), 255.0);
									}
									++lnPixelsRendered;
								}

								// Move to next pixel on both
								++lbgraDst;
								++lbgraSrc;
							}

						} else if (bmpDst->bi.biBitCount == 1) {
							// 32-bit to 1-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lbgraSrc->alp != 0)
								{
									// Determine the pixel offset, AND, and OR bits
									monoDst = (u8*)bmpDst->bd + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (lnXDst / 8);
									mono1   = lnXDst % 8;
									mono0   = ~mono1;

									// Is this bit on?
									llBit   = ((((f32)lbgrSrc->red * 0.35f) + ((f32)lbgrSrc->grn * 0.54f) + ((f32)lbgrSrc->blu * 0.11f)) <= 0.5f);
									if (llBit)	*monoDst &= mono0;	// On,  set bit to 0 (black)
									else		*monoDst |= mono1;	// Off, set bit to 1 (white)

									// Increase our pixel count
									++lnPixelsRendered;
								}

								// Move to next pixel on both
								++lbgrSrc;
							}
						}
					}
				}
			}
		}


		//////////
		// Indicate how many pixels were rendered
		//////
			return(lnPixelsRendered);
	}




//////////
//
// Called to blt the bitmap rotated 90, 180, or 270 degrees.
//
//////
	u32 iBmp_bitBlt_rotated(SBitmap* bmpDst, RECT* trc, SBitmap* bmpSrc, s32 tnDegrees_90_180_270)
	{
		u32			lnPixelsRendered;
		s32			lnY, lnX, lnYDst, lnXDst, lnYDstStart, lnXDstStart, lnYDstEnd, lnXDstEnd, lnXinc_onX, lnXinc_onY, lnYinc_onX, lnYinc_onY;
		f64			lfAlp, lfMalp;
		SBgr*		lbgrDst;
		SBgr*		lbgrSrc;
		SBgra*		lbgraDst;
		SBgra*		lbgraSrc;


		// Make sure the environment is sane
		lnPixelsRendered = 0;
		if (bmpDst && bmpSrc && trc)
		{
			//////////
			// Validate the angle
			//////
				switch (tnDegrees_90_180_270)
				{
					case 0:
						// Do a normal bitBlt
						return(iBmp_bitBlt(bmpDst, trc, bmpSrc));

					case 90:
						// Clock-wise 90 degrees
						lnXDstStart	= trc->right;
						lnYDstStart	= trc->top;
						lnXDstEnd	= trc->left - 1;
						lnYDstEnd	= trc->bottom + 1;
						lnXinc_onY	= -1;
						lnYinc_onY	= 0;
						lnXinc_onX	= 0;
						lnYinc_onX	= 1;
						break;

					case 180:
						// Clock-wise 180 degrees
						lnXDstStart	= trc->right;
						lnYDstStart	= trc->bottom;
						lnXDstEnd	= trc->left - 1;
						lnYDstEnd	= trc->top - 1;
						lnXinc_onY	= 0;
						lnYinc_onY	= -1;
						lnXinc_onX	= -1;
						lnYinc_onX	= 0;
						break;

					case 270:
						// Clock-wise 270 degrees
						lnXDstStart	= trc->left;
						lnYDstStart	= trc->bottom;
						lnXDstEnd	= trc->right + 1;
						lnYDstEnd	= trc->top - 1;
						lnXinc_onY	= 1;
						lnYinc_onY	= 0;
						lnXinc_onX	= 0;
						lnYinc_onX	= -1;
						break;

					default:
						// Failure
						return(0);
				}


			//////////
			// Draw it
			//////
				for (
						lnY = 0, lnXDst = lnXDstStart, lnYDst = lnYDstStart;
						lnY < bmpSrc->bi.biHeight && lnYDst != lnYDstEnd && lnXDst != lnXDstEnd;
						lnXDstStart += lnXinc_onY, lnYDstStart += lnYinc_onY, lnXDst = lnYDstStart, lnYDst = lnYDstStart, lnY++
					)
				{
					// Are we on the image?
					if (lnYDst >= 0 && lnYDst < bmpDst->bi.biHeight)
					{
						// Build the pointers
						lbgrSrc		= (SBgr*) ((s8*)bmpSrc->bd + ((bmpSrc->bi.biHeight - lnY - 1) * bmpSrc->rowWidth));
						lbgraSrc	= (SBgra*)((s8*)bmpSrc->bd + ((bmpSrc->bi.biHeight - lnY - 1) * bmpSrc->rowWidth));

						// What exactly are we copying?
						if (bmpSrc->bi.biBitCount == 24)
						{
							// 24-bit source
							if (bmpDst->bi.biBitCount == 24)
							{
								// 24-bit to 24-bit
								// Iterate through every visible column
								for (lnX = 0, lnXDst = lnXDstStart; lnX < bmpSrc->bi.biWidth && lnXDst != lnXDstEnd; lnXDst += lnXinc_onX, lnYDst += lnYinc_onX, lnX++)
								{
									// Build our destination pointer
									lbgrDst = (SBgr*)((s8*)bmpDst->bd + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (lnXDst * (bmpDst->bi.biBitCount / 8)));

									// Are we on the image?
									if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lnYDst >= 0 && lnYDst < bmpDst->bi.biHeight)
									{
										// Copy the pixel
										lbgrDst->red	= lbgrSrc->red;
										lbgrDst->grn	= lbgrSrc->grn;
										lbgrDst->blu	= lbgrSrc->blu;
										++lnPixelsRendered;
									}

									// Move to next pixel
									++lbgrSrc;
								}

							} else {
								// 24-bit to 32-bit
								// Iterate through every visible column
								for (lnX = 0, lnXDst = lnXDstStart; lnX < bmpSrc->bi.biWidth && lnXDst != lnXDstEnd; lnXDst += lnXinc_onX, lnYDst += lnYinc_onX, lnX++)
								{
									// Build our destination pointer
									lbgraDst = (SBgra*)((s8*)bmpDst->bd + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (lnXDst * (bmpDst->bi.biBitCount / 8)));

									// Are we on the image?
									if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lnYDst >= 0 && lnYDst < bmpDst->bi.biHeight)
									{
										// Copy the pixel
										lbgraDst->alp	= 255;
										lbgraDst->red	= lbgrSrc->red;
										lbgraDst->grn	= lbgrSrc->grn;
										lbgraDst->blu	= lbgrSrc->blu;
										++lnPixelsRendered;
									}

									// Move to next pixel
									++lbgrSrc;
								}
							}

						} else if (bmpSrc->bi.biBitCount == 32) {
							// 32-bit source
							if (bmpDst->bi.biBitCount == 24)
							{
								// 32-bit to 24-bit
								// Iterate through every visible column
								for (lnX = 0, lnXDst = lnXDstStart; lnX < bmpSrc->bi.biWidth && lnXDst != lnXDstEnd; lnXDst += lnXinc_onX, lnYDst += lnYinc_onX, lnX++)
								{
									// Build our destination pointers
									lbgrDst = (SBgr*)((s8*)bmpDst->bd + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (lnXDst * (bmpDst->bi.biBitCount / 8)));

									// Are we on the image?
									if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lnYDst >= 0 && lnYDst < bmpDst->bi.biHeight && lbgraSrc->alp != 0)
									{
										// Copy the pixel
										if (lbgraSrc->alp == 255)
										{
											// Opaque
											lbgrDst->red	= lbgraSrc->red;
											lbgrDst->grn	= lbgraSrc->grn;
											lbgrDst->blu	= lbgraSrc->blu;

										} else {
											// Some degree of transparency
											lfAlp			= ((f64)lbgraSrc->alp / 255.0);
											lfMalp			= 1.0 - lfAlp;
											lbgrDst->red	= (u8)min(max(((f64)lbgrDst->red * lfMalp) + (lbgraSrc->red * lfAlp), 0.0), 255.0);
											lbgrDst->grn	= (u8)min(max(((f64)lbgrDst->grn * lfMalp) + (lbgraSrc->grn * lfAlp), 0.0), 255.0);
											lbgrDst->blu	= (u8)min(max(((f64)lbgrDst->blu * lfMalp) + (lbgraSrc->blu * lfAlp), 0.0), 255.0);
										}
										++lnPixelsRendered;
									}

									// Move to next pixel
									++lbgraSrc;
								}

							} else {
								// 32-bit to 32-bit
								// Iterate through every visible column
								for (lnX = 0, lnXDst = lnXDstStart; lnX < bmpSrc->bi.biWidth && lnXDst != lnXDstEnd; lnXDst += lnXinc_onX, lnYDst += lnYinc_onX, lnX++)
								{
									// Build our destination pointers
									lbgraDst = (SBgra*)((s8*)bmpDst->bd + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (lnXDst * (bmpDst->bi.biBitCount / 8)));

									// Are we on the image?
									if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lnYDst >= 0 && lnYDst < bmpDst->bi.biHeight && lbgraSrc->alp != 0)
									{
										// Copy the pixel
										if (lbgraSrc->alp == 255)
										{
											// Opaque
											lbgraDst->alp	= 255;
											lbgraDst->red	= lbgraSrc->red;
											lbgraDst->grn	= lbgraSrc->grn;
											lbgraDst->blu	= lbgraSrc->blu;

										} else {
											// Some degree of transparency
											lfAlp			= ((f64)lbgraSrc->alp / 255.0);
											lfMalp			= 1.0 - lfAlp;
	 										lbgraDst->alp	= 255;
											lbgraDst->red	= (u8)min(max(((f64)lbgraDst->red * lfMalp) + (lbgraSrc->red * lfAlp), 0.0), 255.0);
											lbgraDst->grn	= (u8)min(max(((f64)lbgraDst->grn * lfMalp) + (lbgraSrc->grn * lfAlp), 0.0), 255.0);
											lbgraDst->blu	= (u8)min(max(((f64)lbgraDst->blu * lfMalp) + (lbgraSrc->blu * lfAlp), 0.0), 255.0);
										}
										++lnPixelsRendered;
									}

									// Move to next pixel
									++lbgraSrc;
								}
							}
						}
					}
				}

		}


		//////////
		// Indicate how many pixels were rendered
		//////
			return(lnPixelsRendered);
	}




//////////
//
// Called to overlay with alpha transparency the indicated bitmap
//
//////
	u32 iBmp_bitBltAlpha(SBitmap* bmpDst, RECT* trc, SBitmap* bmpSrc, f32 alpha)
	{
		u32			lnPixelsRendered;
		s32			lnY, lnX, lnYDst, lnXDst;
		f64			lfAlp, lfMalp;
		SBgr*		lbgrDst;
		SBgr*		lbgrSrc;
		SBgra*		lbgraDst;
		SBgra*		lbgraSrc;


		// Make sure the environment is sane
		lnPixelsRendered = 0;
		if (bmpDst && bmpSrc && trc)
		{
			lfAlp	= min(max(alpha, 0.0f), 1.0f);
			lfMalp	= 1.0 - alpha;

		//////////
		// Draw it
		//////
			for (lnY = 0, lnYDst = trc->top; lnY < bmpSrc->bi.biHeight && lnYDst < trc->bottom; lnYDst++, lnY++)
			{
				// Are we on the image?
				if (lnYDst >= 0 && lnYDst < bmpDst->bi.biHeight)
				{
					// Build the pointer
					lbgrDst		= (SBgr*)((s8*)bmpDst->bd  + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (trc->left * (bmpDst->bi.biBitCount / 8)));
					lbgrSrc		= (SBgr*)((s8*)bmpSrc->bd  + ((bmpSrc->bi.biHeight - lnY    - 1) * bmpSrc->rowWidth));
					lbgraDst	= (SBgra*)((s8*)bmpDst->bd + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (trc->left * (bmpDst->bi.biBitCount / 8)));
					lbgraSrc	= (SBgra*)((s8*)bmpSrc->bd + ((bmpSrc->bi.biHeight - lnY    - 1) * bmpSrc->rowWidth));

					// What exactly are we copying?
					if (bmpSrc->bi.biBitCount == 24)
					{
						// 24-bit source
						if (bmpDst->bi.biBitCount == 24)
						{
							// 24-bit to 24-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth)
								{
									// Copy the pixel
									lbgrDst->red	= (u8)min(max(((f64)lbgrDst->red * lfMalp) + (lbgrSrc->red * lfAlp), 0.0), 255.0);
									lbgrDst->grn	= (u8)min(max(((f64)lbgrDst->grn * lfMalp) + (lbgrSrc->grn * lfAlp), 0.0), 255.0);
									lbgrDst->blu	= (u8)min(max(((f64)lbgrDst->blu * lfMalp) + (lbgrSrc->blu * lfAlp), 0.0), 255.0);
									++lnPixelsRendered;
								}

								// Move to next pixel on both
								++lbgrDst;
								++lbgrSrc;
							}

						} else {
							// 24-bit to 32-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth)
								{
									// Copy the pixel
									lbgraDst->alp	= 255;
									lbgraDst->red	= (u8)min(max(((f64)lbgraDst->red * lfMalp) + (lbgrSrc->red * lfAlp), 0.0), 255.0);
									lbgraDst->grn	= (u8)min(max(((f64)lbgraDst->grn * lfMalp) + (lbgrSrc->grn * lfAlp), 0.0), 255.0);
									lbgraDst->blu	= (u8)min(max(((f64)lbgraDst->blu * lfMalp) + (lbgrSrc->blu * lfAlp), 0.0), 255.0);
									++lnPixelsRendered;
								}

								// Move to next pixel on both
								++lbgraDst;
								++lbgrSrc;
							}
						}

					} else if (bmpSrc->bi.biBitCount == 32) {
						// 32-bit source
						if (bmpDst->bi.biBitCount == 24)
						{
							// 32-bit to 24-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lbgraSrc->alp != 0)
								{
									// Copy the pixel
									lbgrDst->red	= (u8)min(max(((f64)lbgrDst->red * lfMalp) + (lbgraSrc->red * lfAlp), 0.0), 255.0);
									lbgrDst->grn	= (u8)min(max(((f64)lbgrDst->grn * lfMalp) + (lbgraSrc->grn * lfAlp), 0.0), 255.0);
									lbgrDst->blu	= (u8)min(max(((f64)lbgrDst->blu * lfMalp) + (lbgraSrc->blu * lfAlp), 0.0), 255.0);
									++lnPixelsRendered;
								}

								// Move to next pixel on both
								++lbgrDst;
								++lbgraSrc;
							}

						} else {
							// 32-bit to 32-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lbgraSrc->alp != 0)
								{
									// Copy the pixel
 									lbgraDst->alp	= 255;
									lbgraDst->red	= (u8)min(max(((f64)lbgraDst->red * lfMalp) + (lbgraSrc->red * lfAlp), 0.0), 255.0);
									lbgraDst->grn	= (u8)min(max(((f64)lbgraDst->grn * lfMalp) + (lbgraSrc->grn * lfAlp), 0.0), 255.0);
									lbgraDst->blu	= (u8)min(max(((f64)lbgraDst->blu * lfMalp) + (lbgraSrc->blu * lfAlp), 0.0), 255.0);
									++lnPixelsRendered;
								}

								// Move to next pixel on both
								++lbgraDst;
								++lbgraSrc;
							}
						}
					}
				}
			}
		}


		//////////
		// Indicate how many pixels were rendered
		//////
			return(lnPixelsRendered);
	}




//////////
//
// Called to overlay an alpha bitmap over another bitmap
//
//////
	u32 iBmp_bitBltAlphaBlend(SBitmap* bmpDst, RECT* trc, SBitmap* bmpSrc)
	{
		u32			lnPixelsRendered;
		s32			lnY, lnX, lnYDst, lnXDst;
		f32			lfAlp, lfMalp;
		SBgr*		lbgrDst;
		SBgr*		lbgrSrc;
		SBgra*		lbgraDst;
		SBgra*		lbgraSrc;


		// Make sure the environment is sane
		lnPixelsRendered = 0;
		if (bmpDst && bmpSrc && trc)
		{

		//////////
		// Draw it
		//////
			for (lnY = 0, lnYDst = trc->top; lnY < bmpSrc->bi.biHeight && lnYDst < trc->bottom; lnYDst++, lnY++)
			{
				// Are we on the image?
				if (lnYDst >= 0 && lnYDst < bmpDst->bi.biHeight)
				{
					// Build the pointer
					lbgrDst		= (SBgr*)((s8*)bmpDst->bd  + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (trc->left * (bmpDst->bi.biBitCount / 8)));
					lbgrSrc		= (SBgr*)((s8*)bmpSrc->bd  + ((bmpSrc->bi.biHeight - lnY    - 1) * bmpSrc->rowWidth));
					lbgraDst	= (SBgra*)((s8*)bmpDst->bd + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (trc->left * (bmpDst->bi.biBitCount / 8)));
					lbgraSrc	= (SBgra*)((s8*)bmpSrc->bd + ((bmpSrc->bi.biHeight - lnY    - 1) * bmpSrc->rowWidth));

					// What exactly are we copying?
					if (bmpSrc->bi.biBitCount == 32)
					{
						// 32-bit source
						if (bmpDst->bi.biBitCount == 24)
						{
							// 32-bit to 24-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lbgraSrc->alp != 0)
								{
									// Copy the pixel
									lfAlp			= (f32)lbgraSrc->alp / 255.0f;
									lfMalp			= 1.0f - lfAlp;
									lbgrDst->red	= (u8)min(max(((f64)lbgrDst->red * lfMalp) + (lbgraSrc->red * lfAlp), 0.0), 255.0);
									lbgrDst->grn	= (u8)min(max(((f64)lbgrDst->grn * lfMalp) + (lbgraSrc->grn * lfAlp), 0.0), 255.0);
									lbgrDst->blu	= (u8)min(max(((f64)lbgrDst->blu * lfMalp) + (lbgraSrc->blu * lfAlp), 0.0), 255.0);
									++lnPixelsRendered;
								}

								// Move to next pixel on both
								++lbgrDst;
								++lbgraSrc;
							}

						} else {
							// 32-bit to 32-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lbgraSrc->alp != 0)
								{
									// Copy the pixel
									lfAlp			= (f32)lbgraSrc->alp / 255.0f;
									lfMalp			= 1.0f - lfAlp;
									lbgraDst->red	= (u8)min(max(((f64)lbgraDst->red * lfMalp) + (lbgraSrc->red * lfAlp), 0.0), 255.0);
									lbgraDst->grn	= (u8)min(max(((f64)lbgraDst->grn * lfMalp) + (lbgraSrc->grn * lfAlp), 0.0), 255.0);
									lbgraDst->blu	= (u8)min(max(((f64)lbgraDst->blu * lfMalp) + (lbgraSrc->blu * lfAlp), 0.0), 255.0);
									++lnPixelsRendered;
								}

								// Move to next pixel on both
								++lbgraDst;
								++lbgraSrc;
							}
						}
					}
				}
			}
		}


		//////////
		// Indicate how many pixels were rendered
		//////
			return(lnPixelsRendered);
	}




//////////
//
// Called to grayscale a bitmap, or a portion of a bitmap
//
//////
	u32 iBmp_grayscale(SBitmap* bmp, RECT* trc, f32 tfAlpha)
	{
		u32		lnPixelsRendered;
		s32		lnY, lnX;
		f32		lfAlp, lfMalp, gray;
		SBgr*	lbgr;
		SBgra*	lbgra;


		lnPixelsRendered = 0;
		if (bmp && trc)
		{
			// Make sure the tfAlpha is ranged between 0.0f and 1.0f
			if (tfAlpha < 0.0f)		tfAlpha = 0.0f;
			if (tfAlpha > 1.0f)		tfAlpha = 1.0f;

			// Calculate the alp and malp
			lfAlp  = tfAlpha;
			lfMalp = 1.0f - lfAlp;


		//////////
		// Draw it
		//////
			for (lnY = trc->top; lnY < bmp->bi.biHeight && lnY < trc->bottom; lnY++)
			{
				// Are we on the image?
				if (lnY >= 0)
				{
					// What exactly are we copying?
					if (bmp->bi.biBitCount == 24)
					{
						// Build the pointer
						lbgr = (SBgr*)((s8*)bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth));

						// Iterate through every visible column
						for (lnX = trc->left; lnX < bmp->bi.biWidth && lnX < trc->right; lnX++)
						{
							// Are we on the image?
							if (lnX >= 0)
							{
								// Compute the grayscale
								gray = ((f32)lbgr->red * 0.35f) + ((f32)lbgr->grn * 0.54f) + ((f32)lbgr->blu * 0.11f);

								// Copy the pixel
								lbgr->red	= (u8)((gray * lfAlp) + ((f32)lbgr->red * lfMalp));
								lbgr->grn	= (u8)((gray * lfAlp) + ((f32)lbgr->grn * lfMalp));
								lbgr->blu	= (u8)((gray * lfAlp) + ((f32)lbgr->blu * lfMalp));
								++lnPixelsRendered;
							}

							// Move to next pixel
							++lbgr;
						}

					} else if (bmp->bi.biBitCount == 32) {
						// Build the pointer
						lbgra = (SBgra*)((s8*)bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth));

						// Iterate through every visible column
						for (lnX = trc->left; lnX < bmp->bi.biWidth && lnX < trc->right; lnX++)
						{
							// Are we on the image?
							if (lnX >= 0)
							{
								// Compute the grayscale
								gray = (((f32)lbgra->red * 0.35f) + ((f32)lbgra->grn * 0.54f) + ((f32)lbgra->blu * 0.11f));

								// Copy the pixel
								lbgra->red	= (u8)((gray * lfAlp) + ((f32)lbgra->red * lfMalp));
								lbgra->grn	= (u8)((gray * lfAlp) + ((f32)lbgra->grn * lfMalp));
								lbgra->blu	= (u8)((gray * lfAlp) + ((f32)lbgra->blu * lfMalp));
								++lnPixelsRendered;
							}

							// Move to next pixel
							++lbgra;
						}
					}
				}
			}
		}


		//////////
		// Indicate how many pixels were rendered
		//////
			return lnPixelsRendered;
	}




//////////
//
// Called to colorize the bitmap, or a portion of the bitmap.  If the color is clamped,
// no scaling from its current RGB() values less than 255 up to 255 takes place, but the
// color is used exactly as it is, meaning the pixel is grayscaled, and then that grayscale
// value is applied to each value of the RGB() component.  maskColor pixels are colored fully.
//
//////
	u32 iBmp_colorize(SBitmap* bmp, RECT* trc, SBgra colorTemplate, f32 tfAlpha)
	{
		u32		lnPixelsRendered;
		s32		lnY, lnX;
		f32		lfGray, lfRed, lfGrn, lfBlu, lfAlp, lfMalp;
		SRgb*	lrgb;
		SRgba*	lrgba;


		// Make sure the environment is sane
		lnPixelsRendered = 0;
		if (bmp && trc)
		{
			// Build the actual color
			lfRed	= (f32)colorTemplate.red;
			lfGrn	= (f32)colorTemplate.grn;
			lfBlu	= (f32)colorTemplate.blu;

			// Grab our constants
			lfAlp	= tfAlpha;
			lfMalp	= 1.0f - lfAlp;


		//////////
		// Draw it
		//////
			for (lnY = trc->top; lnY < bmp->bi.biHeight && lnY < trc->bottom; lnY++)
			{
				// Are we on the image?
				if (lnY >= 0)
				{
					// What exactly are we copying?
					if (bmp->bi.biBitCount == 24)
					{
						// Build the pointer
						lrgb = (SRgb*)((s8*)bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth) + (trc->left * 3));

						// Iterate through every visible column
						for (lnX = trc->left; lnX < bmp->bi.biWidth && lnX < trc->right; lnX++)
						{
							// Are we on the image?
							if (lnX >= 0 && lnX < bmp->bi.biWidth)
							{
								if (lrgb->red == 222 && lrgb->grn == 22 && lrgb->blu == 222)
								{
									// Every transparent color gets the colorized color
									lrgb->red = (u8)lfRed;
									lrgb->grn = (u8)lfGrn;
									lrgb->blu = (u8)lfBlu;

								} else {
									// Compute the grayscale
									lfGray = min(max(((f32)lrgb->red * 0.35f + (f32)lrgb->grn * 0.54f + (f32)lrgb->blu * 0.11f), 0.0f), 255.0f) / 255.0f;

									// Apply the color proportionally
									lrgb->red = (u8)(((lfGray * lfRed) * lfAlp) + ((f32)lrgb->red * lfMalp));
									lrgb->grn = (u8)(((lfGray * lfGrn) * lfAlp) + ((f32)lrgb->grn * lfMalp));
									lrgb->blu = (u8)(((lfGray * lfBlu) * lfAlp) + ((f32)lrgb->blu * lfMalp));
								}
								++lnPixelsRendered;
							}

							// Move to next pixel
							++lrgb;
						}

					} else if (bmp->bi.biBitCount == 32) {
						// Build the pointer
						lrgba = (SRgba*)((s8*)bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth) + (trc->left * 4));

						// Iterate through every visible column
						for (lnX = trc->left; lnX < bmp->bi.biWidth && lnX < trc->right; lnX++)
						{
							// Are we on the image?
							if (lnX >= 0 && lnX < bmp->bi.biWidth)
							{
								if (lrgba->red == 222 && lrgba->grn == 22 && lrgba->blu == 222)
								{
									// Every transparent color gets the colorized color
									lrgba->red = (u8)lfRed;
									lrgba->grn = (u8)lfGrn;
									lrgba->blu = (u8)lfBlu;

								} else {
									// Compute the grayscale
									lfGray = min(max(((f32)lrgba->red * 0.35f + (f32)lrgba->grn * 0.54f + (f32)lrgba->blu * 0.11f), 0.0f), 255.0f) / 255.0f;

									// Apply the color proportionally
									lrgba->red = (u8)(((lfGray * lfRed) * lfAlp) + ((f32)lrgba->red * lfMalp));
									lrgba->grn = (u8)(((lfGray * lfGrn) * lfAlp) + ((f32)lrgba->grn * lfMalp));
									lrgba->blu = (u8)(((lfGray * lfBlu) * lfAlp) + ((f32)lrgba->blu * lfMalp));
								}
								++lnPixelsRendered;
							}

							// Move to next pixel
							++lrgba;
						}
					}
				}
			}
		}


		//////////
		// Indicate how many pixels were rendered
		//////
			return(lnPixelsRendered);
	}




//////////
//
// Called to colorize the bitmap, or a portion of the bitmap.  If the color is clamped,
// no scaling from its current RGB() values less than 255 up to 255 takes place, but the
// color is used exactly as it is, meaning the pixel is grayscaled, and then that grayscale
// value is applied to each value of the RGB() component.  maskColor pixels are ignored.
//
//////
	u32 iBmp_colorizeMask(SBitmap* bmp, RECT* trc, SBgra colorTemplate, f32 tfAlpha)
	{
		return iBmp_colorize(bmp, trc, colorTemplate, tfAlpha);
	}




//////////
//
// Called to change one color to another
//
//////
	u32 iBmp_swapColors(SBitmap* bmp, SBgra colorOld, SBgra colorNew)
	{
		s32		lnX, lnY;
		u32		lnPixelsRendered;
		SBgr*	lbgr;
		SBgra*	lbgra;


		lnPixelsRendered = 0;
		if (bmp)
		{
			if (bmp->bi.biBitCount == 24)
			{
				// Iterate through every row
				for (lnY = 0; lnY < bmp->bi.biHeight; lnY++)
				{
					// Grab the pointer for this row
					lbgr = (SBgr*)(bmp->bd + (bmp->bi.biHeight - lnY - 1) * bmp->rowWidth);

					// Iterate through every column
					for (lnX = 0; lnX < bmp->bi.biWidth; lnX++, lbgr++)
					{
						// Is this our color?
						if (lbgr->red == colorOld.red && lbgr->grn == colorOld.grn && lbgr->blu == colorOld.blu)
						{
							// Swap the color
							lbgr->red = colorNew.red;
							lbgr->grn = colorNew.grn;
							lbgr->blu = colorNew.blu;
							++lnPixelsRendered;
						}
					}
				}

			} else if (bmp->bi.biBitCount == 32) {
				// Iterate through every row
				for (lnY = 0; lnY < bmp->bi.biHeight; lnY++)
				{
					// Grab the pointer for this row
					lbgra = (SBgra*)(bmp->bd + (bmp->bi.biHeight - lnY - 1) * bmp->rowWidth);

					// Iterate through every column
					for (lnX = 0; lnX < bmp->bi.biWidth; lnX++, lbgra++)
					{
						// Is this our color?
						if (lbgra->red == colorOld.red && lbgra->grn == colorOld.grn && lbgra->blu == colorOld.blu)
						{
							// Swap the color
							lbgra->red = colorNew.red;
							lbgra->grn = colorNew.grn;
							lbgra->blu = colorNew.blu;
							++lnPixelsRendered;
						}
					}
				}
			}
		}
		// Indicate our status
		return(lnPixelsRendered);
	}




//////////
//
// Called to apply an alpha
//
//////
	u32 iBmp_alphaColorizeMask(SBitmap* bmp, RECT* trc, SBgra colorAlpha, f32 alpha)
	{
		u32		lnPixelsRendered;
		s32		lnY, lnX;
		f32		lfAlp, lfMalp, lfRed, lfGrn, lfBlu;
		SBgr*	lbgr;
		SBgra*	lbgra;


		// Make sure the environment is sane
		lnPixelsRendered = 0;
		if (bmp && trc)
		{
			// Verify our alpha is in range 0..1
			lfAlp	= min(max(alpha, 0.0f), 1.0f);
			lfMalp	= 1.0f - lfAlp;

			// Grab the color
			lfRed = (f32)colorAlpha.red * lfAlp;
			lfGrn = (f32)colorAlpha.grn * lfAlp;
			lfBlu = (f32)colorAlpha.blu * lfAlp;

			//////////
			// Draw it
			//////
				for (lnY = trc->top; lnY < bmp->bi.biHeight && lnY < trc->bottom; lnY++)
				{
					// Are we on the image?
					if (lnY >= 0)
					{
						// What exactly are we copying?
						if (bmp->bi.biBitCount == 24)
						{
							// Build the pointer
							lbgr = (SBgr*)((s8*)bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth));

							// Iterate through every visible column
							for (lnX = trc->left; lnX < bmp->bi.biWidth && lnX < trc->right; lnX++)
							{
								// Are we on the image?
								if (lnX >= 0)
								{
									if (!(lbgr->red == 222 && lbgr->grn == 22 && lbgr->blu == 222))
									{
										// Apply the color proportionally
										lbgr->red = (u8)(((f32)lbgr->red * lfMalp) + lfRed);
										lbgr->grn = (u8)(((f32)lbgr->grn * lfMalp) + lfGrn);
										lbgr->blu = (u8)(((f32)lbgr->blu * lfMalp) + lfBlu);
										++lnPixelsRendered;
									}
								}

								// Move to next pixel
								++lbgr;
							}

						} else if (bmp->bi.biBitCount == 32) {
							// Build the pointer
							lbgra = (SBgra*)((s8*)bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth));

							// Iterate through every visible column
							for (lnX = trc->left; lnX < bmp->bi.biWidth && lnX < trc->right; lnX++)
							{
								// Are we on the image?
								if (lnX >= 0)
								{
									if (!(lbgra->red == 222 && lbgra->grn == 22 && lbgra->blu == 222))
									{
										// Apply the color proportionally
										lbgra->red = (u8)(((f32)lbgra->red * lfMalp) + lfRed);
										lbgra->grn = (u8)(((f32)lbgra->grn * lfMalp) + lfGrn);
										lbgra->blu = (u8)(((f32)lbgra->blu * lfMalp) + lfBlu);
										++lnPixelsRendered;
									}
								}

								// Move to next pixel
								++lbgra;
							}
						}
					}
				}
		}


		//////////
		// Indicate how many pixels were rendered
		//////
			return(lnPixelsRendered);
	}




//////////
//
// Physically render the bitmap atop the bitmap, with without the mask bits rgb(222,22,222)
//
//////
	u32 iBmp_bitBltMask(SBitmap* bmpDst, RECT* trc, SBitmap* bmpSrc)
	{
		u32			lnPixelsRendered;
		s32			lnY, lnX, lnYDst, lnXDst;
		f64			lfAlp, lfMalp;
		SBgr*		lbgrDst;
		SBgr*		lbgrSrc;
		SBgra*		lbgraDst;
		SBgra*		lbgraSrc;


		lnPixelsRendered = 0;
		if (bmpDst && trc && bmpSrc)
		{
		//////////
		// Draw it
		//////
			for (lnY = 0, lnYDst = trc->top; lnY < bmpSrc->bi.biHeight && lnYDst < trc->bottom; lnYDst++, lnY++)
			{
				// Are we on the image?
				if (lnYDst >= 0 && lnYDst < bmpDst->bi.biHeight)
				{
					// Build the pointer
					lbgrDst		= (SBgr*)((s8*)bmpDst->bd  + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (trc->left * (bmpDst->bi.biBitCount / 8)));
					lbgrSrc		= (SBgr*)((s8*)bmpSrc->bd  + ((bmpSrc->bi.biHeight - lnY    - 1) * bmpSrc->rowWidth));
					lbgraDst	= (SBgra*)((s8*)bmpDst->bd + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (trc->left * (bmpDst->bi.biBitCount / 8)));
					lbgraSrc	= (SBgra*)((s8*)bmpSrc->bd + ((bmpSrc->bi.biHeight - lnY    - 1) * bmpSrc->rowWidth));

					// What exactly are we copying?
					if (bmpSrc->bi.biBitCount == 24)
					{
						// 24-bit source
						if (bmpDst->bi.biBitCount == 24)
						{
							// 24-bit to 24-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth)
								{
									// Copy the pixel if it's not a mask pixel
									if (!(lbgrSrc->red == 222 && lbgrSrc->grn == 22 && lbgrSrc->blu == 222))
									{
										lbgrDst->red	= lbgrSrc->red;
										lbgrDst->grn	= lbgrSrc->grn;
										lbgrDst->blu	= lbgrSrc->blu;
										++lnPixelsRendered;
									}
								}

								// Move to next pixel on both
								++lbgrDst;
								++lbgrSrc;
							}

						} else {
							// 24-bit to 32-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth)
								{
									// Copy the pixel if it's not a mask pixel
									if (!(lbgrSrc->red == 222 && lbgrSrc->grn == 22 && lbgrSrc->blu == 222))
									{
										lbgraDst->alp	= 255;
										lbgraDst->red	= lbgrSrc->red;
										lbgraDst->grn	= lbgrSrc->grn;
										lbgraDst->blu	= lbgrSrc->blu;
										++lnPixelsRendered;
									}
								}

								// Move to next pixel on both
								++lbgraDst;
								++lbgrSrc;
							}
						}

					} else {
						// 32-bit source
						if (bmpDst->bi.biBitCount == 24)
						{
							// 32-bit to 24-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lbgraSrc->alp != 0)
								{
									// Copy the pixel if it's not a mask pixel
									if (!(lbgraSrc->red == 222 && lbgraSrc->grn == 22 && lbgraSrc->blu == 222))
									{
										if (lbgraSrc->alp == 255)
										{
											// Opaque
											lbgrDst->red	= lbgraSrc->red;
											lbgrDst->grn	= lbgraSrc->grn;
											lbgrDst->blu	= lbgraSrc->blu;

										} else {
											// Some degree of transparency
											lfAlp			= ((f64)lbgraSrc->alp / 255.0);
											lfMalp			= 1.0 - lfAlp;
											lbgrDst->red	= (u8)min(max(((f64)lbgrDst->red * lfMalp) + (lbgraSrc->red * lfAlp), 0.0), 255.0);
											lbgrDst->grn	= (u8)min(max(((f64)lbgrDst->grn * lfMalp) + (lbgraSrc->grn * lfAlp), 0.0), 255.0);
											lbgrDst->blu	= (u8)min(max(((f64)lbgrDst->blu * lfMalp) + (lbgraSrc->blu * lfAlp), 0.0), 255.0);
										}
										++lnPixelsRendered;
									}
								}

								// Move to next pixel on both
								++lbgrDst;
								++lbgraSrc;
							}

						} else {
							// 32-bit to 32-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lbgraSrc->alp != 0)
								{
									// Copy the pixel if it's not a mask pixel
									if (!(lbgraSrc->red == 222 && lbgraSrc->grn == 22 && lbgraSrc->blu == 222))
									{
										if (lbgraSrc->alp == 255)
										{
											// Opaque
											lbgraDst->alp	= 255;
											lbgraDst->red	= lbgraSrc->red;
											lbgraDst->grn	= lbgraSrc->grn;
											lbgraDst->blu	= lbgraSrc->blu;

										} else {
											// Some degree of transparency
											lfAlp			= ((f64)lbgraSrc->alp / 255.0);
											lfMalp			= 1.0 - lfAlp;
											lbgraDst->alp	= 255;
											lbgraDst->red	= (u8)min(max(((f64)lbgraDst->red * lfMalp) + (lbgraSrc->red * lfAlp), 0.0), 255.0);
											lbgraDst->grn	= (u8)min(max(((f64)lbgraDst->grn * lfMalp) + (lbgraSrc->grn * lfAlp), 0.0), 255.0);
											lbgraDst->blu	= (u8)min(max(((f64)lbgraDst->blu * lfMalp) + (lbgraSrc->blu * lfAlp), 0.0), 255.0);
										}
										++lnPixelsRendered;
									}
								}

								// Move to next pixel on both
								++lbgraDst;
								++lbgraSrc;
							}
						}
					}
				}
			}
		}
		// Indicate how many pixels were drawn
		return(lnPixelsRendered);
	}




//////////
//
// Called to render the indicated image by its grayscale value (not computed, but assumed
// as only red is sampled), with the indicated color applied onto the bmpDst bitmap.
//
//////
	u32 iBmp_bitBlt_byGraymask(SBitmap* bmpDst, RECT* trc, SBitmap* bmpSrc, SBgra color)
	{
		u32			lnPixelsRendered;
		s32			lnY, lnX, lnYDst, lnXDst;
		f64			lfAlp, lfMalp, lfRed, lfGrn, lfBlu;
		SBgr*		lbgrDst;
		SBgr*		lbgrSrc;
		SBgra*		lbgraDst;
		SBgra*		lbgraSrc;


		lnPixelsRendered = 0;
		if (bmpDst && trc && bmpSrc)
		{
		//////////
		// Draw it
		//////
			lfRed = (f64)color.red;
			lfGrn = (f64)color.grn;
			lfBlu = (f64)color.blu;
			for (lnY = 0, lnYDst = trc->top; lnY < bmpSrc->bi.biHeight && lnYDst < trc->bottom; lnYDst++, lnY++)
			{
				// Are we on the image?
				if (lnYDst >= 0 && lnYDst < bmpDst->bi.biHeight)
				{
					// Build the pointer
					lbgrDst		= (SBgr*)((s8*)bmpDst->bd  + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (trc->left * (bmpDst->bi.biBitCount / 8)));
					lbgrSrc		= (SBgr*)((s8*)bmpSrc->bd  + ((bmpSrc->bi.biHeight - lnY    - 1) * bmpSrc->rowWidth));
					lbgraDst	= (SBgra*)((s8*)bmpDst->bd + ((bmpDst->bi.biHeight - lnYDst - 1) * bmpDst->rowWidth) + (trc->left * (bmpDst->bi.biBitCount / 8)));
					lbgraSrc	= (SBgra*)((s8*)bmpSrc->bd + ((bmpSrc->bi.biHeight - lnY    - 1) * bmpSrc->rowWidth));

					// What exactly are we copying?
					if (bmpSrc->bi.biBitCount == 24)
					{
						// 24-bit source
						if (bmpDst->bi.biBitCount == 24)
						{
							// 24-bit to 24-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth)
								{
									// Copy the pixel if it's not a mask pixel
									if (!(lbgrSrc->red == 255 && lbgrSrc->grn == 255 && lbgrSrc->blu == 255))
									{
										lfAlp			= (f64)(255 - lbgrSrc->red) / 255.0;
										lfMalp			= 1.0 - lfAlp;
										lbgrDst->red	= (u8)(((f64)lbgrDst->red * lfMalp) + (lfRed * lfAlp));
										lbgrDst->grn	= (u8)(((f64)lbgrDst->grn * lfMalp) + (lfGrn * lfAlp));
										lbgrDst->blu	= (u8)(((f64)lbgrDst->blu * lfMalp) + (lfBlu * lfAlp));
										++lnPixelsRendered;
									}
								}

								// Move to next pixel on both
								++lbgrDst;
								++lbgrSrc;
							}

						} else {
							// 24-bit to 32-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth)
								{
									// Copy the pixel if it's not a mask pixel
									if (!(lbgrSrc->red == 255 && lbgrSrc->grn == 255 && lbgrSrc->blu == 255))
									{
										lfAlp			= (f64)(255 - lbgrSrc->red) / 255.0;
										lfMalp			= 1.0 - lfAlp;
										lbgraDst->alp	= 255;
										lbgraDst->red	= (u8)(((f64)lbgraDst->red * lfMalp) + (lfRed * lfAlp));
										lbgraDst->grn	= (u8)(((f64)lbgraDst->grn * lfMalp) + (lfGrn * lfAlp));
										lbgraDst->blu	= (u8)(((f64)lbgraDst->blu * lfMalp) + (lfBlu * lfAlp));
										++lnPixelsRendered;
									}
								}

								// Move to next pixel on both
								++lbgraDst;
								++lbgrSrc;
							}
						}

					} else {
						// 32-bit source
						if (bmpDst->bi.biBitCount == 24)
						{
							// 32-bit to 24-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lbgraSrc->alp != 0)
								{
									// Copy the pixel if it's not a mask pixel
									if (!(lbgraSrc->red == 255 && lbgraSrc->grn == 255 && lbgraSrc->blu == 255))
									{
										lfAlp			= (f64)(255 - lbgraSrc->red) / 255.0;
										lfMalp			= 1.0 - lfAlp;
										lbgrDst->red	= (u8)(((f64)lbgrDst->red * lfMalp) + (lfRed * lfAlp));
										lbgrDst->grn	= (u8)(((f64)lbgrDst->grn * lfMalp) + (lfGrn * lfAlp));
										lbgrDst->blu	= (u8)(((f64)lbgrDst->blu * lfMalp) + (lfBlu * lfAlp));
										++lnPixelsRendered;
									}
								}

								// Move to next pixel on both
								++lbgrDst;
								++lbgraSrc;
							}

						} else {
							// 32-bit to 32-bit
							// Iterate through every visible column
							for (lnX = 0, lnXDst = trc->left; lnX < bmpSrc->bi.biWidth && lnXDst < trc->right; lnXDst++, lnX++)
							{
								// Are we on the image?
								if (lnXDst >= 0 && lnXDst < bmpDst->bi.biWidth && lbgraSrc->alp != 0)
								{
									// Copy the pixel if it's not a mask pixel
									if (!(lbgraSrc->red == 255 && lbgraSrc->grn == 255 && lbgraSrc->blu == 255))
									{
										lfAlp			= (f64)(255 - lbgraSrc->red) / 255.0;
										lfMalp			= 1.0 - lfAlp;
										lbgraDst->alp	= 255;
										lbgraDst->red	= (u8)(((f64)lbgraDst->red * lfMalp) + (lfRed * lfAlp));
										lbgraDst->grn	= (u8)(((f64)lbgraDst->grn * lfMalp) + (lfGrn * lfAlp));
										lbgraDst->blu	= (u8)(((f64)lbgraDst->blu * lfMalp) + (lfBlu * lfAlp));
										++lnPixelsRendered;
									}
								}

								// Move to next pixel on both
								++lbgraDst;
								++lbgraSrc;
							}
						}
					}
				}
			}
		}
		// Indicate how many pixels were drawn
		return(lnPixelsRendered);
	}




//////////
//
// Called to create a bitmap and extract by explicit coordinates, rather than RECT.
//
//////
	SBitmap* iBmp_createAndExtractRect(SBitmap* bmpSrc, s32 tnUlX, s32 tnUlY, s32 tnLrX, s32 tnLrY)
	{
		SBitmap*	bmpNew;
		RECT		lrc;


		// Make sure our environment is sane
		if (bmpSrc)
		{
			// Create the new bitmap
			bmpNew = iBmp_allocate();
			iBmp_createBySize(bmpNew, tnLrX - tnUlX, tnLrY - tnUlY, bmpSrc->bi.biBitCount);

			// Extract it
			SetRect(&lrc, tnUlX, tnUlY, tnLrX, tnLrY);
			BitBlt(bmpNew->hdc, 0, 0, bmpNew->bi.biWidth, bmpNew->bi.biHeight, bmpSrc->hdc, tnUlX, tnUlY, SRCCOPY);

			// Indicate success
			return(bmpNew);

		}
		// If we get here, invalid
		return(NULL);
	}




//////////
//
// Called to derive the arbitrary region using the RGB(222,22,222) color as the mask.
// This means all RGB(222,22,222) pixels will be in the region.  To get the opposite
// condition use it and the RGN_XOR operator to derive the opposite.
//
//////
	HRGN iBmp_extractRgnByMask(SBitmap* bmp, RECT* trc)
	{
		s32		lnY, lnX;
		HRGN	lrgnAccumulated, lrgnRemoveArea;
		RECT	lrc;
		SBgr*	lbgr;
		SBgra*	lbgra;


		// Create a region encompassing the entire bitmap
		lrgnAccumulated = CreateRectRgnIndirect(trc);

		// Make sure our environment is sane
		if (bmp)
		{
			// Based on its, process it
			if (bmp->bi.biBitCount == 24)
			{
				// Iterate for every row
				for (lnY = trc->top; lnY < bmp->bi.biHeight && lnY <= trc->bottom; lnY++)
				{
					// Grab the pointer to this row
					lbgr = (SBgr*)(bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth) + (trc->left * 3));

					// Iterate for every column, combining where we should
					for (lnX = trc->left; lnX < bmp->bi.biWidth && lnX < trc->right; lnX++, lbgr++)
					{
						// If this is one, remove this part
						if (lbgr->red == 222 && lbgr->grn == 22 && lbgr->blu == 222)
						{
							SetRect(&lrc, lnX, lnY, lnX + 1, lnY + 1);
							lrgnRemoveArea = CreateRectRgnIndirect(&lrc);
							CombineRgn(lrgnAccumulated, lrgnAccumulated, lrgnRemoveArea, RGN_XOR);		// Mask out lrgnRemove from lrgnMain
							DeleteObject((HGDIOBJ)lrgnRemoveArea);
						}
					}
				}

			} else if (bmp->bi.biBitCount == 32) {
				// Iterate for every row
				for (lnY = trc->top; lnY < bmp->bi.biHeight; lnY++)
				{
					// Grab the pointer to this row
					lbgra = (SBgra*)(bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth) + (trc->left * 4));

					// Iterate for every column, combining where we should
					for (lnX = trc->left; lnX < bmp->bi.biWidth && lnX <= trc->right; lnX++, lbgra++)
					{
						// If this is one, remove this part
						if (lbgra->red == 222 && lbgra->grn == 22 && lbgra->blu == 222)
						{
							SetRect(&lrc, lnX, lnY, lnX + 1, lnY + 1);
							lrgnRemoveArea = CreateRectRgnIndirect(&lrc);
							CombineRgn(lrgnAccumulated, lrgnAccumulated, lrgnRemoveArea, RGN_XOR);		// Mask out lrgnRemove from lrgnMain
							DeleteObject((HGDIOBJ)lrgnRemoveArea);
						}
					}
				}
			}
		}

		// Return the region
		return(lrgnAccumulated);
	}




//////////
//
// Draws a wavy line in the rectangle based on the indicated color.  The wavy line
// looks like this, beginning from the bottom, going up/down at a 45 degree angle:
//
//		/\/\/\/\/\/\/
//
//////
	u32 iBmp_wavyLine(SBitmap* bmp, RECT* trc, SBgra color, f32 tfAlpha)
	{
		s32		lnY, lnX, lnXX, lnY_inc;
		u32		lnPixelsRendered;
		f32		lfAlp, lfMalp;
		SRgb*	lrgb;
		SRgba*	lrgba;


		// Make sure our environment is sane
		lnPixelsRendered = 0;
		if (bmp)
		{
			// Compute our alpha
			if (tfAlpha < 0.0f)		tfAlpha = 0.0f;
			if (tfAlpha > 1.0f)		tfAlpha = 1.0f;
			lfAlp	= tfAlpha;
			lfMalp	= 1.0f - lfAlp;

			// Begin at the lower-left
			lnY		= trc->bottom;
			lnY_inc	= -1;
			if (bmp->bi.biBitCount == 24)
			{
				// Process across the rectangle
				for (lnX = trc->left, lnXX = 0; lnX < bmp->bi.biWidth && lnX < trc->right; lnX += (lnXX & 1), lnXX++)
				{
					// Grab the pointer for this part of the wavy line
					lrgb = (SRgb*)(bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth) + (lnX * 3));

					// Draw the pixel
					lrgb->red = (u8)(((f32)color.red * lfAlp) + ((f32)lrgb->red * lfMalp));
					lrgb->grn = (u8)(((f32)color.grn * lfAlp) + ((f32)lrgb->grn * lfMalp));
					lrgb->blu = (u8)(((f32)color.blu * lfAlp) + ((f32)lrgb->blu * lfMalp));

					// Adjust for the next pixel
					lnY += lnY_inc;

					// Constrain
					if (lnY <= trc->top)
					{
						// We're above, head the other way
						lnY_inc	= 1;

					} else if (lnY >= trc->bottom) {
						// We're below, head the other way
						lnY_inc	= -1;
					}
				}

			} else if (bmp->bi.biBitCount == 32) {
				// Process across the rectangle
				for (lnX = trc->left, lnXX = 0; lnX < bmp->bi.biWidth && lnX <= trc->right; lnX += (lnXX & 1), lnXX++)
				{
					// Grab the pointer for this part of the wavy line
					lrgba = (SRgba*)(bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth) + (lnX * 4));


					// Draw the pixel
					lrgba->red = (u8)(((f32)color.red * lfAlp) + ((f32)lrgba->red * lfMalp));
					lrgba->grn = (u8)(((f32)color.grn * lfAlp) + ((f32)lrgba->grn * lfMalp));
					lrgba->blu = (u8)(((f32)color.blu * lfAlp) + ((f32)lrgba->blu * lfMalp));

					// Adjust for the next pixel
					lnY += lnY_inc;

					// Constrain
					if (lnY <= trc->top)
					{
						// We're above, head the other way
						lnY_inc	= 1;

					} else if (lnY >= trc->bottom) {
						// We're below, head the other way
						lnY_inc	= -1;
					}
				}
			}
		}

		// Return the region
		return(lnPixelsRendered);
	}




//////////
//
// Called to flip a bitmap horizontally.
// Creates a new bitmap of equal size.
//
//////
	SBitmap* iBmp_flipHorizontal(SBitmap* bmp)
	{
		s32			lnX, lnY;
		SRgb*		lrgbS;
		SRgb*		lrgbD;
		SRgba*		lrgbaS;
		SRgba*		lrgbaD;
		SBitmap*	bmpNew;


		// Create a new image
		if (!bmp || !(bmp->bi.biBitCount == 24 || bmp->bi.biBitCount == 32) || !(bmpNew = iBmp_allocate()))
			return NULL;

		// Physically create it
		iBmp_createBySize(bmpNew, bmp->bi.biWidth, bmp->bi.biHeight, bmp->bi.biBitCount);

		// Render it
		if (bmp->bi.biBitCount == 24)
		{
			// Iterate through line by line
			for (lnY = 0; lnY < bmp->bi.biHeight; ++lnY)
			{
				// Get our pointers
				lrgbS = (SRgb*)(bmp->bd    + ((bmp->bi.biHeight    - lnY - 1) * bmp->rowWidth));
				lrgbD = (SRgb*)(bmpNew->bd + ((bmpNew->bi.biHeight - lnY - 1) * bmpNew->rowWidth) + ((bmpNew->bi.biWidth - 1) * (bmp->bi.biBitCount / 8)));

				// Iterate across this row
				for (lnX = 0; lnX < bmp->bi.biWidth; ++lnX, ++lrgbS, --lrgbD)
					*lrgbD = *lrgbS;
			}

		} else if (bmp->bi.biBitCount == 32) {
			// Iterate through line by line
			for (lnY = 0; lnY < bmp->bi.biHeight; ++lnY)
			{
				// Get our pointers
				lrgbaS = (SRgba*)(bmp->bd    + ((bmp->bi.biHeight    - lnY - 1) * bmp->rowWidth));
				lrgbaD = (SRgba*)(bmpNew->bd + ((bmpNew->bi.biHeight - lnY - 1) * bmpNew->rowWidth) + ((bmpNew->bi.biWidth - 1) * 4));

				// Iterate across this row
				for (lnX = 0; lnX < bmp->bi.biWidth; ++lnX, ++lrgbaS, --lrgbaD)
					*lrgbaD = *lrgbaS;
			}
		}

		// Signify
		return bmpNew;
	}




//////////
//
// Called to flip an image vertically
//
//////
	SBitmap* iBmp_flipVertical(SBitmap* bmp)
	{
		s32			lnY;
		SRgb*		lrgbS;
		SRgb*		lrgbD;
		SRgba*		lrgbaS;
		SRgba*		lrgbaD;
		SBitmap*	bmpNew;


		// Create a new image
		if (!bmp || !(bmp->bi.biBitCount == 24 || bmp->bi.biBitCount == 32) || !(bmpNew = iBmp_allocate()))
			return NULL;

		// Physically create it
		iBmp_createBySize(bmpNew, bmp->bi.biWidth, bmp->bi.biHeight, bmp->bi.biBitCount);

		// Render it
		if (bmp->bi.biBitCount == 24)
		{
			// Iterate through line by line
			for (lnY = 0; lnY < bmp->bi.biHeight; ++lnY)
			{
				// Get our pointers
				lrgbS = (SRgb*)(bmp->bd    + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth));
				lrgbD = (SRgb*)(bmpNew->bd + (lnY * bmpNew->rowWidth));

				// Copy the entire row
				memcpy(lrgbD, lrgbS, bmp->rowWidth);
			}

		} else if (bmp->bi.biBitCount == 32) {
			// Iterate through line by line
			for (lnY = 0; lnY < bmp->bi.biHeight; ++lnY)
			{
				// Get our pointers
				lrgbaS = (SRgba*)(bmp->bd    + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth));
				lrgbaD = (SRgba*)(bmpNew->bd + (lnY * bmpNew->rowWidth));

				// Copy the entire row
				memcpy(lrgbaD, lrgbaS, bmp->rowWidth);
			}
		}

		// Signify
		return bmpNew;
	}




//////////
//
// Called to rotate an image by 90 degrees counter-clockwise
//
//////
	SBitmap* iBmp_rotate90DegreesCCW(SBitmap* bmp)
	{
		s32			lnXS, lnYS;
		SRgb*		lrgbS;
		SRgb*		lrgbD;
		SRgba*		lrgbaS;
		SRgba*		lrgbaD;
		SBitmap*	bmpNew;


		// Create a new image
		if (!bmp || !(bmp->bi.biBitCount == 24 || bmp->bi.biBitCount == 32) || !(bmpNew = iBmp_allocate()))
			return NULL;

		// Physically create it
		iBmp_createBySize(bmpNew, bmp->bi.biHeight, bmp->bi.biWidth, bmp->bi.biBitCount);

		// Render it
		if (bmp->bi.biBitCount == 24)
		{
			// Iterate through line by line
			for (lnYS = 0; lnYS < bmp->bi.biHeight; ++lnYS)
			{
				// Get our pointers
				lrgbS = (SRgb*)(bmp->bd    + ((bmp->bi.biHeight   - lnYS - 1) * bmp->rowWidth));
				lrgbD = (SRgb*)(bmpNew->bd + (lnYS                            * (bmp->bi.biBitCount / 8)));

				// Iterate across this row
				for (lnXS = 0; lnXS < bmp->bi.biWidth; ++lnXS, ++lrgbS, lrgbD = (SRgb*)((s8*)lrgbD + bmpNew->rowWidth))
					*lrgbD = *lrgbS;	// Copy the pixel
			}

		} else if (bmp->bi.biBitCount == 32) {
			// Iterate through line by line
			for (lnYS = 0; lnYS < bmp->bi.biHeight; ++lnYS)
			{
				// Get our pointers
				lrgbaS = (SRgba*)(bmp->bd    + ((bmp->bi.biHeight   - lnYS - 1) * bmp->rowWidth));
				lrgbaD = (SRgba*)(bmpNew->bd + (lnYS                            * 4));

				// Iterate across this row
				for (lnXS = 0; lnXS < bmp->bi.biWidth; ++lnXS, ++lrgbaS, lrgbaD = (SRgba*)((s8*)lrgbaD + bmpNew->rowWidth))
					*lrgbaD = *lrgbaS;	// Copy the pixel
			}
		}

		// Signify
		return bmpNew;
	}




//////////
//
// Called to round the corners off the indicated bitmap.
//
//////
	u32 iBmp_roundCorners(SBitmap* bmp, s32 tnType, SBgra color)
	{
		u32	lnPixelsRendered;
		s32	lnI, lnMax;


		// Make sure our environment is sane
		lnPixelsRendered = 0;
		if (bmp)
		{
			// How are we rounding?
			switch (tnType)
			{
				case 1:
					// Single pixels off the corners
					iiBmp_setPixel(bmp, 0,						0,						color);
					iiBmp_setPixel(bmp, bmp->bi.biWidth - 1,	0,						color);
					iiBmp_setPixel(bmp, 0,						bmp->bi.biHeight - 1,	color);
					iiBmp_setPixel(bmp, bmp->bi.biWidth - 1,	bmp->bi.biHeight - 1,	color);
					break;

				case 2:
					// Two pixels off each corner in each direction (three pixels total)
					if (bmp->bi.biWidth >= 2 && bmp->bi.biHeight >= 2)
					{
						lnMax = min(2, bmp->bi.biWidth);
						for (lnI = 0; lnI < 2; lnI++)
						{
							// Upper-left
							iiBmp_setPixel(bmp, 0,		0,		color);
							iiBmp_setPixel(bmp, lnI,	lnI,	color);

							// Upper-right
							iiBmp_setPixel(bmp, bmp->bi.biWidth - 1 - lnI,	0,		color);
							iiBmp_setPixel(bmp, bmp->bi.biWidth - 1,		lnI,	color);

							// Lower-left
							iiBmp_setPixel(bmp, lnI,	bmp->bi.biHeight - 1,			color);
							iiBmp_setPixel(bmp, 0,		bmp->bi.biHeight - 1 - lnI,		color);

							// Lower-right
							iiBmp_setPixel(bmp, bmp->bi.biWidth - 1,			bmp->bi.biHeight - 1 - lnI,		color);
							iiBmp_setPixel(bmp, bmp->bi.biWidth - 1 - lnI,		bmp->bi.biHeight - 1,			color);
						}
					}
					break;

				case 3:
					// Three pixels off each corner in each direction (five pixels total), plus two diagonally from each corner (add one, for six pixels total)
					if (bmp->bi.biWidth >= 3 && bmp->bi.biHeight >= 3)
					{
						lnMax = min(3, bmp->bi.biWidth);
						for (lnI = 0; lnI < lnMax; lnI++)
						{
							// Upper-left
							iiBmp_setPixel(bmp, lnI,	0,		color);
							iiBmp_setPixel(bmp, 0,		lnI,	color);

							// Upper-right
							iiBmp_setPixel(bmp, bmp->bi.biWidth - 1 - lnI,	0,		color);
							iiBmp_setPixel(bmp, bmp->bi.biWidth - 1,		lnI,	color);

							// Lower-left
							iiBmp_setPixel(bmp, lnI,	bmp->bi.biHeight - 1,			color);
							iiBmp_setPixel(bmp, 0,		bmp->bi.biHeight - 1 - lnI,		color);

							// Lower-right
							iiBmp_setPixel(bmp, bmp->bi.biWidth - 1,			bmp->bi.biHeight - 1 - lnI,		color);
							iiBmp_setPixel(bmp, bmp->bi.biWidth - 1 - lnI,		bmp->bi.biHeight - 1,			color);

// 							// Inset pixels
// 							if (lnI == 0)
// 							{
// 								// Draw pixels
// 								iiBmp_setPixel(bmp, 1,						1,						color);
// 								iiBmp_setPixel(bmp, bmp->bi.biWidth - 2,	1,						color);
// 								iiBmp_setPixel(bmp, 1,						bmp->bi.biHeight - 2,	color);
// 								iiBmp_setPixel(bmp, bmp->bi.biWidth - 2,	bmp->bi.biHeight - 2,	color);
// 							}
						}
					}
					break;
			}
		}

		// Indicate our status
		return(lnPixelsRendered);
	}




//////////
//
// Extracts the color at the indicated point
//
//////
	SBgra iBmp_extractColorAtPoint(SBitmap* bmp, s32 tnX, s32 tnY)
	{
		SBgra	invalidColor;
		SBgr*	lbgr;
		SBgra*	lbgra;
		SBgra	color;


		// Make sure our environment is sane
		if (bmp && tnX >= 0 && tnX < bmp->bi.biWidth && tnY >= 0 && tnY < bmp->bi.biHeight)
		{
			// Based on its, process it
			if (bmp->bi.biBitCount == 24)
			{
				// Grab the pointer to this pixel
				lbgr = (SBgr*)(bmp->bd + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + tnX * 3);

				// Construct the color
				color.alp = 255;
				color.red = lbgr->red;
				color.grn = lbgr->grn;
				color.blu = lbgr->blu;

				// Return the color
				return(color);

			} else if (bmp->bi.biBitCount == 32) {
				// Grab the pointer to this pixel
				lbgra = (SBgra*)(bmp->bd + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX * 4));

				// Return directly
				return *lbgra;
			}
		}

		// If we get here, invalid
		invalidColor.color = rgba(0,0,0,255);
		return invalidColor;
	}




//////////
//
// Called to combine the two colors with the ratio of:
//		(color1 * tfAlp) + (color2 * (1.0 - tfAlp)).
//
//////
	SBgra iBmp_colorCombine(SBgra color1, SBgra color2, f32 tfAlp)
	{
		SBgra	color;
		f32		lfMalp;


		// Initialize
		tfAlp	= min(max(tfAlp, 0.0f), 1.0f);
		lfMalp	= 1.0f - tfAlp;

		// Combine
		color.red	= (u8)min(max(((f32)color1.red * tfAlp) + ((f32)color2.red * lfMalp), 0.0f), 255.0f);
		color.grn	= (u8)min(max(((f32)color1.grn * tfAlp) + ((f32)color2.grn * lfMalp), 0.0f), 255.0f);
		color.blu	= (u8)min(max(((f32)color1.blu * tfAlp) + ((f32)color2.blu * lfMalp), 0.0f), 255.0f);

		// Return our value
		return(color);
	}




//////////
//
// Called to lift the colors until one of them reaches 255
//
//////
	SBgra* iBmp_colorLift(SBgra color, SBgra* colorOut)
	{
		u8	lnMinDelta, lnRedDelta, lnGrnDelta, lnBluDelta;


		// Calculate the channel deltas
		lnRedDelta = (u8)255 - color.red;
		lnGrnDelta = (u8)255 - color.grn;
		lnBluDelta = (u8)255 - color.blu;

		// Find out our minimum movement
		lnMinDelta =                 lnRedDelta;
		lnMinDelta = min(lnMinDelta, lnGrnDelta);
		lnMinDelta = min(lnMinDelta, lnBluDelta);

		// Lift (if we need be)
		if (lnMinDelta != 0)
		{
			// Lift
			colorOut->red	= color.red + lnMinDelta;
			colorOut->grn	= color.grn + lnMinDelta;
			colorOut->blu	= color.blu + lnMinDelta;

		} else {
			// Store the color as is (it's already lifted)
			*colorOut = color;
		}

		// Pass-thru
		return(colorOut);
	}




//////////
//
// Called to locate a marker and return its position, and optionally update the source
// to cover up the marker by mixing the pixels to either side away.  A marker is a tiny
// upper-left coordinate marker which is a pixel, the same pixel to its right, and the
// same pixel on the next row beneath the left-most pixel, all in the indicated red, grn,
// and blu color.
//
//////
	bool iBmp_locateMarker(SBitmap* bmp, u8 red, u8 grn, u8 blu, u32* tnX, u32* tnY, bool tlOverwriteMarker)
	{
		s32		lnY, lnX;
		SBgr*	lbgr;
		SBgr*	lbgrRight;
		SBgr*	lbgrBelow;
		SBgra*	lbgra;
		SBgra*	lbgraRight;
		SBgra*	lbgraBelow;


		// Make sure our environment is sane
		if (bmp)
		{
			// Based on its, process it
			if (bmp->bi.biBitCount == 24)
			{
				// Iterate for every row
				for (lnY = 0; lnY < bmp->bi.biHeight; lnY++)
				{
					// Grab the pointer to this row
					lbgr = (SBgr*)(bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth));

					// Iterate for every column, combining where we should
					for (lnX = 0; lnX < bmp->bi.biWidth; lnX++, lbgr++)
					{
						// If this is one, remove this part
						if (lbgr->red == red && lbgr->grn == grn && lbgr->blu == blu)
						{
							// Grab our right pixel, and the one below it
							lbgrRight	= lbgr + 1;
							lbgrBelow	= (SBgr*)((s8*)lbgr - bmp->rowWidth);

							// Do the pixels match?
							if (lbgrRight->red == red && lbgrRight->grn == grn && lbgrRight->blu == blu)
							{
								// Right matches
								if (lbgrBelow->red == red && lbgrBelow->grn == grn && lbgrBelow->blu == blu)
								{
									// And below matches, this is our marker
									// They match, this is our marker
									if (tlOverwriteMarker)
									{
										// Overlay it
										if (lnX >= 1)
										{
											// Grab the pixel to the left of both the lbgr and lbgrBelow
											// Upper-left
											lbgr->red		= (lbgr-1)->red;
											lbgr->grn		= (lbgr-1)->grn;
											lbgr->blu		= (lbgr-1)->blu;

											// Below
											lbgrBelow->red = (lbgrBelow - 1)->red;
											lbgrBelow->grn = (lbgrBelow - 1)->grn;
											lbgrBelow->blu = (lbgrBelow - 1)->blu;

											if (lnX < bmp->bi.biHeight - 1)
											{
												// Grab the pixel to the right of lbgrRight and copy it
												lbgrRight->red	= (lbgrRight + 1)->red;
												lbgrRight->grn	= (lbgrRight + 1)->grn;
												lbgrRight->blu	= (lbgrRight + 1)->blu;

											} else {
												// Duplicate lbgr into lbgrRight
												lbgrRight->red	= lbgr->red;
												lbgrRight->grn	= lbgr->grn;
												lbgrRight->blu	= lbgr->blu;
											}

										} else {
											// Grab the pixel to the right of lbgrRight and copy to all three
											if (lnX < bmp->bi.biHeight - 1)
											{
												// Pixel to the right
												lbgrRight->red	= (lbgrRight + 1)->red;
												lbgrRight->grn	= (lbgrRight + 1)->grn;
												lbgrRight->blu	= (lbgrRight + 1)->blu;

												// Upper left pixel
												lbgr->red		= lbgrRight->red;
												lbgr->grn		= lbgrRight->grn;
												lbgr->blu		= lbgrRight->blu;

												// Pixel on the row below
												lbgrBelow->red	= lbgrRight->red;
												lbgrBelow->grn	= lbgrRight->grn;
												lbgrBelow->blu	= lbgrRight->blu;

											} else {
												// We can't update it.  The image is too narrow
												// We don't do anything.  I just note this condition here in the comments.
											}
										}
									}

									// Set our values
									if (tnX)	*tnX = lnX;
									if (tnY)	*tnY = lnY;

									// Indicate success
									return(true);
								}
							}
						}
					}
				}

			} else if (bmp->bi.biBitCount == 32) {
				// Iterate for every row
				for (lnY = 0; lnY < bmp->bi.biHeight; lnY++)
				{
					// Grab the pointer to this row
					lbgra = (SBgra*)(bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth));

					// Iterate for every column, combining where we should
					for (lnX = 0; lnX < bmp->bi.biWidth; lnX++, lbgra++)
					{
						// If this is one, remove this part
						if (lbgra->red == red && lbgra->grn == grn && lbgra->blu == blu)
						{
							// Grab our right pixel, and the one below it
							lbgraRight	= lbgra + 1;
							lbgraBelow	= (SBgra*)((s8*)lbgra - bmp->rowWidth);

							// Do the pixels match?
							if (lbgraRight->red == red && lbgraRight->grn == grn && lbgraRight->blu == blu)
							{
								// Right matches
								if (lbgraBelow->red == red && lbgraBelow->grn == grn && lbgraBelow->blu == blu)
								{
									// And below matches, this is our marker
									// They match, this is our marker
									if (tlOverwriteMarker)
									{
										// Overlay it
										if (lnX >= 1)
										{
											// Grab the pixel to the left of both the lbgra and lbgraBelow
											// Upper-left
											lbgra->red		= (lbgra-1)->red;
											lbgra->grn		= (lbgra-1)->grn;
											lbgra->blu		= (lbgra-1)->blu;

											// Below
											lbgraBelow->red	= (lbgraBelow-1)->red;
											lbgraBelow->grn	= (lbgraBelow-1)->grn;
											lbgraBelow->blu	= (lbgraBelow-1)->blu;

											if (lnX < bmp->bi.biHeight - 1)
											{
												// Grab the pixel to the right of lbgraRight and copy it
												lbgraRight->red	= (lbgraRight+1)->red;
												lbgraRight->grn	= (lbgraRight+1)->grn;
												lbgraRight->blu	= (lbgraRight+1)->blu;

											} else {
												// Duplicate lbgra into lbgraRight
												lbgraRight->red	= lbgra->red;
												lbgraRight->grn	= lbgra->grn;
												lbgraRight->blu	= lbgra->blu;
											}

										} else {
											// Grab the pixel to the right of lbgraRight and copy to all three
											if (lnX < bmp->bi.biHeight - 1)
											{
												// Pixel to the right
												lbgraRight->red	= (lbgraRight+1)->red;
												lbgraRight->grn	= (lbgraRight+1)->grn;
												lbgraRight->blu	= (lbgraRight+1)->blu;

												// Upper left pixel
												lbgra->red		= lbgraRight->red;
												lbgra->grn		= lbgraRight->grn;
												lbgra->blu		= lbgraRight->blu;

												// Pixel on the row below
												lbgraBelow->red	= lbgraRight->red;
												lbgraBelow->grn	= lbgraRight->grn;
												lbgraBelow->blu	= lbgraRight->blu;

											} else {
												// We can't update it.  The image is too narrow
												// We don't do anything.  I just note this condition here in the comments.
											}
										}
									}

									// Set our values
									if (tnX)	*tnX = lnX;
									if (tnY)	*tnY = lnY;

									// Indicate success
									return(true);
								}
							}
						}
					}
				}
			}
		}

		// If we get here, not found
		return(false);
	}




//////////
//
// Called to create a cask bitmap scaled to the indicated width and height
//
//////
#if !defined(_EXCLUDE_iBmp_cask_createAndPopulate)
	SBitmap* iBmp_cask_createAndPopulate(s32 iCode, s32 tnWidth, s32 tnHeight, s32* tnSkipChars, u32 tnTextLength, SBgra* caskColor, SBgra textColor, SBgra backgroundColor, bool tlOverrideColors)
	{
		s32			lnI, lnWidth, lnStop, lnLeft;
		bool		llAddParams;
		RECT		lrc;
		SBitmap*	bmpCask;
		SBitmap*	bmpNew;
		SBitmap*	bmpLeft;
		SBitmap*	bmpRight;


		// Make sure our environment is sane
		if (iCode >= _ICODE_CASK_MINIMUM && iCode <= _ICODE_CASK_MAXIMUM)
		{
			// Find out what kind of cask it is
			llAddParams = false;
			switch (iCode)
			{
				case _ICODE_CASK_ROUND_PARAMS:
					// (||round||)
					*tnSkipChars	= 3;
					bmpLeft			= bmpCaskRoundLeft;
					bmpRight		= bmpCaskRoundRight;
					if (tlOverrideColors)
					{
						caskColor->color	= pastelGreenColor.color;
						textColor.color		= darkGreenColor.color;
					}
					llAddParams		= true;
					break;

				case _ICODE_CASK_SQUARE_PARAMS:
					// [||square||]
					*tnSkipChars	= 3;
					bmpLeft			= bmpCaskSquareLeft;
					bmpRight		= bmpCaskSquareRight;
					if (tlOverrideColors)
					{
						caskColor->color	= pastelOrangeColor.color;
						textColor.color		= darkOrangeColor.color;
					}
					llAddParams		= true;
					break;

				case _ICODE_CASK_TRIANGLE_PARAMS:
					// <||triangle||>
					*tnSkipChars	= 3;
					bmpLeft			= bmpCaskTriangleLeft;
					bmpRight		= bmpCaskTriangleRight;
					if (tlOverrideColors)
					{
						caskColor->color	= pastelYellowColor.color;
						textColor.color		= blackColor.color;
					}
					llAddParams		= true;
					break;

				case _ICODE_CASK_TILDE_PARAMS:
					// ~||tilde||~
					*tnSkipChars	= 3;
					bmpLeft			= bmpCaskTildeLeft;
					bmpRight		= bmpCaskTildeRight;
					if (tlOverrideColors)
					{
						caskColor->color	= pastelBlueColor.color;
						textColor.color		= whiteColor.color;
					}
					llAddParams		= true;
					break;

				case _ICODE_CASK_CODE_PARAMS:
					// \||code||/
					*tnSkipChars	= 3;
					bmpLeft			= bmpCaskCodeLeft;
					bmpRight		= bmpCaskCodeRight;
					if (tlOverrideColors)
					{
						caskColor->color	= pastelRedColor.color;
						textColor.color		= whiteColor.color;
					}
					llAddParams		= true;
					break;

				case _ICODE_CASK_AUTO_PARAMS:
					// /||code||\ cask
					*tnSkipChars	= 3;
					bmpLeft			= bmpCaskAutoLeft;
					bmpRight		= bmpCaskAutoRight;
					if (tlOverrideColors)
					{
						caskColor->color	= whiteColor.color;
						textColor.color		= blackColor.color;
					}
					llAddParams		= true;
					break;

				case _ICODE_CASK_ROUND:
					// (|round|)
					*tnSkipChars	= 2;
					bmpLeft			= bmpCaskRoundLeft;
					bmpRight		= bmpCaskRoundRight;
					if (tlOverrideColors)
					{
						caskColor->color	= pastelGreenColor.color;
						textColor.color		= darkGreenColor.color;
					}
					break;

				case _ICODE_CASK_SQUARE:
					// [|square|]
					*tnSkipChars	= 2;
					bmpLeft			= bmpCaskSquareLeft;
					bmpRight		= bmpCaskSquareRight;
					if (tlOverrideColors)
					{
						caskColor->color	= pastelOrangeColor.color;
						textColor.color		= darkOrangeColor.color;
					}
					break;

				case _ICODE_CASK_TRIANGLE:
					// <|triangle|>
					*tnSkipChars	= 2;
					bmpLeft			= bmpCaskTriangleLeft;
					bmpRight		= bmpCaskTriangleRight;
					if (tlOverrideColors)
					{
						caskColor->color	= pastelYellowColor.color;
						textColor.color		= blackColor.color;
					}
					break;

				default:
					error_silent;
				case _ICODE_CASK_TILDE:
					// ~|tilde|~
					*tnSkipChars	= 2;
					bmpLeft			= bmpCaskTildeLeft;
					bmpRight		= bmpCaskTildeRight;
					if (tlOverrideColors)
					{
						caskColor->color	= pastelBlueColor.color;
						textColor.color		= whiteColor.color;
					}
					break;

				case _ICODE_CASK_CODE:
					// \|code|/
					*tnSkipChars	= 2;
					bmpLeft			= bmpCaskCodeLeft;
					bmpRight		= bmpCaskCodeRight;
					if (tlOverrideColors)
					{
						caskColor->color	= pastelRedColor.color;
						textColor.color		= whiteColor.color;
					}
					break;

				case _ICODE_CASK_AUTO:
					// /|code|\ cask
					*tnSkipChars	= 2;
					bmpLeft			= bmpCaskAutoLeft;
					bmpRight		= bmpCaskAutoRight;
					if (tlOverrideColors)
					{
						caskColor->color	= whiteColor.color;
						textColor.color		= blackColor.color;
					}
					break;
			}


			//////////
			// Create our cask at its standard size, and big enough for the sides and text
			//////
				lnWidth = bmpLeft->bi.biWidth + bmpRight->bi.biWidth + ((tnTextLength - (*tnSkipChars * 2)) * gsFontCask->tm.tmAveCharWidth);
				bmpCask	= iBmp_allocate();
				iBmp_createBySize(bmpCask, lnWidth, bmpLeft->bi.biHeight, 24);


			//////////
			// Fill everything with our mask color
			//////
				SetRect(&lrc, 0, 0, bmpCask->bi.biWidth, bmpCask->bi.biHeight);
				iBmp_fillRect(bmpCask, &lrc, maskColor, maskColor, maskColor, maskColor, false, NULL, false);


			/////////
			// Left side
			//////
				SetRect(&lrc, 0, 0, bmpLeft->bi.biWidth, bmpLeft->bi.biHeight);
				iBmp_bitBlt(bmpCask, &lrc, bmpLeft);
				if (llAddParams)
				{
					// The cask plus params
					lrc.left	= lrc.right;
					lrc.right	= lrc.left + bmpCaskExtenderLeft1->bi.biWidth;
					iBmp_bitBlt(bmpCask, &lrc, bmpCaskExtenderLeft1);

					lrc.left	= lrc.right;
					lrc.right	= lrc.left + bmpCaskExtenderMiddle->bi.biWidth;
					iBmp_bitBlt(bmpCask, &lrc, bmpCaskExtenderMiddle);

					lrc.left	= lrc.right;
					lrc.right	= lrc.left + bmpCaskExtenderLeft2->bi.biWidth;
					iBmp_bitBlt(bmpCask, &lrc, bmpCaskExtenderLeft2);

					lnLeft = bmpLeft->bi.biWidth + bmpCaskExtenderLeft1->bi.biWidth + bmpCaskExtenderMiddle->bi.biWidth + bmpCaskExtenderLeft2->bi.biWidth;

				} else {
					// Just the cask
					lnLeft = bmpLeft->bi.biWidth;
				}


			//////////
			// Fill the middle
			//////
				lnStop = bmpCask->bi.biWidth - bmpRight->bi.biWidth;
				for (lnI = lnLeft; lnI < lnStop; lnI += bmpCaskSideExtender->bi.biWidth)
				{
					SetRect(&lrc, lnI, 0, lnI + bmpCaskSideExtender->bi.biWidth, bmpCaskSideExtender->bi.biHeight);
					iBmp_bitBlt(bmpCask, &lrc, bmpCaskSideExtender);
				}


			//////////
			// Populate right side
			//////
				SetRect(&lrc, lnStop, 0, bmpCask->bi.biWidth, bmpRight->bi.biHeight);
				iBmp_bitBlt(bmpCask, &lrc, bmpRight);
				if (llAddParams)
				{
					// The cask plus params
					lrc.right	= lrc.left;
					lrc.left	= lrc.right - bmpCaskExtenderRight1->bi.biWidth;
					iBmp_bitBlt(bmpCask, &lrc, bmpCaskExtenderRight1);

					lrc.right	= lrc.left;
					lrc.left	= lrc.right - bmpCaskExtenderMiddle->bi.biWidth;
					iBmp_bitBlt(bmpCask, &lrc, bmpCaskExtenderMiddle);

					lrc.right	= lrc.left;
					lrc.left	= lrc.right - bmpCaskExtenderRight2->bi.biWidth;
					iBmp_bitBlt(bmpCask, &lrc, bmpCaskExtenderRight2);
				}


			//////////
			// Colorize the cask to the caskColor
			//////
				SetRect(&lrc, 0, 0, bmpCask->bi.biWidth, bmpCask->bi.biHeight);
				iBmp_colorizeMask(bmpCask, &lrc, *caskColor, false, 0.0f);


			//////////
			// Convert the mask color to the background color
			//////
				iBmp_swapColors(bmpCask, maskColor, backgroundColor);


			//////////
			// Scale the bitmap into its target size
			//////
				if (tnWidth == bmpCask->bi.biWidth && tnHeight == bmpCask->bi.biHeight)
					return(bmpCask);	// We already have the correct size

				// Build the scaled version
				bmpNew = iBmp_allocate();
				iBmp_createBySize(bmpNew, tnWidth, tnHeight, 24);
				iBmp_scale(bmpNew, bmpCask);


			//////////
			// Delete the cask
			//////
				iBmp_delete(&bmpCask, true, true);


			// Indicate our status
			return(bmpNew);
		}

		// If we get here, invalid
		return(NULL);
	}
#endif




//////////
//
// Called to render a non-breaking-space component, replacing each nbsp with a half-space, and presenting the
// name centered in the middle of the pixel area with the left and right sides containing a visible cue color
// so as to indicate the scope of the nbsp name.
//
//////
#if !defined(_EXCLUDE_iBmp_nbsp_createAndPopulate)
	SBitmap* iBmp_nbsp_createAndPopulate(SComp* comp, SFont* font, f32 tfMinColor, f32 tfMaxColor, s32 tnWidth, s32 tnHeight, SBgra backgroundColor, SBgra textColor)
	{
		s32			lnI, lnCount, lnDrawCount;
		RECT		lrc;
		SBgra		tempColor1, tempColor2;
		SBitmap*	bmpNbsp;


		//////////
		// Create our cask at its standard size, and big enough for the sides and text
		//////
			bmpNbsp	= iBmp_allocate();
			iBmp_createBySize(bmpNbsp, tnWidth, tnHeight, 24);


		//////////
		// Fill everything with our background color
		//////
			SetRect(&lrc, 0, 0, bmpNbsp->bi.biWidth, bmpNbsp->bi.biHeight);
			iBmp_fillRect(bmpNbsp, &lrc, backgroundColor, backgroundColor, backgroundColor, backgroundColor, false, NULL, false);


		/////////
		// Bottom gradient
		//////
			SetRect(&lrc, 0, tnHeight * 3 / 4, tnWidth, tnHeight);
			tempColor1 = iBmp_colorCombine(blueColor, whiteColor, tfMinColor);
			tempColor2 = iBmp_colorCombine(blueColor, whiteColor, tfMaxColor);
			iBmp_fillRect(bmpNbsp, &lrc, tempColor1, tempColor1, tempColor2, tempColor2, true, NULL, false);


		//////////
		// Render our text portions over the top
		//////
			SelectObject(bmpNbsp->hdc, font->hfont);
			SetBkMode(bmpNbsp->hdc, TRANSPARENT);
			SetTextColor(bmpNbsp->hdc, RGB(textColor.red, textColor.grn, textColor.blu));
			SetRect(&lrc, 0, 0, 0, tnHeight);
			for (lnI = 0, lnCount = 0; lnI < comp->length; lnI++)
			{
				// Have we reached the non-breaking-space yet?
				if (comp->line->sourceCode->data_u8[comp->start + lnI] == 255 || (lnI + 1) == comp->length)
				{
					// Adjust if we're the last character
					lnDrawCount = lnCount + (((lnI + 1) == comp->length) ? 1 : 0);

					// Adjust the right-side of our rect
					lrc.right = lrc.left + (lnDrawCount * font->tm.tmAveCharWidth);

					// This is a non-breaking-space character, we need to draw what we have, and move over a half space
					if (lnCount != 0)
						DrawText(bmpNbsp->hdc, comp->line->sourceCode->data_s8 + comp->start + lnI - lnCount, lnDrawCount, &lrc, DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_NOPREFIX);

					// Update our rectangle
					lrc.left += (lnCount + 1) * font->tm.tmAveCharWidth;

					// Reset our count
					lnCount = 0;

				} else {
					// Just increasing our count
					++lnCount;
				}
			}


		//////////
		// Round off the corners
		//////
			iBmp_roundCorners(bmpNbsp, 1, whiteColor);


		// Indicate our status
		return(bmpNbsp);
	}
#endif




//////////
//
// Called to build a common tooltip background
//
//////
#if !defined(_EXCLUDE_iBmp_colorizeAsCommonTooltipBackground)
	void iBmp_colorizeAsCommonTooltipBackground(SBitmap* bmp)
	{
		RECT lrc;


		// Make sure our environment is sane
		if (bmp)
		{
			SetRect(&lrc, 0, 0, bmp->bi.biWidth, bmp->bi.biHeight);
			iBmp_fillRect(bmp, &lrc, tooltipNwBackColor, tooltipNeBackColor, tooltipSwBackColor, tooltipSeBackColor, true, NULL, false);
			iBmp_dapple(bmp, bmpDapple1, bmpDapple1Tmp, 200.0f, 10);
			iBmp_frameRect(bmp, &lrc, tooltipForecolor, tooltipForecolor, tooltipForecolor, tooltipForecolor, false, NULL, false);
		}
	}
#endif




//////////
//
// Colorize the content from left to right using the opaque ratio
//
//////
	void iBmp_colorizeHighlightGradient(SBitmap* bmp, RECT* rc, SBgra color, f32 tfLeftOpaque, f32 tfRightOpaque)
	{
		s32		lnX, lnY;
		f32		lfStep, lfOpaque, lfMopaque, lfGray, lfRed, lfGrn, lfBlu;
		SBgr*	lbgr;
		SBgra*	lbgra;


		// Make sure the environment is sane
		if (bmp && rc->left < bmp->bi.biWidth && rc->top < bmp->bi.biHeight)
		{
			// Make sure our parameters are sane
			tfLeftOpaque	= max(min(tfLeftOpaque,  1.0f), 0.0f);
			tfRightOpaque	= max(min(tfRightOpaque, 1.0f), 0.0f);
			lfStep			= (tfLeftOpaque - tfRightOpaque) / (f32)(rc->right - rc->left);
			lfRed			= (f32)color.red;
			lfGrn			= (f32)color.grn;
			lfBlu			= (f32)color.blu;

			// Process
			if (bmp->bi.biBitCount == 24)
			{
				// Iterate through every row
				for (lnY = rc->top; lnY <= rc->bottom && lnY < bmp->bi.biHeight; lnY++)
				{
					// Build the pointer for this part of the line
					lbgr = (SBgr*)(bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth) + (rc->left * 3));

					// Iterate through every column
					for (lnX = rc->left, lfOpaque = tfLeftOpaque, lfMopaque = 1.0f - tfLeftOpaque; lnX <= rc->right && lnX < bmp->bi.biWidth; lnX++, lfOpaque += lfStep, lfMopaque -= lfStep, lbgr++)
					{
						// Get the grayscale value
						lfGray = (((f32)lbgr->red * 0.35f) + ((f32)lbgr->grn * 0.54f) + ((f32)lbgr->blu * 0.11f)) / 255.0f;

						// Apply our opaqueness
						lbgr->red = (u8)min((s32)(((f32)lbgr->red * lfMopaque) + (lfRed * lfGray * lfOpaque)), 255);
						lbgr->grn = (u8)min((s32)(((f32)lbgr->grn * lfMopaque) + (lfGrn * lfGray * lfOpaque)), 255);
						lbgr->blu = (u8)min((s32)(((f32)lbgr->blu * lfMopaque) + (lfBlu * lfGray * lfOpaque)), 255);
					}
				}

			} else if (bmp->bi.biBitCount == 32) {
				// Iterate through every row
				for (lnY = rc->top; lnY <= rc->bottom && lnY < bmp->bi.biHeight; lnY++)
				{
					// Build the pointer for this part of the line
					lbgra = (SBgra*)(bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth) + (rc->left * 4));

					// Iterate through every column
					for (lnX = rc->left, lfOpaque = tfLeftOpaque, lfMopaque = 1.0f - tfLeftOpaque; lnX <= rc->right && lnX < bmp->bi.biWidth; lnX++, lfOpaque += lfStep, lfMopaque -= lfStep, lbgra++)
					{
						// Get the grayscale value
						lfGray = (((f32)lbgra->red * 0.35f) + ((f32)lbgra->grn * 0.54f) + ((f32)lbgra->blu * 0.11f)) / 255.0f;

						// Apply our opaqueness
						lbgra->red = (u8)min((s32)(((f32)lbgra->red * lfMopaque) + (lfRed * lfGray * lfOpaque)), 255);
						lbgra->grn = (u8)min((s32)(((f32)lbgra->grn * lfMopaque) + (lfGrn * lfGray * lfOpaque)), 255);
						lbgra->blu = (u8)min((s32)(((f32)lbgra->blu * lfMopaque) + (lfBlu * lfGray * lfOpaque)), 255);
					}
				}
			}
		}
	}




//////////
//
// Called to dapple the bitmap using a template dappler
//
//////
	// bmpDappleTmp should be the same dimensions as bmpDapple
	void iBmp_dapple(SBitmap* bmp, SBitmap* bmpDapple, SBitmap* bmpDappleTmp, f32 tfBias, f32 tfInfluence)
	{
		bool		llAllocated;
		s32			lnX, lnY, lnX2, lnY2;
		f32			lfGray;
		SBgr*		lbgrd;
		SBgr*		lbgrs;
		SBgr*		lbgrt;
		SBgra*		lbgrad;
		SBgra*		lbgras;
		SBgra*		lbgrat;


		// Make sure the environment is sane
		if (bmp && bmpDapple)
		{
			//////////
			// Temporary dapple bitmap
			//////
				// Make sure we have a temporary dapple bitmap
				if (!bmpDappleTmp)
				{
					// We need to allocate a copy
					llAllocated = true;
					bmpDappleTmp = iBmp_copy(bmpDapple);

				} else {
					// We use what they provided
					llAllocated = false;
				}


			//////////
			// This algorithm is math-intensive, so we pre-compute the values and just do addition
			//////
				if (bmpDapple->bi.biBitCount == 24)
				{
					// 24-bit dapple
					for (lnY = 0; lnY < bmpDappleTmp->bi.biHeight; lnY++)
					{
						// Grab our pointer
						lbgrs = (SBgr*)(bmpDappleTmp->bd + ((bmpDappleTmp->bi.biHeight - lnY - 1) * bmpDappleTmp->rowWidth));

						// Iterate through every pixel creating the mathematical adjustment for each
						for (lnX = 0; lnX < bmpDapple->bi.biWidth; lnX++, lbgrs++)
						{
							lfGray			= (((f32)lbgrs->red * 0.35f) + ((f32)lbgrs->grn * 0.54f) + ((f32)lbgrs->blu * 0.11f));
							lbgrs->blu_s8	= (s8)min(max((tfInfluence * lfGray / tfBias), -128.0f), 128.0f);
						}
					}

				} else if (bmpDapple->bi.biBitCount == 32) {
					// 32-bit dapple
					for (lnY = 0; lnY < bmpDappleTmp->bi.biHeight; lnY++)
					{
						// Grab our pointer
						lbgras = (SBgra*)(bmpDappleTmp->bd + ((bmpDappleTmp->bi.biHeight - lnY - 1) * bmpDappleTmp->rowWidth));

						// Iterate through every pixel creating the mathematical adjustment for each
						for (lnX = 0; lnX < bmpDapple->bi.biWidth; lnX++, lbgras++)
						{
							lfGray				= (((f32)lbgras->red * 0.35f) + ((f32)lbgras->grn * 0.54f) + ((f32)lbgras->blu * 0.11f));
							lbgras->blu_s8	= (s8)min(max((tfInfluence * lfGray / tfBias), -128.0f), 128.0f);
						}
					}

				} else {
					// Unknown format
					if (llAllocated)
						iBmp_delete(&bmpDappleTmp, true, true);

					// All done
					return;
				}


			//////////
			// Apply dappling
			//////
				tfInfluence	= (f32)((s32)tfInfluence);
				if (bmp->bi.biBitCount == 24)
				{
					if (bmpDapple->bi.biBitCount == 24)
					{
						// 24-bit to 24-bit
						for (lnY = 0; lnY < bmp->bi.biHeight; lnY += bmpDapple->bi.biHeight)
						{
							for (lnX = 0; lnX < bmp->bi.biWidth; lnX += bmpDapple->bi.biWidth)
							{
								for (lnY2 = 0; lnY2 < bmpDapple->bi.biHeight && lnY + lnY2 < bmp->bi.biHeight; lnY2++)
								{
									// Grab the pointer2
									lbgrd	= (SBgr*)(bmp->bd          + ((bmp->bi.biHeight          - lnY - lnY2 - 1) * bmp->rowWidth)       + (lnX * 3));
									lbgrs	= (SBgr*)(bmpDapple->bd    + ((bmpDapple->bi.biHeight          - lnY2 - 1) * bmpDapple->rowWidth));
									lbgrt	= (SBgr*)(bmpDappleTmp->bd + ((bmpDappleTmp->bi.biHeight       - lnY2 - 1) * bmpDappleTmp->rowWidth));

									// Iterate across the dapple source
									for (lnX2 = 0; lnX2 < bmpDapple->bi.biWidth && lnX + lnX2 < bmp->bi.biWidth; lnX2++, lbgrs++, lbgrd++)
									{
										lbgrd->red	= (u8)min(((s32)lbgrd->red + lbgrt->blu), 255);
										lbgrd->grn	= (u8)min(((s32)lbgrd->grn + lbgrt->blu), 255);
										lbgrd->blu	= (u8)min(((s32)lbgrd->blu + lbgrt->blu), 255);
									}
								}
							}
						}

					} else if (bmpDapple->bi.biBitCount == 32) {
						// 32-bit to 24-bit
						for (lnY = 0; lnY < bmp->bi.biHeight; lnY += bmpDapple->bi.biHeight)
						{
							for (lnX = 0; lnX < bmp->bi.biWidth; lnX += bmpDapple->bi.biWidth)
							{
								for (lnY2 = 0; lnY2 < bmpDapple->bi.biHeight && lnY + lnY2 < bmp->bi.biHeight; lnY2++)
								{
									// Grab the pointer2
									lbgrd	= (SBgr*) (bmp->bd          + ((bmp->bi.biHeight          - lnY - lnY2 - 1) * bmp->rowWidth)          + (lnX * 3));
									lbgras	= (SBgra*)(bmpDapple->bd    + ((bmpDapple->bi.biHeight          - lnY2 - 1) * bmpDapple->rowWidth));
									lbgrat	= (SBgra*)(bmpDappleTmp->bd + ((bmpDappleTmp->bi.biHeight       - lnY2 - 1) * bmpDappleTmp->rowWidth));

									// Iterate across the dapple source
									for (lnX2 = 0; lnX2 < bmpDapple->bi.biWidth && lnX + lnX2 < bmp->bi.biWidth; lnX2++, lbgras++, lbgrd++)
									{
										lbgrd->red	= (u8)min(((s32)lbgrd->red + lbgrat->blu), 255);
										lbgrd->grn	= (u8)min(((s32)lbgrd->grn + lbgrat->blu), 255);
										lbgrd->blu	= (u8)min(((s32)lbgrd->blu + lbgrat->blu), 255);
									}
								}
							}
						}
					}

				} else if (bmp->bi.biBitCount == 32) {
					if (bmpDapple->bi.biBitCount == 24)
					{
						// 24-bit to 32-bit
						for (lnY = 0; lnY < bmp->bi.biHeight; lnY += bmpDapple->bi.biHeight)
						{
							for (lnX = 0; lnX < bmp->bi.biWidth; lnX += bmpDapple->bi.biWidth)
							{
								for (lnY2 = 0; lnY2 < bmpDapple->bi.biHeight && lnY + lnY2 < bmp->bi.biHeight; lnY2++)
								{
									// Grab the pointer2
									lbgrad	= (SBgra*)(bmp->bd          + ((bmp->bi.biHeight          - lnY - lnY2 - 1) * bmp->rowWidth)       + (lnX * 4));
									lbgrs	= (SBgr*) (bmpDapple->bd    + ((bmpDapple->bi.biHeight          - lnY2 - 1) * bmpDapple->rowWidth));
									lbgrt	= (SBgr*) (bmpDappleTmp->bd + ((bmpDappleTmp->bi.biHeight       - lnY2 - 1) * bmpDappleTmp->rowWidth));

									// Iterate across the dapple source
									for (lnX2 = 0; lnX2 < bmpDapple->bi.biWidth && lnX + lnX2 < bmp->bi.biWidth; lnX2++, lbgrs++, lbgrad++)
									{
										lbgrad->red	= (u8)min(((s32)lbgrad->red + lbgrt->blu), 255);
										lbgrad->grn	= (u8)min(((s32)lbgrad->grn + lbgrt->blu), 255);
										lbgrad->blu	= (u8)min(((s32)lbgrad->blu + lbgrt->blu), 255);
									}
								}
							}
						}

					} else if (bmpDapple->bi.biBitCount == 32) {
						// 32-bit to 32-bit
						for (lnY = 0; lnY < bmp->bi.biHeight; lnY += bmpDapple->bi.biHeight)
						{
							for (lnX = 0; lnX < bmp->bi.biWidth; lnX += bmpDapple->bi.biWidth)
							{
								for (lnY2 = 0; lnY2 < bmpDapple->bi.biHeight && lnY + lnY2 < bmp->bi.biHeight; lnY2++)
								{
									// Grab the pointer2
									lbgrad	= (SBgra*)(bmp->bd          + ((bmp->bi.biHeight          - lnY - lnY2 - 1) * bmp->rowWidth)       + (lnX * 4));
									lbgras	= (SBgra*)(bmpDapple->bd    + ((bmpDapple->bi.biHeight          - lnY2 - 1) * bmpDapple->rowWidth));
									lbgrat	= (SBgra*)(bmpDappleTmp->bd + ((bmpDappleTmp->bi.biHeight       - lnY2 - 1) * bmpDappleTmp->rowWidth));

									// Iterate across the dapple source
									for (lnX2 = 0; lnX2 < bmpDapple->bi.biWidth && lnX + lnX2 < bmp->bi.biWidth; lnX2++, lbgras++, lbgrad++)
									{
										lbgrad->red	= (u8)min(((s32)lbgrad->red + lbgrat->blu), 255);
										lbgrad->grn	= (u8)min(((s32)lbgrad->grn + lbgrat->blu), 255);
										lbgrad->blu	= (u8)min(((s32)lbgrad->blu + lbgrat->blu), 255);
									}
								}
							}
						}
					}
				}


			//////////
			// Clean up
			//////
				if (llAllocated)
					iBmp_delete(&bmpDappleTmp, true, true);

		}
	}




//////////
//
// Called to draw the indicated text in a TrueType font
//
//////
	s32 iBmp_drawFontTT(SBitmap* bmp, s8* tcFontName, s32 tnPointSize, bool tlBold, bool tlItalic, bool tlUnderline, bool tlStrikethru, s8* tcText, s32 tnX, s32 tnY, SBgra color)
	{
		s32			lnSize, lnLength;
		RECT		lrc = { };
		HFONT		hfont, hfontOld;

		// We use this as a return value, encoded with the width and height of the rectange rendered for the text
		union
		{
			POINTS		pt;
			s32			lnCoords;
		};


		// Create the font
		lnSize	= -MulDiv(tnPointSize, GetDeviceCaps(GetDC(GetDesktopWindow()), LOGPIXELSY), 72);
		hfont	= CreateFont(	lnSize, 0, 0, 0,
								((tlBold)		? FW_BOLD	: FW_NORMAL),
								((tlItalic)		? 1			: 0),
								((tlUnderline)	? 1			: 0),
								((tlStrikethru)	? 1			: 0),
								ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								CLEARTYPE_NATURAL_QUALITY, FF_DONTCARE,
								tcFontName	);

		// Calculate the rectangle
		hfontOld = (HFONT)SelectObject(bmp->hdc, hfont);
		lnLength = (s32)strlen(tcText);
		SetRect(&lrc, 0, 0, 0, 0);
		DrawText(bmp->hdc, tcText, lnLength, &lrc, DT_SINGLELINE | DT_CALCRECT);

		// Store it encoded in our return variable
		pt.x = (s16)(lrc.right - lrc.left);
		pt.y = (s16)(lrc.bottom - lrc.top);

		// Setup for the render
		SetBkMode(bmp->hdc, TRANSPARENT);
		SetTextColor(bmp->hdc, RGB(color.red, color.grn, color.blu));
		OffsetRect(&lrc, tnX, tnY);
		DrawText(bmp->hdc, tcText, lnLength, &lrc, DT_SINGLELINE);

		// Clean up
		SelectObject(bmp->hdc, hfontOld);
		DeleteObject((HGDIOBJ)hfont);

		// Signify
		return lnCoords;
	}




//////////
//
// Draws a single character at the indicated offset in a random font
//
//////
	void iiBmp_drawFontBitmap(SBitmap* bmp, s32 fontX, s32 fontY, s8* tcText, s32 tnTextLength, RECT* rc, SBgra foreColor, SBgra backColor)
	{
		s32		lnI;


		// Iterate for every character
		for (lnI = 0; lnI < tnTextLength; ++lnI, OffsetRect(rc, fontX, 0))
			iBmp_drawFontBitmap_char(bmp, fontX, fontY, tcText[lnI], rc, foreColor, backColor);

	}

	void iBmp_drawFontBitmap_char(SBitmap* bmp, s32 fontX, s32 fontY, u8 c, RECT* rc, SBgra foreColor, SBgra backColor)
	{
		u8			lcC, lnBit;
		s32			lnI, lnY, lnX, lnPixelRows;
		u8*			fontBase;
		SBgr*		lbgr;


		// What font size are we using?
		if (fontX == 8)
		{
			switch (fontY)
			{
				case 6:		{	fontBase = (u8*)&gxFontBase_8x6[(u32)c * 6];					lnPixelRows = 1;	break;	};
				case 8:		{	fontBase = (u8*)&gxFontBase_8x8[(u32)c * 8];					lnPixelRows = 1;	break;	};
				case 12:	{	fontBase = (u8*)&gxFontBase_8x6[(u32)c * 6];		fontY = 6;	lnPixelRows = 2;	break;	};
				case 14:	{	fontBase = (u8*)&gxFontBase_8x14[(u32)c * 14];					lnPixelRows = 1;	break;	};
				case 16:	{	fontBase = (u8*)&gxFontBase_8x16[(u32)c * 16];					lnPixelRows = 1;	break;	};
					break;

				default:
					// Error
					return;
			}

			// Draw each pixel
			for (lnY = 0; lnY < fontY; ++lnY, ++fontBase)
			{
				// Grab the start of this row
				for (lnI = 0; lnI < 2; ++lnI)
				{
					lbgr = (SBgr*)(bmp->bd + ((bmp->bi.biHeight - ((lnY * lnPixelRows) + lnI + 1) - rc->top - 1) * bmp->rowWidth) + (rc->left * 3));

					// Iterate across this pixel bit by bit
					lcC = *fontBase;
					for (lnX = 0, lnBit = 0x80; lnBit != 0; ++lnX, ++lbgr, lnBit >>= 1)
					{
						// Are we within our rectangle
						// Is this bit on or off?
						if (lcC & lnBit)
						{
							// It's a foreground pixel
							lbgr->red = foreColor.red;
							lbgr->grn = foreColor.grn;
							lbgr->blu = foreColor.blu;

						} else {
							// It's a background pixel
							lbgr->red = backColor.red;
							lbgr->grn = backColor.grn;
							lbgr->blu = backColor.blu;
						}
					}
				}
			}


		} else if (fontX == 16) {
			if (fontY == 32)
				iBmp_drawFontBitmap_char_16x32(bmp, c, rc, foreColor, backColor);
		}
	}




//////////
//
// Draws a single character at the indicated offset in a 16x32 bitmap font
//
//////
	void iBmp_drawFontBitmap_char_16x32(SBitmap* bmp, u8 c, RECT* rc, SBgra foreColor, SBgra backColor)
	{
		u16			lcC, lnBit;
		s32			lnY, lnX;
		u16*		fontBase;
		SBgr*		lbgr;


		// Get our starting point into the bitmap array
		fontBase  = (u16*)&gxFontBase_16x32[0];
		fontBase += (u32)c * 32;

		// Draw each pixel
		for (lnY = 0; lnY < 32; ++lnY, ++fontBase)
		{
			// Grab the start of this row
			lbgr = (SBgr*)(bmp->bd + ((bmp->bi.biHeight - lnY - rc->top - 1) * bmp->rowWidth) + (rc->left * 3));

			// Iterate across this pixel bit by bit
			lcC = *fontBase;
			for (lnX = 0, lnBit = 0x8000; lnBit != 0; ++lnX, ++lbgr, lnBit >>= 1)
			{
				// Is this bit on or off?
				if (lcC & lnBit)
				{
					// It's a foreground pixel
					lbgr->red = foreColor.red;
					lbgr->grn = foreColor.grn;
					lbgr->blu = foreColor.blu;

				} else {
					// It's a background pixel
					lbgr->red = backColor.red;
					lbgr->grn = backColor.grn;
					lbgr->blu = backColor.blu;
				}
			}
		}
	}




//////////
//
// Called to draw a point
//
//////
	void iBmp_drawPoint(SBitmap* bmp, s32 tnX, s32 tnY, SBgra color, f32 tfAlpha)
	{
		f32		lfAlp, lfMalp;
		SBgr*	lbgr;
		SBgra*	lbgra;


		if (bmp)
		{
			// Grab our factors
			lfAlp  = tfAlpha;
			lfMalp = 1.0f - lfAlp;

			// Make sure our coordinates are valid
			if (tnX >= 0 && tnX < bmp->bi.biWidth && tnY >= 0 && tnY < bmp->bi.biHeight)
			{
				if (bmp->bi.biBitCount == 24)
				{
					// Get our offset
					lbgr = (SBgr*)(bmp->bd + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX * 3));

					// Draw it
					lbgr->red = (u8)(((f32)color.red * lfAlp) + ((f32)lbgr->red * lfMalp));
					lbgr->grn = (u8)(((f32)color.grn * lfAlp) + ((f32)lbgr->grn * lfMalp));
					lbgr->blu = (u8)(((f32)color.blu * lfAlp) + ((f32)lbgr->blu * lfMalp));

				} else if (bmp->bi.biBitCount == 32) {
					// Get our offset
					lbgra = (SBgra*)(bmp->bd + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX * 4));

					// Draw it
					lbgra->red = (u8)(((f32)color.red * lfAlp) + ((f32)lbgra->red * lfMalp));
					lbgra->grn = (u8)(((f32)color.grn * lfAlp) + ((f32)lbgra->grn * lfMalp));
					lbgra->blu = (u8)(((f32)color.blu * lfAlp) + ((f32)lbgra->blu * lfMalp));
				}
			}
		}
	}

	void iBmp_drawBullet(SBitmap* bmp, s32 tnX, s32 tnY, SBgra color, f32 tfAlpha)
	{
		iBmp_drawPoint(bmp, tnX-1,	tnY-1,	color,	tfAlpha);		// 1
		iBmp_drawPoint(bmp, tnX,	tnY-1,	color,	tfAlpha);		// 2
		iBmp_drawPoint(bmp, tnX+1,	tnY-1,	color,	tfAlpha);		// 3

		iBmp_drawPoint(bmp, tnX,	tnY-2,	color,	tfAlpha);		// 4      Draws one of these using 11 points:
		iBmp_drawPoint(bmp, tnX,	tnY-1,	color,	tfAlpha);		// 5                      123
		iBmp_drawPoint(bmp, tnX,	tnY,	color,	tfAlpha);		// 6                     45678
		iBmp_drawPoint(bmp, tnX,	tnY+1,	color,	tfAlpha);		// 7                      901
		iBmp_drawPoint(bmp, tnX,	tnY+2,	color,	tfAlpha);		// 8

		iBmp_drawPoint(bmp, tnX-1,	tnY+1,	color,	tfAlpha);		// 9
		iBmp_drawPoint(bmp, tnX,	tnY+1,	color,	tfAlpha);		// 10
		iBmp_drawPoint(bmp, tnX+1,	tnY+1,	color,	tfAlpha);		// 11
	}

	void iBmp_fillRect(SBitmap* bmp, RECT* rc, SBgra colorNW, SBgra colorNE, SBgra colorSW, SBgra colorSE, bool tlUseGradient, RECT* rcClip, bool tluseClip)
	{
		s32		lnY;
		f32		lfRed, lfGrn, lfBlu, lfRedTo, lfGrnTo, lfBluTo, lfRedInc, lfGrnInc, lfBluInc, lfPercent, lfPercentInc, lfHeight, lfWidth;


		if (bmp && rc)
		{
			// Is it a solid color?
#ifdef _MSC_VER
			if (!tlUseGradient && !tluseClip)
			{
				// Use OS facilities for speedup on solid colors
				HBRUSH hbr = CreateSolidBrush(RGB(colorNW.red, colorNW.grn, colorNW.blu));
				FillRect(bmp->hdc, rc, hbr);
				DeleteObject((HGDIOBJ)hbr);
				return;
			}
#endif
			// Process manually with the gradient
			lfWidth			= (f32)(rc->right  - 1 - rc->left);
			lfHeight		= (f32)(rc->bottom - 1 - rc->top);
			lfPercentInc	= 1.0f / lfHeight;
			for (lfPercent = 0.0, lnY = rc->top; lnY < rc->bottom; lnY++, lfPercent += lfPercentInc)
			{
				if (tlUseGradient)
				{
					//////////
					// Compute FROM colors
					//////
						lfRed		= (f32)colorNW.red + (((f32)colorSW.red - (f32)colorNW.red) * lfPercent);
						lfGrn		= (f32)colorNW.grn + (((f32)colorSW.grn - (f32)colorNW.grn) * lfPercent);
						lfBlu		= (f32)colorNW.blu + (((f32)colorSW.blu - (f32)colorNW.blu) * lfPercent);


					//////////
					// Compute TO colors
					//////
						lfRedTo		= (f32)colorNE.red + (((f32)colorSE.red - (f32)colorNE.red) * lfPercent);
						lfGrnTo		= (f32)colorNE.grn + (((f32)colorSE.grn - (f32)colorNE.grn) * lfPercent);
						lfBluTo		= (f32)colorNE.blu + (((f32)colorSE.blu - (f32)colorNE.blu) * lfPercent);


					//////////
					// Compute increment
					//////
						lfRedInc	= (lfRedTo - lfRed) / lfWidth;
						lfGrnInc	= (lfGrnTo - lfGrn) / lfWidth;
						lfBluInc	= (lfBluTo - lfBlu) / lfWidth;


					//////////
					// Draw this line with its gradient
					//////
						iBmp_drawHorizontalLineGradient(bmp, rc->left, rc->right - 1, lnY, lfRed, lfGrn, lfBlu, lfRedInc, lfGrnInc, lfBluInc, rcClip, tluseClip);

				} else {
					// Draw this line with the NW color
					iBmp_drawHorizontalLine(bmp, rc->left, rc->right - 1, lnY, colorNW);
				}
			}
		}
	}

	// Uses color as the key for AND and XOR ops
	void iBmp_fillRect_op(SBitmap* bmp, RECT* rc, SBgra colorOn, SBgra colorOff, u32 tnOp)
	{
		s32		lnY, lnX;
		SBgr*	lbgr;
		SBgra*	lbgra;


		// Make sure our environment is sane
		if (bmp && rc)
		{
			//////////
			// Validate our op
			//////
				switch (tnOp)
				{
					case _FILL_RECT_OP_AND:
					case _FILL_RECT_OP_OR:
					case _FILL_RECT_OP_XOR:
						break;

					default:
						return;
				}


			//////////
			// Process vertically
			//////
				for (lnY = rc->top; lnY < rc->bottom; lnY++)
				{
					// Is it inside the bitmap vertically?
					if (lnY >= 0 && lnY <= bmp->bi.biHeight)
					{
						switch (bmp->bi.biBitCount)
						{
							case 24:
								// Iterate across each pixel on this row
								lbgr = (SBgr*) (bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth) + (rc->left * 3));
								for (lnX = rc->left; lnX <= rc->right; lnX++, lbgr++)
								{
									// Is it inside the bitmap horizontally?
									if (lnX >= 0 && lnX < bmp->bi.biWidth)
									{
										// Which op is it?
										switch (tnOp)
										{
											case _FILL_RECT_OP_AND:
												// Maintains the point only if the existing point is already the "ON" color
												if (lbgr->blu == colorOn.blu && lbgr->grn == colorOn.grn && lbgr->red == colorOn.red)
												{
													// It's already the color it needs to be

												} else {
													// We need to set it to the off color
													lbgr->red = colorOff.red;
													lbgr->grn = colorOff.grn;
													lbgr->blu = colorOff.blu;
												}
												break;

											case _FILL_RECT_OP_OR:
												// Always draw
												lbgr->red = colorOn.red;
												lbgr->grn = colorOn.grn;
												lbgr->blu = colorOn.blu;
												break;

											case _FILL_RECT_OP_XOR:
												// If either one of them is different, turn it on, otherwise, turn it off
												if (lbgr->blu != colorOn.blu || lbgr->grn != colorOn.grn || lbgr->red != colorOn.red)
												{
													// At least one is not the same as the on color, so turn it on
													lbgr->red = colorOn.red;
													lbgr->grn = colorOn.grn;
													lbgr->blu = colorOn.blu;

												} else {
													// It needs to be off
													lbgr->red = colorOff.red;
													lbgr->grn = colorOff.grn;
													lbgr->blu = colorOff.blu;
												}
												break;
										}
									}
								}
								break;

							case 32:
								// Iterate across each pixel on this row
								lbgra = (SBgra*)(bmp->bd + ((bmp->bi.biHeight - lnY - 1) * bmp->rowWidth) + (rc->left * 4));
								for (lnX = rc->left; lnX <= rc->right; lnX++, lbgra++)
								{
									// Is it inside the bitmap horizontally?
									if (lnX >= 0 && lnX < bmp->bi.biWidth)
									{
										// Which op is it?
										switch (tnOp)
										{
											case _FILL_RECT_OP_AND:
												// Maintains the point only if the existing point is already the "ON" color
												if (lbgra->blu == colorOn.blu && lbgra->grn == colorOn.grn && lbgra->red == colorOn.red)
												{
													// It's already the color it needs to be

												} else {
													// We need to set it to the off color
													lbgra->color = colorOff.color;
												}
												break;

											case _FILL_RECT_OP_OR:
												// Always draw
												lbgra->color = colorOn.color;
												break;

											case _FILL_RECT_OP_XOR:
												// If either one of them is different, turn it on, otherwise, turn it off
												if (lbgra->blu != colorOn.blu || lbgra->grn != colorOn.grn || lbgra->red != colorOn.red)
												{
													// At least one is not the same as the on color, so turn it on
													lbgra->color = colorOn.color;

												} else {
													// It needs to be off
													lbgra->color = colorOff.color;
												}
												break;
										}
									}
								}
								break;
						}
					}
				}

		}
	}

	void iBmp_frameRect(SBitmap* bmp, RECT* rc, SBgra colorNW, SBgra colorNE, SBgra colorSW, SBgra colorSE, bool tlUseGradient, RECT* rcClip, bool tluseClip)
	{
		f32		lfRed, lfGrn, lfBlu, lfRedTo, lfGrnTo, lfBluTo, lfRedInc, lfGrnInc, lfBluInc, lfWidth;
		HBRUSH	hbr;


		if (bmp && rc)
		{
			if (tlUseGradient)
			{
				// Compute standards
				lfWidth		= (f32)(rc->right  - 1 - rc->left);
//				lfHeight	= (f32)(rc->bottom - 1 - rc->top);

				//////////
				// Top (NW to NE)
				//////
					// Compute FROM and TO colors
					lfRed = (f32)colorNW.red;			lfRedTo = (f32)colorNE.red;
					lfGrn = (f32)colorNW.grn;			lfGrnTo = (f32)colorNE.grn;
					lfBlu = (f32)colorNW.blu;			lfBluTo = (f32)colorNE.blu;

					// Compute increment
					lfRedInc	= (lfRedTo - lfRed) / lfWidth;
					lfGrnInc	= (lfGrnTo - lfGrn) / lfWidth;
					lfBluInc	= (lfBluTo - lfBlu) / lfWidth;

					// Draw it
					iBmp_drawHorizontalLineGradient(bmp, rc->left, rc->right - 1, rc->top, lfRed, lfGrn, lfBlu, lfRedInc, lfGrnInc, lfBluInc, rcClip, true);


				//////////
				// Bottom (SW to SE)
				//////
					// Compute FROM and TO colors
					lfRed = (f32)colorSW.red;			lfRedTo = (f32)colorSE.red;
					lfGrn = (f32)colorSW.grn;			lfGrnTo = (f32)colorSE.grn;
					lfBlu = (f32)colorSW.blu;			lfBluTo = (f32)colorSE.blu;

					// Compute increment
					lfRedInc	= (lfRedTo - lfRed) / lfWidth;
					lfGrnInc	= (lfGrnTo - lfGrn) / lfWidth;
					lfBluInc	= (lfBluTo - lfBlu) / lfWidth;

					// Draw it
					iBmp_drawHorizontalLineGradient(bmp, rc->left, rc->right - 1, rc->bottom - 1, lfRed, lfGrn, lfBlu, lfRedInc, lfGrnInc, lfBluInc, rcClip, true);


				//////////
				// Left (NW to SW)
				//////
					// Compute FROM and TO colors
					lfRed = (f32)colorNW.red;			lfRedTo = (f32)colorSW.red;
					lfGrn = (f32)colorNW.grn;			lfGrnTo = (f32)colorSW.grn;
					lfBlu = (f32)colorNW.blu;			lfBluTo = (f32)colorSW.blu;

					// Compute increment
					lfRedInc	= (lfRedTo - lfRed) / lfWidth;
					lfGrnInc	= (lfGrnTo - lfGrn) / lfWidth;
					lfBluInc	= (lfBluTo - lfBlu) / lfWidth;

					// Draw it
					iBmp_drawVerticalLineGradient(bmp, rc->top, rc->bottom - 1, rc->left, lfRed, lfGrn, lfBlu, lfRedInc, lfGrnInc, lfBluInc, rcClip, true);


				//////////
				// Right (NE to SE)
				//////
					// Compute FROM and TO colors
					lfRed = (f32)colorNE.red;			lfRedTo = (f32)colorSE.red;
					lfGrn = (f32)colorNE.grn;			lfGrnTo = (f32)colorSE.grn;
					lfBlu = (f32)colorNE.blu;			lfBluTo = (f32)colorSE.blu;

					// Compute increment
					lfRedInc	= (lfRedTo - lfRed) / lfWidth;
					lfGrnInc	= (lfGrnTo - lfGrn) / lfWidth;
					lfBluInc	= (lfBluTo - lfBlu) / lfWidth;

					// Draw it
					iBmp_drawVerticalLineGradient(bmp, rc->top, rc->bottom - 1, rc->right - 1, lfRed, lfGrn, lfBlu, lfRedInc, lfGrnInc, lfBluInc, rcClip, true);

			} else {
				// Use OS services for faster rendering
				hbr = CreateSolidBrush(RGB(colorNW.red, colorNW.grn, colorNW.blu));
				FrameRect(bmp->hdc, rc, hbr);
				DeleteObject((HGDIOBJ)hbr);

// 				// Just draw in a solid color
// 				iBmp_drawHorizontalLine(bmp, rc->left, rc->right - 1, rc->top, colorNW);
// 				iBmp_drawHorizontalLine(bmp, rc->left, rc->right - 1, rc->bottom - 1, colorNW);
// 				iBmp_drawVerticalLine(bmp, rc->top, rc->bottom - 1, rc->left, colorNW);
// 				iBmp_drawVerticalLine(bmp, rc->top, rc->bottom - 1, rc->right - 1, colorNW);
			}
		}
	}

	// Like iBmp_fillRect(), except only colorizes
	void iBmp_colorizeRect(SBitmap* bmp, RECT* rc, SBgra colorNW, SBgra colorNE, SBgra colorSW, SBgra colorSE, bool tlUseGradient, RECT* rcClip, bool tluseClip, f32 alpha)
	{
		s32		lnY;
		f32		lfRed, lfGrn, lfBlu, lfRedTo, lfGrnTo, lfBluTo, lfRedInc, lfGrnInc, lfBluInc, lfPercent, lfPercentInc, lfHeight, lfWidth;


		if (bmp && rc)
		{

		//////////
		// Fill every row
		//////
			lfWidth			= (f32)(rc->right  - 1 - rc->left);
			lfHeight		= (f32)(rc->bottom - 1 - rc->top);
			lfPercentInc	= 1.0f / lfHeight;
			for (lfPercent = 0.0, lnY = rc->top; lnY <= rc->bottom; ++lnY, lfPercent += lfPercentInc)
			{
				if (tlUseGradient)
				{
					//////////
					// Compute FROM colors
					//////
						lfRed		= (f32)colorNW.red + (((f32)colorSW.red - (f32)colorNW.red) * lfPercent);
						lfGrn		= (f32)colorNW.grn + (((f32)colorSW.grn - (f32)colorNW.grn) * lfPercent);
						lfBlu		= (f32)colorNW.blu + (((f32)colorSW.blu - (f32)colorNW.blu) * lfPercent);


					//////////
					// Compute TO colors
					//////
						lfRedTo		= (f32)colorNE.red + (((f32)colorSE.red - (f32)colorNE.red) * lfPercent);
						lfGrnTo		= (f32)colorNE.grn + (((f32)colorSE.grn - (f32)colorNE.grn) * lfPercent);
						lfBluTo		= (f32)colorNE.blu + (((f32)colorSE.blu - (f32)colorNE.blu) * lfPercent);


					//////////
					// Compute increment
					//////
						lfRedInc	= (lfRedTo - lfRed) / lfWidth;
						lfGrnInc	= (lfGrnTo - lfGrn) / lfWidth;
						lfBluInc	= (lfBluTo - lfBlu) / lfWidth;


					//////////
					// Draw this line with its gradient
					//////
						iBmp_colorizeHorizontalLineGradient(bmp, rc->left, rc->right - 1, lnY, lfRed, lfGrn, lfBlu, lfRedInc, lfGrnInc, lfBluInc, rcClip, tluseClip, alpha);

				} else {
					// Draw this line with the NW color
					iBmp_colorizeHorizontalLine(bmp, rc->left, rc->right - 1, lnY, colorNW, alpha);
				}
			}
		}
	}




//////////
//
// Frame the indicated rectangle using the nine parts of an image:
//
//		 _______________________
//		|       |       |       |
//		|   1   |   2   |   3   |
//		|_______|_______|_______|
//		|       |       |       |
//		|   4   |   5   |   6   |
//		|_______|_______|_______|
//		|       |       |       |
//		|   7   |   8   |   9   |
//		|_______|_______|_______|
//
// Parts 1, 3, 7, and 9, are rendered at the corners.
// Parts 2 and 8 are repeated across the top and bottom.
// Parts 4 and 6 are repeated down the sides.
// The remainder of the image is colorized with the pixel color at the center of 5.
//
//////
	void iiBmp_frameInNineParts(SBitmap* bmpDst, RECT* trc, SBitmap* bmpFrame)
	{
		s32		lnX, lnY, lnWidth, lnHeight, lnDstRight, lnDstBottom;
		u8		lcRed, lcGrn, lcBlu;
		SBgr*	lbgr;


		//////////
		// Determine the coordinates for each part
		//////
			lnWidth		= max(bmpFrame->bi.biWidth  / 3, 1);
			lnHeight	= max(bmpFrame->bi.biHeight / 3, 1);


		//////////
		// Overlay the corners
		//////
			// Top
			iiBmp_bitBltPortion(bmpDst, trc->left,				trc->top, lnWidth, lnHeight, bmpFrame, 0,								0);
			iiBmp_bitBltPortion(bmpDst, trc->right - lnWidth,	trc->top, lnWidth, lnHeight, bmpFrame, bmpFrame->bi.biWidth - lnWidth,	0);
			// Bottom
			iiBmp_bitBltPortion(bmpDst, trc->left,				trc->bottom - lnHeight, lnWidth, lnHeight, bmpFrame, 0,									bmpFrame->bi.biHeight - lnHeight);
			iiBmp_bitBltPortion(bmpDst, trc->right - lnWidth,	trc->bottom - lnHeight, lnWidth, lnHeight, bmpFrame, bmpFrame->bi.biWidth - lnWidth,	bmpFrame->bi.biHeight - lnHeight);


		//////////
		// Repeatedly overlay the middle and sides
		//////
			lnDstRight	= trc->right - trc->left - lnWidth;
			lnDstBottom = trc->bottom - trc->top - lnHeight;
			for (lnX = lnWidth; lnX < lnDstRight; lnX++)
			{
				// Middle top and bottom
				iiBmp_bitBltPortion(bmpDst, trc->left + lnX, trc->top,					1, lnHeight, bmpFrame, lnWidth, 0);
				iiBmp_bitBltPortion(bmpDst, trc->left + lnX, trc->bottom - lnHeight,	1, lnHeight, bmpFrame, lnWidth,	bmpFrame->bi.biHeight - lnHeight);
			}
			for (lnY = lnHeight; lnY < lnDstBottom; lnY++)
			{
				// Sides
				iiBmp_bitBltPortion(bmpDst, trc->left,				trc->top + lnY,	lnWidth, 1, bmpFrame, 0,								lnHeight);
				iiBmp_bitBltPortion(bmpDst, trc->right - lnWidth,	trc->top + lnY,	lnWidth, 1, bmpFrame, bmpFrame->bi.biWidth - lnWidth,	lnHeight);
			}


		//////////
		// Fill the middle completely with the middle-most pixel color
		//////
			lbgr = (SBgr*)(bmpFrame->bd + (((bmpFrame->bi.biHeight / 2) - 1) * bmpFrame->rowWidth) + (((bmpFrame->bi.biWidth / 2) - 1) * 3));
			lcRed	= lbgr->red;
			lcGrn	= lbgr->grn;
			lcBlu	= lbgr->blu;

			// Iterate for the inner portion
			for (lnY = lnHeight; lnY < lnDstBottom; lnY++)
			{
				if (lnY + trc->top + lnHeight >= 0 && lnY + trc->top + lnHeight < bmpDst->bi.biHeight)
				{
					lbgr = (SBgr*)(bmpDst->bd + ((bmpDst->bi.biHeight - trc->top - lnY - 1) * bmpDst->rowWidth) + ((trc->left + lnWidth) * 3));
					for (lnX = lnWidth; lnX < lnDstRight; lnX++, lbgr++)
					{
						if (lnX + trc->left + lnWidth >= 0 && lnX + trc->left + lnWidth < bmpDst->bi.biWidth)
						{
							// Populate this entry
							lbgr->red = lcRed;
							lbgr->grn = lcGrn;
							lbgr->blu = lcBlu;
						}
					}
				}
			}
	}




//////////
//
// Called to bitBlt a portion of a bitmap from source to destination
//
//////
	void iiBmp_bitBltPortion(SBitmap* bmpDst, s32 tnX, s32 tnY, s32 tnWidth, s32 tnHeight, SBitmap* bmpSrc, s32 tnXStart, s32 tnYStart)
	{
		s32			lnY, lnX;
		SBgr*		lbgrd;
		SBgr*		lbgrs;


		// Is there some work to do?
		if (tnX < bmpDst->bi.biWidth && tnXStart + bmpSrc->bi.biWidth >= 0 && tnY < bmpDst->bi.biHeight && tnYStart + bmpSrc->bi.biHeight >= 0)
		{
			// Draw the item
			for (lnY = 0; tnYStart + lnY < bmpSrc->bi.biHeight && lnY < tnHeight; lnY++)
			{
				if (lnY + tnY >= 0 && lnY + tnYStart >= 0 && lnY + tnY < bmpDst->bi.biHeight && lnY + tnYStart < bmpSrc->bi.biHeight)
				{
					// Compute the destination and source
					lbgrd = (SBgr*)(bmpDst->bd + ((bmpDst->bi.biHeight - tnY      - lnY - 1) * bmpDst->rowWidth) + (tnX      * 3));
					lbgrs = (SBgr*)(bmpSrc->bd + ((bmpSrc->bi.biHeight - tnYStart - lnY - 1) * bmpSrc->rowWidth) + (tnXStart * 3));

					// Repeat for every pixel horizontally
					for (lnX = 0; tnXStart + lnX < bmpSrc->bi.biWidth && lnX < tnWidth; lnX++)
					{
						// Will it fit?
						if (lnX + tnX >= 0 && lnX + tnXStart >= 0 && lnX + tnX < bmpDst->bi.biWidth && lnX + tnXStart < bmpSrc->bi.biWidth)
						{
							// Copy it
							lbgrd->red	= lbgrs->red;
							lbgrd->grn	= lbgrs->grn;
							lbgrd->blu	= lbgrs->blu;
						}

						// Move to next pixel
						++lbgrd;
						++lbgrs;
					}
				}
			}
		}
	}


	void iBmp_drawArbitraryLine(SBitmap* bmp, s32 tnX1, s32 tnY1, s32 tnX2, s32 tnY2, SBgra color)
	{
		f32 lfX, lfY, lfXStep, lfYStep, lfRadius, lfDeltaX, lfDeltaY;


		// Compute the distance
		lfDeltaX	= (f32)(tnX2 - tnX1);
		lfDeltaY	= (f32)(tnY2 - tnY1);
		lfRadius	= (f32)sqrt((lfDeltaX*lfDeltaX) + (lfDeltaY*lfDeltaY));
		if (lfRadius < 1.0f)
			return;

		// Compute our steps per pixel
		lfXStep		= lfDeltaX / lfRadius;
		lfYStep		= lfDeltaY / lfRadius;

		// Iterate for each point
		for (lfX = (f32)tnX1, lfY = (f32)tnY1; lfRadius > 0.0f; lfRadius--, lfX += lfXStep, lfY += lfYStep)
			iBmp_drawPoint(bmp, (s32)lfX, (s32)lfY, color);

	}

	void iBmp_drawColorizedLineGradient(SBitmap* bmp, s32 tnX1, s32 tnY1, s32 tnX2, s32 tnY2, SBgra colorBeg, SBgra colorEnd, f32 tfAlpha)
	{
		f32		lfX, lfY, lfXStep, lfYStep, lfSteps, lfDeltaX, lfDeltaY;
		f32		lfRed, lfGrn, lfBlu, lfRedInc, lfGrnInc, lfBluInc;
		SBgra	color;


		// Compute the distance
		lfDeltaX = (f32)(tnX2 - tnX1);
		lfDeltaY = (f32)(tnY2 - tnY1);
		lfSteps  = (f32)sqrt((lfDeltaX*lfDeltaX) + (lfDeltaY*lfDeltaY));
		if (lfSteps < 1.0f)
			return;

		// Compute our steps per pixel
		lfXStep = lfDeltaX / lfSteps;
		lfYStep = lfDeltaY / lfSteps;

		// Compute our color changes
		lfRed    = (f32)colorBeg.red;		lfRedInc = (f32)(colorEnd.red - colorBeg.red) / lfSteps;
		lfGrn    = (f32)colorBeg.grn;		lfGrnInc = (f32)(colorEnd.grn - colorBeg.grn) / lfSteps;
		lfBlu    = (f32)colorBeg.blu;		lfBluInc = (f32)(colorEnd.blu - colorBeg.blu) / lfSteps;

		// Iterate for each point
		for (lfX = (f32)tnX1, lfY = (f32)tnY1; lfSteps > 0.0f; lfSteps--, lfX += lfXStep, lfY += lfYStep)
		{
			// Grab the color for this stage
			color.red = (u8)lfRed;
			color.grn = (u8)lfGrn;
			color.blu = (u8)lfBlu;

			// Render this point
			iBmp_drawPoint(bmp, (s32)lfX, (s32)lfY, color, tfAlpha);

			// Color increment
			lfRed += lfRedInc;
			lfGrn += lfGrnInc;
			lfBlu += lfBluInc;
		}

	}

	#define _PI2 1.570796327

	struct SXYS32
	{
		s32			x;
		s32			y;
	};

	struct SXYF32
	{
		f32			x;
		f32			y;
	};

	struct SLineF32
	{
		union {
			SXYF32	start;						// [input] Starting point
			SXYF32	p1;
		};

		union {
			SXYF32	end;						// [input] Ending point
			SXYF32	p2;
		};

		// The following are computed with vvm_math_computeLine()
		SXYF32		delta;						// Delta between start and end
		SXYF32		mid;						// Midpoint
		union {
			f32		length;						// Length
			f32		radius;						// Radius for the trig function
		};
		f32			m;							// Slope
		f32			mp;							// Perpendicular slope

		// Only used and computed if trig is true
		bool		trig;						// Should trig functions be computed?
		f32			theta;						// Theta (from p1 to p2, note: add _PI to reverse the angle from p2 to p1)

		// Only used and computed if ints is true
		bool		ints;						// Should integer approximations be calculated?
		union {
			SXYS32	starti;						// Starting x,y
			SXYS32	p1i;
		};
		union {
			SXYS32	endi;						// Ending x,y
			SXYS32	p2i;
		};

		// Only used and computed if quads is true
		bool		quads;						// Should we compute quadrants?
		union {
			s32		start_quad;					// What quadrant is start/p1 in?
			s32		p1_quad;
		};
		union {
			s32		end_quad;					// What quadrant is end/p2 in?
			s32		p2_quad;
		};
	};

	s32 iivvm_math_computeQuad(SXYF32* p)
	{
		if (p->x >= 0.0)
		{
			// Either 1 or 4
			if (p->y >= 0.0)		return(1);		// X is positive, Y is positive
			else					return(4);		// X is positive, Y is negative

		} else {
			// Either 2 or 3
			if (p->y >= 0.0)		return(2);		// X is negative, Y is positive
			else					return(3);		// X is negative, Y is negative
		}
	}

	f32 iivvm_math_adjustTheta(f32 tfTheta)
	{
		// Validate theta is positive
		while (tfTheta < 0.0)
			tfTheta += (f32)_2PI;

		// Validate theta is 0..2pi
		while (tfTheta > _2PI)
			tfTheta -= (f32)_2PI;

		return(tfTheta);
	}

	void iivvm_math_computeLine(SLineF32* line)
	{
		// Midpoint = (x2-x1)/2, (y2-y1)/2
		line->mid.x			= (line->p1.x + line->p2.x) / 2.0f;
		line->mid.y			= (line->p1.y + line->p2.y) / 2.0f;

		// Compute our deltas
		line->delta.x		= line->p2.x - line->p1.x;
		line->delta.y		= line->p2.y - line->p1.y;

		// Length
		line->length		= sqrt(line->delta.x*line->delta.x + line->delta.y*line->delta.y);

		// Slope = rise over run
		line->m				= line->delta.y / ((line->delta.x == 0.0f) ? 0.000001f : line->delta.x);

		// Perpendicular slope = -1/m
		line->mp			= -1.0f / ((line->m == 0.0) ? 0.000001f : line->m);


		//////////
		// Compute theta if need be (radius is same as length)
		/////
			if (line->trig)
				line->theta		= iivvm_math_adjustTheta(atan2(line->delta.y, line->delta.x));


		//////////
		// Compute the integer roundings if need be
		//////
			if (line->ints)
			{
				// Start
				line->p1i.x		= (s32)line->p1.x;
				line->p1i.y		= (s32)line->p1.y;
				// End
				line->p2i.x		= (s32)line->p2.x;
				line->p2i.y		= (s32)line->p2.y;
			}


		//////////
		// Compute the quadrants if need be
		//////
			if (line->quads)
			{
				// Quads 1..4
				line->p1_quad	= iivvm_math_computeQuad(&line->p1);
				line->p2_quad	= iivvm_math_computeQuad(&line->p2);
			}

	}

	void iBmp_drawArbitraryQuad(SBitmap* bmp, s32 tnX1, s32 tnY1, s32 tnX2, s32 tnY2, s32 tnWidth, bool tlDrawEnds, SBgra color)
	{
		f32			lfX1, lfY1, lfX2, lfY2, lfX3, lfY3, lfX4, lfY4;
		SLineF32	line;


		//////////
		// Compute the line
		/////
			line.p1.x = (f32)tnX1;
			line.p1.y = (f32)tnY1;
			line.p2.x = (f32)tnX2;
			line.p2.y = (f32)tnY2;
			iivvm_math_computeLine(&line);


		//////////
		// Draw the line
		//////
			lfX1 = line.p1.x + ((f32)cos(line.theta + _PI2) * ((f32)tnWidth / 2.0f));
			lfY1 = line.p1.y + ((f32)sin(line.theta + _PI2) * ((f32)tnWidth / 2.0f));
			lfX2 = line.p1.x + ((f32)cos(line.theta - _PI2) * ((f32)tnWidth / 2.0f));
			lfY2 = line.p1.y + ((f32)sin(line.theta - _PI2) * ((f32)tnWidth / 2.0f));
			if (tlDrawEnds)
				iBmp_drawArbitraryLine(bmp, (s32)lfX1, (s32)lfY1, (s32)lfX2, (s32)lfY2, color);

			lfX3 = line.p2.x + ((f32)cos(line.theta + _PI2) * ((f32)tnWidth / 2.0f));
			lfY3 = line.p2.y + ((f32)sin(line.theta + _PI2) * ((f32)tnWidth / 2.0f));
			lfX4 = line.p2.x + ((f32)cos(line.theta - _PI2) * ((f32)tnWidth / 2.0f));
			lfY4 = line.p2.y + ((f32)sin(line.theta - _PI2) * ((f32)tnWidth / 2.0f));
			if (tlDrawEnds)
				iBmp_drawArbitraryLine(bmp, (s32)lfX3, (s32)lfY3, (s32)lfX4, (s32)lfY4, color);

			iBmp_drawArbitraryLine(bmp, (s32)lfX1, (s32)lfY1, (s32)lfX3, (s32)lfY3, color);
			iBmp_drawArbitraryLine(bmp, (s32)lfX2, (s32)lfY2, (s32)lfX4, (s32)lfY4, color);

	}

	void iBmp_drawHorizontalLine(SBitmap* bmp, s32 tnX1, s32 tnX2, s32 tnY, SBgra color)
	{
		s32		lnX;
		SBgr*	lbgr;
		SBgra*	lbgra;


		if (bmp)
		{
			if (tnY >= 0 && tnY < bmp->bi.biHeight)
			{
				// Get our starting point
				lbgr	= (SBgr*)(bmp->bd  + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX1 * (bmp->bi.biBitCount / 8)));
				lbgra	= (SBgra*)(bmp->bd + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX1 * (bmp->bi.biBitCount / 8)));

				if (bmp->bi.biBitCount == 24)
				{
					// Iterate for each column
					for (lnX = tnX1; lnX <= tnX2; lnX++)
					{
						// Are we on the bitmap?
						if (lnX >= 0 && lnX < bmp->bi.biWidth)
						{
							// Draw the pixel
							lbgr->red	= color.red;
							lbgr->grn	= color.grn;
							lbgr->blu	= color.blu;
						}
						// Move to next column
						++lbgr;
					}

				} else if (bmp->bi.biBitCount == 32) {
					// Iterate for each column
					for (lnX = tnX1; lnX <= tnX2; lnX++)
					{
						// Are we on the bitmap?
						if (lnX >= 0 && lnX < bmp->bi.biWidth)
						{
							// Draw the pixel
							lbgra->alp	= 255;
							lbgra->red	= color.red;
							lbgra->grn	= color.grn;
							lbgra->blu	= color.blu;
						}
						// Move to next column
						++lbgra;
					}
				}
			}
		}
	}

	void iBmp_drawVerticalLine(SBitmap* bmp, s32 tnY1, s32 tnY2, s32 tnX, SBgra color)
	{
		s32		lnY;
		SBgr*	lbgr;
		SBgra*	lbgra;


		if (bmp)
		{
			if (tnX >= 0 && tnX < bmp->bi.biWidth)
			{
				// Get our starting point
				lbgr	= (SBgr*)(bmp->bd  + ((bmp->bi.biHeight - tnY1 - 1) * bmp->rowWidth) + (tnX * (bmp->bi.biBitCount / 8)));
				lbgra	= (SBgra*)(bmp->bd + ((bmp->bi.biHeight - tnY1 - 1) * bmp->rowWidth) + (tnX * (bmp->bi.biBitCount / 8)));

				if (bmp->bi.biBitCount == 24)
				{
					// Iterate for each column
					for (lnY = tnY1; lnY <= tnY2; lnY++)
					{
						// Are we on the bitmap?
						if (lnY >= 0 && lnY < bmp->bi.biHeight)
						{
							// Draw the pixel
							lbgr->red	= color.red;
							lbgr->grn	= color.grn;
							lbgr->blu	= color.blu;
						}
						// Move to next row
						lbgr = (SBgr*)((s8*)lbgr - bmp->rowWidth);
					}

				} else if (bmp->bi.biBitCount == 32) {
					// Iterate for each column
					for (lnY = tnY1; lnY <= tnY2; lnY++)
					{
						// Are we on the bitmap?
						if (lnY >= 0 && lnY < bmp->bi.biHeight)
						{
							// Draw the pixel
							lbgra->alp	= 255;
							lbgra->red	= color.red;
							lbgra->grn	= color.grn;
							lbgra->blu	= color.blu;
						}
						// Move to next row
						lbgra = (SBgra*)((s8*)lbgra - bmp->rowWidth);
					}
				}
			}
		}
	}




//////////
//
// Gradient line algorithms
//
//////
	void iBmp_drawHorizontalLineGradient(SBitmap* bmp, s32 tnX1, s32 tnX2, s32 tnY, f32 tfRed, f32 tfGrn, f32 tfBlu, f32 tfRedInc, f32 tfGrnInc, f32 tfBluInc, RECT* rcClip, bool tluseClip)
	{
		s32		lnX;
		SBgr*	lbgr;
		SBgra*	lbgra;


		if (bmp && tnY >= 0 && tnY < bmp->bi.biHeight)
		{
			// Get our starting point
			lbgr	= (SBgr*)(bmp->bd  + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX1 * (bmp->bi.biBitCount / 8)));
			lbgra	= (SBgra*)(bmp->bd + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX1 * (bmp->bi.biBitCount / 8)));

			if (bmp->bi.biBitCount == 24)
			{
				// Iterate for each column
				for (lnX = tnX1; lnX <= tnX2; lnX++, tfRed += tfRedInc, tfGrn += tfGrnInc, tfBlu += tfBluInc)
				{
					// Are we on the bitmap?
					if ((!tluseClip || !(tnY >= rcClip->top && tnY <= rcClip->bottom && lnX >= rcClip->left && lnX <= rcClip->right)) && lnX >= 0 && lnX < bmp->bi.biWidth)
					{
						// Draw the pixel
						lbgr->red	= (u8)tfRed;
						lbgr->grn	= (u8)tfGrn;
						lbgr->blu	= (u8)tfBlu;
					}
					// Move to next column
					++lbgr;
				}

			} else if (bmp->bi.biBitCount == 32) {
				// Iterate for each column
				for (lnX = tnX1; lnX <= tnX2; lnX++, tfRed += tfRedInc, tfGrn += tfGrnInc, tfBlu += tfBluInc)
				{
					// Are we on the bitmap?
					if ((!tluseClip || !(tnY >= rcClip->top && tnY <= rcClip->bottom && lnX >= rcClip->left && lnX <= rcClip->right)) && lnX >= 0 && lnX < bmp->bi.biWidth)
					{
						// Draw the pixel
						lbgra->alp	= 255;
						lbgra->red	= (u8)tfRed;
						lbgra->grn	= (u8)tfGrn;
						lbgra->blu	= (u8)tfBlu;
					}
					// Move to next column
					++lbgra;
				}
			}
		}
	}

	void iBmp_drawVerticalLineGradient(SBitmap* bmp, s32 tnY1, s32 tnY2, s32 tnX, f32 tfRed, f32 tfGrn, f32 tfBlu, f32 tfRedInc, f32 tfGrnInc, f32 tfBluInc, RECT* rcClip, bool tluseClip)
	{
		s32		lnY;
		SBgr*	lbgr;
		SBgra*	lbgra;


		if (bmp && tnX >= 0 && tnX < bmp->bi.biWidth)
		{
			// Get our starting point
			lbgr	= (SBgr*)(bmp->bd  + ((bmp->bi.biHeight - tnY1 - 1) * bmp->rowWidth) + (tnX * (bmp->bi.biBitCount / 8)));
			lbgra	= (SBgra*)(bmp->bd + ((bmp->bi.biHeight - tnY1 - 1) * bmp->rowWidth) + (tnX * (bmp->bi.biBitCount / 8)));

			if (bmp->bi.biBitCount == 24)
			{
				// Iterate for each column
				for (lnY = tnY1; lnY <= tnY2; lnY++, tfRed += tfRedInc, tfGrn += tfGrnInc, tfBlu += tfBluInc)
				{
					// Are we on the bitmap?
					if (!tluseClip || (!(lnY >= rcClip->top && lnY <= rcClip->bottom && tnX >= rcClip->left && tnX <= rcClip->right) && lnY >= 0 && lnY < bmp->bi.biHeight))
					{
						// Draw the pixel
						lbgr->red	= (u8)tfRed;
						lbgr->grn	= (u8)tfGrn;
						lbgr->blu	= (u8)tfBlu;
					}
					// Move to next row
					lbgr = (SBgr*)((s8*)lbgr - bmp->rowWidth);
				}

			} else if (bmp->bi.biBitCount == 32) {
				// Iterate for each column
				for (lnY = tnY1; lnY <= tnY2; lnY++, tfRed += tfRedInc, tfGrn += tfGrnInc, tfBlu += tfBluInc)
				{
					// Are we on the bitmap?
					if (!tluseClip || (!(lnY >= rcClip->top && lnY <= rcClip->bottom && tnX >= rcClip->left && tnX <= rcClip->right) && lnY >= 0 && lnY < bmp->bi.biHeight))
					{
						// Draw the pixel
						lbgra->alp	= 255;
						lbgra->red	= (u8)tfRed;
						lbgra->grn	= (u8)tfGrn;
						lbgra->blu	= (u8)tfBlu;
					}
					// Move to next row
					lbgra = (SBgra*)((s8*)lbgra - bmp->rowWidth);
				}
			}
		}
	}




//////////
//
// Called to colorize a horizontal line
//
//////
	void iBmp_colorizeHorizontalLine(SBitmap* bmp, s32 tnX1, s32 tnX2, s32 tnY, SBgra color, f32 alpha)
	{
		s32		lnX;
		f32		lfMalp;
		SBgr*	lbgr;
		SBgra*	lbgra;


		if (bmp)
		{
			if (tnY >= 0 && tnY < bmp->bi.biHeight)
			{
				// Compute alpha
				lfMalp = 1.0f - alpha;

				// Get our starting point
				lbgr	= (SBgr*)(bmp->bd  + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX1 * (bmp->bi.biBitCount / 8)));
				lbgra	= (SBgra*)(bmp->bd + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX1 * (bmp->bi.biBitCount / 8)));

				if (bmp->bi.biBitCount == 24)
				{
					// Iterate for each column
					for (lnX = tnX1; lnX <= tnX2; lnX++)
					{
						// Are we on the bitmap?
						if (lnX >= 0 && lnX < bmp->bi.biWidth)
						{
							// Colorize the pixel
							lbgr->red	= (u8)max(min((alpha * (f32)color.red) + (lfMalp * (f32)lbgr->red), 255.0f), 0.0f);
							lbgr->grn	= (u8)max(min((alpha * (f32)color.grn) + (lfMalp * (f32)lbgr->grn), 255.0f), 0.0f);
							lbgr->blu	= (u8)max(min((alpha * (f32)color.blu) + (lfMalp * (f32)lbgr->blu), 255.0f), 0.0f);
						}
						// Move to next column
						++lbgr;
					}

				} else if (bmp->bi.biBitCount == 32) {
					// Iterate for each column
					for (lnX = tnX1; lnX <= tnX2; lnX++)
					{
						// Are we on the bitmap?
						if (lnX >= 0 && lnX < bmp->bi.biWidth)
						{
							// Colorize the pixel
							lbgra->red	= (u8)max(min((alpha * (f32)color.red) + (lfMalp * (f32)lbgra->red), 255.0f), 0.0f);
							lbgra->grn	= (u8)max(min((alpha * (f32)color.grn) + (lfMalp * (f32)lbgra->grn), 255.0f), 0.0f);
							lbgra->blu	= (u8)max(min((alpha * (f32)color.blu) + (lfMalp * (f32)lbgra->blu), 255.0f), 0.0f);
						}
						// Move to next column
						++lbgra;
					}
				}
			}
		}
	}




//////////
//
// Called to colorize a vertical line
//
//////
	void iBmp_colorizeVerticalLine(SBitmap* bmp, s32 tnY1, s32 tnY2, s32 tnX, SBgra color, f32 alpha)
	{
		s32		lnY;
		f32		lfMalp;
		SBgr*	lbgr;
		SBgra*	lbgra;


		if (bmp)
		{
			if (tnX >= 0 && tnX < bmp->bi.biWidth)
			{
				// Compute alpha
				lfMalp = 1.0f - alpha;

				// Get our starting point
				lbgr	= (SBgr*)(bmp->bd  + ((bmp->bi.biHeight - tnY1 - 1) * bmp->rowWidth) + (tnX * (bmp->bi.biBitCount / 8)));
				lbgra	= (SBgra*)(bmp->bd + ((bmp->bi.biHeight - tnY1 - 1) * bmp->rowWidth) + (tnX * (bmp->bi.biBitCount / 8)));

				if (bmp->bi.biBitCount == 24)
				{
					// Iterate for each column
					for (lnY = tnY1; lnY <= tnY2; lnY++)
					{
						// Are we on the bitmap?
						if (lnY >= 0 && lnY < bmp->bi.biHeight)
						{
							// Colorize the pixel
							lbgr->red	= (u8)max(min((alpha * (f32)color.red) + (lfMalp * (f32)lbgr->red), 255.0f), 0.0f);
							lbgr->grn	= (u8)max(min((alpha * (f32)color.grn) + (lfMalp * (f32)lbgr->grn), 255.0f), 0.0f);
							lbgr->blu	= (u8)max(min((alpha * (f32)color.blu) + (lfMalp * (f32)lbgr->blu), 255.0f), 0.0f);
						}
						// Move to next row
						lbgr = (SBgr*)((s8*)lbgr - bmp->rowWidth);
					}

				} else if (bmp->bi.biBitCount == 32) {
					// Iterate for each column
					for (lnY = tnY1; lnY <= tnY2; lnY++)
					{
						// Are we on the bitmap?
						if (lnY >= 0 && lnY < bmp->bi.biHeight)
						{
							// Colorize the pixel
							lbgra->red	= (u8)max(min((alpha * (f32)color.red) + (lfMalp * (f32)lbgra->red), 255.0f), 0.0f);
							lbgra->grn	= (u8)max(min((alpha * (f32)color.grn) + (lfMalp * (f32)lbgra->grn), 255.0f), 0.0f);
							lbgra->blu	= (u8)max(min((alpha * (f32)color.blu) + (lfMalp * (f32)lbgra->blu), 255.0f), 0.0f);
						}
						// Move to next row
						lbgra = (SBgra*)((s8*)lbgra - bmp->rowWidth);
					}
				}
			}
		}
	}




//////////
//
// Called to colorize a horizontal line on a gradient
//
//////
	void iBmp_colorizeHorizontalLineGradient(SBitmap* bmp, s32 tnX1, s32 tnX2, s32 tnY, f32 tfRed, f32 tfGrn, f32 tfBlu, f32 tfRedInc, f32 tfGrnInc, f32 tfBluInc, RECT* rcClip, bool tluseClip, f32 alpha)
	{
		s32		lnX;
		f32		lfMalp, lfGray;
		SBgr*	lbgr;
		SBgra*	lbgra;


		if (bmp && tnY >= 0 && tnY < bmp->bi.biHeight)
		{
			// Get our starting point
			lbgr	= (SBgr*)(bmp->bd  + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX1 * (bmp->bi.biBitCount / 8)));
			lbgra	= (SBgra*)(bmp->bd + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX1 * (bmp->bi.biBitCount / 8)));

			if (bmp->bi.biBitCount == 24)
			{
				// Iterate for each column
				for (lnX = tnX1; lnX <= tnX2; lnX++, tfRed += tfRedInc, tfGrn += tfGrnInc, tfBlu += tfBluInc)
				{
					// Are we on the bitmap?
					if ((!tluseClip || !(tnY >= rcClip->top && tnY <= rcClip->bottom && lnX >= rcClip->left && lnX <= rcClip->right)) && lnX >= 0 && lnX < bmp->bi.biWidth)
					{
						// Colorize the pixel
						lfGray		= ((0.35f * (f32)lbgr->red) + (0.54f * (f32)lbgr->grn) + (0.11f * (f32)lbgr->blu)) / 255.0f;
						lfMalp		= 1.0f - lfGray;
						lbgr->red	= (u8)max(min((lfGray * tfRed) + (lfMalp * (f32)lbgr->red), 255.0f), 0.0f);
						lbgr->grn	= (u8)max(min((lfGray * tfGrn) + (lfMalp * (f32)lbgr->grn), 255.0f), 0.0f);
						lbgr->blu	= (u8)max(min((lfGray * tfBlu) + (lfMalp * (f32)lbgr->blu), 255.0f), 0.0f);
					}
					// Move to next column
					++lbgr;
				}

			} else if (bmp->bi.biBitCount == 32) {
				// Iterate for each column
				for (lnX = tnX1; lnX <= tnX2; lnX++, tfRed += tfRedInc, tfGrn += tfGrnInc, tfBlu += tfBluInc)
				{
					// Are we on the bitmap?
					if ((!tluseClip || !(tnY >= rcClip->top && tnY <= rcClip->bottom && lnX >= rcClip->left && lnX <= rcClip->right)) && lnX >= 0 && lnX < bmp->bi.biWidth)
					{
						// Colorize the pixel
						lfGray		= alpha * ((0.35f * (f32)lbgra->red) + (0.54f * (f32)lbgra->grn) + (0.11f * (f32)lbgra->blu)) / 255.0f;
						lfMalp		= 1.0f - lfGray;
						lbgra->red	= (u8)max(min((lfGray * tfRed) + (lfMalp * (f32)lbgra->red), 255.0f), 0.0f);
						lbgra->grn	= (u8)max(min((lfGray * tfGrn) + (lfMalp * (f32)lbgra->grn), 255.0f), 0.0f);
						lbgra->blu	= (u8)max(min((lfGray * tfBlu) + (lfMalp * (f32)lbgra->blu), 255.0f), 0.0f);
					}
					// Move to next column
					++lbgra;
				}
			}
		}
	}




//////////
//
// Called to colorize a vertical line on a gradient
//
//////
	void iBmp_colorizeVerticalLineGradient(SBitmap* bmp, s32 tnY1, s32 tnY2, s32 tnX, f32 tfRed, f32 tfGrn, f32 tfBlu, f32 tfRedInc, f32 tfGrnInc, f32 tfBluInc, RECT* rcClip, bool tluseClip, f32 alpha)
	{
		s32		lnY;
		f32		lfMalp, lfGray;
		SBgr*	lbgr;
		SBgra*	lbgra;


		if (bmp && tnX >= 0 && tnX < bmp->bi.biWidth)
		{
			// Compute alpha
			lfMalp	= 1.0f - alpha;

			// Get our starting point
			lbgr	= (SBgr*)(bmp->bd  + ((bmp->bi.biHeight - tnY1 - 1) * bmp->rowWidth) + (tnX * (bmp->bi.biBitCount / 8)));
			lbgra	= (SBgra*)(bmp->bd + ((bmp->bi.biHeight - tnY1 - 1) * bmp->rowWidth) + (tnX * (bmp->bi.biBitCount / 8)));

			if (bmp->bi.biBitCount == 24)
			{
				// Iterate for each column
				for (lnY = tnY1; lnY <= tnY2; lnY++, tfRed += tfRedInc, tfGrn += tfGrnInc, tfBlu += tfBluInc)
				{
					// Are we on the bitmap?
					if (!(lnY >= rcClip->top && lnY <= rcClip->bottom && tnX >= rcClip->left && tnX <= rcClip->right) && lnY >= 0 && lnY < bmp->bi.biHeight)
					{
						// Colorize the pixel
						lfGray		= alpha * ((0.35f * (f32)lbgr->red) + (0.54f * (f32)lbgr->grn) + (0.11f * (f32)lbgr->blu)) / 255.0f;
						lbgr->red	= (u8)max(min((lfGray * tfRed) + (lfMalp * (f32)lbgr->red), 255.0f), 0.0f);
						lbgr->grn	= (u8)max(min((lfGray * tfGrn) + (lfMalp * (f32)lbgr->grn), 255.0f), 0.0f);
						lbgr->blu	= (u8)max(min((lfGray * tfBlu) + (lfMalp * (f32)lbgr->blu), 255.0f), 0.0f);
					}
					// Move to next row
					lbgr = (SBgr*)((s8*)lbgr - bmp->rowWidth);
				}

			} else if (bmp->bi.biBitCount == 32) {
				// Iterate for each column
				for (lnY = tnY1; lnY <= tnY2; lnY++, tfRed += tfRedInc, tfGrn += tfGrnInc, tfBlu += tfBluInc)
				{
					// Are we on the bitmap?
					if (!(lnY >= rcClip->top && lnY <= rcClip->bottom && tnX >= rcClip->left && tnX <= rcClip->right) && lnY >= 0 && lnY < bmp->bi.biHeight)
					{
						// Colorize the pixel
						lfGray		= alpha * ((0.35f * (f32)lbgra->red) + (0.54f * (f32)lbgra->grn) + (0.11f * (f32)lbgra->blu)) / 255.0f;
						lbgra->red	= (u8)max(min((lfGray * tfRed) + (lfMalp * (f32)lbgra->red), 255.0f), 0.0f);
						lbgra->grn	= (u8)max(min((lfGray * tfGrn) + (lfMalp * (f32)lbgra->grn), 255.0f), 0.0f);
						lbgra->blu	= (u8)max(min((lfGray * tfBlu) + (lfMalp * (f32)lbgra->blu), 255.0f), 0.0f);
					}
					// Move to next row
					lbgra = (SBgra*)((s8*)lbgra - bmp->rowWidth);
				}
			}
		}
	}




//////////
//
// minimumRatio is in the range 0..1
//
//////
	f32 iiBmp_squeezeColorChannel(f32 colorChannel, f32 minimumRatio)
	{
		f32 newRatio, colorBase;


		newRatio		= 1.0f - minimumRatio;
		colorBase		= minimumRatio * 255.0f;

		return(colorBase + (newRatio * colorChannel));
	}




//////////
//
// Called to set the indicated pixel to the indicated color
//
//////
	u32 iiBmp_setPixel(SBitmap* bmp, s32 tnX, s32 tnY, SBgra color)
	{
		u32		lnPixelsRendered;
		SBgr*	lbgr;
		SBgra*	lbgra;


		// Make sure the pixel will fit
		lnPixelsRendered = 0;
		if (tnX >= 0 && tnY >= 0 && tnX < bmp->bi.biWidth && tnY < bmp->bi.biHeight)
		{
			if (bmp->bi.biBitCount == 24)
			{
				// Get our offset
				lbgr = (SBgr*)(bmp->bd + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX * 3));

				// Draw it
				lbgr->red = color.red;
				lbgr->grn = color.grn;
				lbgr->blu = color.blu;

				// Indicate our pixel count
				++lnPixelsRendered;

			} else if (bmp->bi.biBitCount == 32) {
				// Get our offset
				lbgra = (SBgra*)(bmp->bd + ((bmp->bi.biHeight - tnY - 1) * bmp->rowWidth) + (tnX * 4));

				// Draw it
				lbgra->red = color.red;
				lbgra->grn = color.grn;
				lbgra->blu = color.blu;

				// Indicate our pixel count
				++lnPixelsRendered;
			}
		}

		// Indicate our draw count
		return(lnPixelsRendered);
	}

	f32 iBmp_getWidthInches(SBitmap* bmp)
	{
		f32		lfDpiX;


		if (!bmp)
			return -1.0f;

		lfDpiX = iBmp_getPixelsPerInchX(bmp);
		return (f32)bmp->bi.biWidth / lfDpiX;
	}

	f32 iBmp_getHeightInches(SBitmap* bmp)
	{
		f32		lfDpiY;


		if (!bmp)
			return -1.0f;

		lfDpiY = iBmp_getPixelsPerInchX(bmp);
		return (f32)bmp->bi.biHeight / lfDpiY;
	}

	// Formula = ppmx / 39.3700787402
	f32 iBmp_getPixelsPerInchX(SBitmap* bmp)
	{
		if (!bmp)
			return -1.0f;

		return (f32)bmp->bi.biXPelsPerMeter / 39.3700787402f;
	}

	// Formula = ppmy / 39.3700787402
	f32 iBmp_getPixelsPerInchY(SBitmap* bmp)
	{
		if (!bmp)
			return -1.0f;

		return (f32)bmp->bi.biYPelsPerMeter / 39.3700787402f;
	}

	bool iIsNeedleInHaystack(wchar_t* needle, s32 tnNeedleLength, wchar_t* haystack, s32 tnHaystackLength)
	{
		s32		lnI, lnStop;

		// Calculate our stopping point
		lnStop = (tnHaystackLength - tnNeedleLength);
		if (lnStop < 0)
			return false;	// Needle is longer than haystack

		// Iterate until we've tested everything
		for (lnI = 0; lnI <= lnStop; ++lnI)
		{
			if (_wcsnicmp(needle, haystack + lnI, tnNeedleLength) == 0)
				return true;
		}

		// If we get here, not found
		return false;
	}




//////////
//
// Called to render the node to a bitmap
//
//////
#ifdef SComp
	void iBmp_node_renderComp(SNode* node, s32 tnMaxTokenLength, s32 tnMaxOverallLength, bool tlIncludeExtraInfo, SNodeProps props[], s32 tnPropsCount, u32 tnIter_uid)
	{
		s32			lnI, lnStart, lnEnd, lnWidth, lnHeight;
		bool		llDeleteFont;
		s8*			lciCodeName;
		RECT		lrc;
		SFont*		font;
		HGDIOBJ		lhOldFont;
		SNodeProps	_propsLocal;
		SNodeProps*	prop;
		s8			buffer[2048];
		

		// Make sure our environment is sane
		if (node && node->comp && node->comp->line && node->comp->line->sourceCode_populatedLength > 0 && node->comp->line->sourceCode->_data)
		{

			//////////
			// Get the correct props
			//////
				if (node->render_override)
				{
					// Use their custom instance override
					prop = node->render_override;

				} else if (tnPropsCount > 0 && node->render.propsIndex >= 0 && node->render.propsIndex <= tnPropsCount) {
					// We can grab it from the array
					prop = &props[node->render.propsIndex];

				} else {
					// Create a default entry
					_propsLocal.backColor		= whiteColor;
					_propsLocal.foreColor		= blackColor;
					_propsLocal.marginWidth		= 4;
					_propsLocal.fillColor		= whiteColor;
					_propsLocal.borderWidth		= 2;
					_propsLocal.borderColor		= blackColor;
					_propsLocal.font			= iFont_create(cgcFontName_defaultFixed);
					prop						= &_propsLocal;
				}

				// Validate border and margin
				prop->marginWidth	= min(max(0, prop->marginWidth), 64);		// Max of 64-pixel margin
				prop->borderWidth	= min(max(0, prop->borderWidth), 16);		// Max of 16-pixel border

				// Create our font
				if (prop->font)
				{
					// Use their happy font
					llDeleteFont	= false;
					font			= prop->font;

				} else {
					// Create our own font
					llDeleteFont	= true;
					font			= iFont_create(cgcFontName_defaultFixed, 9, FW_NORMAL, 0, 0);
				}



			//////////
			// Determine the text that will be rendered
			//////
				memset(buffer, 0, sizeof(buffer));

				// Prepend a "]" if need be
				if (tlIncludeExtraInfo)
					sprintf(buffer, "[");

				// Copy the contents of the physical token
				memcpy(buffer + (s32)strlen(buffer), node->comp->line->sourceCode->data._s8 + node->comp->start, min(node->comp->length, (s32)sizeof(buffer) - 20));

				// Include the iCode, start and length of the token on the line
				if (tlIncludeExtraInfo)
				{
					// Try to do a reverse lookup on the iCode
					if ((lciCodeName = iiComps_visualize_lookup_iCode(node->comp->iCode)))
					{
						// Grab the name
						lnStart = (s32)strlen(buffer);
						sprintf(buffer + (s32)strlen(buffer), " %s,", lciCodeName);
						lnEnd	= (s32)strlen(buffer);

						// Make sure the length isn't longer than their max length
						if (lnEnd - lnStart > tnMaxTokenLength)
							buffer[lnStart + tnMaxTokenLength] = 0;

					} else {
						// The name could not be looked up, so we just use the number, and in this case we ignore the max length and use the whole number
						sprintf(buffer + (s32)strlen(buffer), " %d,", node->comp->iCode);
					}

					// Close it out
					sprintf(buffer + (s32)strlen(buffer), "%u,%u]", node->comp->start, node->comp->length);
				}

				// Make sure it's not too long overall
				if (strlen(buffer) > tnMaxOverallLength)
				{
					// Close it out at the max length distance with ]
					buffer[tnMaxOverallLength - 1]	= ']';
					buffer[tnMaxOverallLength]		= 0;
				}


			//////////
			// Make sure the bitmap exists
			//////
				if (!node->render.bmp)
				{
					// Create a default image
					node->render.bmp = iBmp_allocate();
					if (!node->render.bmp)
						return;

					// Create a 20x20 image
					iBmp_createBySize(node->render.bmp, 20, 20, 24);
				}


			//////////
			// Find out how big it would be
			//////
				SetRect(&lrc, 0, 0, 0, 0);
				lhOldFont = SelectObject(node->render.bmp->hdc, font->hfont);
				DrawText(node->render.bmp->hdc, buffer, (int)strlen(buffer), &lrc, DT_CALCRECT | DT_SINGLELINE);
				SelectObject(node->render.bmp->hdc, lhOldFont);


			//////////
			// Compute the true size
			//////
				lnWidth		= (lrc.right - lrc.left) + (prop->marginWidth * 2) + (prop->borderWidth * 2);
				lnHeight	= (lrc.bottom - lrc.top) + (prop->marginWidth * 2) + (prop->borderWidth * 2);
			

			//////////
			// Adjust the bitmap size if need be
			//////
				if (node->render.bmp->bi.biWidth != lnWidth || node->render.bmp->bi.biHeight != lnHeight)
					node->render.bmp = iBmp_verifySizeOrResize(node->render.bmp, lnWidth, lnHeight, node->render.bmp->bi.biBitCount);


			//////////
			// Render
			//////
				// Frame
				SetRect(&lrc, 0, 0, lnWidth, lnHeight);
				for (lnI = max(0, min(prop->borderWidth, lnHeight / 2 - 1)); lnI > 0; lnI--)
				{
					iBmp_frameRect(node->render.bmp, &lrc, prop->borderColor);
					InflateRect(&lrc, -1, -1);
				}

				// Fill
				iBmp_fillRect(node->render.bmp, &lrc, prop->fillColor);
				InflateRect(&lrc, -prop->marginWidth, -prop->marginWidth);

				// Text
				SetTextColor(node->render.bmp->hdc, RGB(prop->foreColor.red, prop->foreColor.grn, prop->foreColor.blu));
				SetBkColor(node->render.bmp->hdc, RGB(prop->backColor.red, prop->backColor.grn, prop->backColor.blu));
				SetBkMode(node->render.bmp->hdc, OPAQUE);
				lhOldFont = SelectObject(node->render.bmp->hdc, font->hfont);
				DrawText(node->render.bmp->hdc, buffer, (int)strlen(buffer), &lrc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				SelectObject(node->render.bmp->hdc, lhOldFont);


			//////////
			// Colorize
			//////
				if (prop->colorize)
				{
					SetRect(&lrc, 0, 0, lnWidth, lnHeight);
					iBmp_colorize(node->render.bmp, &lrc, prop->colorizeColor, false, 0.0f);	// true, prop->colorizeMinColor);
				}


			//////////
			// Clean house
			//////
				if (llDeleteFont)
					iFont_delete(&font, true);

				// Update iteration
				node->render.iter_uid = tnIter_uid;

//////////
// Added for debugging:
// s8 buffer2[32];
// memset(buffer2, 0, sizeof(buffer2));
// memcpy(buffer2, node->comp->line->sourceCode->data._s8 + node->comp->start, min(node->comp->length, 16));
// sprintf(buffer, "c:\\temp\\node_render__%s.bmp", buffer2);
// iBmp_saveToDisk(node->render.bmp, buffer);
//////
		}
	}
#endif




//////////
//
// Called to scale one bitmap into another.
//
// Note:  All of this code was adapted from the Visual FreePro.
// Note:  See https://github.com/RickCHodgin/libsf (in vvm\core\).
//
//////
	SBitmap* iBmp_scale(SBitmap* bmpSrc, f32 scale)
	{
		SBitmap* bmpDst;


		// Make sure our environment is sane
		bmpDst = NULL;
		if (bmpSrc)
		{
			// Create a target bitmap of the appropriate size
			bmpDst = iBmp_allocate();
			iBmp_createBySize(bmpDst, (s32)((f32)bmpSrc->bi.biWidth * scale), (s32)((f32)bmpSrc->bi.biHeight * scale), bmpSrc->bi.biBitCount);

			// Physially scale
			iBmp_scale(bmpDst, bmpSrc);
		}

		// Signify
		return bmpDst;
	}
	u32 iBmp_scale(SBitmap* bmpDst, SBitmap* bmpSrc)
	{
		u32		lnResult;
		f32		lfVertical, lfHorizontal;
		RECT	lrc;


		// Make sure our environment is sane
		lnResult = -1;
		if (bmpDst && bmpSrc)
		{
			if (bmpDst->bi.biBitCount == 24 || bmpDst->bi.biBitCount == 32)
			{
				if (bmpSrc->bi.biBitCount == 24 || bmpSrc->bi.biBitCount == 32)
				{
					// We have valid source and destination bitmaps
					if (bmpSrc->bi.biWidth == bmpDst->bi.biWidth && bmpSrc->bi.biHeight == bmpDst->bi.biHeight)
					{
						// They're the same size
						if (bmpSrc->bi.biBitCount == bmpDst->bi.biBitCount)
						{
							// They're the same bit counts, do a fast copy
							memcpy(bmpDst->bd, bmpSrc->bd, bmpSrc->bi.biSizeImage);

						} else {
							// Do a bitBlt to translate bit counts
							SetRect(&lrc, 0, 0, bmpDst->bi.biWidth, bmpDst->bi.biHeight);
							iBmp_bitBlt(bmpDst, &lrc, bmpSrc);
						}
						// Indicate success
						lnResult = 1;

					} else {
						// We need to scale
						lfVertical		= (f32)bmpSrc->bi.biHeight / (f32)bmpDst->bi.biHeight;
						lfHorizontal	= (f32)bmpSrc->bi.biWidth  / (f32)bmpDst->bi.biWidth;
						lnResult		= iiBmp_scale_Process(bmpDst, bmpSrc, lfVertical, lfHorizontal);
					}
				}
			}
		}

		// Indicate our failure or success
		return(lnResult);
	}




//////////
//
// Takes a 24-bit bitmap, and scales it up or down to the specified size, with a
// minimum of a 1x1 pixel size.
//
// Returns:
//		-1		- Input file could not be opened
//		-2		- Invalid input bitmap
//		-3		- Unable to allocate memory for input bitmap
//		-4		- Error reading input bitmap
//		-5		- Unable to allocate memory for output bitmap
//		-6		- Unable to create the output file
//		-7		- Unable to write to output file
//
//////
	u32 iiBmp_scale_Process(SBitmap* bmpDst, SBitmap* bmpSrc, f32 tfVerticalScaler, f32 tfHorizontalScaler)
	{
		s32				lnY, lnX;
		SBitmapProcess	bp;


		// Being the scaling procedure
		bp.src		= bmpSrc;
		bp.ratioV	= (f32)bmpSrc->bi.biHeight	/ (f32)bmpDst->bi.biHeight;
		bp.ratioH	= (f32)bmpSrc->bi.biWidth	/ (f32)bmpDst->bi.biWidth;
		bp.pixels	= (SBgraf*)malloc(((u32)bp.ratioV + 16) * ((u32)bp.ratioH + 16) * sizeof(SBgraf));

		// Iterate through every pixel
		for (lnY = 0; lnY < bmpDst->bi.biHeight; lnY++)
		{
			// Grab the offset for this line
			if (bmpDst->bi.biBitCount == 24)		bp.optr		= (SBgr*)( bmpDst->bd + ((bmpDst->bi.biHeight - lnY - 1) * bmpDst->rowWidth));
			else									bp.optra	= (SBgra*)(bmpDst->bd + ((bmpDst->bi.biHeight - lnY - 1) * bmpDst->rowWidth));

			// Repeat for every pixel across this row
			for (lnX = 0; lnX < bmpDst->bi.biWidth; lnX++)
			{
				// Compute data for this spanned pixel
				bp.uly	= min((f32)lnY * bp.ratioV, (f32)bmpSrc->bi.biHeight - bp.ratioV);
				bp.ulx	= min((f32)lnX * bp.ratioH, (f32)bmpSrc->bi.biWidth  - bp.ratioH);
				bp.lry	= bp.uly + bp.ratioV;
				bp.lrx	= bp.ulx + bp.ratioH;

				// Get all the color information for this potentially spanned pixel
				iiBmp_scale_processPixels(&bp);

				// Store the color
				if (bmpDst->bi.biBitCount == 24)
				{
					bp.optr->red = (u8)bp.red;
					bp.optr->grn = (u8)bp.grn;
					bp.optr->blu = (u8)bp.blu;
					// Move to the next pixel
					++bp.optr;

				} else {
					bp.optra->red = (u8)bp.red;
					bp.optra->grn = (u8)bp.grn;
					bp.optra->blu = (u8)bp.blu;
					// Move to the next pixel
					++bp.optra;
				}
			}
		}
		// When we get here, we've computed everything

		// Release our pixel accumulator buffer
		free(bp.pixels);

		// Finished, indicate the pixel count
		return(bmpDst->bi.biHeight * bmpDst->bi.biWidth * sizeof(SBgr));
	}




//////////
//
// Get spanned pixel data, meaning the input (bii, bdi) values are scanned based on the
// location of tnY,tnX and the relationship between bii and bio, meaning the input and
// output sizes.  If bii is bigger, then each bio pixel maps to more than one bii pixel.
// If they're identical, it's 1:1.  If bii is smaller, then each bio pixel maps to less
// than one full bii pixel.  There are no other options. :-)  This algorithm should not
// be used for 1:1 ratio conversions.
//
// Note that each of the above conditions applies to both width and height, meaning the
// relationship between bii and bio is analyzed on each axis, resulting in nine possible
// states (wider+taller, wider+equal, wider+shorter, equal+taller, equal+equal, equal+shorter,
// narrower+taller, narrower+equal, narrower+shorter).
//
// This natural relationship breaks down into nine general point forms:
//		Original pixels:			Output pixels span original pixels:
//		 ______________ 			 ______________ 			 ______________
//		|    |    |    |			|    |    |    |			|1   | 2  |   3|
//		|____|____|____|			|__+--------+__|			|__+--------+__|
//		|    |    |    |	==>		|  |        |  |	==>		|4 |   5    | 6|
//		|____|____|____|	==>		|__|        |__|	==>		|__|        |__|
//		|    |    |    |			|  +--------+  |			|7 +---8----+ 9|
//		|____|____|____|			|____|____|____|			|____|____|____|
//
// This form is comprised of 9 general parts, eight of which may not be present in all
// relationships, and five of which may span multiple columns, rows or both.
//
// These are:
//		1	- upper-left	(always,	spans at most one pixel)
//		2	- upper-middle	(optional,	spans at most multiple partial or full pixels)
//		3	- upper-right	(optional,	spans at most one pixel)
//		4	- middle-left	(optional,	spans at most multiple partial or full pixels)
//		5	- middle-middle	(optional,	can span multiple partial or full pixels)
//		6	- middle-right	(optional,	spans at most multiple partial or full pixels)
//		7	- lower-left	(optional,	spans at most one pixel)
//		8	- lower-middle	(optional,	spans at most multiple partial or full pixels)
//		9	- lower-right	(optional,	spans at most one pixel)
//
//////
	void iiBmp_scale_processPixels(SBitmapProcess* bp)
	{
		u32		lnI;
		f32		lfRed, lfGrn, lfBlu, lfAlp, lfAreaAccumulator;


		// Raise the flags for which portions are valid / required
		bp->spans2H		= (iiBmp_scale_processGetIntegersBetween(bp->ulx, bp->lrx) >= 1);		// It occupies at least two pixels horizontally (itself and one more)
		bp->spans3H		= (iiBmp_scale_processGetIntegersBetween(bp->ulx, bp->lrx) >= 2);		// It occupies at least three pixels horizontally (itself, at least one in the middle, and one at the right)
		bp->spans2V		= (iiBmp_scale_processGetIntegersBetween(bp->uly, bp->lry) >= 1);		// It occupies at least two pixels vertically (itself and one more)
		bp->spans3V		= (iiBmp_scale_processGetIntegersBetween(bp->uly, bp->lry) >= 2);		// It occupies at least three pixels vertically (itself, at least one in the middle, and one at the right)

		// Reset the point count
		bp->count		= 0;

		// Indicate the start of this input line
		if (bp->src->bi.biBitCount == 24)
		{
			// 24-bit bitmap
			bp->iptr		= (SBgr*)(bp->src->bd + ((bp->src->bi.biHeight - (s32)bp->uly - 1) * bp->src->rowWidth));	// current line
			bp->iptrAnchor	= (SBgr*)(bp->src->bd + ((bp->src->bi.biHeight                   ) * bp->src->rowWidth));	// root anchor (does not include the conversion from base-1)

		} else {
			// 32-bit bitmap
			bp->iptra		= (SBgra*)(bp->src->bd + ((bp->src->bi.biHeight - (s32)bp->uly - 1) * bp->src->rowWidth));	// current line
			bp->iptrAnchora	= (SBgra*)(bp->src->bd + ((bp->src->bi.biHeight                   ) * bp->src->rowWidth));	// root anchor (does not include the conversion from base-1)
		}


		//////////
		// The following functions (if called) update the number of pieces of picture data to add to the output
		//////
			//////////
			// 1 - upper-left (always, spans at most one pixel)
			//////
				iiBmp_scale_processSpannedPixel1(bp);

			//////////
			// 2 - upper-middle (optional, spans at most multiple partial or full pixels, but only if 1, 2 and 3 exist)
			//////
				if (bp->spans3H)
					iiBmp_scale_processSpannedPixel2(bp);


			//////////
			// 3 - upper-right (optional, spans at most one pixel, but only if 1 and 3 exist (as 1 and 2))
			//////
				if (bp->spans2H || bp->spans3H)
					iiBmp_scale_processSpannedPixel3(bp);


			//////////
			// 4 - middle-left (optional, spans at most multiple partial or full pixels)
			//////
				if (bp->spans3V && bp->spans2V)
					iiBmp_scale_processSpannedPixel4(bp);


			//////////
			// 5 - middle-middle (optional, can span multiple partial or full pixels)
			//////
				if (bp->spans3V && bp->spans3H)
					iiBmp_scale_processSpannedPixel5(bp);


			//////////
			// 6 - middle-right (optional, spans at most multiple partial or full pixels)
			//////
				if (bp->spans3V && (bp->spans2H || bp->spans3H))
					iiBmp_scale_processSpannedPixel6(bp);


			//////////
			// 7 - lower-left (optional, spans at most one pixel)
			//////
				if (bp->spans2V)
					iiBmp_scale_processSpannedPixel7(bp);


			//////////
			// 8 - lower-middle (optional, spans at most multiple partial or full pixels)
			//////
				if (bp->spans2V && bp->spans3H)
					iiBmp_scale_processSpannedPixel8(bp);


			//////////
			// 9 - lower-right (optional, spans at most one pixel)
			//////
				if (bp->spans2V && (bp->spans2H || bp->spans3H))
					iiBmp_scale_processSpannedPixel9(bp);


		//////////
		// Add up all the pixels to compute the specified value
		//////
			lfAreaAccumulator = 0.0;
			for (lnI = 0; lnI < bp->count; lnI++)
				lfAreaAccumulator += bp->pixels[lnI].area;

			// Initialize
			lfRed	= 0;
			lfGrn	= 0;
			lfBlu	= 0;
			lfAlp	= 0;

			// Now, compute each component as its part of the total area
			for (lnI = 0; lnI < bp->count; lnI++)
			{
				// Derive this portion component
				lfRed	+=		bp->pixels[lnI].red	*	(bp->pixels[lnI].area / lfAreaAccumulator);
				lfGrn	+=		bp->pixels[lnI].grn	*	(bp->pixels[lnI].area / lfAreaAccumulator);
				lfBlu	+=		bp->pixels[lnI].blu	*	(bp->pixels[lnI].area / lfAreaAccumulator);
				lfAlp	+=		bp->pixels[lnI].alp	*	(bp->pixels[lnI].area / lfAreaAccumulator);
			}

			// When we get here, we have our values, now create the final summed up output
			bp->red = (u8)(lfRed + 0.5);
			bp->grn = (u8)(lfGrn + 0.5);
			bp->blu = (u8)(lfBlu + 0.5);
			bp->alp = (u8)(lfAlp + 0.5);
	}




//////////
//
// 1 - upper-left (see iGetSpannedPixelColors() above)
// Upper left pixels is ALWAYS computed. It may be the ONLY one computed, but it is always computed.
//
//////
	void iiBmp_scale_processSpannedPixel1(SBitmapProcess* bp)
	{
		// Store left- and right-sides for this spanned pixel
		bp->left			= (s32)min(bp->ulx,			(f32)bp->src->bi.biWidth - 1.0f);
		bp->right			= (s32)min(bp->lrx,			(f32)bp->src->bi.biWidth - 1.0f);

		// Compute the middle section in pixels
		// Note: -2 is for -1 base 0, and -1 because we want the max value to be one before the width/height
		bp->middleStartH	= (s32)min(bp->ulx + 1.0f,	(f32)bp->src->bi.biWidth  - 1.0f);		// next pixel right of upper-left
		bp->middleFinishH	= (s32)min(bp->lrx - 1.0f,	(f32)bp->src->bi.biWidth  - 1.0f);		// next pixel left of upper-right (which is actually lower-right, but on this upper line)
		bp->middleStartV	= (s32)min(bp->uly + 1.0f,	(f32)bp->src->bi.biHeight - 1.0f);		// next pixel right of left-side pixels
		bp->middleFinishV	= (s32)min(bp->lry - 1.0f,	(f32)bp->src->bi.biHeight - 1.0f);		// next pixel left of right-side pixels

		// Find out where this upper-left pixel falls
		if (!bp->spans2H)	bp->widthLeft	=      bp->lrx          - bp->ulx;		// Entire width is within this one pixel, so it's only a portion of the pixel's overall width
		else				bp->widthLeft	= (f32)bp->middleStartH - bp->ulx;		// It spans from where it is to the right of the pixel square

 		if (!bp->spans2V)	bp->height		=      bp->lry          - bp->uly;			// It's entire height is within this one pixel, so it's only a portion of the pixel's overall height
		else				bp->height		= (f32)bp->middleStartV - bp->uly;			// It spans from where it is to the bottom of the pixel square

		// Compute the area for this pixel component
		bp->area = bp->widthLeft * bp->height;

		// Store the colors for this point
		if (bp->src->bi.biBitCount == 24)
		{
			// 24-bit bitmap
			(bp->pixels[bp->count]).red		= (f32)(((SBgr*)((u8*)bp->iptr + (bp->left * 3)))->red);
			(bp->pixels[bp->count]).grn		= (f32)(((SBgr*)((u8*)bp->iptr + (bp->left * 3)))->grn);
			(bp->pixels[bp->count]).blu		= (f32)(((SBgr*)((u8*)bp->iptr + (bp->left * 3)))->blu);
			(bp->pixels[bp->count]).area	= bp->area;

		} else {
			// 32-bit bitmap
			(bp->pixels[bp->count]).red		= (f32)(((SBgra*)((u8*)bp->iptra + (bp->left * 4)))->red);
			(bp->pixels[bp->count]).grn		= (f32)(((SBgra*)((u8*)bp->iptra + (bp->left * 4)))->grn);
			(bp->pixels[bp->count]).blu		= (f32)(((SBgra*)((u8*)bp->iptra + (bp->left * 4)))->blu);
			(bp->pixels[bp->count]).alp		= (f32)(((SBgra*)((u8*)bp->iptra + (bp->left * 4)))->alp);
			(bp->pixels[bp->count]).area	= bp->area;
		}

		// Move over for the next point
		++bp->count;
	}




//////////
//
// 2 - upper-middle (see iGetSpannedPixelColors() above)
// It is known when this function is called that there is at least one, full, middle pixel
//
//////
	void iiBmp_scale_processSpannedPixel2(SBitmapProcess* bp)
	{
		s32 lnPixel;


		// For every middle pixel, apply these values
		for (lnPixel = bp->middleStartH; lnPixel < bp->middleFinishH; lnPixel++)
		{
			// Store this pixel data
			if (bp->src->bi.biBitCount == 24)
			{
				// 24-bit bitmap
				(bp->pixels[bp->count]).red		= (f32)(((SBgr*)((u8*)bp->iptr + (lnPixel * 3)))->red);
				(bp->pixels[bp->count]).grn		= (f32)(((SBgr*)((u8*)bp->iptr + (lnPixel * 3)))->grn);
				(bp->pixels[bp->count]).blu		= (f32)(((SBgr*)((u8*)bp->iptr + (lnPixel * 3)))->blu);
				(bp->pixels[bp->count]).area	= bp->height;

			} else {
				// 32-bit bitmap
				(bp->pixels[bp->count]).red		= (f32)(((SBgra*)((u8*)bp->iptra + (lnPixel * 4)))->red);
				(bp->pixels[bp->count]).grn		= (f32)(((SBgra*)((u8*)bp->iptra + (lnPixel * 4)))->grn);
				(bp->pixels[bp->count]).blu		= (f32)(((SBgra*)((u8*)bp->iptra + (lnPixel * 4)))->blu);
				(bp->pixels[bp->count]).alp		= (f32)(((SBgra*)((u8*)bp->iptra + (lnPixel * 4)))->alp);
				(bp->pixels[bp->count]).area	= bp->height;
			}

			// Move over for the next point
			++bp->count;
		}
	}




//////////
//
// 3 - upper-right (see iGetSpannedPixelColors() above)
// It is known when this function is called that there is at least a second row
//
//////
	void iiBmp_scale_processSpannedPixel3(SBitmapProcess* bp)
	{
		// Find out where this upper-left pixel falls
		bp->widthRight = bp->lrx - (f32)bp->right;		// It spans from the start of the right-most pixel to wherever it falls therein

		// Compute the area for this pixel component
		bp->area = bp->widthRight * bp->height;

		// Store this pixel data
		if (bp->src->bi.biBitCount == 24)
		{
			// 24-bit bitmap
			(bp->pixels[bp->count]).red		= (f32)(((SBgr*)((u8*)bp->iptr + (bp->right * 3)))->red);
			(bp->pixels[bp->count]).grn		= (f32)(((SBgr*)((u8*)bp->iptr + (bp->right * 3)))->grn);
			(bp->pixels[bp->count]).blu		= (f32)(((SBgr*)((u8*)bp->iptr + (bp->right * 3)))->blu);
			(bp->pixels[bp->count]).area	= bp->area;

		} else {
			// 32-bit bitmap
			(bp->pixels[bp->count]).red		= (f32)(((SBgra*)((u8*)bp->iptra + (bp->right * 4)))->red);
			(bp->pixels[bp->count]).grn		= (f32)(((SBgra*)((u8*)bp->iptra + (bp->right * 4)))->grn);
			(bp->pixels[bp->count]).blu		= (f32)(((SBgra*)((u8*)bp->iptra + (bp->right * 4)))->blu);
			(bp->pixels[bp->count]).alp		= (f32)(((SBgra*)((u8*)bp->iptra + (bp->right * 4)))->alp);
			(bp->pixels[bp->count]).area	= bp->area;
		}

		// Move over for the next point
		++bp->count;
	}




//////////
//
// 4 - middle-left (see iGetSpannedPixelColors() above)
// It is known when this function is called that there is at least an entire second row
//
//////
// 	void iValidatePoint(SBitmapProcess* bp, s32 tnX, s32 tnY)
// 	{
// 		if (tnX >= bp->bii->biWidth)
// 			__nop();
//
// 		if (tnY >= bp->bii->biHeight)
// 			__nop();
// 	}

	void iiBmp_scale_processSpannedPixel4(SBitmapProcess* bp)
	{
		s32 lnPixelY;


		// Repeat for each middle pixel
		for (lnPixelY = bp->middleStartV; lnPixelY <= bp->middleFinishV; lnPixelY++)
		{
			// Store the colors for this point
			if (bp->src->bi.biBitCount == 24)
			{
				// 24-bit bitmap
				(bp->pixels[bp->count]).red		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - (lnPixelY * bp->src->rowWidth) + (bp->left * 3)))->red);
				(bp->pixels[bp->count]).grn		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - (lnPixelY * bp->src->rowWidth) + (bp->left * 3)))->grn);
				(bp->pixels[bp->count]).blu		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - (lnPixelY * bp->src->rowWidth) + (bp->left * 3)))->blu);
				(bp->pixels[bp->count]).area	= bp->widthLeft;

			} else {
				// 32-bit bitmap
				(bp->pixels[bp->count]).red		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - (lnPixelY * bp->src->rowWidth) + (bp->left * 4)))->red);
				(bp->pixels[bp->count]).grn		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - (lnPixelY * bp->src->rowWidth) + (bp->left * 4)))->grn);
				(bp->pixels[bp->count]).blu		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - (lnPixelY * bp->src->rowWidth) + (bp->left * 4)))->blu);
				(bp->pixels[bp->count]).alp		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - (lnPixelY * bp->src->rowWidth) + (bp->left * 4)))->alp);
				(bp->pixels[bp->count]).area	= bp->widthLeft;
			}

			// Move over for the next point
			++bp->count;
		}
	}




//////////
//
// 5 - middle-middle (see iGetSpannedPixelColors() above)
// It is known when this function is called that there is at least a second row, and at least
// one pixel in the middle
//
//////
	void iiBmp_scale_processSpannedPixel5(SBitmapProcess* bp)
	{
		s32 lnPixelY, lnPixelX;


		// Iterate for each pixel row vertically
		for (lnPixelY = bp->middleStartV; lnPixelY <= bp->middleFinishV; lnPixelY++)
		{
			// And each individual pixel horizontally
			for (lnPixelX = bp->middleStartH; lnPixelX <= bp->middleFinishH; lnPixelX++)
			{
				// Store the colors for this point
				if (bp->src->bi.biBitCount == 24)
				{
					// 24-bit bitmap
					(bp->pixels[bp->count]).red		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - (lnPixelY * bp->src->rowWidth) + (lnPixelX * 3)))->red);
					(bp->pixels[bp->count]).grn		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - (lnPixelY * bp->src->rowWidth) + (lnPixelX * 3)))->grn);
					(bp->pixels[bp->count]).blu		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - (lnPixelY * bp->src->rowWidth) + (lnPixelX * 3)))->blu);
					(bp->pixels[bp->count]).area	= 1.0;

				} else {
					// 32-bit bitmap
					(bp->pixels[bp->count]).red		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - (lnPixelY * bp->src->rowWidth) + (lnPixelX * 4)))->red);
					(bp->pixels[bp->count]).grn		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - (lnPixelY * bp->src->rowWidth) + (lnPixelX * 4)))->grn);
					(bp->pixels[bp->count]).blu		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - (lnPixelY * bp->src->rowWidth) + (lnPixelX * 4)))->blu);
					(bp->pixels[bp->count]).alp		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - (lnPixelY * bp->src->rowWidth) + (lnPixelX * 4)))->alp);
					(bp->pixels[bp->count]).area	= 1.0;
				}

				// Move over for the next point
				++bp->count;
			}
		}
	}




//////////
//
// 6 - middle-right (see iGetSpannedPixelColors() above)
// It is known when this function is called that there is at least a second row, and a right pixel
//
//////
	void iiBmp_scale_processSpannedPixel6(SBitmapProcess* bp)
	{
		s32 lnPixelY;


		// Repeat for each middle pixel
		for (lnPixelY = bp->middleStartV; lnPixelY <= bp->middleFinishV; lnPixelY++)
		{
			// Store the colors for this point
			if (bp->src->bi.biBitCount == 24)
			{
				// 24-bit bitmap
				(bp->pixels[bp->count]).red		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - (lnPixelY * bp->src->rowWidth) + (bp->right * 3)))->red);
				(bp->pixels[bp->count]).grn		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - (lnPixelY * bp->src->rowWidth) + (bp->right * 3)))->grn);
				(bp->pixels[bp->count]).blu		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - (lnPixelY * bp->src->rowWidth) + (bp->right * 3)))->blu);
				(bp->pixels[bp->count]).area	= bp->widthRight;

			} else {
				// 32-bit bitmap
				(bp->pixels[bp->count]).red		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - (lnPixelY * bp->src->rowWidth) + (bp->right * 4)))->red);
				(bp->pixels[bp->count]).grn		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - (lnPixelY * bp->src->rowWidth) + (bp->right * 4)))->grn);
				(bp->pixels[bp->count]).blu		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - (lnPixelY * bp->src->rowWidth) + (bp->right * 4)))->blu);
				(bp->pixels[bp->count]).alp		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - (lnPixelY * bp->src->rowWidth) + (bp->right * 4)))->alp);
				(bp->pixels[bp->count]).area	= bp->widthRight;
			}

			// Move over for the next point
			++bp->count;
		}
	}




//////////
//
// 7 - lower-left (see iGetSpannedPixelColors() above)
// It is known when this function is called that there is at least a second row
//
//////
	void iiBmp_scale_processSpannedPixel7(SBitmapProcess* bp)
	{
		// Compute the area
		bp->height	= bp->lry - (f32)((s32)bp->lry);
		bp->area	= bp->widthLeft * bp->height;

		// Store the colors for this point
		if (bp->src->bi.biBitCount == 24)
		{
			// 24-bit bitmap
			(bp->pixels[bp->count]).red		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->left * 3)))->red);
			(bp->pixels[bp->count]).grn		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->left * 3)))->grn);
			(bp->pixels[bp->count]).blu		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->left * 3)))->blu);
			(bp->pixels[bp->count]).area	= bp->area;

		} else {
			// 32-bit bitmap
			(bp->pixels[bp->count]).red		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->left * 4)))->red);
			(bp->pixels[bp->count]).grn		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->left * 4)))->grn);
			(bp->pixels[bp->count]).blu		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->left * 4)))->blu);
			(bp->pixels[bp->count]).alp		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->left * 4)))->alp);
			(bp->pixels[bp->count]).area	= bp->area;
		}

		// Move over for the next point
		++bp->count;
	}




//////////
//
// 8 - lower-middle (see iGetSpannedPixelColors() above)
// It is known when this function is called that there is at least a second row, and at least one
// pixel in the middle
//
//////
	void iiBmp_scale_processSpannedPixel8(SBitmapProcess* bp)
	{
		s32 lnPixelX;


		// For every middle pixel, apply these values
		for (lnPixelX = bp->middleStartH; lnPixelX <= bp->middleFinishH; lnPixelX++)
		{
			// Store the colors for this point
			if (bp->src->bi.biBitCount == 24)
			{
				// 24-bit bitmap
				(bp->pixels[bp->count]).red		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (lnPixelX * 3)))->red);
				(bp->pixels[bp->count]).grn		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (lnPixelX * 3)))->grn);
				(bp->pixels[bp->count]).blu		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (lnPixelX * 3)))->blu);
				(bp->pixels[bp->count]).area	= bp->height;

			} else {
				// 32-bit bitmap
				(bp->pixels[bp->count]).red		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (lnPixelX * 4)))->red);
				(bp->pixels[bp->count]).grn		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (lnPixelX * 4)))->grn);
				(bp->pixels[bp->count]).blu		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (lnPixelX * 4)))->blu);
				(bp->pixels[bp->count]).alp		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (lnPixelX * 4)))->alp);
				(bp->pixels[bp->count]).area	= bp->height;
			}

			// Move over for the next point
			++bp->count;
		}
	}




//////////
//
// 9 - lower-right (see iGetSpannedPixelColors() above)
// It is known when this function is called that there is at least a second row, and a right pixel
//
//////
	void iiBmp_scale_processSpannedPixel9(SBitmapProcess* bp)
	{
		// Compute the area
		bp->area = bp->widthRight * bp->height;

		// Store the colors for this point
		if (bp->src->bi.biBitCount == 24)
		{
			// 24-bit bitmap
			(bp->pixels[bp->count]).red		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->right * 3)))->red);
			(bp->pixels[bp->count]).grn		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->right * 3)))->grn);
			(bp->pixels[bp->count]).blu		= (f32)(((SBgr*)((u8*)bp->iptrAnchor - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->right * 3)))->blu);
			(bp->pixels[bp->count]).area	= bp->area;

		} else {
			// 32-bit bitmap
			(bp->pixels[bp->count]).red		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->right * 4)))->red);
			(bp->pixels[bp->count]).grn		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->right * 4)))->grn);
			(bp->pixels[bp->count]).blu		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->right * 4)))->blu);
			(bp->pixels[bp->count]).alp		= (f32)(((SBgra*)((u8*)bp->iptrAnchora - ((bp->middleFinishV + 1) * bp->src->rowWidth) + (bp->right * 4)))->alp);
			(bp->pixels[bp->count]).area	= bp->area;
		}

		// Move over for the next point
		++bp->count;
	}




//////////
//
// Integers between means which maximum integer is touched?
// Basically, chopping off decimals reveals the range, such that values of
// 1.001 and 2.999 only touch integers 1 and 2, even though with rounding
// they would go from 1 to 3.  Their numerical roots are in only integers
// 1 and 2.
//
//////
	u32 iiBmp_scale_processGetIntegersBetween(f32 p1, f32 p2)
	{
		u32 lfMin, lfMax;


		// Grab the integer values (without rounding)
		lfMin = (u32)min(p1,p2);
		lfMax = (u32)max(p1,p2);

		// Indicate the number between
		return(lfMax - lfMin);
	}
