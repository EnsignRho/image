#ifndef __DISK_H__
#define __DISK_H__

// Constants for iDisk_exists()
cs32 _FILE_ARCHIVE		= 1 << 0;
cs32  _FILE_READONLY	= 1 << 1;
cs32 _FILE_SYSTEM		= 1 << 2;
cs32 _FILE_HIDDEN		= 1 << 3;
cs32 _FILE_DIRECTORY	= 1 << 4;



//////////
// The SDisk structure.
//////
	// Note:  For each SBuilder in use for the function calls below, there needs
	//        to be external semaphore locking.
	struct SDiskLock
	{
		bool		isValid;				// Is this lock currently valid (is it an active lock)?

		s32			nFile;					// File handle
		s64			nOffset;				// Offset for start of lock
		s32			nLength;				// Length of lock

		// user-defined extra data associated with this lock
		uptr		extra1;
		uptr		extra2;
		uptr		extra3;
		uptr		extra4;
	};

	struct SDiskLockCallback
	{
		SDiskLock*	dl;
		uptr		extra;


		//////////
		// For the unlock callback
		//////
			union {
				uptr	_diskUnlockCallback;
				bool	(*diskUnlockCallback)(SDiskLockCallback* dcb);
			};


		//////////
		// For the retry callback
		//////
			SYSTEMTIME	timeStart;
			SYSTEMTIME	timeNow;
			union {
				uptr	_diskRetryLockCallback;
				// Note:  There is no delay involved in the retry process, so the callback should issue a Sleep() before retrying, probably a Sleep(100)
				bool	(*diskRetryLockCallback)(SDiskLockCallback* dcb, s32 tnAttempts, s32 tnMillisecondsSpentThusFar);
			};
	};

	// Return values for iFile_exists()
	struct SFileExists
	{
		u32			nAttributes;
		u64			nFilesize;

		FILETIME*	last;							// Pointer to either last_read or last_write, whichever is newer
		FILETIME	created;						// Date created
		FILETIME	last_read;						// Date last read
		FILETIME	last_write;						// Date last write

		s8			cFullFileName[MAX_PATH];		// Full filename
		s8			cShortFileName[14];				// Short filename
	};

	struct SDiskFind
	{
		s8					path[_MAX_PATH];
		SDatum				pattern;
		SDatum				attributes;
		HANDLE				hFind;
		WIN32_FIND_DATA		fd;
	};

	struct SDirListingEntry
	{
		SYSTEMTIME		created;
		SYSTEMTIME		modified;
		SYSTEMTIME		accessed;
		u64				fileSize;
		SDatum*			pathname;

		// Can be used for any purpose, and is not touched
		void*			extra;
	};




//////////
// Forward declarations
//////
	s32				iDisk_open								(cs8* tcPathname, s32 tnType, s32 tnShare, bool tlCreateIfCannotOpen, bool* error = NULL, u32* errorNum = NULL);
	s32				iDisk_openAs							(cs8* tcPathname, s32 tnType, bool tlCreateIfCannotOpen, bool tlExclusive, bool* error = NULL, u32* errorNum = NULL);
	s32				iDisk_openShared						(cs8* tcPathname, s32 tnType, bool tlCreateIfCannotOpen, bool* error = NULL, u32* errorNum = NULL);
	s32				iDisk_openExclusive						(cs8* tcPathname, s32 tnType, bool tlCreateIfCannotOpen, bool* error = NULL, u32* errorNum = NULL);
	s32				iDisk_deleteFile						(cs8* tcPathname);
	s32				iDisk_close								(s32 tnFile, bool* error = NULL, u32* errorNum = NULL);
	bool			iDisk_doesFileExist						(s8* tcPathname);
	s32				iDisk_getTemporaryPathname				(s8* tcPathnameOut, cs8* tcPrefix = NULL, u32 tnUniqueNumber = -1, cs8* tcPostfix = NULL, cs8* tcExtension = NULL);
	s32				iDisk_getCurrentDirectory				(s8* tcPathnameOut);
	s8*				iDisk_buildPath							(s8* tcPathnameOut, cs8* tcPath, cs8* tcFilename);
	s64				iDisk_getFileSize						(s8* tcPathname);
	s64				iDisk_getFileSize						(s32 tnFile, bool* error = NULL, u32* errorNum = NULL);
	s64				iDisk_getFilePosition					(s32 tnFile, bool* error = NULL, u32* errorNum = NULL);
	s32				iDisk_getFileExtension					(cs8* tcPathname, s8* tcExtensionOut, bool tlIncludePeriod = false);
	s64				iDisk_setFilePosition					(s32 tnFile, s64 tnSeekOffset, bool* error = NULL, u32* errorNum = NULL);
	s32				iDisk_commit							(s32 tnFile, bool* error = NULL, u32* errorNum = NULL);
	s32				iDisk_read								(s32 tnFile, s64 tnSeekOffset, void* tcData, s32 tnReadCount,  bool* tlError = NULL, u32* tnErrorNum = NULL);
	s32				iDisk_write								(s32 tnFile, s64 tnSeekOffset, void* tcData, s32 tnWriteCount, bool* tlError = NULL, u32* tnErrorNum = NULL);
	s32				iDisk_readShared_withRetryCallback		(SBuilder* lockRoot, s32 tnFile, s64 tnSeekOffset, void* tcData, s32 tnReadCount,  bool* tlError, u32* tnErrorNum, uptr tnCallbackFunction, uptr tnExtra, SDiskLock** diskLock, bool tlUnlockAfter);
	s32				iDisk_writeShared_withRetryCallback		(SBuilder* lockRoot, s32 tnFile, s64 tnSeekOffset, void* tcData, s32 tnWriteCount, bool* tlError, u32* tnErrorNum, uptr tnCallbackFunction, uptr tnExtra, SDiskLock** diskLock, bool tlUnlockAfter);
	s32				iiDisk_rwShared_withRetryCallback_common(SBuilder* lockRoot, s32 tnFile, s64 tnSeekOffset, void* tcData, s32 tnCount, bool* tlError, u32* tnErrorNum, uptr tnCallbackFunction, uptr tnExtra, SDiskLock** diskLock, bool tlUnlockAfter, u32 tnErrorNumIfError, bool tlRead);
	s32				iDisk_duplicateFileHandle				(s32 tnFile);

	// Directory functions
	bool			iDisk_doesPathExist						(cs8* tcPathname);
	SBuilder*		iDisk_directoryListing_get				(SBuilder* dirAddTo, cs8* tcFolder, cs8* tcFilespec, bool tlSortList = false);
	int				iiDisk_directoryListing_get__qsort		(const void* left, const void* right);
	void			iDisk_directoryListing_free				(SBuilder* dirs);
	SBuilder*		iDisk_find_first						(cs8* tcPathname, cs8* tcPattern, cs8* tcAttributes, bool tlRecurseSubDirs, s8* tcPathname_out, s8* tcPath_out = NULL, s8* tcFilename_out = NULL, WIN32_FIND_DATA* fd = NULL);
	SBuilder*		iDisk_find_next							(SBuilder* findData, s8* tcPathname_out, s8* tcPath_out = NULL, s8* tcFilename_out = NULL, WIN32_FIND_DATA* fd = NULL);
	bool			iDisk_find_cancel						(SBuilder* findData);
	bool			iiDisk_find_isValid						(SBuilder* findData);
	SDiskFind*		iiDisk_find_delete_df					(SBuilder** findData, SDiskFind* df);
	void			iiDisk_find_filterByAttributes			(SBuilder** findDataRoot, SDiskFind* df, bool tlRecurseSubDirs, s8* tcPathname_out, s8* tcPath_out, s8* tcFilename_out, WIN32_FIND_DATA* fd);
	bool			iiDisk_find_filterByAttributes_test		(SDiskFind* df, WIN32_FIND_DATA* fd);


//////////
// Locking
// BEGIN
//////
	// Standard instance validation
	SDiskLock*		iDisk_lock_range_retryCallback			(SBuilder* lockRoot, s32 tnFile, s64 tnOffset, s32 tnLength, uptr tnCallbackFunction, uptr tnExtra = 0);
	SDiskLock*		iDisk_lock_range						(SBuilder* lockRoot, s32 tnFile, s64 tnOffset, s32 tnLength, uptr tnExtra = 0);
	bool			iDisk_unlock							(SBuilder* lockRoot, SDiskLock* dl);
	void			iDisk_unlock_all						(SBuilder* lockRoot);
	s32				iDisk_unlock_all_byCallback				(SBuilder* lockRoot, uptr tnCallbackFunction, uptr tnExtra);

	// Faster functions with no validation
	SDiskLock*		iiDisk_lock_range						(SBuilder* lockRoot, s32 tnFile, s64 tnOffset, s32 tnLength, uptr tnExtra);
	bool			iiDisk_unlock							(SBuilder* lockRoot, SDiskLock* dl);
	void			iiDisk_unlock_all						(SBuilder* lockRoot);
	s32				iiDisk_unlock_all_byCallback			(SBuilder* lockRoot, uptr tnCallbackFunction, uptr tnExtra);

	// Support functions
	bool			iiDisk_isValidLockHandle				(SBuilder* buffRoot, SDiskLock* dl);
//////
// END
//////////


	// File functions
	bool			iFile_exists							(cs8* tcPathname, SFileExists* fe = NULL);
	s32				iFile_time_compare						(FILETIME* ftLeft, FILETIME* ftRight);
	bool			iFile_readContents						(s8* tcFilename, FILE** tfh, s8** data, u32* dataLength);
	bool			iFile_get_justfname						(s8* tcPathname, s32 tnFilenameLength, s8** tcFname, s32* tnFnameLength);
	s32				iFile_parseIntoLines					(SLine** firstLine, s8* data, u32 dataLength);

#endif	// __DISK_H__
