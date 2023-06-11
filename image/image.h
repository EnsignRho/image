//////////
//
// image.h
//
//////
//
// This file is licensed under the PBL v1.0, see pbl_v1.txt
//
//


#ifndef WINVER                  // Specifies that the minimum required platform is Windows XP.
    #define WINVER 0x0501           // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows XP.
    #define _WIN32_WINNT 0x0501     // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows XP.
    #define _WIN32_WINDOWS 0x0501   // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE               // Specifies that the minimum required platform is Internet Explorer 6.0.
    #define _WIN32_IE 0x0600        // Change this to the appropriate value to target other versions of IE.
#endif

//#define WIN32_LEAN_AND_MEAN       // Removed because of gdiplus
#include <windows.h>
#include <gdiplus.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <sys/locking.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <share.h>
#include <math.h>
#include <errno.h>
#include <shlwapi.h>

using namespace Gdiplus;

#include "common_types.h"

cs8* iiAddBackslash(cs8* tcTest)
{
	s32 lnLength;

	// Scan backwards from the end
	lnLength = (s32)strlen(tcTest);

	// If it doesn't end with a \, need to add one
	if (lnLength > 0 && tcTest[lnLength - 1] != '\\')
		return "\\";

	// No backslash
	return "";
}

#include "errors.h"
#include "ll1.h"
#include "ll2.h"
#include "datum1.h"
#include "datum2.h"
#include "builder1.h"
#include "builder2.h"
#include "time1.h"
#include "time2.h"
#include "disk1.h"
#include "disk2.h"
#include "bitmaps1.h"
#include "bitmaps2.h"

#define IMAGE_API __declspec(dllexport)

