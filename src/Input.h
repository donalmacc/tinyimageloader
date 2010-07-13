#ifndef _INPUT_H_
#define _INPUT_H_

#include "Settings.h"

namespace Tmpl8
{
	struct Renderer;
}

namespace tb
{

#ifdef INPUT_SDL

	typedef unsigned char Key;

	const Key QKEY_ESCAPE		= 1;

	const Key QKEY_LEFT			= 75;
	const Key QKEY_UP			= 72;
	const Key QKEY_RIGHT		= 77;
	const Key QKEY_DOWN			= 80;

	const Key QKEY_SPACE		= 57;
	const Key QKEY_ENTER		= 28;
	const Key QKEY_BACKSPACE	= 14;
	const Key QKEY_TAB			= 15;

	const Key QKEY_LSHIFT		= 42;		
	const Key QKEY_RSHIFT		= 54;
	const Key QKEY_LALT			= 56;
	const Key QKEY_RALT			= 29;
	const Key QKEY_LCTRL		= 29;
	const Key QKEY_RCTRL		= 29;

	const Key QKEY_F1			= 59;
	const Key QKEY_F2			= 60;
	const Key QKEY_F3			= 61;
	const Key QKEY_F4			= 62;
	const Key QKEY_F5			= 63;
	const Key QKEY_F6			= 64;
	const Key QKEY_F7			= 65;
	const Key QKEY_F8			= 66;
	const Key QKEY_F9			= 67;
	const Key QKEY_F10			= 68;
	const Key QKEY_F11			= 87;
	const Key QKEY_F12			= 0;		// not verified

	const Key QKEY_TILDE		= 41;
	const Key QKEY_0			= 11;
	const Key QKEY_1			= 2;
	const Key QKEY_2			= 3;
	const Key QKEY_3			= 4;
	const Key QKEY_4			= 5;
	const Key QKEY_5			= 6;
	const Key QKEY_6			= 7;
	const Key QKEY_7			= 8;
	const Key QKEY_8			= 9;
	const Key QKEY_9			= 10;
	const Key QKEY_MIN			= 12;
	const Key QKEY_EQUALS		= 13;

	const Key QKEY_Q			= 16;
	const Key QKEY_W			= 17;
	const Key QKEY_E			= 18;
	const Key QKEY_R			= 19;
	const Key QKEY_T			= 20;
	const Key QKEY_Y			= 21;
	const Key QKEY_U			= 22;
	const Key QKEY_I			= 23;
	const Key QKEY_O			= 24;
	const Key QKEY_P			= 25;
	const Key QKEY_LSBRACKET	= 26;
	const Key QKEY_RSBRACKET	= 27;
	const Key QKEY_RSLASH		= 43;

	const Key QKEY_A			= 30;
	const Key QKEY_S			= 31;
	const Key QKEY_D			= 32;
	const Key QKEY_F			= 33;
	const Key QKEY_G			= 34;
	const Key QKEY_H			= 35;
	const Key QKEY_J			= 36;
	const Key QKEY_K			= 37;
	const Key QKEY_L			= 38;
	const Key QKEY_SEMICOLON	= 39;
	const Key QKEY_APOSTROPHE	= 40;

	const Key QKEY_Z			= 44;
	const Key QKEY_X			= 45;
	const Key QKEY_C			= 46;
	const Key QKEY_V			= 47;
	const Key QKEY_B			= 48;
	const Key QKEY_N			= 49;
	const Key QKEY_M			= 50;
	const Key QKEY_COMMA		= 51;
	const Key QKEY_PERIOD		= 52;
	const Key QKEY_LSLASH		= 53;

	const Key QKEY_HOME			= 71;
	const Key QKEY_END			= 79;
	const Key QKEY_PAGEUP		= 73;
	const Key QKEY_PAGEDOWN		= 81;
	const Key QKEY_DELETE		= 83;
	const Key QKEY_INSERT		= 82;
	const Key QKEY_PAUSE		= 69;
	const Key QKEY_PRINTSCREEN	= 55;

	const Key QKEY_NP_1			= 79;
	const Key QKEY_NP_2			= 80;
	const Key QKEY_NP_3			= 81;
	const Key QKEY_NP_4			= 75;
	const Key QKEY_NP_5			= 76;
	const Key QKEY_NP_6			= 77;
	const Key QKEY_NP_7			= 71;
	const Key QKEY_NP_8			= 72;
	const Key QKEY_NP_9			= 73;
	const Key QKEY_NP_0			= 82;
	const Key QKEY_NP_PLUS		= 78;
	const Key QKEY_NP_MINUS		= 74;
	const Key QKEY_NP_MULTIPLY	= 55;
	const Key QKEY_NP_DIVIDE	= 54;
	const Key QKEY_NP_PERIOD	= 83;
	const Key QKEY_NP_ENTER		= 28;

	const Key QKEY_CAPSLOCK		= 58;
	const Key QKEY_NUMLOCK		= 69;
	const Key QKEY_SCROLLLOCK	= 70;

	typedef unsigned char Button;

	const Button QBUT_LEFT			= 1;
	const Button QBUT_MIDDLE		= 2;
	const Button QBUT_RIGHT			= 3;
	const Button QBUT_SCROLLUP		= 4;
	const Button QBUT_SCROLLDOWN	= 5;

#elif defined(INPUT_WIN32)

	typedef __w64 unsigned int Key;

	const Key QKEY_ESCAPE		= 27;

	const Key QKEY_LEFT			= 37;
	const Key QKEY_UP			= 38;
	const Key QKEY_RIGHT		= 39;
	const Key QKEY_DOWN			= 40;

	const Key QKEY_SPACE		= 32;
	const Key QKEY_ENTER		= 13;
	const Key QKEY_BACKSPACE	= 8;
	const Key QKEY_TAB			= 9;

	// Note: WIN32 doesn't differentiate between 
	// left shift and right shift,
	// left alt and right alt
	// and left control and right control
	const Key QKEY_LSHIFT		= 16;		
	const Key QKEY_RSHIFT		= 16;
	const Key QKEY_LALT			= 0;
	const Key QKEY_RALT			= 18;
	const Key QKEY_LCTRL		= 17;
	const Key QKEY_RCTRL		= 17;

	const Key QKEY_F1			= 112;
	const Key QKEY_F2			= 113;
	const Key QKEY_F3			= 114;
	const Key QKEY_F4			= 115;
	const Key QKEY_F5			= 116;
	const Key QKEY_F6			= 117;
	const Key QKEY_F7			= 118;
	const Key QKEY_F8			= 119;
	const Key QKEY_F9			= 120;
	const Key QKEY_F10			= 121;
	const Key QKEY_F11			= 122;
	const Key QKEY_F12			= 123;

	const Key QKEY_TILDE		= 192;
	const Key QKEY_1			= 49;
	const Key QKEY_2			= 50;
	const Key QKEY_3			= 51;
	const Key QKEY_4			= 52;
	const Key QKEY_5			= 53;
	const Key QKEY_6			= 54;
	const Key QKEY_7			= 55;
	const Key QKEY_8			= 56;
	const Key QKEY_9			= 57;
	const Key QKEY_0			= 48;
	const Key QKEY_MINUS		= 189;
	const Key QKEY_EQUAL		= 187;

	const Key QKEY_Q			= 81;
	const Key QKEY_W			= 87;
	const Key QKEY_E			= 69;
	const Key QKEY_R			= 82;
	const Key QKEY_T			= 84;
	const Key QKEY_Y			= 89;
	const Key QKEY_U			= 85;
	const Key QKEY_I			= 73;
	const Key QKEY_O			= 79;
	const Key QKEY_P			= 80;
	const Key QKEY_LSBRACKET	= 219;
	const Key QKEY_RSBRACKET	= 221;
	const Key QKEY_RSLASH		= 220;

	const Key QKEY_A			= 65;
	const Key QKEY_S			= 83;
	const Key QKEY_D			= 68;
	const Key QKEY_F			= 70;
	const Key QKEY_G			= 71;
	const Key QKEY_H			= 72;
	const Key QKEY_J			= 74;
	const Key QKEY_K			= 75;
	const Key QKEY_L			= 76;
	const Key QKEY_SEMICOLON	= 186;
	const Key QKEY_APOSTROPHE	= 222;

	const Key QKEY_Z			= 90;
	const Key QKEY_X			= 88;
	const Key QKEY_C			= 67;
	const Key QKEY_V			= 86;
	const Key QKEY_B			= 66;
	const Key QKEY_N			= 78;
	const Key QKEY_M			= 77;
	const Key QKEY_COMMA		= 188;
	const Key QKEY_PERIOD		= 190;
	const Key QKEY_LSLASH		= 191;

	const Key QKEY_HOME			= 36;
	const Key QKEY_END			= 35;
	const Key QKEY_PAGEUP		= 33;
	const Key QKEY_PAGEDOWN		= 34;
	const Key QKEY_DELETE		= 46;
	const Key QKEY_INSERT		= 45;
	const Key QKEY_PAUSE		= 19;

	const Key QKEY_NP_1			= 97;
	const Key QKEY_NP_2			= 98;
	const Key QKEY_NP_3			= 99;
	const Key QKEY_NP_4			= 100;
	const Key QKEY_NP_5			= 101;
	const Key QKEY_NP_6			= 102;
	const Key QKEY_NP_7			= 103;
	const Key QKEY_NP_8			= 104;
	const Key QKEY_NP_9			= 105;
	const Key QKEY_NP_0			= 96;
	const Key QKEY_NP_PLUS		= 107;
	const Key QKEY_NP_MINUS		= 109;
	const Key QKEY_NP_MULTIPLY	= 106;
	const Key QKEY_NP_DIVIDE	= 111;
	const Key QKEY_NP_PERIOD	= 110;
	const Key QKEY_NP_ENTER		= 13;

	const Key QKEY_CAPSLOCK		= 20;
	const Key QKEY_NUMLOCK		= 144;
	const Key QKEY_SCROLLLOCK	= 145;

	typedef __w64 unsigned int Button;

	const Button QBUT_LEFT			= 1;
	const Button QBUT_MIDDLE		= 2;
	const Button QBUT_RIGHT			= 3;
	const Button QBUT_SCROLLUP		= 4;
	const Button QBUT_SCROLLDOWN	= 5;

#endif

	struct Keyboard
	{
		friend struct Tmpl8::Renderer;

		static bool Pressed		(Key a_Key);
		static bool Released	(Key a_Key);

	private:

		static void Init();
		static void Clear();
		static void Press				(Key a_Key);
		static void Release				(Key a_Key);

		static bool pressed[323];
		static bool released[323];

	}; // struct Keyboard

	struct Mouse
	{

		friend struct Tmpl8::Renderer;

		static bool Pressed				(Button a_Button);
		static bool Released			(Button a_Button);
		static bool DoubleClicked		(Button a_Button);
		static float GetSpeedX() { return speed_x; }
		static float GetSpeedY() { return speed_y; }

	private:

		static void Init();
		static void Clear();
		static void UpdatePosition		(float a_PosX, float a_PosY);
		static void CalculateSpeed		();
		static void UpdateSpeed			(float a_SpeedX, float a_SpeedY);
		static void Update				(float a_PosX, float a_PosY, float a_SpeedX, float a_SpeedY);
		static void Scroll				(Button a_Button);
		static void Press				(Button a_Button);
		static void Release				(Button a_Button);
		static void DoubleClick			(Button a_Button, float a_DeltaTime);

		static bool pressed[5];
		static bool double_clicked[5];
		static bool released[5];

		static float prev_x, prev_y;
		static float pos_x, pos_y;
		static float speed_x, speed_y;
		static float speed_scroll;
		static float accel_scroll;
		static float max_speed_scroll;
		static float friction_scroll;
		static float delta_doubleclick;
		static Button button_lastclick;

	}; // struct Mouse

}; // namespace tb

#endif