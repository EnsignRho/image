* This product is licensed under the PBL v1.0, see pbl_v1.txt
LOCAL img AS ImageBitmap

img = CREATEOBJECT("ImageBitmap")
img.open("trig_series.png")
img.crop()
img.get_attributes()

* Resize to 80% of its current size
img.resize(0.80)
img.save("trig_series_resized.jpg")
img.save("trig_series_resized.png")
img.close()




DEFINE CLASS ImageBitmap AS Custom

	* Image handle
	nHandle		= 0
	
	* Original attributes (when opened)
	cWO			= SPACE(0)
	cHO			= SPACE(0)
	cBppO		= SPACE(0)
	cPpiXO		= SPACE(0)
	cPpiYO		= SPACE(0)
	
	* Current attributes (from last get_attributes())
	cW			= SPACE(0)
	cH			= SPACE(0)
	cBpp		= SPACE(0)
	cPpiX		= SPACE(0)
	cPpiY		= SPACE(0)

	
	PROCEDURE Init
		this.load_dll()
	ENDPROC
	
	* Returns:  0=loaded, negative=errors
	PROCEDURE load_dll
	LPARAMETERS tcDll
	
		* Grab our dll
		IF PCOUNT() < 1 OR TYPE("tcDll") != "C"
			tcDll = "image.dll"
		ENDIF
		IF NOT FILE(tcDll)
			RETURN -1
		ENDIF
		
		* Register our DLL functions
		DECLARE INTEGER bmp_open_file		IN image.dll STRING cPathname									&& Returns a handle for accessing the bitmap in future operations
		DECLARE INTEGER bmp_save_file		IN image.dll INTEGER nHandle, STRING@ cNewBmpPathname			&& Handle remains open after a save, must call bmp_close() to close it
		DECLARE INTEGER bmp_close			IN image.dll INTEGER nHandle									&& Called to close a handle
		
		DECLARE INTEGER bmp_crop_to_content	IN image.dll INTEGER nHandle, INTEGER nRgb, INTEGER nType		&& nType is:  0=use white, 1=use black, 2=use nRgb, 3=auto-detect
		DECLARE INTEGER bmp_get_attributes	IN image.dll INTEGER nHandle, STRING@ cWidthOutInches8, STRING@ cHeightOutInches8, ;	&& Returns floating point in inches
																		  STRING@ cBitsPerPixel8, ;									&& Bits per pixels of intermediate bitmap, typically 24 or 32
																		  STRING@ cPixelsPerInchX8, STRING@ cPixelsPerInchY8		&& Multiply X by width to get pixels, and Y by height

		* Returns a handle for accessing the bitmap in future operations
		DECLARE INTEGER bmp_resize			IN image.dll INTEGER nHandle, SINGLE fScaleFactor				&& Resize by some proportion to get the pixel count down
		
		* Signify
		RETURN 0
		
	ENDPROC
	
	PROCEDURE reset_attributes
	LPARAMETERS tlDoNotResetCurrent, tlDoNotResetOriginal
		
		* Current attributes
		IF NOT tlDoNotResetCurrent
			this.cW    = SPACE(0)
			this.cH    = SPACE(0)
			this.cBpp  = SPACE(0)
			this.cPpiX = SPACE(0)
			this.cPpiY = SPACE(0)
		ENDIF
		
		* Original attributes when opened
		IF NOT tlDoNotResetOriginal
			this.cWO    = SPACE(0)
			this.cHO    = SPACE(0)
			this.cBppO  = SPACE(0)
			this.cPpiXO = SPACE(0)
			this.cPpiYO = SPACE(0)
		ENDIF
		
	ENDPROC
	
	* Returns:  0=loaded, negative=errors
	PROCEDURE open
	LPARAMETERS tcImagePathname
	LOCAL lnHandle
	
		* Verify the file exists
		this.nHandle = 0
		IF NOT FILE(tcImagePathname)
			RETURN -1
		ENDIF
		
		* Open the image
		lnHandle = bmp_open_file("trig_series.png")
		IF BETWEEN(lnHandle, -10, -1)
			RETURN -2
		ENDIF
		
		* It's loaded
		this.nHandle = lnHandle
		
		* Grab the original attributes
		this.get_attributes()
		this.cWO    = this.cW
		this.cHO    = this.cH
		this.cBppO  = this.cBpp
		this.cPpiXO = this.cPpiX
		this.cPpiYO = this.cPpiY
		
		* Reset the current attributes only
		this.reset_attributes(.f., .t.)
		
		* Signify
		RETURN 0
		
	ENDPROC
	
	PROCEDURE get_attributes
	LOCAL lnResult, lcW, lcH, lcBpp, lcPpiX, lcPpiY
	
		* Initialize our temp variables
		STORE SPACE(8) TO lcW, lcH, lcBpp, lcPpiX, lcPpiY
		
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Get its current attributes
		lnResult = bmp_get_attributes(this.nHandle, @lcW, @lcH, @lcBpp, @lcPpiX, @lcPpiY)
		IF BETWEEN(lnResult, -10, -1)
			RETURN -2
		ENDIF
		
		* Copy over
		this.cW    = ALLTRIM(CHRTRAN(lcW,    CHR(0), SPACE(0)))
		this.cH    = ALLTRIM(CHRTRAN(lcH,    CHR(0), SPACE(0)))
		this.cBpp  = ALLTRIM(CHRTRAN(lcBpp,  CHR(0), SPACE(0)))
		this.cPpiX = ALLTRIM(CHRTRAN(lcPpiX, CHR(0), SPACE(0)))
		this.cPpiY = ALLTRIM(CHRTRAN(lcPpiY, CHR(0), SPACE(0)))
		
		* Signify
		RETURN 0
		
	ENDPROC
	
	PROCEDURE crop
	LPARAMETERS tnRgb, tnType
	LOCAL lnResult
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Makek sure we have valid parameters
		IF PCOUNT() = 2
			* Two parameters
			* Make sure our type is valid:  0=use white, 1=use black, 2=use nRgb, 3=auto-detect
			IF NOT INLIST(tnType, 0, 1, 2, 3)
				* Invalid choice
				RETURN -2
			ENDIF
				
		ELSE
			* One or zero parameters
			* Set the type to use white
			tnType = 0
		ENDIF
		
		* Physically crop
		lnResult = bmp_crop_to_content(this.nHandle, tnRgb, tnType)
		IF BETWEEN(lnResult, -10, -1)
			RETURN -3
		ENDIF
		
		* Signify
		RETURN 0
		
	ENDPROC
	
	PROCEDURE resize
	LPARAMETERS tfScale
	LOCAL lnResult
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Scale
		lnResult = bmp_resize(this.nHandle, tfScale)
		IF BETWEEN(lnResult, -10, -1)
			RETURN -2
		ENDIF
		
		* Delete the existing image
		bmp_close(this.nHandle)
		this.nHandle = lnResult
		
		* Signify
		RETURN 0
		
	ENDPROC
	
	PROCEDURE save
	LPARAMETERS tcOutputPathname
	LOCAL lnResult
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Save as indicated
		lnResult = bmp_save_file(this.nHandle, tcOutputPathname)
		IF BETWEEN(lnResult, -10, -1)
			RETURN -2
		ENDIF
		
		* Signify
		RETURN 0
		
	ENDPROC

	PROCEDURE close
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Close
		bmp_close(this.nHandle)
		
		* Signify
		RETURN 0
		
	ENDPROC
	
ENDDEFINE
