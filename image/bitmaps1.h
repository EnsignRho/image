//////////
//
// /libsf/source/vjr/bitmaps.h
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
// Version 0.58
// Copyright (c) 2014-2015 by Rick C. Hodgin
//////
// Last update:
//     Nov.27.2014
//////
// Change log:
//     Nov.27.2014 - Initial creation
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
//



class CXml;
struct SNode;
struct SNodeProps;
struct SObject;

#include "font8x6.h"
#include "font8x8.h"
#include "font8x14.h"
#include "font8x16.h"
#include "font16x32.h"

#define _EXCLUDE_iBmp_nbsp_createAndPopulate

//////////
// Color constants
//////
	cu32			_disabledBackColor					= rgba(255, 255, 255, 255);
	cu32			_disabledForeColor					= rgba(192, 192, 230, 255);

	cu32			_focusObjColor_readWrite_container	= rgba(97,31,128, 255);
	cu32			_focusObjColor_readWrite_obj		= rgba(112,164,255, 255);
	cu32			_focusObjColor_readOnly				= rgba(255, 112, 112, 255);


	// Color theme per Stefano D'Amico, suggested Mar.25.2015
	cu32			_nwColor_form						= rgba(35,101,178,255);
	cu32			_neColor_form						= rgba(37,108,192,255);
	cu32			_swColor_form						= rgba(37,108,192,255);
	cu32			_seColor_form						= rgba(40,116,204,255);

	cu32			_nwColor_subform					= rgba(164,192,255,255);
	cu32			_neColor_subform					= rgba(133,185,245,255);
	cu32			_seColor_subform					= rgba(133,185,245,255);
	cu32			_swColor_subform					= rgba(164,192,255,255);

	cu32			_nwColor_focus						= rgba(205, 172, 255, 255);
	cu32			_neColor_focus						= rgba(192, 164, 235, 255);
	cu32			_swColor_focus						= rgba(192, 164, 235, 255);
	cu32			_seColor_focus						= rgba(182, 153, 226, 255);

	cu32			_whiteColor							= rgba(255, 255, 255, 255);
	cu32			_eggshellColor						= rgba(250, 245, 240, 255);
	cu32			_silverColor						= rgba(225, 225, 225, 255);
	cu32			_grayColor							= rgba(192, 192, 192, 255);
	cu32			_darkGrayColor						= rgba(128, 128, 128, 255);
	cu32			_charcoalColor						= rgba(64, 64, 64, 255);
	cu32			_blackColor							= rgba(0, 0, 0, 255);
	cu32			_selectedBackColor					= rgba(220, 235, 255, 255);
	cu32			_selectedForeColor					= rgba(0, 0, 164, 255);

	const SBgra		nwColor_form						= { _nwColor_form };
	const SBgra		neColor_form						= { _neColor_form };
	const SBgra		seColor_form						= { _seColor_form };
	const SBgra		swColor_form						= { _swColor_form };

	const SBgra		nwColor_subform						= { _nwColor_subform };
	const SBgra		neColor_subform						= { _neColor_subform };
	const SBgra		seColor_subform						= { _seColor_subform };
	const SBgra		swColor_subform						= { _swColor_subform };


	// Forms four-corner window color schemes (eventually these will be loaded from themes.dbf)
// Orange theme:
	const SBgra		nwColor_focus						= { _nwColor_focus };
	const SBgra		neColor_focus						= { _neColor_focus };
	const SBgra		swColor_focus						= { _swColor_focus };
	const SBgra		seColor_focus						= { _seColor_focus };

	const SBgra		nwColor_nonfocus					= { _nwColor_form };
	const SBgra		neColor_nonfocus					= { _neColor_form };
	const SBgra		swColor_nonfocus					= { _swColor_form };
	const SBgra		seColor_nonfocus					= { _seColor_form };

	// Colors for checkbox corners
	const SBgra		nwColor_checkboxOn					= { rgba(24, 153, 2, 255) };			// Green
	const SBgra		neColor_checkboxOn					= { rgba(37, 163, 3, 255) };
	const SBgra		swColor_checkboxOn					= { rgba(5, 140, 0, 255) };
	const SBgra		seColor_checkboxOn					= { rgba(131, 220, 11, 255) };
	const SBgra		nwColor_checkboxOff					= { rgba(193, 34, 34, 255) };			// Red
	const SBgra		neColor_checkboxOff					= { rgba(181, 64, 64, 255) };
	const SBgra		swColor_checkboxOff					= { rgba(171, 92, 94, 255) };
	const SBgra		seColor_checkboxOff					= { rgba(212, 128, 131, 255) };


//////////
// Colors
//////
	SBgra			tooltipNwBackColor					= { rgba(255, 254, 230, 255) };			// Pastel yellow
	SBgra			tooltipNeBackColor					= { rgba(252, 242, 192, 25) };			// Less pastel yellow, somewhat pale
	SBgra			tooltipSwBackColor					= { rgba(249, 222, 133, 255) };			// Orange/golden yellow
	SBgra			tooltipSeBackColor					= { rgba(247, 210, 96, 255) };			// More orange/golden yellow
	SBgra			tooltipForecolor					= { rgba(84, 56, 12, 255) };			// Dark chocolate brown
	SBgra			lineNumberFillColor					= { rgba(225, 245, 240, 255) };
	SBgra			lineNumberBackColor					= { rgba(215, 235, 230, 255) };
	SBgra			lineNumberForeColor					= { rgba(140, 160, 140, 255) };
	SBgra			breadcrumbBackColor					= { rgba(180, 220, 240, 255) };			// Cyanish
	SBgra			breadcrumbForeColor					= { rgba(0, 0, 164, 255) };				// Semidark blue
	SBgra			breakpointBackColor					= { rgba(180, 140, 220, 255) };			// Purplish
	SBgra			breakpointForeColor					= { rgba(64, 32, 92, 255) };			// Dark purple
	SBgra			currentStatementBackColor			= { rgba(225, 255, 192, 255) };			// Pastel lime greenish
	SBgra			currentStatementForeColor			= { rgba(0, 64, 0, 255) };				// Dark green
	SBgra			overrideMatchingBackColor			= { rgba(0, 255, 0, 255) };				// Green
	SBgra			overrideMatchingForeColor			= { rgba(0, 0, 0, 255) };				// Black
	SBgra			overrideMatchingBackColorMultiple	= { rgba(0, 255, 0, 255) };				// Green
	SBgra			overrideMatchingForeColorMultiple	= { rgba(0, 0, 0, 255) };				// Black
	SBgra			linkForeColor						= { rgba(0, 0, 64, 255) };				// Dark blue
	SBgra			linkBackColor						= { rgba(245, 245, 255, 255) };			// Pastel blue
	SBgra			selectedBackColor					= { _selectedBackColor };				// Pastel turquoise
	SBgra			selectedForeColor					= { _selectedForeColor };				// Darkish blue
	SBgra			disabledBackColor					= { _disabledBackColor };
	SBgra			disabledForeColor					= { _disabledForeColor };
	SBgra			disabledItemBackColor				= { _disabledBackColor };
	SBgra			disabledItemForeColor				= { _disabledForeColor };
	SBgra			highlightSymbolBackColor			= { rgba(235, 220, 255, 255) };			// Very pastel purple
	SBgra			colorTracking						= { rgba(0, 0, 255, 255) };				// Blue
	f32				trackingRatio						= 0.025f;
	SBgra			colorMouseOver						= { rgba(255, 255, 0, 255) };			// Yellow
	SBgra			colorMouseDown						= { rgba(0, 255, 0, 255) };				// Green
	SBgra			editNewColor						= { rgba(64, 200, 64, 255) };			// Greenish
	SBgra			editChangedColor					= { rgba(255, 200, 64, 255) };			// Amberish
	SBgra			carouselFrameColor					= { rgba(94, 94, 128, 255) };			// Dark purleish-gray
	SBgra			carouselFrameInactiveColor			= { rgba(132, 132, 192, 255) };			// Lighter purpleish-gray
	SBgra			carouselBackColor					= { rgba(255, 255, 255, 255) };			// White
	SBgra			carouselTabBackColor				= { rgba(255, 255, 255, 255) };			// White
	SBgra			carouselTabForeColor				= { rgba(64, 64, 88, 255) };			// Darker purpleish-gray
	SBgra			carouselTabForeColorHighlight		= { rgba(32, 32, 44, 255) };			// Darkest purpleish-gray
	SBgra			toolbarBackColor					= { _nwColor_subform };

	// Colors for the focus window (highlights controls)
	SBgra			focusObjColor_readWrite_container	= { _focusObjColor_readWrite_container };
	SBgra			focusObjColor_readWrite_obj			= { _focusObjColor_readWrite_obj };
	SBgra			focusObjColor_readOnly				= { _focusObjColor_readOnly };



//////////
// Constants
//////
	#define	_FILL_RECT_OP_AND										1
	#define	_FILL_RECT_OP_OR										2
	#define	_FILL_RECT_OP_XOR										3

	// Colors
	const SBgra				whiteColor								= { _whiteColor };
	const SBgra				eggshellColor							= { _eggshellColor };
	const SBgra				silverColor								= { _silverColor };
	const SBgra				grayColor								= { _grayColor };
	const SBgra				blackColor								= { _blackColor };
	const SBgra				darkGrayColor							= { _darkGrayColor };
	const SBgra				charcoalColor							= { _charcoalColor };
	const SBgra				yellowColor								= { rgba(255, 255, 0, 255) };
	const SBgra				redColor								= { rgba(255, 0, 0, 255) };
	const SBgra				greenColor								= { rgba(0, 255, 0, 255) };
	const SBgra				blueColor								= { rgba(0, 0, 255, 255) };
	const SBgra				purpleColor								= { rgba(92, 48, 112, 255) };
	const SBgra				cyanColor								= { rgba(0, 255, 255, 255) };
	const SBgra				orangeColor								= { rgba(255, 200, 64, 255) };
	const SBgra				fuchsiaColor							= { rgba(255, 255, 0, 255) };
	const SBgra				darkRedColor							= { rgba(80, 0, 0, 255) };
	const SBgra				darkOrangeColor							= { rgba(128, 64, 0, 255) };
	const SBgra				darkGreenColor							= { rgba(0, 80, 0, 255) };
	const SBgra				darkBlueColor							= { rgba(0, 0, 80, 255) };
	const SBgra				pastelYellowColor						= { rgba(255, 255, 128, 255) };
	const SBgra				pastelRedColor							= { rgba(255, 200, 200, 255) };
	const SBgra				pastelOrangeColor						= { rgba(255, 205, 155, 255) };
	const SBgra				pastelGreenColor						= { rgba(200, 255, 200, 255) };
	const SBgra				pastelBlueColor							= { rgba(215, 215, 255, 255) };
	const SBgra				pastelLavender							= { rgba(235, 235, 250, 255) };
	const SBgra				maskColor								= { rgba(222, 22, 222, 255) };			// Fuscia

	const SBgra colorArray[] = {
		redColor,
		orangeColor,
		yellowColor,
		greenColor,
		blueColor,
		cyanColor,
		purpleColor,
		eggshellColor,
		silverColor,
		fuchsiaColor,
		pastelRedColor,
		pastelOrangeColor,
		pastelYellowColor,
		pastelGreenColor,
		pastelBlueColor,
		pastelLavender,
		maskColor,
	};

	// Attribute tag names
	cs8					cgcTag_vertical[]						= "vertical";
	cs8					cgcTag_horizontal[]						= "horizontal";
	cs8					cgcTag_layout[]							= "layout";
	cs8					cgcTag_name[]							= "name";
	cs8					cgcTag_value[]							= "val";
	cs8					cgcTag_typeDetail[]						= "td";
	cs8					cgcTag_object[]							= "obj";
	cs8					cgcTag_baseclass[]						= "bc";
	cs8					cgcTag_class[]							= "c";
	cs8					cgcTag_classLibrary[]					= "cl";
	cs8					cgcTag_p[]								= "p";			// Fixed properties
	cs8					cgcTag_props[]							= "props";		// Class properties container
	cs8					cgcTag_prop[]							= "prop";		// Class property
	cs8					cgcTag_h[]								= "h";			// Height
	cs8					cgcTag_x[]								= "x";			// X
	cs8					cgcTag_y[]								= "y";			// Y
	cs8					cgcTag_w[]								= "w";			// Width
	cs8					cgcTag_width[]							= "width";
	cs8					cgcTag_height[]							= "height";
	cs8					cgcTag_iconWidth[]						= "iconwidth";
	cs8					cgcTag_iconHeight[]						= "iconheight";
	cs8					cgcTag_visible[]						= "visible";

	// Save file types
	cs8					cgcPng[]								= "png";
	cs8					cgcJpg[]								= "jpg";
	cs8					cgcGif[]								= "gif";
	cs8					cgcTif[]								= "tif";
	cs8					cgcBmp[]								= "bmp";	// Not used, because any type not recognized is auto-saved as a BMP file type
	// Wide-char versions
	wchar_t				cgwPng[]								= L"png";
	wchar_t				cgwJpg[]								= L"jpg";
	wchar_t				cgwGif[]								= L"gif";
	wchar_t				cgwTif[]								= L"tif";
	wchar_t				cgwBmp[]								= L"bmp";	// Not used, because any type not recognized is auto-saved as a BMP file type




//////////
// Structures
//////
// 	struct SFreetypeBit
// 	{
// 		f32				fore;					// Calculation for the fore overlay for this pixel, from 0.0 to 1.0
// 		f32				back;					// Calculation for the back overlay for this pixel, from 0.0 to 1.0
// 	};
// 
// 	struct SFreetypeChar
// 	{
// 		SFreetypeBit*	bits;					// Bitmap data for this font (bits[height * width])
// 		POINT			ul;						// How far to index into the pen's y,x location to write the upper-left pixel
// 		s32				advance_x;				// How far to advance for this character
// 		s32				width;					// How wide is the bits array?
// 		s32				height;					// How tall is the bits array?
// 	};
// 
// 	// Related to gsFreetypeGlyphs, allows reuse of previously generated bitmaps for each glyph at each size/setting
// 	struct SFreetypeGlyphs
// 	{
// 		bool		isUsed;
// 
// 		// Font attributes
// 		s8*			fontPathname;
// 		s32			fontSize;
// 		bool		lBold;
// 		bool		lItalics;
// 		bool		lUnderline;
// 		bool		lStrikethru;
// 
// 		// Font metrics
// 		s32			advance_y;
// 		s32			advance_x;
// 		bool		lOtherProcessing;
// 		s32			nUnderlineYMin;
// 		s32			nUnderlineYMax;
// 		s32			nStrikethruYMin;
// 		s32			nStrikethruYMax;
// 		SBuilder*	glyphs;
// 	};

	enum EImageFormat
	{
		PNG,
		JPG,
		GIF,
		TIF,
		BMP,
	};

	struct SFont
	{
		HDC			hdc;						// Device context used for its creation
		bool		isUsed;						// Is this font slot used?
		bool		isFreetype;					// is this a freetype font?

		// Current font instance flags
		s8			name[256];					// Name of this font
		s32			charset;					// Font charset
		bool		isBold;						// Is the font bold? (Note: This is independent of the font name itself having bold in it, such as "Ubuntu Bold"
		bool		isItalic;					// Is the font italic?
		bool		isUnderline;				// Is the font underline?
		bool		isStrikethru;				// Is the font strikethru?
		bool		isCondensed;				// Is the font condensed?
		bool		isExtended;					// Is the font extended?
		bool		isOutline;					// Is the font outlined?
		bool		isShadow;					// Is the font shadowed?

		// OS font handle
		HFONT		hfont;

		// Internal Windows settings
		s32			_sizeUsedForCreateFont;		// Computed value based on passed font size
		u32			_size;						// Actual point size
		u32			_weight;					// Actual weight
		u32			_italics;					// Actual italics setting
		u32			_underline;					// Actual underline setting
		TEXTMETRIC	tm;							// Text metrics computed at the time of creation

		// Miscellaneous
		RECT		rc;

		// User-set values (not automatically setup, not automatically cleared)
		SBgra		user_foreColor;				// Foreground color
		SBgra		user_backColor;				// Background color
		bool		user_isMonospace;			// Is the font a monospace


		//////////
		// Freetype-related
		//////
// 			FT_Face				face;			// The font face
// 			SFreetypeGlyphs*	fg;				// (SFreetypeChar) When the font is created glyphs are generated for every character 0..255
			s32					tabwidth;		// The width for each tab character

	};

	struct SBitmap
	{
		// Device context and bitmap handle to this data
		HDC					hdc;
		HBITMAP				hbmp;
		RECT				rc;
		u32					iter;					// For graphics ops, holds a rendering iteration count
		SFont*				font;					// If a font is associated with this context, it is set here

		// Raw bitmap data (suitable for writing to disk)
		BITMAPFILEHEADER	bh;
		BITMAPINFOHEADER	bi;

		union {
			s8*				bd;						// Legacy access
			s8*				bds;					// Signed char access
			u8*				bdu;					// Unsigned char access
			SBgr*			bdrgb;					// Access as 24-bit rgb
			SBgra*			bdrgba;					// Access as 32-bit rgba
		};
		u32					rowWidth;				// See: iBmp_computeRowWidth()
	};

	// Used for arrays extracted from parent arrays
	struct SBitmapArray
	{
		RECT			rc;							// Reference in parent source
		SBitmap*		bmp;						// Extracted bitmap
	};

	struct SBmpCache
	{
		SBitmap*		bmpCached;

		u32				data1;
		u32				data2;
		u32				data3;
		u32				data4;
		u32				data5;
		u32				data6;
		u32				data7;
		u32				data8;
		u32				data9;
		u32				data10;
		u32				data11;
		u32				data12;
		u32				data13;
		u32				data14;
	};

	// For processing the rotation of bitmaps
	struct SBmpRotate
	{
		SBgra*			src;					// Ptr to the source pixel to obtain color info
		SBgra*			dst;					// Ptr to the destination pixel to update
		f32				fPercent;				// Influence of this color
	};

	// For processing the scaling of bitmaps
	struct SBitmapProcess
	{
		// Holds the source canvas
		SBitmap*			src;

		// Holds storage data for single spanned pixels
		u32					red;
		u32					grn;
		u32					blu;
		u32					alp;

		// tpoints is a buffer created to store the conversion pixel data during accumulation.
		// Use formula:
		//		tpoints	= (SRGBAF*)malloc(		((u32)(1.0/ratioV) + 3)   *
		//										((u32)(1.0/ratioH) + 3))
		//
		f32					ratioV;				// (f32)bio->biHeight	/ (f32)bii->biHeight;
		f32					ratioH;				// (f32)bio->biWidth	/ (f32)bii->biWidth;
		u32					count;				// Number of valid points in tpoints
		SBgraf*				pixels;				// Accumulation buffer for point data needed to feed into destination

		// Temporary variables used for processing
		union {
			SBgra*			optra;				// (For 32-bit bitmaps) Output pointer to the upper-left pixel for this x,y
			SBgr*			optr;				// (For 24-bit bitmaps) Output pointer to the upper-left pixel for this x,y
		};
		union {
			SBgra*			iptrAnchora;		// (For 32-bit bitmaps) Input pointer to the left-most pixel of the first row (the anchor)
			SBgr*			iptrAnchor;			// (For 24-bit bitmaps) Input pointer to the left-most pixel of the first row (the anchor)
		};
		union {
			SBgra*			iptra;				// (For 32-bit bitmaps) Input pointer to the left-most pixel for this y row
			SBgr*			iptr;				// (For 24-bit bitmaps) Input pointer to the left-most pixel for this y row
		};
		f32					ulx;				// Upper-left X
		f32					uly;				// Upper-left Y
		f32					lrx;				// Lower-right X
		f32					lry;				// Lower-right Y
		f32					widthLeft;			// Width for each left-most pixel
		f32					widthRight;			// Width for each right-most pixel
		f32					height;				// Height for a particular pixel portion (upper, lower)
		f32					area;				// Temporary computed area for various pixels
		s32					left;				// Left-side pixel offset into line
		s32					right;				// Number of pixels to reach the right-most pixel
		s32					middleStartH;		// Starting pixel offset for middle span
		s32					middleFinishH;		// Ending pixel offset for middle span
		s32					middleStartV;		// Middle starting pixel
		s32					middleFinishV;		// Middle ending pixel

		// Indicates the span from upper-left corner
		bool				spans2H;			// Spans at least 2 pixels horizontally, meaning it begins somewhere in 1.x, for example, and ends somewhere beyond 2.x
		bool				spans3H;			// Spans at least 3 pixels horizontally, meaning it begins somewhere in 1.x, for example, and ends somewhere beyond 3.x
		bool				spans2V;			// Spans at least 2 pixels vertically, meaning it begins somewhere in 1.x, for example, and ends somewhere beyond 2.x
		bool				spans3V;			// Spans at least 3 pixels vertically, meaning it begins somewhere in 1.x, for example, and ends somewhere beyond 3.x
	};


//////////
// Function prototypes
//////
	SBitmap*				iBmp_allocate							(void);
	SBitmap*				iBmp_copy								(SBitmap* bmpSrc);
	SBitmap*				iBmp_cropToContent						(SBitmap* bmpSrc, int tnRgb, int tnType);
	bool					iibmp_getBorderColors					(SBitmap* bmp, SRgb* cTop, SRgb* cRight, SRgb* cBottom, SRgb* cLeft);
	bool					iBmp_compareColors						(SRgb* cRef, ...);
	SBitmap*				iBmp_copyColorize						(SBitmap* bmpSrc, SBgra overColor, f32 minColor = 0.75f);
	SBitmap*				iBmp_verifyCopyIsSameSize				(SBitmap* bmpCopy, SBitmap* bmp);
	SBitmap*				iBmp_verifySizeOrResize					(SBitmap* bmp, u32 tnWidth, u32 tnHeight, u32 tnBitCount);
	SBitmap*				iBmp_rawLoadImageFile					(cu8* bmpRawFileData, bool tlLeaveAs32Bits = false, bool tlFlipOnLoad = false);
	SBitmap*				iBmp_rawLoad_fromFile					(cu8* rawFileData, u32 tnRawFileDataLength, cu8* fileExtension, bool tlReverseOnLoad = false);
	s32						iBmp_rawSave							(SBitmap* bmp, cs8* tcFilename, EImageFormat format);
	bool					iiBmp_findCodecByExtension				(ImageCodecInfo* codec, wchar_t* twFilenameExtension);
	SBitmap*				iBmp_isValidCache						(SBmpCache** bmpCache,               u32 data1, u32 data2, u32 data3, u32 data4, u32 data5, u32 data6, u32 data7, u32 data8, u32 data9, u32 data10, u32 data11, u32 data12, u32 data13, u32 data14);
	SBitmap*				iBmp_createCache						(SBmpCache** bmpCache, SBitmap* bmp, u32 data1, u32 data2, u32 data3, u32 data4, u32 data5, u32 data6, u32 data7, u32 data8, u32 data9, u32 data10, u32 data11, u32 data12, u32 data13, u32 data14, bool tlCopyBitmap);
	SBitmap*				iBmp_deleteCache						(SBmpCache** bmpCache);
	void					iBmp_saveToDisk							(SBitmap* bmp, s8* tcPathname);
	void					iBmp_convertTo32Bits					(SBitmap* bmp);
	void					iBmp_convertTo24Bits					(SBitmap* bmp);
	void					iBmp_convertFrom_argb					(SBitmap* bmp);
	void					iBmp_copy24To32							(SBitmap* bmp32, SBitmap* bmp24);
	bool					iBmp_validate							(SBitmap* bmp);
	s32						iBmp_validate_bitCount					(s32 tnBitCount, s32 tnBitCount_default = 24);
	s32						iBmp_extractSubRectangleBitmaps			(SBitmap* bmp, SBitmapArray* bmpSubs[], s32 tnMaxCount, SBgra color);
	s32						iiBmp_extractSubRectangleBitmap_scanAndExtract(SBitmap* bmp, s32 tnX, s32 tnY, s32 tnThisSub, SBitmapArray* bmpSubs[], SBgra boundingColor);
	s32						iBmp_computeRowWidth					(SBitmap* bmp);
	void					iBmp_createBySize						(SBitmap* bmp, u32 width, u32 height, u32 tnBitCount = 24);
	void					iBmp_populateBitmapStructure			(SBitmap* bmp, u32 tnWidth, u32 tnHeight, u32 tnBitCount);
	void					iBmp_delete								(SBitmap** bmp, bool tlFreeBits, bool tlDeleteSelf, bool tlDeleteFont = true);
	void					iBmp_invert								(SBitmap* bmp, RECT* rc);
	void					iBmp_invert								(SBitmap* bmp, s32 tnUlX, s32 tnUlY, s32 tnLrX, s32 tnLrY);
	SBgra*					iBmp_colorHalf							(SBgra color, SBgra* colorInv);
	void					iBmp_bitBltObject						(SBitmap* bmpDst, SObject* obj, SBitmap* bmpSrc);
	void					iBmp_bitBltObjectMask					(SBitmap* bmpDst, SObject* obj, SBitmap* bmpSrc);
	u32						iBmp_bitBlt								(SBitmap* bmpDst, RECT* trc, SBitmap* bmpSrc);
	u32						iBmp_bitBlt_rotated						(SBitmap* bmpDst, RECT* trc, SBitmap* bmpSrc, s32 tnDegrees_90_180_270);
	u32						iBmp_bitBltAlpha						(SBitmap* bmpDst, RECT* trc, SBitmap* bmpSrc, f32 alpha);
	u32						iBmp_bitBltAlphaBlend					(SBitmap* bmpDst, RECT* trc, SBitmap* bmpSrc);
	u32						iBmp_grayscale							(SBitmap* bmp, RECT* trc);
	u32						iBmp_colorize							(SBitmap* bmp, RECT* trc, SBgra colorTemplate, bool clampColor, f32 minColor);
	u32						iBmp_colorizeMask						(SBitmap* bmp, RECT* trc, SBgra colorTemplate, bool clampColor, f32 minColor);
	u32						iBmp_swapColors							(SBitmap* bmp, SBgra colorOld, SBgra colorNew);
	u32						iBmp_alphaColorizeMask					(SBitmap* bmp, RECT* trc, SBgra colorAlpha, f32 alpha);
	u32						iBmp_bitBltMask							(SBitmap* bmpDst, RECT* trc, SBitmap* bmpSrc);
	u32						iBmp_bitBlt_byGraymask					(SBitmap* bmpDst, RECT* trc, SBitmap* bmpSrc, SBgra color);
	SBitmap*				iBmp_createAndExtractRect				(SBitmap* bmpSrc, s32 tnUlX, s32 tnUlY, s32 tnLrX, s32 tnLrY);
	HRGN					iBmp_extractRgnByMask					(SBitmap* bmp, RECT* trc);
	u32						iBmp_wavyLine							(SBitmap* bmp, RECT* trc, SBgra color);
	u32						iBmp_roundCorners						(SBitmap* bmp, s32 tnType, SBgra color);
	SBgra					iBmp_extractColorAtPoint				(SBitmap* bmp, s32 tnX, s32 tnY);
	SBgra					iBmp_colorCombine						(SBgra color1, SBgra color2, f32 tfAlp);
	SBgra*					iBmp_colorLift							(SBgra color, SBgra* colorOut);
	bool					iBmp_locateMarker						(SBitmap* bmp, u8 red, u8 grn, u8 blu, u32* tnX, u32* tnY, bool tlOverwriteMarker);
	SBitmap*				iBmp_cask_createAndPopulate				(s32 iCode, s32 tnWidth, s32 tnHeight, s32* tnSkipChars, u32 tnTextLength, SBgra* backColor, SBgra textColor, SBgra backgroundColor, bool tlOverrideColors);
#ifndef _EXCLUDE_iBmp_nbsp_createAndPopulate
	SBitmap*				iBmp_nbsp_createAndPopulate				(SComp* comp, SFont* font, f32 tfMinColor, f32 tfMaxColor, s32 tnWidth, s32 tnHeight, SBgra backColor, SBgra textColor);
#endif
	void					iBmp_colorizeAsCommonTooltipBackground	(SBitmap* bmp);
	void					iBmp_colorizeHighlightGradient			(SBitmap* bmp, RECT* rc, SBgra color, f32 tfLeftOpaque, f32 tfRightOpaque);
	void					iBmp_dapple								(SBitmap* bmp, SBitmap* bmpDapple, SBitmap* bmpDappleTmp, f32 tfBias, f32 tfInfluence);
	void					iBmp_drawFontBitmap_char				(SBitmap* bmp, s32 fontX, s32 fontY, u8 c, RECT* rc, SBgra foreColor, SBgra backColor);
	void					iBmp_drawFontBitmap_char_16x32			(SBitmap* bmp, u8 c, RECT* rc, SBgra foreColor, SBgra backColor);
// TODO:  The following void functions need to be changed to u32 and indicate how many pixels were rendered
	void					iBmp_drawPoint							(SBitmap* bmp, s32 tnX, s32 tnY, SBgra color);
	void					iBmp_drawBullet							(SBitmap* bmp, s32 tnX, s32 tnY, SBgra color);
	void					iBmp_fillRect							(SBitmap* bmp, RECT* rc, SBgra colorNW, SBgra colorNE = whiteColor, SBgra colorSW = whiteColor, SBgra colorSE = whiteColor, bool tlUseGradient = false, RECT* rcClip = NULL, bool tluseClip = false);
	void					iBmp_fillRect_op						(SBitmap* bmp, RECT* rc, SBgra colorOn, SBgra colorOff, u32 tnOp);
	void					iBmp_frameRect							(SBitmap* bmp, RECT* rc, SBgra colorNW, SBgra colorNE = whiteColor, SBgra colorSW = whiteColor, SBgra colorSE = whiteColor, bool tlUseGradient = false, RECT* rcClip = NULL, bool tluseClip = false);
	void					iBmp_colorizeRect						(SBitmap* bmp, RECT* rc, SBgra colorNW, SBgra colorNE = whiteColor, SBgra colorSW = whiteColor, SBgra colorSE = whiteColor, bool tlUseGradient = false, RECT* rcClip = NULL, bool tluseClip = false, f32 alpha = 1.0f);
	void					iiBmp_frameInNineParts					(SBitmap* bmpDst, RECT* trc, SBitmap* bmpFrame);
	void					iiBmp_bitBltPortion						(SBitmap* bmpDst, s32 tnX, s32 tnY, s32 tnWidth, s32 tnHeight, SBitmap* bmpSrc, s32 tnXStart, s32 tnYStart);
	void					iBmp_drawArbitraryLine					(SBitmap* bmp, s32 tnX1, s32 tnY1, s32 tnX2, s32 tnY2, SBgra color);
	void					iBmp_drawArbitraryQuad					(SBitmap* bmp, s32 tnX1, s32 tnY1, s32 tnX2, s32 tnY2, s32 tnWidth, bool tlDrawEnds, SBgra color);
	void					iBmp_drawHorizontalLine					(SBitmap* bmp, s32 tnX1, s32 tnX2, s32 tnY, SBgra color);
	void					iBmp_drawVerticalLine					(SBitmap* bmp, s32 tnY1, s32 tnY2, s32 tnX, SBgra color);
	void					iBmp_drawHorizontalLineGradient			(SBitmap* bmp, s32 tnX1, s32 tnX2, s32 tnY, f32 tfRed, f32 tfGrn, f32 tfBlu, f32 tfRedInc, f32 tfGrnInc, f32 tfBluInc, RECT* rcClip, bool tluseClip);
	void					iBmp_drawVerticalLineGradient			(SBitmap* bmp, s32 tnY1, s32 tnY2, s32 tnX, f32 tfRed, f32 tfGrn, f32 tfBlu, f32 tfRedInc, f32 tfGrnInc, f32 tfBluInc, RECT* rcClip, bool tluseClip);
	void					iBmp_colorizeHorizontalLine				(SBitmap* bmp, s32 tnX1, s32 tnX2, s32 tnY, SBgra color, f32 alpha);
	void					iBmp_colorizeVerticalLine				(SBitmap* bmp, s32 tnY1, s32 tnY2, s32 tnX, SBgra color, f32 alpha);
	void					iBmp_colorizeHorizontalLineGradient		(SBitmap* bmp, s32 tnX1, s32 tnX2, s32 tnY, f32 tfRed, f32 tfGrn, f32 tfBlu, f32 tfRedInc, f32 tfGrnInc, f32 tfBluInc, RECT* rcClip, bool tluseClip, f32 alpha);
	void					iBmp_colorizeVerticalLineGradient		(SBitmap* bmp, s32 tnY1, s32 tnY2, s32 tnX, f32 tfRed, f32 tfGrn, f32 tfBlu, f32 tfRedInc, f32 tfGrnInc, f32 tfBluInc, RECT* rcClip, bool tluseClip, f32 alpha);
	f32						iiBmp_squeezeColorChannel				(f32 color, f32 minColor);
	u32						iiBmp_setPixel							(SBitmap* bmp, s32 tnX, s32 tnY, SBgra color);
	f32						iBmp_getWidthInches						(SBitmap* bmp);
	f32						iBmp_getHeightInches					(SBitmap* bmp);
	f32						iBmp_getPixelsPerInchX					(SBitmap* bmp);
	f32						iBmp_getPixelsPerInchY					(SBitmap* bmp);
	bool					iIsNeedleInHaystack						(wchar_t* needle, s32 tnNeedleLength, wchar_t* haystack, s32 tnHaystackLength);

	// For nodes
	void					iBmp_node_renderComp					(SNode* node, s32 tnMaxTokenLength, s32 tnMaxOverallLength, bool tlIncludeExtraInfo, SNodeProps props[], s32 tnPropsCount, u32 tnIter_uid);

	//////////
	// For scaling
	//////
		u32					iBmp_scale								(SBitmap* bmpScaled, SBitmap* bmp);
		u32					iiBmp_scale_Process						(SBitmap* bmpDst, SBitmap* bmpSrc, f32 tfVerticalScaler, f32 tfHorizontalScaler);
		void				iiBmp_scale_processPixels				(SBitmapProcess* bp);
		void				iiBmp_scale_processSpannedPixel1		(SBitmapProcess* bp);
		void				iiBmp_scale_processSpannedPixel2		(SBitmapProcess* bp);
		void				iiBmp_scale_processSpannedPixel3		(SBitmapProcess* bp);
		void				iiBmp_scale_processSpannedPixel4		(SBitmapProcess* bp);
		void				iiBmp_scale_processSpannedPixel5		(SBitmapProcess* bp);
		void				iiBmp_scale_processSpannedPixel6		(SBitmapProcess* bp);
		void				iiBmp_scale_processSpannedPixel7		(SBitmapProcess* bp);
		void				iiBmp_scale_processSpannedPixel8		(SBitmapProcess* bp);
		void				iiBmp_scale_processSpannedPixel9		(SBitmapProcess* bp);
		u32					iiBmp_scale_processGetIntegersBetween	(f32 p1, f32 p2);
	//////
	// End
	//////////
