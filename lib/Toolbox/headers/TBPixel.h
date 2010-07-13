#ifndef _TBPIXEL_H_
#define _TBPIXEL_H_

#include "TBSettings.h"

namespace tb
{
	
#ifndef TB_COLOR_DEPTH

	#define TB_COLOR_DEPTH                   TB_COLOR_DEPTH_A8R8G8B8

#endif

#if (TB_COLOR_DEPTH == TB_COLOR_DEPTH_A8R8G8B8)

	typedef TB_COLOR_TYPE_A8R8G8B8           Pixel;

	static const Pixel TB_COLOR_SHIFT =      TB_COLOR_TYPE_A8R8G8B8_SHIFT;
	static const Pixel TB_COLOR_SHIFT_A =    TB_COLOR_TYPE_A8R8G8B8_SHIFT_A;
	static const Pixel TB_COLOR_SHIFT_R =    TB_COLOR_TYPE_A8R8G8B8_SHIFT_R;
	static const Pixel TB_COLOR_SHIFT_G =    TB_COLOR_TYPE_A8R8G8B8_SHIFT_G;
	static const Pixel TB_COLOR_SHIFT_B =    TB_COLOR_TYPE_A8R8G8B8_SHIFT_B;

	static const Pixel TB_COLOR_BITMASK_R =  TB_COLOR_TYPE_A8R8G8B8_BITMASK_R;
	static const Pixel TB_COLOR_BITMASK_G =  TB_COLOR_TYPE_A8R8G8B8_BITMASK_G;
	static const Pixel TB_COLOR_BITMASK_B =  TB_COLOR_TYPE_A8R8G8B8_BITMASK_B;

	static const Pixel TB_COLOR_MASK_A =     TB_COLOR_TYPE_A8R8G8B8_MASK_A;
	static const Pixel TB_COLOR_MASK_C =     TB_COLOR_TYPE_A8R8G8B8_MASK_C;
	static const Pixel TB_COLOR_MASK_R =     TB_COLOR_TYPE_A8R8G8B8_MASK_R;
	static const Pixel TB_COLOR_MASK_G =     TB_COLOR_TYPE_A8R8G8B8_MASK_G;
	static const Pixel TB_COLOR_MASK_B =     TB_COLOR_TYPE_A8R8G8B8_MASK_B;
	static const Pixel TB_COLOR_MASK_RB =    TB_COLOR_TYPE_A8R8G8B8_MASK_RB;
	static const Pixel TB_COLOR_MASK_AG =    TB_COLOR_TYPE_A8R8G8B8_MASK_AG;


#elif (TB_COLOR_DEPTH == TB_COLOR_DEPTH_R8G8B8)

	typedef TB_COLOR_TYPE_R8G8B8             Pixel;

	static const Pixel TB_COLOR_SHIFT =      TB_COLOR_TYPE_R8G8B8_SHIFT;
	static const Pixel TB_COLOR_SHIFT_A =    TB_COLOR_TYPE_R8G8B8_SHIFT_A;
	static const Pixel TB_COLOR_SHIFT_R =    TB_COLOR_TYPE_R8G8B8_SHIFT_R;
	static const Pixel TB_COLOR_SHIFT_G =    TB_COLOR_TYPE_R8G8B8_SHIFT_G;
	static const Pixel TB_COLOR_SHIFT_B =    TB_COLOR_TYPE_R8G8B8_SHIFT_B;

	static const Pixel TB_COLOR_BITMASK_R =  TB_COLOR_TYPE_R8G8B8_BITMASK_R;
	static const Pixel TB_COLOR_BITMASK_G =  TB_COLOR_TYPE_R8G8B8_BITMASK_G;
	static const Pixel TB_COLOR_BITMASK_B =  TB_COLOR_TYPE_R8G8B8_BITMASK_B;

	static const Pixel TB_COLOR_MASK_A =     TB_COLOR_TYPE_R8G8B8_MASK_A;
	static const Pixel TB_COLOR_MASK_C =     TB_COLOR_TYPE_R8G8B8_MASK_C;
	static const Pixel TB_COLOR_MASK_R =     TB_COLOR_TYPE_R8G8B8_MASK_R;
	static const Pixel TB_COLOR_MASK_G =     TB_COLOR_TYPE_R8G8B8_MASK_G;
	static const Pixel TB_COLOR_MASK_B =     TB_COLOR_TYPE_R8G8B8_MASK_B;
	static const Pixel TB_COLOR_MASK_RB =    TB_COLOR_TYPE_R8G8B8_MASK_RB;
	static const Pixel TB_COLOR_MASK_AG =    TB_COLOR_TYPE_R8G8B8_MASK_AG;

#elif (TB_COLOR_DEPTH == TB_COLOR_DEPTH_R5G6B5)

	typedef TB_COLOR_TYPE_R5G6B5             Pixel;

	static const Pixel TB_COLOR_SHIFT =      TB_COLOR_TYPE_R5G6B5_SHIFT;
	static const Pixel TB_COLOR_SHIFT_A =    TB_COLOR_TYPE_R5G6B5_SHIFT_A;
	static const Pixel TB_COLOR_SHIFT_R =    TB_COLOR_TYPE_R5G6B5_SHIFT_R;
	static const Pixel TB_COLOR_SHIFT_G =    TB_COLOR_TYPE_R5G6B5_SHIFT_G;
	static const Pixel TB_COLOR_SHIFT_B =    TB_COLOR_TYPE_R5G6B5_SHIFT_B;

	static const Pixel TB_COLOR_BITMASK_R =  TB_COLOR_TYPE_R5G6B5_BITMASK_R;
	static const Pixel TB_COLOR_BITMASK_G =  TB_COLOR_TYPE_R5G6B5_BITMASK_G;
	static const Pixel TB_COLOR_BITMASK_B =  TB_COLOR_TYPE_R5G6B5_BITMASK_B;

	static const Pixel TB_COLOR_MASK_A =     TB_COLOR_TYPE_R5G6B5_MASK_A;
	static const Pixel TB_COLOR_MASK_C =     TB_COLOR_TYPE_R5G6B5_MASK_C;
	static const Pixel TB_COLOR_MASK_R =     TB_COLOR_TYPE_R5G6B5_MASK_R;
	static const Pixel TB_COLOR_MASK_G =     TB_COLOR_TYPE_R5G6B5_MASK_G;
	static const Pixel TB_COLOR_MASK_B =     TB_COLOR_TYPE_R5G6B5_MASK_B;
	static const Pixel TB_COLOR_MASK_RB =    TB_COLOR_TYPE_R5G6B5_MASK_RB;
	static const Pixel TB_COLOR_MASK_AG =    TB_COLOR_TYPE_R5G6B5_MASK_AG;

#endif

	static const float TB_COLOR_BITMASK_FR = (float)TB_COLOR_BITMASK_R;
	static const float TB_COLOR_BITMASK_FG = (float)TB_COLOR_BITMASK_G;
	static const float TB_COLOR_BITMASK_FB = (float)TB_COLOR_BITMASK_B;

	// =========================================
	// Make
	// =========================================

#if (TB_COLOR_DEPTH == TB_COLOR_DEPTH_A8R8G8B8)

	static Pixel PixelMake(unsigned char a_Red, unsigned char a_Green, unsigned a_Blue)
	{
		return (
			((a_Red   << TB_COLOR_SHIFT_R) & TB_COLOR_MASK_R) |
			((a_Green << TB_COLOR_SHIFT_G) & TB_COLOR_MASK_G) |
			((a_Blue  << TB_COLOR_SHIFT_B) & TB_COLOR_MASK_B)
		);
	}

	static Pixel PixelMake(unsigned char a_Red, unsigned char a_Green, unsigned a_Blue, unsigned char a_Alpha)
	{
		return (
			((a_Alpha << TB_COLOR_SHIFT_A) & TB_COLOR_MASK_A) |
			((a_Red   << TB_COLOR_SHIFT_R) & TB_COLOR_MASK_R) |
			((a_Green << TB_COLOR_SHIFT_G) & TB_COLOR_MASK_G) |
			((a_Blue  << TB_COLOR_SHIFT_B) & TB_COLOR_MASK_B)
		);
	}

#elif (TB_COLOR_DEPTH == TB_COLOR_DEPTH_R8G8B8)

	static Pixel PixelMake(unsigned char a_Red, unsigned char a_Green, unsigned a_Blue)
	{
		return (
			((a_Red   << TB_COLOR_SHIFT_R) & TB_COLOR_MASK_R) |
			((a_Green << TB_COLOR_SHIFT_G) & TB_COLOR_MASK_G) |
			((a_Blue  << TB_COLOR_SHIFT_B) & TB_COLOR_MASK_B)
		);
	}

	static Pixel PixelMake(unsigned char a_Red, unsigned char a_Green, unsigned a_Blue, unsigned char a_Alpha)
	{
		return (
			((a_Red   << TB_COLOR_SHIFT_R) & TB_COLOR_MASK_R) |
			((a_Green << TB_COLOR_SHIFT_G) & TB_COLOR_MASK_G) |
			((a_Blue  << TB_COLOR_SHIFT_B) & TB_COLOR_MASK_B)
		);
	}

#elif (TB_COLOR_DEPTH == TB_COLOR_DEPTH_R5G6B5)

	static Pixel PixelMake(unsigned char a_Red, unsigned char a_Green, unsigned a_Blue)
	{
		return (
			(((a_Red   * TB_COLOR_MASK_R) >> 8) & TB_COLOR_MASK_R) |
			(((a_Green * TB_COLOR_MASK_G) >> 7) & TB_COLOR_MASK_G) |
			(((a_Blue  * TB_COLOR_MASK_B) >> 8) & TB_COLOR_MASK_B)
		);
	}

	static Pixel PixelMake(unsigned char a_Red, unsigned char a_Green, unsigned a_Blue, unsigned char a_Alpha)
	{
		return (
			(((a_Red   * TB_COLOR_MASK_R) >> 8) & TB_COLOR_MASK_R) |
			(((a_Green * TB_COLOR_MASK_G) >> 7) & TB_COLOR_MASK_G) |
			(((a_Blue  * TB_COLOR_MASK_B) >> 8) & TB_COLOR_MASK_B)
		);
	}

#endif

	// =========================================
	// Getters
	// =========================================

	static Pixel PixelGetRedComponent(Pixel a_Color)    
	{ 
		return ((a_Color >> TB_COLOR_SHIFT_R) & TB_COLOR_MASK_R);
	}
	static Pixel PixelGetGreenComponent(Pixel a_Color)  
	{ 
		return ((a_Color >> TB_COLOR_SHIFT_G) & TB_COLOR_MASK_G); 
	}
	static Pixel PixelGetBlueComponent(Pixel a_Color)   
	{ 
		return ((a_Color >> TB_COLOR_SHIFT_B) & TB_COLOR_MASK_B);
	}

#if (TB_COLOR_DEPTH == TB_COLOR_DEPTH_A8R8G8B8)

	static Pixel PixelGetRed(Pixel a_Color)             
	{ 
		return ((a_Color >> TB_COLOR_SHIFT_R) & TB_COLOR_BITMASK_R); 
	}
	static Pixel PixelGetGreen(Pixel a_Color)           
	{ 
		return ((a_Color >> TB_COLOR_SHIFT_G) & TB_COLOR_BITMASK_G); 
	}
	static Pixel PixelGetBlue(Pixel a_Color)            
	{ 
		return ((a_Color >> TB_COLOR_SHIFT_B) & TB_COLOR_BITMASK_B); 
	}

#elif (TB_COLOR_DEPTH == TB_COLOR_DEPTH_R8G8B8)

	static Pixel PixelGetRed(Pixel a_Color)             
	{ 
		return ((a_Color >> TB_COLOR_SHIFT_R) & TB_COLOR_BITMASK_R); 
	}
	static Pixel PixelGetGreen(Pixel a_Color)           
	{ 
		return ((a_Color >> TB_COLOR_SHIFT_G) & TB_COLOR_BITMASK_G); 
	}
	static Pixel PixelGetBlue(Pixel a_Color)            
	{ 
		return ((a_Color >> TB_COLOR_SHIFT_B) & TB_COLOR_BITMASK_B); 
	}

#elif (TB_COLOR_DEPTH == TB_COLOR_DEPTH_R5G6B5)

	static Pixel PixelGetRed(Pixel a_Color)             
	{ 
		return ((((a_Color & TB_COLOR_MASK_R) >> TB_COLOR_SHIFT_R) << 8) / TB_COLOR_BITMASK_R);
	}
	static Pixel PixelGetGreen(Pixel a_Color)           
	{ 
		return ((((a_Color & TB_COLOR_MASK_G) >> TB_COLOR_SHIFT_G) << 7) / TB_COLOR_BITMASK_G);
	}
	static Pixel PixelGetBlue(Pixel a_Color)            
	{ 
		return ((((a_Color & TB_COLOR_MASK_B) >> TB_COLOR_SHIFT_B) << 8) / TB_COLOR_BITMASK_B);
	}

#endif

	// =========================================
	// Blending
	// =========================================

	static Pixel PixelBlend(Pixel a_Left, Pixel a_Right, unsigned char a_Amount)
	{
		const Pixel Arb = a_Left & TB_COLOR_MASK_RB;
		const Pixel Ag  = a_Left & TB_COLOR_MASK_G;

		const Pixel RB = TB_COLOR_MASK_RB & (Arb + ((((a_Right & TB_COLOR_MASK_RB) - Arb) * a_Amount) >> TB_COLOR_SHIFT));
		const Pixel G = TB_COLOR_MASK_G & (Ag + ((((a_Right & TB_COLOR_MASK_G) - Ag) * a_Amount) >> TB_COLOR_SHIFT));

		return (RB | G | TB_COLOR_MASK_A);
	}

	// =========================================
	// Operations
	// =========================================

#if (TB_COLOR_DEPTH == TB_COLOR_DEPTH_A8R8G8B8)
	
	static Pixel PixelAdd(Pixel a_Left, Pixel a_Right)
	{
		Pixel a  = ((a_Left & TB_COLOR_MASK_A)  + (a_Right & TB_COLOR_MASK_A))  & TB_COLOR_MASK_A;
		Pixel rb = ((a_Left & TB_COLOR_MASK_RB) + (a_Right & TB_COLOR_MASK_RB)) & TB_COLOR_MASK_RB;
		Pixel g  = ((a_Left & TB_COLOR_MASK_G)  + (a_Right & TB_COLOR_MASK_G))  & TB_COLOR_MASK_G;

		return (a + rb + g);
	}

#elif (TB_COLOR_DEPTH == TB_COLOR_DEPTH_R8G8B8)

	static Pixel PixelAdd(Pixel a_Left, Pixel a_Right)
	{
		Pixel rb = ((a_Left & TB_COLOR_MASK_RB) + (a_Right & TB_COLOR_MASK_RB)) & TB_COLOR_MASK_RB;
		Pixel g  = ((a_Left & TB_COLOR_MASK_G)  + (a_Right & TB_COLOR_MASK_G))  & TB_COLOR_MASK_G;

		return (rb + g);
	}

#elif (TB_COLOR_DEPTH == TB_COLOR_DEPTH_R5G6B5)

	static Pixel PixelAdd(Pixel a_Left, Pixel a_Right)
	{
		Pixel rb = ((a_Left & TB_COLOR_MASK_RB) + (a_Right & TB_COLOR_MASK_RB)) & TB_COLOR_MASK_RB;
		Pixel g  = ((a_Left & TB_COLOR_MASK_G)  + (a_Right & TB_COLOR_MASK_G))  & TB_COLOR_MASK_G;

		return (rb + g);
	}

#endif

	// =========================================
	// Conversions
	// =========================================

#if (TB_COLOR_DEPTH == TB_COLOR_DEPTH_A8R8G8B8)

	static Pixel PixelConvert_ARGB_RGBA(Pixel a_Color)
	{
		Pixel a   = (a_Color & TB_COLOR_MASK_A) >> TB_COLOR_SHIFT_A;
		Pixel rgb = (a_Color & TB_COLOR_MASK_C) << TB_COLOR_SHIFT;
		return (rgb + a);
	}

	static Pixel PixelConvert_ARGB_ABGR(Pixel a_Color)
	{
		Pixel ag = (a_Color & TB_COLOR_MASK_AG);
		Pixel r = (a_Color & TB_COLOR_MASK_R) >> TB_COLOR_SHIFT_R;
		Pixel b = (a_Color & TB_COLOR_MASK_B) << TB_COLOR_SHIFT_R;

		return (ag + b + r);
	}

#endif

	static Pixel PixelConvert_RGB_BGR(Pixel a_Color)
	{
		Pixel g = (a_Color & TB_COLOR_MASK_G);
		Pixel r = (a_Color & TB_COLOR_MASK_R) >> TB_COLOR_SHIFT_R;
		Pixel b = (a_Color & TB_COLOR_MASK_B) << TB_COLOR_SHIFT_R;

		return (b + g + r);
	}

	// =========================================
	// R8G8B8
	// =========================================

	static TB_COLOR_TYPE_R5G6B5 PixelConvert_R8G8B8_R5G6B5(TB_COLOR_TYPE_R8G8B8 a_Color)
	{
		TB_COLOR_TYPE_R5G6B5 r = 
			((a_Color & TB_COLOR_TYPE_R8G8B8_MASK_R) >> TB_COLOR_TYPE_R8G8B8_SHIFT_R) << TB_COLOR_TYPE_R5G6B5_SHIFT_R;
		TB_COLOR_TYPE_R5G6B5 g = 
			((a_Color & TB_COLOR_TYPE_R8G8B8_MASK_G) >> TB_COLOR_TYPE_R8G8B8_SHIFT_G) << TB_COLOR_TYPE_R5G6B5_SHIFT_G;
		TB_COLOR_TYPE_R5G6B5 b = 
			((a_Color & TB_COLOR_TYPE_R8G8B8_MASK_B) >> TB_COLOR_TYPE_R8G8B8_SHIFT_B) << TB_COLOR_TYPE_R5G6B5_SHIFT_B;

		return (r + g + b);
	}

	static TB_COLOR_TYPE_A8R8G8B8 PixelConvert_R8G8B8_A8R8G8B8(TB_COLOR_TYPE_R8G8B8 a_Color)
	{
		return (a_Color);
	}

	// =========================================
	// A8R8G8B8
	// =========================================

	static TB_COLOR_TYPE_R5G6B5 PixelConvert_A8R8G8B8_R5G6B5(TB_COLOR_TYPE_A8R8G8B8 a_Color)
	{
		TB_COLOR_TYPE_R5G6B5 r = 
			((a_Color & TB_COLOR_TYPE_A8R8G8B8_MASK_R) >> TB_COLOR_TYPE_A8R8G8B8_SHIFT_R) << TB_COLOR_TYPE_R5G6B5_SHIFT_R;
		TB_COLOR_TYPE_R5G6B5 g = 
			((a_Color & TB_COLOR_TYPE_A8R8G8B8_MASK_G) >> TB_COLOR_TYPE_A8R8G8B8_SHIFT_G) << TB_COLOR_TYPE_R5G6B5_SHIFT_G;
		TB_COLOR_TYPE_R5G6B5 b = 
			((a_Color & TB_COLOR_TYPE_A8R8G8B8_MASK_B) >> TB_COLOR_TYPE_A8R8G8B8_SHIFT_B) << TB_COLOR_TYPE_R5G6B5_SHIFT_B;

		return (r + g + b);
	}

	static TB_COLOR_TYPE_R8G8B8 PixelConvert_A8R8G8B8_R8G8B8(TB_COLOR_TYPE_A8R8G8B8 a_Color)
	{
		return (a_Color & TB_COLOR_MASK_C);
	}

	// =========================================
	// R5G6B5
	// =========================================

	static TB_COLOR_TYPE_R8G8B8 PixelConvert_R5G6B5_R8G8B8(TB_COLOR_TYPE_R5G6B5 a_Color)
	{
		TB_COLOR_TYPE_R8G8B8 r = 
			((((a_Color & TB_COLOR_MASK_R) >> TB_COLOR_SHIFT_R) << 8) / TB_COLOR_BITMASK_R) << TB_COLOR_TYPE_R8G8B8_SHIFT_R;
		TB_COLOR_TYPE_R8G8B8 g = 
			((((a_Color & TB_COLOR_MASK_G) >> TB_COLOR_SHIFT_G) << 7) / TB_COLOR_BITMASK_G) << TB_COLOR_TYPE_R8G8B8_SHIFT_G;
		TB_COLOR_TYPE_R8G8B8 b = 
			((((a_Color & TB_COLOR_MASK_B) >> TB_COLOR_SHIFT_B) << 8) / TB_COLOR_BITMASK_B) << TB_COLOR_TYPE_R8G8B8_SHIFT_B;

		return (r + g + b);
	}

	static TB_COLOR_TYPE_R8G8B8 PixelConvert_R5G6B5_A8R8G8B8(TB_COLOR_TYPE_R5G6B5 a_Color)
	{
		TB_COLOR_TYPE_A8R8G8B8 r = 
			((((a_Color & TB_COLOR_MASK_R) >> TB_COLOR_SHIFT_R) << 8) / TB_COLOR_BITMASK_R) << TB_COLOR_TYPE_A8R8G8B8_SHIFT_R;
		TB_COLOR_TYPE_A8R8G8B8 g = 
			((((a_Color & TB_COLOR_MASK_G) >> TB_COLOR_SHIFT_G) << 7) / TB_COLOR_BITMASK_G) << TB_COLOR_TYPE_A8R8G8B8_SHIFT_G;
		TB_COLOR_TYPE_A8R8G8B8 b = 
			((((a_Color & TB_COLOR_MASK_B) >> TB_COLOR_SHIFT_B) << 8) / TB_COLOR_BITMASK_B) << TB_COLOR_TYPE_A8R8G8B8_SHIFT_B;

		return (r + g + b);
	}

}; // namespace tb

#endif