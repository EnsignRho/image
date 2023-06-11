// common_types.h

#include <limits.h>

// 32-bit compiler
#define uptr unsigned __int32
#define sptr __int32
#define debug_break _asm int 3;
#define debug_nop _asm nop;

#define u64 unsigned __int64
#define u32 unsigned __int32
#define u16 unsigned __int16
#define u8 unsigned char
#define u8z unsigned char

// Wide
#define w16 wchar_t
#define cw16 const wchar_t

// Signed
#define s64 __int64
#define s32 __int32
#define s16 __int16
#define s8 char
#define s8z char

// Constant signed
#define cs8z const char
#define cs8 const char
#define cs16 const s16
#define cs32 const s32
#define cs64 const s64

// Constant unsigned
#define cu8z const u8z
#define cu8 const u8
#define cu16 const u16
#define cu32 const u32
#define cu64 const u64

#if !defined(size_t)
	#define size_t sptr
#endif

// Floating point
#define f32 float
#define f64 double

// Constant floating point
#define cf32 const f32
#define cf64 const f64

// Constant pointers
#define cvp const void*
#define cs8p cs8*
#define cs16p cs16*
#define cs32p cs32*
#define cs64p cs64*
#define cu8p cu8*
#define cu16p cu16*
#define cu32p cu32*
#define cu64p cu64*

// Constants for iAccess codes
cu32	_8BIT				= 1 << 0;
cu32	_16BIT				= 1 << 1;
cu32	_32BIT				= 1 << 2;
cu32	_64BIT				= 1 << 3;
cu32	_POINTER			= 1 << 27;
cu32	_POINTER_POINTER	= 1 << 28;
cu32	_FLOATING_POINT		= 1 << 29;
cu32	_UNSIGNED			= 1 << 30;
cu32	_CONSTANT			= 1 << 31;

// Base types
cu32	_S8					= _8BIT;
cu32	_U8					= _8BIT | _UNSIGNED;
cu32	_S8Z				= _8BIT;
cu32	_U8Z				= _8BIT | _UNSIGNED;
cu32	_S16				= _16BIT;
cu32	_U16				= _16BIT | _UNSIGNED;
cu32	_S32				= _32BIT;
cu32	_U32				= _32BIT | _UNSIGNED;
cu32	_S64				= _64BIT;
cu32	_U64				= _64BIT | _UNSIGNED;
cu32	_F32				= _32BIT | _FLOATING_POINT;
cu32	_F64				= _64BIT | _FLOATING_POINT;

// Pointers
cu32	_S8P				= _8BIT | _POINTER;
cu32	_U8P				= _8BIT | _POINTER | _UNSIGNED;
cu32	_S8ZP				= _8BIT | _POINTER;
cu32	_U8ZP				= _8BIT | _POINTER | _UNSIGNED;
cu32	_S16P				= _16BIT | _POINTER;
cu32	_U16P				= _16BIT | _POINTER | _UNSIGNED;
cu32	_S32P				= _32BIT | _POINTER;
cu32	_U32P				= _32BIT | _POINTER | _UNSIGNED;
cu32	_S64P				= _64BIT | _POINTER;
cu32	_U64P				= _64BIT | _POINTER | _UNSIGNED;
cu32	_F32P				= _32BIT | _POINTER | _FLOATING_POINT;
cu32	_F64P				= _64BIT | _POINTER | _FLOATING_POINT;

// Pointer to pointers
cu32	_S8PP				= _8BIT | _POINTER_POINTER;
cu32	_U8PP				= _8BIT | _POINTER_POINTER | _UNSIGNED;
cu32	_S8ZPP				= _8BIT | _POINTER_POINTER;
cu32	_U8ZPP				= _8BIT | _POINTER_POINTER | _UNSIGNED;
cu32	_S16PP				= _16BIT | _POINTER_POINTER;
cu32	_U16PP				= _16BIT | _POINTER_POINTER | _UNSIGNED;
cu32	_S32PP				= _32BIT | _POINTER_POINTER;
cu32	_U32PP				= _32BIT | _POINTER_POINTER | _UNSIGNED;
cu32	_S64PP				= _64BIT | _POINTER_POINTER;
cu32	_U64PP				= _64BIT | _POINTER_POINTER | _UNSIGNED;
cu32	_F32PP				= _32BIT | _POINTER_POINTER | _FLOATING_POINT;
cu32	_F64PP				= _64BIT | _POINTER_POINTER | _FLOATING_POINT;

// Min/max
cs64	_i32_min			= _I32_MIN;
cs64	_i32_max			= _I32_MAX;
cs64	_i64_min			= _I64_MIN;
cs64	_i64_max			= _I64_MAX;

#define	M_PI				3.14159265358979323846
cf64	_PI					= 3.14159265358979;		// pi
cf64	_2PI				= 6.28318530717959;		// 2*pi
cf64	_3PI_2				= 4.71238898038469;		// 3*pi/2
cf64	_4PI_3				= 4.18879020478639;		// 4*pi/3
cf64	_2PI_3				= 2.09439510239320;		// 2*pi/3
cf64	_PI_2				= 1.57079632679490;		// pi/2
cf64	_PI_3				= 1.04719755119660;		// pi/3
cf64	_PI_4				= 0.78539816339745;		// pi/4
cf64	_1PI_4				= 1.0 * _PI_4;
cf64	_3PI_4				= 3.0 * _PI_4;
cf64	_5PI_4				= 5.0 * _PI_4;
cf64	_7PI_4				= 7.0 * _PI_4;
cf64	_PI_6				= 0.52359877559830;		// pi/6
cf64	_PI_8				= 0.392699081698724;	// pi/8
cf64	_E					= 2.718281828459045;	// e
cf64	_GR					= 1.618033988749895;	// golden ratio

// Standard forms
#define nocode						do { } while(0)
#define between(value, lo, hi)		((value >= lo) && (value <= hi))
#define _union(x, y, z)				union { x z; y _ ## z; };

// Usage:  abc = newAlloc(SAbc, gsAbcRoot);
#define newAlloc(str, var)			(str*)iLl_appendNew__llAtEnd((SLL**)&var, sizeof(str))

// Logging
#ifndef logfunc
	#define logfunc(x)
#endif


//////////
// Color macros
//////
	#define rgba(r,g,b,a)				(u32)(((a & 0xff) << 24) + \
											 ((r & 0xff)   << 16) + \
											 ((g & 0xff)   <<  8) + \
											 ((b & 0xff)))

	#define rgb(r,g,b)					(u32)((0xff     << 24) + \
											 ((r & 0xff) << 16) + \
											 ((g & 0xff) <<  8) + \
											 ((b & 0xff)))

	#define bgra(b,g,r,a)				(u32)(((a & 0xff) << 24) + \
											 ((r & 0xff)   << 16) + \
											 ((g & 0xff)   <<  8) + \
											 ((b & 0xff)))

	#define bgr(b,g,r)					(u32)((0xff     << 24) + \
											 ((r & 0xff) << 16) + \
											 ((g & 0xff) <<  8) + \
											 ((b & 0xff)))

	#define alp(rgbaColor)					(u8)((rgbaColor >> 24) & 0xff)
	#define red(rgbaColor)					(u8)((rgbaColor >> 16) & 0xff)
	#define grn(rgbaColor)					(u8)((rgbaColor >> 8)  & 0xff)
	#define blu(rgbaColor)					(u8)( rgbaColor        & 0xff)


// Shortcuts for non-NULL data arrays by arbitrary string
#define data_s8(a,b)	s8	a[sizeof(b) - 1]	= { (s8*)b }
#define data_cs8(a,b)	cs8	a[sizeof(b) - 1]	= { (cs8*)b }
#define data_u8(a,b)	u8	a[sizeof(b) - 1]	= { (u8*)b }
#define data_cu8(a,b)	cu8	a[sizeof(b) - 1]	= { (cu8*)b }
// Usages:	s8_data(name, "xyz");		// Creates s8 name[3] = "xyz";
//			cs8_data(name, "xyz");		// Creates cs8 name[3] = "xyz";
//			u8_data(name, "xyz");		// Creates u8 name[3] = "xyz";
//			cu8_data(name, "xyz");		// Creates cu8 name[3] = "xyz";

#ifndef _UPTR_ERROR
	#if defined(__64_BIT_COMPILER__)
		#define _UPTR_ERROR 0xffffffffffffffff
	#else
		#define _UPTR_ERROR 0xffffffff
	#endif
#endif

#ifndef null0
	// For a null that is not considered a pointer
	#define null0 0
#endif

#ifndef null
	// For a null that is lower-case ... as it should be. :-)
	#define null NULL
#endif

struct SRgb
{
	union {
		u8	red;
		u8	red_u8;
		s8	red_s8;
	};

	union {
		u8	grn;
		u8	grn_u8;
		s8	grn_s8;
	};

	union {
		u8	blu;
		u8	blu_u8;
		s8	blu_s8;
	};
};

struct SBgr
{
	union {
		u8	blu;
		u8	blu_u8;
		s8	blu_s8;
	};

	union {
		u8	grn;
		u8	grn_u8;
		s8	grn_s8;
	};

	union {
		u8	red;
		u8	red_u8;
		s8	red_s8;
	};
};

struct SBgraf
{
	f32		blu;				// 24-bit RGB values in bitmap files are physically stored as BGR
	f32		grn;
	f32		red;
	f32		alp;				// For 32-bit bitmaps
	f32		area;				// Holds area
};

struct SBgra
{
	union {
		u32		color;
		struct {
			union {
				u8	blu;
				u8	blu_u8;
				s8	blu_s8;
			};
			union {
				u8	grn;
				u8	grn_u8;
				s8	grn_s8;
			};
			union {
				u8	red;
				u8	red_u8;
				s8	red_s8;
			};
			union {
				u8	alp;
				u8	alp_u8;
				s8	alp_s8;
			};
        };
    };
};

struct SRgba
{
	union {
		u32		color;
		struct {
			union {
				u8	red;
				u8	red_u8;
				s8	red_s8;
			};
			union {
				u8	grn;
				u8	grn_u8;
				s8	grn_s8;
			};
			union {
				u8	blu;
				u8	blu_u8;
				s8	blu_s8;
			};
			union {
				u8	alp;
				u8	alp_u8;
				s8	alp_s8;
			};
		};
	};
};

struct SBgr_af64
{
	u8	blu;
	u8	grn;
	u8	red;
	f64	falp;
};

struct SXy_f32
{
	f32	x;
	f32	y;
};

struct SXy_f64
{
	f64	x;
	f64	y;
};

struct SXy_s32
{
	s32	xi;
	s32	yi;
};

struct SXy_s64
{
	s64	xi;
	s64	yi;
};

// Added to allow simple iteration through a builder
#define iterate(i, builder, p, structure)			for (i = 0; i < (builder)->populatedLength; i += sizeof(structure)) \
													{ \
														/* Grab the pointer */ \
														p = (structure*)((builder)->buffer + i);

#define iterate1(i, builder, p, structure)			for (i = 0, p = (structure*)(builder)->buffer; (s32)i < (s32)(builder)->populatedLength; i += sizeof(structure), ++p)

#define iterate_ptr(i, builder, p, structure)		iterate(i, (builder), p, structure)

#define iterate_with_count(i, builder, p, structure, count) \
													/* Note:  Count should already be set to starting value*/ \
													for (i = 0; i < (builder)->populatedLength; i += sizeof(structure), ++count) \
													{ \
														/* Grab the pointer */ \
														p = (structure*)((builder)->buffer + i);
#define iterate_end }

#define builder_allocate(builder, structure)		(structure*)iBuilder_allocateBytes(builder, sizeof(structure))

#ifndef _NO_COMMON_TYPES_FUNCS
	// Convert char to wchar_t
	// Note that the size of w must be able to support a convered to wchar_t
	wchar_t* c2w(s8* a, s32 a_len, wchar_t* w)
	{
		// Make sure we have a valid length
		if (a_len <= 0)
			a_len = (s32)strlen(a);

		// Convert
		MultiByteToWideChar(CP_THREAD_ACP, MB_PRECOMPOSED, a, a_len, w, a_len * sizeof(wchar_t));

		// Pass-thru
		return(w);
	}
#endif
