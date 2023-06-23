* This has been backed up in z:\rick\image\
LOCAL img AS ImageBitmap
LOCAL lnW, lnH

LOCAL lnBlackRgb, lnRedRgb
lnWhiteRgb = RGB(255,255,255)
lnBlackRgb = RGB(0,0,0)
lnBlueRgb  = RGB(0,0,255)
lnRedRgb   = RGB(0,0,255)

img = CREATEOBJECT("ImageBitmap")
imgFrame = CREATEOBJECT("ImageBitmap")
*img.open("trig.png")
*img.save_mono("trig_mono.bmp")

img.new(800, 600, 24)
imgFrame.open("button.png")

*img.frame_rect(100, 100, 700, 500, lnBlackRgb, lnBlackRgb, lnBlackRgb, lnBlackRgb, .f.)

*img.swap_colors(lnWhiteRgb, lnBlueRgb)
*img.save("z:\pictures\swap_colors_white_for_blue.png")

* Set and get some pixels, and partial and full grayscale
*FOR lnY = 10 TO 410
*	FOR lnX = 100 TO 500
*		lnRgb1 = RGB(INT(RAND() * 255), INT(RAND() * 255), INT(RAND() * 255))
*		img.set_pixel(lnX, lnY, lnRgb1)
*		lnRgb2 = img.get_pixel(lnX, lnY)
*		IF lnRgb1 != lnRgb2
*			* The color read did not work
*			SET STEP ON
*		ENDIF
*	NEXT
*NEXT
*img.save("z:\pictures\set_pixels.png")
*img.grayscale(0.25)
*img.save("z:\pictures\grayscale_25_percent.png")
*img.grayscale(1.0)
*img.save("z:\pictures\grayscale_100_percent.png")

*img.colorize(0, 0, 800, 600, lnRedRgb, 0.25)
*img.save("z:\pictures\colorize_red_25.png")
*img.colorize(0, 0, 800, 600, lnRedRgb, 0.1)
*img.save("z:\pictures\colorize_red_50.png")

*img.frame_in_nine_parts(400, 400, 464, 432, imgFrame)
*img.save("z:\pictures\framed_in_nine_parts.png")

*FOR lnI = 1 TO 50
*	lnX1  = INT(RAND() * img.nW)
*	lnX2  = INT(RAND() * img.nW)
*	lnY1  = INT(RAND() * img.nH)
*	lnY2  = INT(RAND() * img.nH)
*	lnRgb = RGB(RAND() * 255, RAND() * 255, RAND() * 255)
*	lnBegRgb = RGB(RAND() * 255, RAND() * 255, RAND() * 255)
*	lnEndRgb = RGB(RAND() * 255, RAND() * 255, RAND() * 255)
*	lfAlpha  = RAND()
*	
*	*img.draw_line(lnX1, lnY1, lnX2, lnY2, lnRgb)
*	
*	*lnWidth = INT(RAND() * 40)
*	*img.draw_quad(lnX1, lnY1, lnX2, lnY2, lnWidth, .t., lnRgb)
*	
*	img.draw_colorized_line_gradient(lnX1, lnY1, lnX2, lnY2, lnBegRgb, lnEndRgb, lfAlpha)
*NEXT
*img.save("z:\pictures\draw_lines.png")
*img.save_mono("z:\pictures\draw_lines_mono.bmp")
*
*img.save("z:\pictures\draw_quad.png")
*
*img.save("z:\pictures\draw_colorized_line_gradient.png")
*img.save_mono("z:\pictures\draw_colorized_line_gradient_mono.bmp")

* Draw some text
*at_say(img, @lnW, @lnH, 10, 10, "Blue text @ 10,10",  RGB(0,0,255))
*at_say(img, @lnW, @lnH, 11, 10, "Green text @ 11,10", RGB(0,255,0))
*at_say(img, @lnW, @lnH, 12, 10, "Red text @ 12,10",   RGB(255,0,0))
*at_say_fixed(img, @lnW, @lnH,  8,  6, "8x6 font",   8,  6, lnBlueRgb, lnWhiteRgb)
*at_say_fixed(img, @lnW, @lnH,  8,  8, "8x8 font",   8, 12, lnBlueRgb, lnWhiteRgb)
*at_say_fixed(img, @lnW, @lnH,  8, 12, "8x12 font",  8, 20, lnBlueRgb, lnWhiteRgb)
*at_say_fixed(img, @lnW, @lnH,  8, 14, "8x14 font",  8, 32, lnBlueRgb, lnWhiteRgb)
*at_say_fixed(img, @lnW, @lnH,  8, 16, "8x16 font",  8, 46, lnBlueRgb, lnWhiteRgb)
*at_say_fixed(img, @lnW, @lnH, 16, 32, "16x32 font", 8, 62, lnBlueRgb, lnWhiteRgb)
*img.save("z:\pictures\text.png")
*img.wavy_line(8, 32 + lnH - 4, 8 + lnW, 32 + lnH, lnRedRgb, 0.25)
*img.save("z:\pictures\wavy_line.png")

*img.flip(.t.)
*img.save("horizontal.png")
*img.flip(.f.)
*img.save("vertical.png")

*img.rotate_90_degrees_ccw()
*img.save("rotated_90_1.png")
*img.rotate_90_degrees_ccw()
*img.save("rotated_90_2.png")
*img.rotate_90_degrees_ccw()
*img.save("rotated_90_3.png")
*img.rotate_90_degrees_ccw()
*img.save("rotated_90_4.png")

img.close()


**********
* Full list of available functions
*****
	*img.reset_attributes(tlDoNotResetCurrent, tlDoNotResetOriginal)
	*img.new(tnWidth, tnHeight, tnBitCount)
	*img.open(tcImagePathname)
	*img.get_attributes()
	*img.crop(tnRgb, tnType, tfThreshold)
	*img.resize(tfScale)
	*img.save(tcOutputPathname)
	*img.save_mono(tcOutputPathname)
	*img.close()
	*img.flip(tlHorizontal)
	*img.rotate_90_degrees_ccw()
	*img.grayscale(tfAlpha)
	*img.set_pixel(tnX, tnY, tnRgb)
	*img.get_pixel(tnX, tnY)
	*img.colorize(tnULX, tnULY, tnLRX, tnLRY, tnColorTemplateRgb, tfAlpha)
	*img.swap_colors(tnColorOldRgb, tnColorNewRgb)
	*img.wavy_line(tnULX, tnULY, tnLRX, tnLRY, tnColorLineRgb, 1.0)
	*img.dapple(toImageBitmapDapple, tfInfluence)
	*img.draw_font(tcFontName, tnPointSize, tlBold, tlItalic, tlUnderline, tlStrikethru, tcText, tnX, tnY, tnTextColorRgb)
	*img.draw_font_fixed_point(tnFontPixelsX, tnFontPixelsY, tcText, tnTextLength, tnX, tnY, tnForeColorRgb, tnBackColorRgb)
	*img.draw_bullet(tnX, tnY, tnColorBulletRgb, 1.0)
	*img.fill_rect(tnULX, tnULY, tnLRX, tnLRY, tnColorNWRgb, tnColorNERgb, tnColorSWRgb, tnColorSERgb, tlUseGradient)
	*img.frame_rect(tnULX, tnULY, tnLRX, tnLRY, tnColorNWRgb, tnColorNERgb, tnColorSWRgb, tnColorSERgb, tlUseGradient)
	*img.frame_in_nine_parts(tnULX, tnULY, tnLRX, tnLRY, toImageBitmapSrc)
	*img.draw_line(tnX1, tnY1, tnX2, tnY2, tnColorRgb)
	*img.draw_quad(tnX1, tnY1, tnX2, tnY2, tnWidth, tlDrawEnds, tnColorLineRgb)
	*img.draw_colorized_line_gradient(tnX1, tnY1, tnX2, tnY2, tnColorBegRgb, tnColorEndRgb, tfAlpha)


* Call with @lnW, @lnH as these are return variables
FUNCTION at_say
LPARAMETERS toImg, lnW, lnH, tnY, tnX, tcText, tnForeColorRgb, tcFontName, tnFontSize, tlBold, tlItalic, tlUnderline, tlStrikethru
LOCAL lnX, lnY, lcFontStyle

	IF TYPE("toImg") != "O" OR LOWER(toImg.class) != "imagebitmap"
		RETURN .f.
	ENDIF
	IF TYPE("tnForeColorRgb") != "N"
		tnForeColorRgb = RGB(0,0,0)
	ENDIF
	IF TYPE("tcFontName") != "C"
		tcFontName = "Arial"
	ENDIF
	IF TYPE("tnFontSize") != "N"
		tnFontSize = 12
	ENDIF
	IF TYPE("tlBold") != "L"
		tlBold = .f.
	ENDIF
	IF TYPE("tlItalic") != "L"
		tlItalic = .f.
	ENDIF
	IF TYPE("tlUnderline") != "L"
		tlUnderline = .f.
	ENDIF
	IF TYPE("tlStrikethru") != "L"
		tlStrikethru = .f.
	ENDIF
	
	* Calculate the actual pixel position
	lcFontStyle	= IIF(tlBold, "B", "") + IIF(tlItalic, "I", "") + IIF(tlUnderline, "U", "") + IIF(tlStrikethru, "-", "")
	lnX			= tnX * FONTMETRIC(6, tcFontName, tnFontSize, lcFontStyle)
	lnY			= tnY * FONTMETRIC(1, tcFontName, tnFontSize, lcFontStyle)
	
	* Physically draw
	toImg.draw_font(tcFontName, tnFontSize, tlBold, tlItalic, tlUnderline, tlStrikethru, tcText, lnX, lnY, tnForeColorRgb)
	
	* Grab the extents
	RETURN toImg.get_draw_font_extents(@lnW, @lnH)

ENDFUNC



* Call with @lnW, @lnH as these are return variables
FUNCTION at_say_fixed
LPARAMETERS toImg, lnW, lnH, tnFontX, tnFontY, tcText, tnX, tnY, tnForeColorRgb, tnBackColorRgb

	* Make sure our object is true
	IF TYPE("toImg") != "O" OR LOWER(toImg.class) != "imagebitmap"
		RETURN .f.
	ENDIF
	
	* Indicate the size
	lnW = LEN(tcText) * tnFontX
	lnH = tnFontY
	
	* Draw text
	RETURN toImg.draw_font_fixed_point(tnFontX, tnFontY, tcText, LEN(tcText), tnX, tnY, tnForeColorRgb, tnBackColorRgb)
	
ENDFUNC



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
	cW			= SPACE(0)	&& Image size in inches
	cH			= SPACE(0)	&& Image size in inches
	cBpp		= SPACE(0)
	cPpiX		= SPACE(0)	&& Pixels per inch
	cPpiY		= SPACE(0)	&& Pixels per inch
	
	* Computed values
	nW			= 0		&& Image width in pixels
	nH			= 0		&& Image height in pixels
	
	* For draw_font()
	nTextWidth	= 0
	nTextHeight	= 0

	
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
		DECLARE INTEGER bmp_new						IN image.dll INTEGER tnWidth, INTEGER tnHeight, INTEGER tnBitCount
		DECLARE INTEGER bmp_open_file				IN image.dll STRING cPathname									&& Returns a handle for accessing the bitmap in future operations
		DECLARE INTEGER bmp_get_attributes			IN image.dll INTEGER nHandle, STRING@ cWidthOutInches8, STRING@ cHeightOutInches8, ;	&& Returns floating point in inches
																				  STRING@ cBitsPerPixel8, ;									&& Bits per pixels of intermediate bitmap, typically 24 or 32
																				  STRING@ cPixelsPerInchX8, STRING@ cPixelsPerInchY8		&& Multiply X by width to get pixels, and Y by height

		* Returns a handle for accessing the new bitmap in future operations
		DECLARE INTEGER bmp_crop_to_content			IN image.dll INTEGER nHandle, INTEGER nRgb, INTEGER nType, ;	&& nType is:  0=use white, 1=use black, 2=use nRgb, 3=auto-detect
																 SINGLE fThreshold									&& fThreshold is the "slack" to use for the color identifier, like 0.15 is 15% slack
		DECLARE INTEGER bmp_resize					IN image.dll INTEGER nHandle, SINGLE fScaleFactor				&& Resize by some proportion to get the pixel count down
		DECLARE INTEGER bmp_save_file				IN image.dll INTEGER nHandle, STRING@ cNewBmpPathname			&& Handle remains open after a save, must call bmp_close() to close it
		DECLARE INTEGER bmp_save_as_monochrome		IN image.dll INTEGER nHandle, STRING@ cNewMonoBmpPathname		&& Handle remains open after a save, must call bmp_close() to close it
		DECLARE INTEGER bmp_close					IN image.dll INTEGER nHandle									&& Called to close a handle
		
		DECLARE INTEGER bmp_flip_horizontal			IN image.dll INTEGER nHandle
		DECLARE INTEGER bmp_flip_vertical			IN image.dll INTEGER nHandle
		DECLARE INTEGER bmp_rotate_90_degrees_ccw	IN image.dll INTEGER nHandle

		DECLARE INTEGER bmp_grayscale				IN image.dll INTEGER nHandle, SINGLE fAlpha
		DECLARE INTEGER bmp_set_pixel				IN image.dll INTEGER nHandle, INTEGER nX, INTEGER nY, INTEGER nRgb
		DECLARE INTEGER bmp_get_pixel				IN image.dll INTEGER nHandle, INTEGER nX, INTEGER nY

		DECLARE INTEGER bmp_colorize				IN image.dll INTEGER nHandle, INTEGER nULX, INTEGER nULY, INTEGER nLRX, INTEGER nLRY, INTEGER colorTemplate, SINGLE fAlpha
		DECLARE INTEGER bmp_swap_colors				IN image.dll INTEGER nHandle, INTEGER colorOld, INTEGER colorNew
		DECLARE INTEGER bmp_wavy_line				IN image.dll INTEGER nHandle, INTEGER nULX, INTEGER nULY, INTEGER nLRX, INTEGER nLRY, INTEGER colorLine, SINGLE fAlpha
		DECLARE INTEGER bmp_dapple					IN image.dll INTEGER nHandle, INTEGER nHandleDapple, SINGLE fInfluence
		DECLARE INTEGER bmp_draw_font				IN image.dll INTEGER nHandle, STRING cFontName, INTEGER nPointSize, INTEGER lBold, INTEGER lItalic, INTEGER lUnderline, INTEGER lStrikethru, STRING cText, INTEGER nX, INTEGER nY, INTEGER textColor
		DECLARE INTEGER bmp_draw_font_fixed_point	IN image.dll INTEGER nHandle, INTEGER fontPixelsX, INTEGER fontPixelsY, STRING cText, INTEGER nTextLength, INTEGER nX, INTEGER nY, INTEGER foreColor, INTEGER backColor
		DECLARE INTEGER bmp_draw_bullet				IN image.dll INTEGER nHandle, INTEGER nX, INTEGER nY, INTEGER colorBullet, STRING fAlpha
		* The rectangles are pixel coordinates from upper-left X,Y (nULX,nULY), to lower-right X,Y (nLRX,nLRY), the clipping rectangle
		DECLARE INTEGER bmp_fill_rect				IN image.dll INTEGER nHandle, INTEGER nULX, INTEGER nULY, INTEGER nLRX, INTEGER nLRY, INTEGER nColorNW, INTEGER nColorNE, INTEGER nColorSW, INTEGER nColorSE, INTEGER lUseGradient
		DECLARE INTEGER bmp_frame_rect				IN image.dll INTEGER nHandle, INTEGER nULX, INTEGER nULY, INTEGER nLRX, INTEGER nLRY, INTEGER nColorNW, INTEGER nColorNE, INTEGER nColorSW, INTEGER nColorSE, INTEGER lUseGradient
		DECLARE INTEGER bmp_frame_in_nine_parts		IN image.dll INTEGER nHandle, INTEGER nULX, INTEGER nULY, INTEGER nLRX, INTEGER nLRY, INTEGER nHandleSrc
		DECLARE INTEGER bmp_draw_line				IN image.dll INTEGER nHandle, INTEGER nX1, INTEGER nY1, INTEGER nX2, INTEGER nY2, INTEGER color
		DECLARE INTEGER bmp_draw_quad				IN image.dll INTEGER nHandle, INTEGER nX1, INTEGER nY1, INTEGER nX2, INTEGER nY2, INTEGER nWidth, INTEGER lDrawEnds, INTEGER colorLine
		DECLARE INTEGER bmp_colorized_line_gradient	IN image.dll INTEGER nHandle, INTEGER nX1, INTEGER nY1, INTEGER nX2, INTEGER nY2, INTEGER colorBeg, INTEGER colorEnd, SINGLE fAlpha

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
	
	* Returns:  0=created, negative=errors
	PROCEDURE new
	LPARAMETERS tnWidth, tnHeight, tnBitCount
	LOCAL lnHandle
		
		* Close the existing bitmap (if any)
		IF this.nHandle != 0
			bmp_close(this.nHandle)
			this.nHandle = 0
		ENDIF
		
		* Create a new entry
		lnHandle = bmp_new(tnWidth, tnHeight, tnBitCount)
		IF BETWEEN(lnHandle, -10, -1)
			RETURN -1
		ENDIF
		
		* Store our new bitmap
		this.nHandle = lnHandle
		
		* Grab our attributes
		this.get_attributes()
		
		* Set the original attributes
		this.cWO    = this.cW
		this.cHO    = this.cH
		this.cBppO  = this.cBpp
		this.cPpiXO = this.cPpiX
		this.cPpiYO = this.cPpiY
		
		* Signify
		RETURN 0
		
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
		lnHandle = bmp_open_file(tcImagePathname)
		IF BETWEEN(lnHandle, -10, -1)
			RETURN -2
		ENDIF
		
		* It's loaded
		this.nHandle = lnHandle
		
		* Grab the attributes
		this.get_attributes()
		
		* Set the original attributes
		this.cWO    = this.cW
		this.cHO    = this.cH
		this.cBppO  = this.cBpp
		this.cPpiXO = this.cPpiX
		this.cPpiYO = this.cPpiY
		
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
		
		* Computed values
		this.nW		= CEIL(VAL(this.cW) * VAL(this.cPpiX))
		this.nH		= CEIL(VAL(this.cH) * VAL(this.cPpiY))
		
		* Signify
		RETURN 0
		
	ENDPROC
	
	**********
	* tnType should be one of:
	*
	*		0 -- use white
	*		1 -- use black
	*		2 -- use nRgb
	*		3 -- auto-detect
	*
	* tfThreshold indicates how much "slop" is allowed
	* in the color being used for crop:
	*
	*		0.15 would be a 15% slack, meaning colors within
	*		15% of the target color will be considered
	*
	PROCEDURE crop
	LPARAMETERS tnRgb, tnType, tfThreshold
	LOCAL lnResult, llTestP2, llTestP3
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Make sure we have valid parameters
		DO CASE
			CASE PCOUNT() = 3
				* Three parameters
				llTestP3 = .t.
				llTestP2 = .t.
				
			CASE PCOUNT() = 2
				* Two parameters
				llTestP2 = .t.
				
			OTHERWISE
				* One or zero parameters
				* Set the type to use white
				tnType = 0
		ENDCASE
		
		* Set the threshold
		IF llTestP3
			IF TYPE("tfThreshold") != "N"
				* Invalid option
				RETURN -2
			ENDIF
		ELSE
			tfThreshold = 0.0
		ENDIF
		tfThreshold = MIN(MAX(tfThreshold, 0.0), 1.0)
		
		* Set the type
		IF llTestP2
			* Make sure our type is valid:  0=use white, 1=use black, 2=use tnRgb, 3=auto-detect
			IF NOT INLIST(tnType, 0, 1, 2, 3)
				* Invalid choice
				RETURN -3
			ENDIF
		ENDIF
		
		* Physically crop
		lnResult = bmp_crop_to_content(this.nHandle, tnRgb, tnType, tfThreshold)
		IF BETWEEN(lnResult, -10, -1)
			RETURN -4
		ENDIF
		
		* Release the original, and set the handle to the cropped version
		bmp_close(this.nHandle)
		this.nHandle = lnResult
		
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
		
		* Delete the existing image, and set the handle to the scaled image
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
	
	PROCEDURE save_mono
	LPARAMETERS tcOutputPathname
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Save as indicated
		lnResult = bmp_save_as_monochrome(this.nHandle, tcOutputPathname)
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

	PROCEDURE flip
	LPARAMETERS tlHorizontal
	LOCAL lnResult
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Process and signify
		IF tlHorizontal
			lnResult = bmp_flip_horizontal(this.nHandle)
		ELSE
			lnResult = bmp_flip_vertical(this.nHandle)
		ENDIF
		
		* How was the information
		IF BETWEEN(lnResult, -10, -1)
			RETURN -2
		ENDIF
		
		* Delete the existing image, and set the handle to the new image
		bmp_close(this.nHandle)
		this.nHandle = lnResult
		
		* Signify
		RETURN lnResult
	
	ENDPROC
	
	PROCEDURE rotate_90_degrees_ccw
	LOCAL lnResult
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Process and signify
		lnResult = bmp_rotate_90_degrees_ccw(this.nHandle)
		
		* How was the information
		IF BETWEEN(lnResult, -10, -1)
			RETURN -2
		ENDIF
		
		* Delete the existing image, and set the handle to the new image
		bmp_close(this.nHandle)
		this.nHandle = lnResult
		
		* Signify
		RETURN lnResult
	
	ENDPROC
	
	PROCEDURE grayscale
	LPARAMETERS tfAlpha
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Make sure our alpha is in range 0.0 to 1.0
		IF TYPE("tfAlpha") != "N"
			tfAlpha = 1.0
		ENDIF
		tfAlpha = MIN(MAX(tfAlpha, 0.0), 1.0)
		
		* Process and signify
		RETURN bmp_grayscale(this.nHandle, tfAlpha)
	
	ENDPROC
	
	PROCEDURE set_pixel
	LPARAMETERS tnX, tnY, tnRgb
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tnX") + TYPE("tnY") + TYPE("tnRgb") != "NNN"
			RETURN -2
		ENDIF
		
		* Process and signify
		RETURN bmp_set_pixel(this.nHandle, tnX, tnY, tnRgb)
		
	ENDPROC
	
	PROCEDURE get_pixel
	LPARAMETERS tnX, tnY
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tnX") + TYPE("tnY") != "NN"
			RETURN -2
		ENDIF
		
		* Process and signify
		RETURN bmp_get_pixel(this.nHandle, tnX, tnY)
		
	ENDPROC
	
	PROCEDURE colorize
	LPARAMETERS tnULX, tnULY, tnLRX, tnLRY, tnColorTemplateRgb, tfAlpha
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tnULX") + TYPE("tnULY") + TYPE("tnLRX") + TYPE("tnLRY") != "NNNN"
			RETURN -2
		ENDIF
		IF TYPE("tnColorTemplateRgb") != "N"
			RETURN -3
		ENDIF
		IF TYPE("tfAlpha") != "N"
			RETURN -4
		ENDIF
		
		* Process and signify
		RETURN bmp_colorize(this.nHandle, tnULX, tnULY, tnLRX, tnLRY, tnColorTemplateRgb, tfAlpha)
		
	ENDPROC
	
	PROCEDURE swap_colors
	LPARAMETERS tnColorOldRgb, tnColorNewRgb
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tnColorOldRgb") + TYPE("tnColorNewRgb") != "NN"
			RETURN -2
		ENDIF
		
		* Process and signify
		RETURN bmp_swap_colors(this.nHandle, tnColorOldRgb, tnColorNewRgb)
	
	ENDPROC
	
	PROCEDURE wavy_line
	LPARAMETERS tnULX, tnULY, tnLRX, tnLRY, tnColorLineRgb, tfAlpha
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tnULX") + TYPE("tnULY") + TYPE("tnLRX") + TYPE("tnLRY") != "NNNN"
			RETURN -2
		ENDIF
		IF TYPE("tnColorLineRgb") != "N"
			RETURN -3
		ENDIF
		IF TYPE("tfAlpha") != "N"
			RETURN -4
		ENDIF
		
		* Process and signify
		RETURN bmp_wavy_line(this.nHandle, tnULX, tnULY, tnLRX, tnLRY, tnColorLineRgb, tfAlpha)
	
	ENDPROC
	
	PROCEDURE dapple
	LPARAMETERS toImageBitmapDapple, tfInfluence
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("toImageBitmapDapple") + TYPE("tfInfluence") != "ON"
			RETURN -2
		ENDIF
		IF LOWER(toImageBitmapDapple.class) = "imagebitmap"
			RETURN -3
		ENDIF
		
		* Process and signify
		RETURN bmp_dapple(this.nHandle, toImageBitmapDapple.nHandle, tfInfluence)
	
	ENDPROC
	
	PROCEDURE draw_font
	LPARAMETERS tcFontName, tnPointSize, tlBold, tlItalic, tlUnderline, tlStrikethru, tcText, tnX, tnY, tnTextColorRgb
	LOCAL lnResult
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tcFontName") + TYPE("tnPointSize") + TYPE("tlBold") + ;
		     TYPE("tlItalic") + TYPE("tlUnderline") + TYPE("tlStrikethru") + ;
		       TYPE("tcText") +         TYPE("tnX") + TYPE("tnY") + TYPE("tnTextColorRgb") != "CNLLLLCNNN"
			RETURN -2
		ENDIF

		* Process and signify
		lnResult = bmp_draw_font(this.nHandle, tcFontName, tnPointSize, tlBold, tlItalic, tlUnderline, tlStrikethru, tcText, tnX, tnY, tnTextColorRgb)
		
		* Store the width and height of the text that was just drawn
		this.nTextWidth  =           BITAND(lnResult, 0x0000ffff)
		this.nTextHeight = BITRSHIFT(BITAND(lnResult, 0xffff0000), 16)
	
	ENDPROC
	
	* Call with @lnWidth, @lnHeight as these are return parameters
	PROCEDURE get_draw_font_extents
	LPARAMETERS lnWidth, lnHeight
	
		lnWidth  = this.nTextWidth
		lnHeight = this.nTextHeight
		
	ENDPROC
	
	PROCEDURE draw_font_fixed_point
	LPARAMETERS tnFontPixelsX, tnFontPixelsY, tcText, tnTextLength, tnX, tnY, tnForeColorRgb, tnBackColorRgb
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF  TYPE("tnFontPixelsX") +  TYPE("tnFontPixelsY") + TYPE("tcText") + ;
		     TYPE("tnTextLength") +            TYPE("tnX") +    TYPE("tnY") + ;
		   TYPE("tnForeColorRgb") + TYPE("tnBackColorRgb") != "NNCNNNNN"
			RETURN -2
		ENDIF

		* Process and signify
		RETURN bmp_draw_font_fixed_point(this.nHandle, tnFontPixelsX, tnFontPixelsY, tcText, tnTextLength, tnX, tnY, tnForeColorRgb, tnBackColorRgb)
	
	ENDPROC
	
	PROCEDURE draw_bullet
	LPARAMETERS tnX, tnY, tnColorBulletRgb, tfAlpha
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tnX") + TYPE("tnY") + TYPE("tnColorBulletRgb") != "NNN"
			RETURN -2
		ENDIF
		
		* Make sure we have an alpha
		IF TYPE("tfAlpha") != "N"
			tfAlpha = 1.0
		ENDIF
		tfAlpha = MAX(MIN(tfAlpha, 1.0), 0.0)

		* Process and signify
		RETURN bmp_draw_bullet(this.nHandle, tnX, tnY, tnColorBulletRgb, tfAlpha)
	
	ENDPROC
	
	* The rectangles are pixel coordinates from upper-left X,Y (nULX,nULY), to lower-right X,Y (nLRX,nLRY), the clipping rectangle
	PROCEDURE fill_rect
	LPARAMETERS tnULX, tnULY, tnLRX, tnLRY, tnColorNWRgb, tnColorNERgb, tnColorSWRgb, tnColorSERgb, tlUseGradient
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tnULX") + TYPE("tnULY") + TYPE("tnLRX") + TYPE("tnLRY") + ;
		   TYPE("tnColorNWRgb") + TYPE("tnColorNERgb") + TYPE("tnColorSWRgb") + ;
		   TYPE("tnColorSERgb") + TYPE("tlUseGradient") != "NNNNNNNNL"
			RETURN -2
		ENDIF

		* Process and signify
		RETURN bmp_fill_rect(this.nHandle, tnULX, tnULY, tnLRX, tnLRY, tnColorNWRgb, tnColorNERgb, tnColorSWRgb, tnColorSERgb, tlUseGradient)
	
	ENDPROC
	
	PROCEDURE frame_rect
	LPARAMETERS tnULX, tnULY, tnLRX, tnLRY, tnColorNWRgb, tnColorNERgb, tnColorSWRgb, tnColorSERgb, tlUseGradient
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tnULX") + TYPE("tnULY") + TYPE("tnLRX") + TYPE("tnLRY") + ;
		   TYPE("tnColorNWRgb") + TYPE("tnColorNERgb") + TYPE("tnColorSWRgb") + ;
		   TYPE("tnColorSERgb") + TYPE("tlUseGradient") != "NNNNNNNNL"
			RETURN -2
		ENDIF

		* Process and signify
		RETURN bmp_frame_rect(this.nHandle, tnULX, tnULY, tnLRX, tnLRY, tnColorNWRgb, tnColorNERgb, tnColorSWRgb, tnColorSERgb, tlUseGradient)
	
	ENDPROC
	
	PROCEDURE frame_in_nine_parts
	LPARAMETERS tnULX, tnULY, tnLRX, tnLRY, toImageBitmapSrc
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tnULX") + TYPE("tnULY") + TYPE("tnLRX") + TYPE("tnLRY") + TYPE("toImageBitmapSrc") != "NNNNO"
			RETURN -2
		ENDIF
		IF LOWER(toImageBitmapSrc.class) != "imagebitmap"
			RETURN -3
		ENDIF

		* Process and signify
		RETURN bmp_frame_in_nine_parts(this.nHandle, tnULX, tnULY, tnLRX, tnLRY, toImageBitmapSrc.nHandle)
	
	ENDPROC
	
	PROCEDURE draw_line
	LPARAMETERS tnX1, tnY1, tnX2, tnY2, tnColorRgb
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tnX1") + TYPE("tnY1") + TYPE("tnX2") + TYPE("tnY2") + TYPE("tnColorRgb") != "NNNNN"
			RETURN -2
		ENDIF

		* Process and signify
		RETURN bmp_draw_line(this.nHandle, tnX1, tnY1, tnX2, tnY2, tnColorRgb)
	
	ENDPROC
	
	PROCEDURE draw_quad
	LPARAMETERS tnX1, tnY1, tnX2, tnY2, tnWidth, tlDrawEnds, tnColorLineRgb
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tnX1") + TYPE("tnY1") + TYPE("tnX2") + TYPE("tnY2") + TYPE("tlDrawEnds") + TYPE("tnColorLineRgb") != "NNNNLN"
			RETURN -2
		ENDIF

		* Process and signify
		RETURN bmp_draw_quad(this.nHandle, tnX1, tnY1, tnX2, tnY2, tnWidth, tlDrawEnds, tnColorLineRgb)
	
	ENDPROC
	
	PROCEDURE draw_colorized_line_gradient
	LPARAMETERS tnX1, tnY1, tnX2, tnY2, tnColorBegRgb, tnColorEndRgb, tfAlpha
	
		* Make sure we have a valid handle
		IF this.nHandle = 0
			RETURN -1
		ENDIF
		
		* Verify parameters
		IF TYPE("tnX1") + TYPE("tnY1") + TYPE("tnX2") + TYPE("tnY2") + TYPE("tnColorBegRgb") + TYPE("tnColorEndRgb") != "NNNNNN"
			RETURN -2
		ENDIF

		* Process and signify
		RETURN bmp_colorized_line_gradient(this.nHandle, tnX1, tnY1, tnX2, tnY2, tnColorBegRgb, tnColorEndRgb, tfAlpha)
	
	ENDPROC
	
ENDDEFINE
