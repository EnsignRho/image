//////////
//
// /libsf/utils/common/cpp/disk.cpp
//
//////
//    _     _ _     _____ _____
//   | |   (_) |__ / ____|  ___|
//   | |   | | '_ \\___ \|  __|
//   | |___| | |_) |___) | |
//   |_____|_|_.__/_____/|_|
//
//   Liberty Software Foundation
// and the Village Freedom Project
//   __     _______     ____
//   \ \   / /  ___| __|  _ \
//    \ \ / /| |_ | '__| |_) |
//     \ V / |  _|| |  |  __/
//      \_/  |_|  |_|  |_|
//
//////
// Version 0.55
// Copyright (c) 2014-2017 by Rick C. Hodgin
//////
// Last update:
//	   Mar.05.2015 - Initial creation
//////
// Change log:
//	   Mar.05.2015 - Initial creation
//////
//
// This document is released as Liberty Software under a Repeat License, as governed
// by the Public Benefit License v1.0 or later (PBL).
//
// The PBL is a public domain license with a caveat:  self accountability unto God.
// You are free to use, copy, modify and share this software for any purpose, however,
// it is the desire of those working on this project that the software remain open.
// It is our request that you maintain it that way.  This is not a legal request unto
// our court systems, but rather a personal matter between you and God.  Our talents
// were received from God, and given to you through this forum.  And it is our wish
// that those talents reach out to as many as possible in a form allowing them to wield
// this content for their own betterment, as you are now considering doing.  And whereas
// we could've forced the issue through something like a copyleft-protected release, the
// truth is we are all giving an account of our lives unto God continually by the daily
// choices we make.  And here again is another case where you get to demonstrate your
// character unto God, and unto your fellow man.
//
// Jesus came to this Earth to set the captives free, to save men's eternal souls from
// the punishment demanded by our sin.  Each one of us is given the opportunity to
// receive Him in this world and be saved.  Whether we choose to receive Him, and follow
// Him, and purpose our lives on the goals He has for each of us (all of which are
// beneficial for all involved), is one way we reveal our character continually.  God
// sees straight through to the heart, bypassing all of the words, all of the facades.
// He is our Creator, and He knows who we are truly.
//
// Jesus is called "Christ" because He saves men from the eternal flames of Hell, the
// just punishment of rebellion against God (rebellion against truth) by eternal beings,
// which each of us are.
//
// Do not let His free gift escape you because of some desire to shortcut what is right
// in your life. Instead, do what is right, and do it because He is who He is, and what
// He has done to save us from such a fate.  Demonstrate to Him, to me, and to all of
// mankind, and to all of the Heavenly hosts, exactly who you are on the inside.  Receive
// Jesus Christ into your heart, learn of Him, of how He came here to guide us into that
// which is of His Kingdom and will be forever hereafter we leave this world.  Do this,
// and live.
//
// Every project released by Liberty Software Foundation will include a copy of the
// pbl.txt document, which can be found at http://www.libsf.org/licenses/.
//
// For additional information about this project, or to view the license, see:
//
//     http://www.libsf.org/
//     http://www.libsf.org/licenses/
//     http://www.visual-freepro.org/vjr/indexmain.html
//     http://www.visual-freepro.org/wiki/index.php/PBL
//
// Thank you.  And may The Lord bless you richly as you lift up your life, your
// talents, your gifts, your praise, unto Him.  In Jesus' name I pray.  Amen.
//
//////
//   _____   _       _
//  |  __ \ (_)     | |
//  | |  | | _  ___ | | __
//  | |  | || |/ __|| |/ /
//  | |__| || |\__ \|   <
//  |_____/ |_||___/|_|\_\
//     Disk Algorithms.
//////////


#define _EXCLUDE_iFile_parseIntoLines


//////////
//
// Called to open the file using custom share settings, or explicitly shared,
// or explicitly exclusive.
//
//	tnType	-- Indicates how the file should be opened (or created).
//			   The following options can be |'d together, like (_O_CREAT | _O_SHORT_LIVED):
//
// 					Access:
// 									_O_RDONLY				Read-only access
// 									_O_RDWR					Read-write access
// 									_O_WRONLY				Write-only access
// 
// 					Data format:
// 									_O_BINARY				Opens in raw/untranslated mode
// 									_O_TEXT					Opens in text/translated mode
// 
// 					General flags:
// 									_O_CREAT				Creates a file (file must not exist)
// 										+--	_O_SHORT_LIVED	Prevents flushing contents to disk if possible
// 										+--	_O_TEMPORARY	Deletes the file when closed
// 										+--	_O_EXCL			Creates a file that's opened exclusively
// 
// 									_O_APPEND				Opens ready to append data
// 									_O_NOINHERIT			Cannot share file handle
// 									_O_RANDOM				A hint that data access is mostly random
// 									_O_SEQUENTIAL			Sequential file access
// 									_O_TRUNC				Truncates a file to 0 bytes when opened
// 
// 					Data type (if not specified, opens in standard ASCII type)
// 									_O_U16TEXT				UTF-16 data
// 									_O_U8TEXT				UTF-8 data
// 									_O_WTEXT				Unicode data
//
//	tnShare	-- Indicates how the data
//
//					Share mode:
// 									_SH_DENYNO				Deny none (allow all shared access)
// 									_SH_DENYRD				Deny read
// 									_SH_DENYWR				Deny write
// 									_SH_DENYRW				Deny read-write
//////
	s32 iDisk_open(cs8* tcPathname, s32 tnType, s32 tnShare, bool tlCreateIfCannotOpen, bool* error, u32* errorNum)
	{
		s32	lnFh, lnErrno;


		//////////
		// Attempt to open if valid
		//////
			if (tcPathname)
			{
				// Try to open existing
				lnFh = _sopen(tcPathname, tnType, tnShare);
				if (lnFh < 0)
				{
					// Grab the errno
					lnErrno = errno;

					// Try to create it
					if (tlCreateIfCannotOpen)
						goto try_to_create;

					// Error opening
					switch (lnErrno)
					{
						default:
						case EACCES:	// The path is a directory, or the file is read-only, but an "open-for-write" was specified
						case EEXIST:	// _O_CREAT and _O_EXCL flags were specified, but filename already exists
						case EINVAL:	// Invalid type or share argument
						case EMFILE:	// No more file descriptors are available
							// All valid errors, indicate failure
							break;

						case ENOENT:
							// File or path was not found
							if (tlCreateIfCannotOpen)
							{
try_to_create:
								// Try to create
								lnFh = _sopen(tcPathname, tnType | _O_CREAT, tnShare);
//								lnFh = _creat(tcPathname, _S_IREAD | _S_IWRITE);
								// Right now, it's either open or not
								if (lnFh >= 0)
								{
									// No error
									if (error)			*error		= false;
									if (errorNum)		*errorNum	= _ERROR_OKAY;

								} else {
									// Error
									if (error)			*error		= true;
									if (errorNum)		*errorNum	= _ERROR_DISK_OPEN_ERROR;
								}
							}
							break;
					}

				} else {
					// It's open
					if (error)			*error		= false;
					if (errorNum)		*errorNum	= _ERROR_OKAY;
				}

				// Indicate our (potentially new) status
				return(lnFh);
			}


		//////////
		// If we get here, could not open (or possibly create)
		//////
			return(-1);
	}

	s32 iDisk_openAs(cs8* tcPathname, s32 tnType, bool tlCreateIfCannotOpen, bool tlExclusive, bool* error, u32* errorNum)
	{
		if (tlExclusive)	return(iDisk_openExclusive	(tcPathname, tnType, tlCreateIfCannotOpen, error, errorNum));
		else				return(iDisk_openShared		(tcPathname, tnType, tlCreateIfCannotOpen, error, errorNum));
	}

	s32 iDisk_openShared(cs8* tcPathname, s32 tnType, bool tlCreateIfCannotOpen, bool* error, u32* errorNum)
	{
		// Attempt to open if valid
		if (tcPathname)
			return(iDisk_open(tcPathname, tnType, _SH_DENYNO, tlCreateIfCannotOpen, error, errorNum));


		// If we get here, invalid filename
		return(-1);
	}

	s32 iDisk_openExclusive(cs8* tcPathname, s32 tnType, bool tlCreateIfCannotOpen, bool* error, u32* errorNum)
	{
		// Attempt to open if valid
		if (tcPathname)
			return(iDisk_open(tcPathname, tnType | _O_EXCL, _SH_DENYRW, tlCreateIfCannotOpen, error, errorNum));


		// If we get here, invalid filename
		return(-1);
	}

	s32 iDisk_deleteFile(cs8* tcPathname)
	{
		return((s32)DeleteFile(tcPathname));
	}




//////////
//
// Called to close the indicated file handle
//
//////
	s32 iDisk_close(s32 tnFile, bool* error, u32* errorNum)
	{
		s32 lnResult, lnErrno;


		// Close
		lnResult = _close(tnFile);
		if (lnResult == 0)
		{
			// No error
			if (error)			*error		= false;
			if (errorNum)		*errorNum	= _ERROR_OKAY;

		} else {
			// An error occurred
			if (error)			*error		= TRUE;
			if (errorNum)		*errorNum	= _ERROR_DISK_CLOSE_ERROR;

			// For debugging
			lnErrno = errno;
		}

		// Indicate the result
		return(lnResult);
	}




//////////
//
// Called to see if the indicated file exists
//
//////
	bool iDisk_doesFileExist(s8* tcPathname)
	{
		HANDLE				h;
		WIN32_FIND_DATA		wfd;


		// Try to find the file
		h = FindFirstFile(tcPathname, &wfd);
		if (h != INVALID_HANDLE_VALUE)
			FindClose(h);

		// Indicate success or failure
		return(h != INVALID_HANDLE_VALUE);
	}




//////////
//
// Called to create a temporary pathname
//
//////
	s32 iDisk_getTemporaryPathname(s8* tcPathnameOut, cs8* tcPrefix, u32 tnUniqueNumber, cs8* tcPostfix, cs8* tcExtension)
	{
		u32		lnI;
		s8		tempPath[_MAX_PATH];
		s8		buffer[16];


		//////////
		// Temporary path
		//////
			memset(tempPath, 0, sizeof(tempPath));
			GetTempPath(sizeof(tempPath), tempPath);


		//////////
		// Validate parameters
		//////
			if (tnUniqueNumber == -1 && !tcPrefix && !tcPostfix)
				tnUniqueNumber = 0;		// They didn't provide anything, so begin at the beginning


		//////////
		// Generate the temporary file name
		//////
			// If we have a number, repeat so long as the file exists
			for (lnI = 0; lnI == 0 || iDisk_doesFileExist(tcPathnameOut); ++lnI)
			{
				// Unique number
				if (tnUniqueNumber != -1)		sprintf(buffer, "%08x", tnUniqueNumber + lnI);
				else							buffer[0] = 0;

				// Actual filename
				sprintf(tcPathnameOut, "%s%s%s%s.%s", tempPath, ((tcPrefix) ? tcPrefix : ""), buffer, ((tcPostfix) ? tcPostfix : ""), ((tcExtension) ? tcExtension : "tmp"));

				// Are we done?
				if (tnUniqueNumber == -1)
					break;
			}


		//////////
		// Copy
		//////
			return(tnUniqueNumber);

	}




//////////
//
// Get the current directory
//
//////
	s32 iDisk_getCurrentDirectory(s8* tcPathnameOut)
	{
		s32		lnLength;


		// Make sure the environment is sane
		if (tcPathnameOut)
		{
			// Get the path
			memset(tcPathnameOut, 0, sizeof(tcPathnameOut));
			GetCurrentDirectory(_MAX_PATH, tcPathnameOut);
			memcpy(tcPathnameOut + (lnLength = (s32)strlen(tcPathnameOut)), "\\\0", 2);

			// Signify
			return lnLength + 1;
		}

		// Invalid
		return -1;
	}




//////////
//
// Called to create the output path:
//
//	tcPath			= "c:\path\to\"
//	tcFilename		= "filename.ext"
//	tcPathnameOut	= "c:\path\to\filename.ext"
//
//////
	s8* iDisk_buildPath(s8* tcPathnameOut, cs8* tcPath, cs8* tcFilename)
	{
		s32		lnLengthPath, lnLengthFilename;


		// Make sure the environment is sane
		if (tcPathnameOut && tcPath && tcFilename)
		{
			// Get the lengths
			lnLengthPath		= (s32)strlen(tcPath);
			lnLengthFilename	= (s32)strlen(tcFilename);

			// Copy the path
			if (lnLengthPath > 0)
			{
				// Copy the path part
				memcpy(tcPathnameOut, tcPath, lnLengthPath);

				// Make sure it has a trailing backslash
				if (tcPathnameOut[lnLengthPath - 1] != '\\')
					tcPathnameOut[lnLengthPath++] = '\\';
			}

			// Add the filename
			if (lnLengthFilename > 0)
				memcpy(tcPathnameOut + lnLengthPath, tcFilename, lnLengthFilename);

			// NULL-terminate
			tcPathnameOut[lnLengthPath + lnLengthFilename] = 0;

			// Signify
			return tcPathnameOut;
		}

		// Invalid
		return NULL;
	}




//////////
//
// Obtain the file size
//
//////
	s64 iDisk_getFileSize(s8* tcPathname)
	{
		s64					lnFilesize;
		HANDLE				hFind;
		WIN32_FIND_DATA		fd = { };


		// Find the first file
		hFind = FindFirstFile(tcPathname, &fd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			lnFilesize = (s64)((u64)fd.nFileSizeHigh << 32 | (u64)fd.nFileSizeLow);
			FindClose(hFind);

		} else {
			// Invalid
			lnFilesize = -1;
		}

		// Signify
		return lnFilesize;
	}

	s64 iDisk_getFileSize(s32 tnFile, bool* error, u32* errorNum)
	{
		// Note:  Will return -1 if there's an error
		if (tnFile > 0)		return(_filelengthi64(tnFile));
		else				return((s64)tnFile);
	}




//////////
//
// Obtain the current file position
//
//////
	s64 iDisk_getFilePosition(s32 tnFile, bool* error, u32* errorNum)
	{
		s32	lnErrno;
		s64 lnResult;


		// Seek
		lnResult = _lseeki64(tnFile, 0, SEEK_CUR);
		if (lnResult == 0)
		{
			// No error
			if (error)			*error		= false;
			if (errorNum)		*errorNum	= _ERROR_OKAY;

		} else {
			// Error
			lnErrno = errno;
			if (error)			*error		= true;
			if (errorNum)		*errorNum	= _ERROR_DISK_SEEK_ERROR;
		}
		return(lnResult);
	}




//////////
//
// Obtain the file's extension
//
//////
	s32 iDisk_getFileExtension(cs8* tcPathname, s8* tcExtensionOut, bool tlIncludePeriod)
	{
		s32		lnI, lnLength;


		// Make sure our environment is sane
		if (!tcPathname || !tcExtensionOut)
			return -1;

		// Iterate through each character from the end backwards
		lnLength = (s32)strlen(tcPathname);
		for (lnI = lnLength - 1; lnI >= 0; --lnI)
		{
			// Is it a period?
			if (tcPathname[lnI] == '.')
			{
				// Calculate our actual length
				lnLength = (lnLength - lnI);

				// Copy that much
				if (tlIncludePeriod)	memcpy(tcExtensionOut, tcPathname + lnI,     lnLength);
				else					memcpy(tcExtensionOut, tcPathname + lnI + 1, --lnLength);

				// NULL-terminate
				tcExtensionOut[lnLength] = 0;

				// Signify
				return lnLength;
			}
		}

		// If we get here, not found
		tcExtensionOut[0] = 0;
		return 0;
	}




//////////
//
// Called to set the file position to the indicated offset
//
//////
	s64 iDisk_setFilePosition(s32 tnFile, s64 tnSeekOffset, bool* error, u32* errorNum)
	{
		s32	lnErrno;
		s64 lnResult;


		// Seek
		lnResult = _lseeki64(tnFile, tnSeekOffset, SEEK_SET);
		if (lnResult == tnSeekOffset)
		{
			// No error
			if (error)			*error		= false;
			if (errorNum)		*errorNum	= _ERROR_OKAY;

		} else {
			// Error
			lnErrno = errno;
			if (error)			*error		= true;
			if (errorNum)		*errorNum	= _ERROR_DISK_SEEK_ERROR;
		}
		return(lnResult);
	}




//////////
//
// Called to flush any buffers to disk
//
//////
	s32 iDisk_commit(s32 tnFile, bool* error, u32* errorNum)
	{
		s32	lnResult;


		//////////
		// Commit uncommitted buffers
		//////
			lnResult = _commit(tnFile);
			if (lnResult == 0)
			{
				// No error
				if (error)			*error		= false;
				if (errorNum)		*errorNum	= _ERROR_OKAY;

			} else {
				// Error
				if (error)			*error		= true;
				if (errorNum)		*errorNum	= _ERROR_DISK_COMMIT_ERROR;
			}


		//////////
		// Indicate our result
		//////
			return(lnResult);

	}




//////////
//
// Called to optionally seek, then read in the indicated size.
// If no seek is required, send a negative value for tnSeekOffset.
//
//////
	s32 iDisk_read(s32 tnFile, s64 tnSeekOffset, void* tcData, s32 tnReadCount, bool* tlError, u32* tnErrorNum)
	{
		s32	lnResult;
		s64	lnSeekOffset;


		//////////
		// Optionally seek
		//////
			if (tnSeekOffset >= 0)
			{

				//////////
				// Seek
				//////
					lnSeekOffset = _lseeki64(tnFile, tnSeekOffset, SEEK_SET);


				//////////
				// Are we there?
				//////
					if (lnSeekOffset != tnSeekOffset)
					{
						if (tlError)		*tlError	= true;
						if (tnErrorNum)		*tnErrorNum	= _ERROR_DISK_SEEK_ERROR;
						return(-1);
					}

			}


		//////////
		// Read
		//////
			lnResult = _read(tnFile, tcData, tnReadCount);
			if (lnResult == tnReadCount)
			{
				// No error
				if (tlError)		*tlError	= false;
				if (tnErrorNum)		*tnErrorNum	= _ERROR_OKAY;

			} else {
				// Error
				if (tlError)		*tlError	= true;
				if (tnErrorNum)		*tnErrorNum	= _ERROR_DISK_READ_ERROR;
			}
			return(lnResult);
	}




//////////
//
// Called to optionally seek, then write out the indicated size.
// If no seek is required, send a negative value for tnSeekOffset.
//
//////
	s32 iDisk_write(s32 tnFile, s64 tnSeekOffset, void* tcData, s32 tnWriteCount, bool* tlError, u32* tnErrorNum)
	{
		s32	lnResult;
		s64	lnSeekOffset;


		//////////
		// Optionally seek
		//////
			if (tnSeekOffset >= 0)
			{

				//////////
				// Seek
				//////
					lnSeekOffset = _lseeki64(tnFile, tnSeekOffset, SEEK_SET);


				//////////
				// Are we there?
				//////
					if (lnSeekOffset != tnSeekOffset)
					{
						if (tlError)		*tlError	= true;
						if (tnErrorNum)		*tnErrorNum	= _ERROR_DISK_SEEK_ERROR;
						return(_ERROR_DISK_SEEK_ERROR);
					}

			}


		//////////
		// Read
		//////
			lnResult = _write(tnFile, tcData, tnWriteCount);
			if (lnResult == 0)
			{
				// No error
				if (tlError)		*tlError	= false;
				if (tnErrorNum)		*tnErrorNum	= _ERROR_OKAY;

			} else {
				// Error
				if (tlError)		*tlError	= true;
				if (tnErrorNum)		*tnErrorNum	= _ERROR_DISK_WRITE_ERROR;
			}
			return(lnResult);

	}




//////////
//
// Called to lock, read, and optionally unlock a file with retry callback
//
//////
	s32 iDisk_readShared_withRetryCallback(SBuilder* lockRoot, s32 tnFile, s64 tnSeekOffset, void* tcData, s32 tnReadCount,  bool* tlError, u32* tnErrorNum, uptr tnCallbackFunction, uptr tnExtra, SDiskLock** diskLock, bool tlUnlockAfter)
	{
		return(iiDisk_rwShared_withRetryCallback_common(lockRoot, tnFile, tnSeekOffset, tcData, tnReadCount, tlError, tnErrorNum, tnCallbackFunction, tnExtra, diskLock, tlUnlockAfter, _ERROR_UNABLE_TO_LOCK_FOR_READ, true));
	}




//////////
//
// Called to lock, write, and optionally unlock a file with retry callback
//
//////
	s32 iDisk_writeShared_withRetryCallback(SBuilder* lockRoot, s32 tnFile, s64 tnSeekOffset, void* tcData, s32 tnWriteCount, bool* tlError, u32* tnErrorNum, uptr tnCallbackFunction, uptr tnExtra, SDiskLock** diskLock, bool tlUnlockAfter)
	{
		return(iiDisk_rwShared_withRetryCallback_common(lockRoot, tnFile, tnSeekOffset, tcData, tnWriteCount, tlError, tnErrorNum, tnCallbackFunction, tnExtra, diskLock, tlUnlockAfter, _ERROR_UNABLE_TO_LOCK_FOR_WRITE, false));
	}




//////////
//
// Common operation for iDisk_readShared_withRetryCallback() and iDisk_writeShared_withRetryCallback()
//
//////
	s32 iiDisk_rwShared_withRetryCallback_common(SBuilder* lockRoot, s32 tnFile, s64 tnSeekOffset, void* tcData, s32 tnCount, bool* tlError, u32* tnErrorNum, uptr tnCallbackFunction, uptr tnExtra, SDiskLock** diskLock, bool tlUnlockAfter, u32 tnErrorNumIfError, bool tlRead)
	{
		s32			lnNum;
		SDiskLock*	dl;


		//////////
		// Initialize
		//////
			if (tlError)		*tlError	= false;
			if (tnErrorNum)		*tnErrorNum	= _ERROR_OKAY;


		//////////
		// Lock
		//////
			dl = iDisk_lock_range_retryCallback(lockRoot, tnFile, tnSeekOffset, tnCount, tnCallbackFunction, tnExtra);
			if (!dl)
			{
				// Error locking
				if (tlError)		*tlError = true;
				if (tnErrorNum)		*tnErrorNum = tnErrorNumIfError;

				// Indicate failure
				return(-1);
			}


		//////////
		// Read/write
		//////
			if (tlRead)		lnNum = iDisk_read (tnFile, -1, tcData, tnCount, tlError, tnErrorNum);
			else			lnNum = iDisk_write(tnFile, -1, tcData, tnCount, tlError, tnErrorNum);
			// Right now, lnNum and error are our keys ... if lnNum == tnCount and !error, it was a success


		//////////
		// Unlock
		//////
			if (tlUnlockAfter)
			{
				// Unlocking
				if (diskLock)
					*diskLock = NULL;

				// Unlock
				iDisk_unlock(lockRoot, dl);

			} else {
				// Not unlocking, signal the disk lock
				if (diskLock)
					*diskLock = dl;
			}


		//////////
		// Indicate the number written
		//////
			return(lnNum);
	}




//////////
//
// Called to duplicate the indicated file handle (if possible).
//
//////
	s32 iDisk_duplicateFileHandle(s32 tnFile)
	{
		// The OS will return a duplicate handle, or -1
		return(_dup(tnFile));
	}




//////////
//
// Directory functions
//
//////
	bool iDisk_doesPathExist(cs8* tcPathname)
	{
		return(((PathFileExists(tcPathname)) ? true : false));
	}




//////////
//
// Get the full directory listing
//
//////
	cs8		cgc_dot[]		= ".";
	cs8		cgc_dot_dot[]	= "..";

	SBuilder* iDisk_directoryListing_get(SBuilder* dirAddTo, cs8* tcFolder, cs8* tcFilespec, bool tlSortList)
	{
		bool				isValid;
		s32					lnLength;
		HANDLE				hFind;
		SBuilder*			dir;	// SDirEntry
		SDirListingEntry*	dle;
		WIN32_FIND_DATA		fd;
		s8					pathname[_MAX_PATH];


		// Create or reuse a builder
		if (!dirAddTo)		iBuilder_createAndInitialize(&dir);
		else				dir = dirAddTo;

		// Search for the first file
		sprintf(pathname, "%s%s%s", tcFolder, iiAddBackslash(tcFolder), tcFilespec);
		hFind	= FindFirstFile(pathname, &fd);
		isValid	= (hFind != INVALID_HANDLE_VALUE);

		// Iterate while we have files
		while (isValid)
		{
			// Skip . and ..
			if (memcmp(fd.cFileName, cgc_dot, sizeof(cgc_dot)) != 0 && memcmp(fd.cFileName, cgc_dot_dot, sizeof(cgc_dot_dot)) != 0)
			{
				// Store the information
				dle = (SDirListingEntry*)iBuilder_allocateBytes(dir, sizeof(SDirListingEntry));
				if (!dle)
					break;

				// Grab our times
				FileTimeToSystemTime(&fd.ftCreationTime,	&dle->created);
				FileTimeToSystemTime(&fd.ftLastAccessTime,	&dle->accessed);
				FileTimeToSystemTime(&fd.ftLastWriteTime,	&dle->modified);

				// Construct the 64-bit filesize
				dle->fileSize = ((u64)fd.nFileSizeHigh << 32) | (u64)fd.nFileSizeLow;

				// Copy the filename
				lnLength = sprintf(pathname, "%s%s%s", tcFolder, iiAddBackslash(tcFolder), fd.cFileName);
				iDatum_duplicate(&dle->pathname, pathname, lnLength);
			}

			// Try next
			isValid = (FindNextFile(hFind, &fd) != 0);
		}
		FindClose(hFind);

		// Sort if need be
		if (tlSortList)
		{
			qsort(dir->buffer, dir->populatedLength / sizeof(SDirListingEntry), sizeof(SDirListingEntry), iiDisk_directoryListing_get__qsort);
		}

		// Signify
		return dir;
	}

	int iiDisk_directoryListing_get__qsort(const void* left, const void* right)
	{
		SDirListingEntry* l = (SDirListingEntry*)left;
		SDirListingEntry* r = (SDirListingEntry*)right;

		return iDatum_compare(l->pathname, r->pathname);
	}




//////////
//
// Release the previously allocated directory listing
//
//////
	void iDisk_directoryListing_free(SBuilder* dirs)
	{
		s32					lnI;
		SDirListingEntry*	dle;


		// Iterate forward
		for (lnI = 0, dle = (SDirListingEntry*)dirs->buffer; lnI < (s32)dirs->populatedLength; lnI += sizeof(SDirListingEntry), ++dle)
			iDatum_delete(&dle->pathname);

		// Release the builder
		iBuilder_freeAndRelease(&dirs);
	}




//////////
//
// Directory find first, filtered by attributes (if present, see iiDisk_find_filterByAttributes_test()):
//
//////
	// Returns a handle
	SBuilder* iDisk_find_first(cs8* tcPathname, cs8* tcPattern, cs8* tcAttributes, bool tlRecurseSubDirs, s8* tcPathname_out, s8* tcPath_out, s8* tcFilename_out, WIN32_FIND_DATA* fd)
	{
		SBuilder*	findData;
		SDiskFind*	df;
		s8			curdir[_MAX_PATH];


		// Make sure our environment is sane
		findData = NULL;
		if (PathFileExists(tcPathname) && (iBuilder_createAndInitialize(&findData), findData) && (df = (SDiskFind*)iBuilder_allocateBytes(findData, sizeof(SDiskFind))))
		{
			// Setup
			GetCurrentDirectory(_MAX_PATH, curdir);
			SetCurrentDirectory(tcPathname);
			if (!tcPattern)			tcPattern	 = "*.*";
			if (!tcAttributes)		tcAttributes = "N";

			// Store the disk info
			StrCpyN(df->path, tcPathname, _MAX_PATH);
			iDatum_duplicate(&df->pattern,		tcPattern);
			iDatum_duplicate(&df->attributes,	tcAttributes);

			// Locate first file
			df->hFind = FindFirstFile(tcPattern, &df->fd);
			iiDisk_find_filterByAttributes(&findData, df, tlRecurseSubDirs, tcPathname_out, tcPath_out, tcFilename_out, fd);

			// Restore path
			SetCurrentDirectory(curdir);

			// Indicate our find result
			return(findData);

		}

		// If we get here, invalid
		if (findData)
			iBuilder_freeAndRelease(&findData);

		// Indicate failure
		return(NULL);
	}




//////////
//
// Directory find next
//
//////

	// Returns a handle if the find is still valid
	SBuilder* iDisk_find_next(SBuilder* findData, s8* tcPathname_out, s8* tcPath_out, s8* tcFilename_out, WIN32_FIND_DATA* fd)
	{
//		SDiskFind*	df;


		// Undeveloped code
		__debugbreak();

		// Make sure our environment is sane
		if (!iiDisk_find_isValid(findData))
			return(NULL);

		// Continue on where we left off
// 		df->hFind = FindFirstFile(cdxName, &ffd);
// 		return(((df->hFind == INVALID_HANDLE_VALUE) ? 0 : 1));
		return(NULL);
	}




//////////
//
// Directory functions
//
//////
	bool iDisk_find_cancel(SBuilder* findData)
	{
		SDiskFind* df;


		// Make sure our environment is sane
		if (iiDisk_find_isValid(findData))
		{
			// Delete the entire find structure
			df = (SDiskFind*)(findData->data + findData->populatedLength - sizeof(SDiskFind));
			while (df)
				df = iiDisk_find_delete_df(&findData, df);

			// Indicate success
			return(true);
		}

		// Failure
		return(false);
	}

	bool iiDisk_find_isValid(SBuilder* findData)
	{
		// Has to be valid, populated, and a multiple of the SDiskFind structure
		return(findData && findData->data && findData->populatedLength > 0 && findData->populatedLength % sizeof(SDiskFind) == 0);
	}

	SDiskFind* iiDisk_find_delete_df(SBuilder** findDataRoot, SDiskFind* df)
	{
		SBuilder* findData;


		// Grab the builder
		findData = *findDataRoot;

		// Close out the find
		FindClose(df->hFind);

		// Delete the datums
		iDatum_delete(&df->attributes,	false);
		iDatum_delete(&df->pattern,		false);

		// Back off this entry
		findData->populatedLength -= sizeof(SDiskFind);
		if (findData->populatedLength == 0)
		{
			// We're done
			iBuilder_freeAndRelease(findDataRoot);

			// Clear out the df
			df = NULL;

		} else {
			// Still room
			--df;
		}

		// Indicate our new position
		return(df);
	}

	void iiDisk_find_filterByAttributes(SBuilder** findDataRoot, SDiskFind* df, bool tlRecurseSubDirs, s8* tcPathname_out, s8* tcPath_out, s8* tcFilename_out, WIN32_FIND_DATA* fd)
	{
		s32			lnLength;
		SBuilder*	findData;


		// Enter a loop to iterate multiple directories if needed
		if ((findData = *findDataRoot))
		{
			// Repeat through different levels
			while (df)
			{
				// Enter a loop to iterate if next file(s) needed
				while (df->hFind != INVALID_HANDLE_VALUE)
				{
					// See if the current fd entry has
					if (iiDisk_find_filterByAttributes_test(df, &df->fd))
					{
						// Copy the whole path+filename
						if (tcPathname_out)
						{
							// Reset everything and copy
							memset(tcPathname_out, 0, _MAX_PATH);
							lnLength = sprintf(tcPathname_out, "%s", df->path);

							// Append the filename
							StrCpyN(tcPathname_out + lnLength, df->fd.cFileName, _MAX_PATH);
						}

						// Copy the path portion
						if (tcPath_out)
						{
							// Reset everything and copy
							memset(tcPath_out, 0, _MAX_PATH);
							StrCpyN(tcPath_out, df->path, _MAX_PATH);
						}

						// Copy the filename portion
						if (tcFilename_out)
						{
							memset(tcFilename_out, 0, _MAX_PATH);
							StrCpyN(tcFilename_out, df->fd.cFileName, _MAX_PATH);
						}

						// Copy the info from Windows
						if (fd)
							memcpy(&fd, &df->fd, sizeof(*fd));

						// We're done
						return;
					}

					// Skip to the next file
					if (!FindNextFile(df->hFind, &df->fd))
						df->hFind = INVALID_HANDLE_VALUE;
				}

				// Skip to the next df
				while (1)
				{
					// Delete this level, and go up
					if (!(df = iiDisk_find_delete_df(findDataRoot, df)))
					{
						// Reset everything
						if (tcPathname_out)		memset(tcPathname_out,	0, _MAX_PATH);
						if (tcPath_out)			memset(tcPath_out,		0, _MAX_PATH);
						if (tcFilename_out)		memset(tcFilename_out,	0, _MAX_PATH);
						if (fd)					memset(&fd,				0, sizeof(*fd));

						// All done
						return;
					}

					// Continue on with the next match
					if (FindNextFile(df->hFind, &df->fd))
						break;
				}
				// When we get here, we're continuing
			}
		}
	}




//////////
//
// Filters the current file based on the attributes
//
//		A - Archive
//		D - Directory
//		E - Encrypted
//		H - Hidden
//		N - Non-directory (regular file, can have other attributes)
//		P - Compressed
//		R - Read only
//		S - System
//		T - Temporary
//		Z - No attributes
//
//////
	bool iiDisk_find_filterByAttributes_test(SDiskFind* df, WIN32_FIND_DATA* fd)
	{
		s32 lnI;


		// Iterate through each attribute and see if it's there
		for (lnI = 0; lnI < df->attributes.length; ++lnI)
		{
			switch (df->attributes.data[lnI])
			{
				case 'A':	// Archive
					if ((fd->dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0)
						return(true);
					break;

				case 'D':	// Directory
					if ((fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
						return(true);
					break;

				case 'E':	// Encrypted
					if ((fd->dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED) != 0)
						return(true);
					break;

				case 'H':	// Hidden
					if ((fd->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0)
						return(true);
					break;

				case 'N':	// Non-directories
					if ((fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
						return(true);
					break;

				case 'P':	// Compressed
					if ((fd->dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0)
						return(true);
					break;

				case 'R':	// Read only
					if ((fd->dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0)
						return(true);
					break;

				case 'S':	// System
					if ((fd->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0)
						return(true);
					break;

				case 'T':	// Temporary
					if ((fd->dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0)
						return(true);
					break;

				case 'Z':	// Normal
					if ((fd->dwFileAttributes & FILE_ATTRIBUTE_NORMAL) != 0)
						return(true);
					break;

				default:
					// Should never happen, but just ignore the attribute
					break;
			}
		}

		// If we get here, it didn't match
		return(false);
	}




//////////
//
// Called to potentially retry so long as the callback returns true.
//
//////
	SDiskLock* iDisk_lock_range_retryCallback(SBuilder* lockRoot, s32 tnFile, s64 tnOffset, s32 tnLength, uptr tnCallbackFunction, uptr tnExtra)
	{
		s32					lnAttempts, lnMillisecondsDelta;
		s64					lnMillisecondsStart;
		SDiskLock*			dl;
		SDiskLockCallback	dcb;


		//////////
		// Initialize
		//////
			memset(&dcb, 0, sizeof(dcb));
			dcb._diskRetryLockCallback	= tnCallbackFunction;
			dcb.extra					= tnExtra;
			lnAttempts = 0;


		//////////
		// Begin the trial
		//////
			GetLocalTime(&dcb.timeStart);
			lnMillisecondsStart = iTime_computeMilliseconds(&dcb.timeStart);
			while (1)
			{

				//////////
				// Try to lock
				//////
					++lnAttempts;
					dl = iDisk_lock_range(lockRoot, tnFile, tnOffset, tnLength, tnExtra);


				//////////
				// Were we successful?
				//////
					if (dl && dl->nLength == tnLength)
						return(dl);	// Yes


				//////////
				// When we get here, we need to see if they want to continue waiting or not
				//////
					GetLocalTime(&dcb.timeNow);
					lnMillisecondsDelta = (s32)iTime_computeMillisecondsBetween(&dcb.timeNow, lnMillisecondsStart);


				//////////
				// Inquire politely about our retry
				//////
					if (!dcb._diskRetryLockCallback || !dcb.diskRetryLockCallback(&dcb, lnAttempts, lnMillisecondsDelta))
					{
						// They inform us:  "No more waiting!"
						return(dl);	// Failure
					}
					// If we get here, try again

			}
	}




//////////
//
// Called to lock a disk range, and append the information to the buffer.
//
// Note:  It returns a pointer to the SDisk entry that was just added, however
//        it should not generally be modified directly, but only used as a handle.
//
// Note:  Examine success with this test:
//
//				SDiskLock* dl;
//				dl = iDisk_lock_range(..., lnLength...);
//				if (dl && dl->nLength == lnLength)
//				{
//				    // Success
//				} else {
//				    // Failure
//				}
//
// Note:  If success, upon exit, the file will be positioned at the locking address.
//
// Note:  If not success, it may be positioned at the locking address depending on
//                        when the error occurred.
//
//////
	SDiskLock* iDisk_lock_range(SBuilder* lockRoot, s32 tnFile, s64 tnOffset, s32 tnLength, uptr tnExtra)
	{
		// Make sure our environment is sane
		if (lockRoot && lockRoot->buffer && lockRoot->populatedLength % sizeof(SDiskLock) == 0)
		{
			// Lock it
			return(iiDisk_lock_range(lockRoot, tnFile, tnOffset, tnLength, tnExtra));

		} else {
			// Indicate error
			return(NULL);
		}
	}

	SDiskLock* iiDisk_lock_range(SBuilder* lockRoot, s32 tnFile, s64 tnOffset, s32 tnLength, uptr tnExtra)
	{
		u32			lnI;
		SDiskLock*	dl;


		//////////
		// Iterate to find an empty slot
		//////
			for (lnI = 0, dl = (SDiskLock*)lockRoot->buffer; lnI < lockRoot->populatedLength; lnI += sizeof(SDiskLock), dl++)
			{
				// Is this slot empty?
				if (!dl->isValid)
					break;	// Yes
			}


		//////////
		// When we get here, we have a slot or not
		//////
			if (lnI >= lockRoot->populatedLength)
			{
				// Create a new one
				dl = (SDiskLock*)iBuilder_appendData(lockRoot, (cs8*)NULL, sizeof(SDiskLock));
				// Note:  disk members are all initialized to 0s
			}


		//////////
		// Physically try the lock
		//////
			dl->isValid = true;
			dl->nFile	= tnFile;
			dl->nOffset	= tnOffset;
			dl->nLength	= 0;

			// Seek to the offset
			if (_lseeki64(tnFile, tnOffset, SEEK_SET) == tnOffset)
			{
				// Lock the bytes
				if (_locking(tnFile, _LK_NBLCK, tnLength) == 0)		// Note:  _LK_NBLCK attempts to lock, and if fails returns immediately, whereas _LK_LOCK would auto-retry after 1 second, and fail after 10 seconds
				{
					// Indicate a successful lock
					dl->nLength = tnLength;
				}

			} else {
				// The length being 0 will indicate the lock failed
				// So we just let it fall through
			}


		//////////
		// Indicate success or failure
		//////
			return(dl);
	}




//////////
//
// Called to unlock a previously locked disk range using iDisk_lock().
//
// Note:  After unlocking, the slot in the original buffRoot is freed, available for re-use.
//
//////
	bool iDisk_unlock(SBuilder* lockRoot, SDiskLock* dl)
	{
		// Make sure our environment is sane
		if (iiDisk_isValidLockHandle(lockRoot, dl))
		{
			// Unlock it
			return(iiDisk_unlock(lockRoot, dl));
		}

		// If we get here, failure
		return(false);
	}

	bool iiDisk_unlock(SBuilder* lockRoot, SDiskLock* dl)
	{
		bool llResult;


		// If it's a valid lock, unlock it
		llResult = false;
		if (dl->isValid)
		{
			// Seek
			if (_lseeki64(dl->nFile, dl->nOffset, SEEK_SET) == dl->nOffset)
			{
				// Unlock
				llResult = (_locking(dl->nFile, _LK_UNLCK, dl->nLength) == 0);

			} else {
				// If we get here, the file is likely no longer open, so all locks are closed
				// But, we just silently fall through
			}

			// If cleared, indicate the lock is no longer valid
			dl->isValid = !llResult;
		}

		// Indicate success or failure
		return(llResult);
	}




//////////
//
// Called to unlock everything contained within the buffer.
//
// Note:  After use, all locks created by iDisk_lock() will be released, and every
//        slot in the buffRoot buffer is available for re-use.
//
//////
	void iDisk_unlock_all(SBuilder* lockRoot)
	{
		// Make sure our environment is sane
		if (lockRoot && lockRoot->buffer && lockRoot->populatedLength % sizeof(SDiskLock) == 0)
		{
			// Unlock it
			iiDisk_unlock_all(lockRoot);
		}
	}

	void iiDisk_unlock_all(SBuilder* lockRoot)
	{
		u32			lnI;
		SDiskLock*	dl;


		// If it's a valid lock, unlock it
		for (lnI = 0, dl = (SDiskLock*)lockRoot->buffer; lnI < lockRoot->populatedLength; lnI += sizeof(SDiskLock), dl++)
		{
			// Is this one locked?
			if (dl->isValid)
			{
				// Seek
				if (_lseeki64(dl->nFile, dl->nOffset, SEEK_SET) == dl->nOffset)
				{
					// Unlock
					_locking(dl->nFile, _LK_UNLCK, dl->nLength);

				} else {
					// If we get here, the file is likely no longer open, so all locks are closed
					// But, we just silently fall through
				}

				// Indicate the lock is no longer valid
				dl->isValid = false;
			}
		}
	}




//////////
//
// Called to unlock a disk range by using a callback test to determine if the
// unlock should be performed on a case-by-case basis.
//
// Note:  Only those entries which are indicated are released.  The rest remain
//        locked unless they are manually unlocked.
//
//////
	s32 iDisk_unlock_all_byCallback(SBuilder* lockRoot, uptr tnFunction, uptr tnExtra)
	{
		// Make sure our environment is sane
		if (lockRoot && lockRoot->buffer && lockRoot->populatedLength % sizeof(SDiskLock) == 0)
		{
			// Unlock it
			return(iiDisk_unlock_all_byCallback(lockRoot, tnFunction, tnExtra));

		} else {
			// Something's awry, if we have a valid buffRoot, then indicate 0 records were unlocked
			return(-1);
		}
	}

	s32 iiDisk_unlock_all_byCallback(SBuilder* lockRoot, uptr tnFunction, uptr tnExtra)
	{
		u32					lnI, lnUnlockedCount;
		SDiskLock*			dl;
		SDiskLockCallback	dcb;


		// Initialize
		memset(&dcb, 0, sizeof(dcb));
		dcb._diskUnlockCallback	= tnFunction;
		dcb.extra				= tnExtra;

		// If it's a valid lock, unlock it
		for (lnI = 0, lnUnlockedCount = 0, dl = (SDiskLock*)lockRoot->buffer; lnI < lockRoot->populatedLength; lnI += sizeof(SDiskLock), dl++)
		{
			// Is this one locked?  And do they want to release it?
			if (dl->isValid && (!dcb._diskUnlockCallback || !dcb.diskUnlockCallback(&dcb)))
			{
				// Seek
				if (_lseeki64(dl->nFile, dl->nOffset, SEEK_SET) == dl->nOffset)
				{
					// Unlock
					_locking(dl->nFile, _LK_UNLCK, dl->nLength);
					++lnUnlockedCount;

				} else {
					// If we get here, the file is likely no longer open, so all locks are closed
					// But, we just silently fall through
				}

				// Indicate the lock is no longer valid
				dl->isValid = false;
			}
		}

		// Indicate our count
		return(lnUnlockedCount);
	}




//////////
//
// Check the handle to make sure it's valid
//
//////
	bool iiDisk_isValidLockHandle(SBuilder* lockRoot, SDiskLock* dl)
	{
		union
		{
			SDiskLock*		ldl;
			uptr			_ldl;
		};


		//////////
		// Verify the handle is valid
		//////
			ldl = dl;
			if (dl && iBuilder_isPointer(lockRoot, _ldl))
				return(dl->isValid);	// It's in range, but the valid condition depends upon its property

			// Indicate failure
			return(false);

	}




//////////
//
// Called to see if the indicated file/directory name exists
//
//////
	bool iFile_exists(cs8* tcPathname, SFileExists* fe)
	{
		HANDLE				h;
		WIN32_FIND_DATA		wfd;


		// Try to find the file
		if ((h = FindFirstFile(tcPathname, &wfd)) != INVALID_HANDLE_VALUE)
		{
			// Store attributes
			if (fe)
			{
				// Reset everything
				memset(fe, 0, sizeof(*fe));

				// Store ARSHD, Archive, Readonly, System, Hidden, Directory
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)		fe->nAttributes |= _FILE_ARCHIVE;
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)		fe->nAttributes |= _FILE_READONLY;
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)		fe->nAttributes |= _FILE_SYSTEM;
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)		fe->nAttributes |= _FILE_HIDDEN;
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	fe->nAttributes |= _FILE_DIRECTORY;

				// File size
				fe->nFilesize = ((u64)wfd.nFileSizeHigh << 32 | (u64)wfd.nFileSizeLow);

				// File dates
				memcpy(&fe->created,	&wfd.ftCreationTime,	sizeof(fe->created));
				memcpy(&fe->last_read,	&wfd.ftLastAccessTime,	sizeof(fe->last_read));
				memcpy(&fe->last_write,	&wfd.ftLastWriteTime,	sizeof(fe->last_write));

				// Set the one that's newest
				if (iFile_time_compare(&fe->last_read, &fe->last_write) <= 0)		fe->last = &fe->last_read;
				else																fe->last = &fe->last_write;

				// File size
				memcpy(&fe->cFullFileName[0],	&wfd.cFileName[0],			sizeof(fe->cFullFileName));
				memcpy(&fe->cShortFileName[0],	&wfd.cAlternateFileName[0],	sizeof(fe->cShortFileName));
			}

			// Clean house
			FindClose(h);

			// Indicate success
			return(true);
		}

		// If we get here, not found
		return(false);
	}




//////////
//
// Called to compare the filetimes from two sources, and indicates their status
//
//////
	s32 iFile_time_compare(FILETIME* ftLeft, FILETIME* ftRight)
	{
		u64 left, right;


		// Grab them into single quantities
		left	= ((u64)ftLeft->dwHighDateTime << 32	| (u64)ftLeft->dwLowDateTime);
		right	= ((u64)ftRight->dwHighDateTime << 32	| (u64)ftRight->dwLowDateTime);

		// Compare the file date times
		     if (left < right)		return(-1);		// Left is less than right
		else if (left > right)		return(1);		// Left is greater than right
		else						return(0);		// Left is equal
	}




//////////
//
// Called to read the contents of the indicated file
//
//////
	bool iFile_readContents(s8* tcFilename, FILE** tfh, s8** data, u32* dataLength)
	{
		u32		lnNumread;
		FILE*	lfh;


		// Make sure our environment is sane
		if (tcFilename && data && dataLength)
		{
			// Try to open the file
			lfh = fopen(tcFilename, "rb+");
			if (lfh)
			{
				// Find out how big it is
				fseek(lfh, 0, SEEK_END);
				*dataLength = ftell(lfh);
				fseek(lfh, 0, SEEK_SET);

				// Allocate a buffer that large
				*data = (s8*)malloc(*dataLength);
				if (*data)
				{
					// Read the contents
					lnNumread = (u32)fread(*data, 1, *dataLength, lfh);
					if (lnNumread == *dataLength)
					{
						// We read everything
						if (tfh)
						{
							// Save the file handle, return it open
							*tfh = lfh;

						} else {
							// Close the file handle
							fclose(lfh);
						}

						// Indicate success
						return(true);
					}

				} else {
					// Error allocating that much memory
					fclose(lfh);
				}
			}
		}
		// If we get here, failure
		return(false);
	}




//////////
//
// Search backwards for the start of the string, or the first \ found
//
//////
	bool iFile_get_justfname(s8* tcPathname, s32 tnFilenameLength, s8** tcFname, s32* tnFnameLength)
	{
		s32 lnI, lnLength;


		// Make sure our environment is sane
		if (tcPathname && tcFname && tnFnameLength && *tnFnameLength)
		{
			// Scan backwards
			for (lnI = tnFilenameLength - 1, lnLength = 1; lnI > 0; lnI--, lnLength++)
			{
				// Have we reached the \ ?
				if (tcPathname[lnI] == '\\')
					break;
			}

			// Indicate our pointer and length
			*tcFname		= tcPathname + lnI;
			*tnFnameLength	= lnLength;

			// Indicate success
			return(true);
		}

		// If we get here, failure
		return(false);
	}




//////////
//
// Called to parse a raw file into lines
//
//////
#ifndef _EXCLUDE_iFile_parseIntoLines
	// Returns line count
	s32 iFile_parseIntoLines(SLine** firstLine, s8* data, u32 dataLength)
	{
		u32			lnI, lnJ, lnLast, lnLineNumber;
		SLine*		line;
		SLine**		lastLine;


		// Copy through lines into the SLine struct
		for (lnI = 0, lnLast = 0, lastLine = firstLine, lnLineNumber = 1; lnI < dataLength; )
		{

			//////////
			// Are we on a CR/LF combination?
			//////
				for (lnJ = 0; (data[lnI] == 13 || data[lnI] == 10) && lnJ < 2 && (u32)lnI < dataLength; lnJ++)
					++lnI;	// Increase also past this CR/LF character


			//////////
			// If we found a CR/LF combination
			//////
				if (lnJ != 0 || lnI >= dataLength)
				{

					//////////
					// Increase our line count
					//////
						++lnLineNumber;


					//////////
					// We've entered into a CR/LF block, append a new line
					//////
						line = (SLine*)iLl_appendNew__llAtEnd((SLL**)lastLine, sizeof(SLine));
						if (!line)
							return(-1);		// Unexpected failure


					//////////
					// Indicate content
					//////
						line->sourceCode	= iDatum_allocate((u8*)data + lnLast, (s32)(lnI - lnJ - lnLast));
						line->lineNumber	= lnLineNumber;


					//////////
					// Indicate where we are now
					//////
						lnLast		= lnI;
						lastLine	= &line;

				} else {
					// Still going
					++lnI;
				}
				// Continue on processing the next line if we have room

		}

		// Indicate how many lines
		return(lnLineNumber);
	}
#endif
