//////////
//
// image.cpp
//
//////
//
// This code is licensed under the PBL v1.0, see pbl_v1.txt.
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
	} u;




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
	IMAGE_API s32 bmp_crop_to_content(s32 tnHandle, int tnRgb, int tnType)
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
		bmpCropped = iBmp_cropToContent(u.bmp, tnRgb, tnType);

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

