// Code by:
// Quinten Lansu (knight666)
// Copyright 2008-2010

#include "Input.h"
#include "Renderer.h"

// ==========================================
// Keyboard
// ==========================================

bool Keyboard::pressed[323];
bool Keyboard::released[323];

void Keyboard::Init()
{
	for (int i = 0; i < 323; i++)
	{ 
		pressed[i] = false;
		released[i] = false; 
	}
}

void Keyboard::Clear()
{
	for (int i = 0; i < 323; i++)
	{
		released[i] = false; 
	}
}

void Keyboard::Press(Key a_Key)
{
	pressed[a_Key] = true;
}

void Keyboard::Release(Key a_Key)
{
	pressed[a_Key] = false;
	released[a_Key] = true;
}

bool Keyboard::Pressed(Key a_Key)
{
	return pressed[a_Key];
}

bool Keyboard::Released(Key a_Key)
{
	return released[a_Key];
}

// ==========================================
// Mouse
// ==========================================

bool Mouse::pressed[5];
bool Mouse::double_clicked[5];
bool Mouse::released[5];

float Mouse::prev_x, Mouse::prev_y;
float Mouse::pos_x, Mouse::pos_y;
float Mouse::speed_x, Mouse::speed_y;
float Mouse::speed_scroll;
float Mouse::accel_scroll = 2.5f;
float Mouse::friction_scroll = 1.f;
float Mouse::max_speed_scroll = 5.f;
float Mouse::delta_doubleclick = 256.f;
Button Mouse::button_lastclick = 0;

void Mouse::Init()
{
	Clear();
}

void Mouse::Clear()
{
	// stop mouse movement

	speed_x = speed_y = 0.f;

	// unrelease and undoubleclick buttons

	for (Button i = 0; i < 5; ++i)
	{
		released[i] = false;
		double_clicked[i] = false;
	}

	// slow scrolling

	if (speed_scroll > -friction_scroll && speed_scroll < friction_scroll) 
	{ 
		speed_scroll = 0.f; 
		return;
	}

	if (speed_scroll > 0) { speed_scroll -= friction_scroll; return; }
	if (speed_scroll < 0) { speed_scroll += friction_scroll; return; }
}

void Mouse::CalculateSpeed()
{
	speed_x += (pos_x - prev_x);
	speed_y += (pos_y - prev_y);
}

void Mouse::UpdatePosition(float a_PosX, float a_PosY)
{
	Mouse::prev_x = Mouse::pos_x;
	Mouse::prev_y = Mouse::pos_y;

	pos_x = a_PosX;
	pos_y = a_PosY;
}

void Mouse::UpdateSpeed(float a_SpeedX, float a_SpeedY)
{
	speed_x += a_SpeedX;
	speed_y += a_SpeedY;
}

void Mouse::Update(float a_PosX, float a_PosY, float a_SpeedX, float a_SpeedY)
{
	Mouse::prev_x = Mouse::pos_x;
	Mouse::prev_y = Mouse::pos_y;

	UpdatePosition(a_PosX, a_PosY);
	UpdateSpeed(a_SpeedX, a_SpeedY);
}

void Mouse::Scroll(Button a_Button)
{
	if (a_Button == QBUT_SCROLLDOWN) 
	{ 
		speed_scroll -= accel_scroll; 
		speed_scroll = (speed_scroll > -max_speed_scroll) ? speed_scroll : -max_speed_scroll;
	}
	if (a_Button == QBUT_SCROLLUP) 
	{ 
		speed_scroll += accel_scroll; 
		speed_scroll = (speed_scroll < max_speed_scroll) ? speed_scroll : max_speed_scroll;
	}
}

void Mouse::Press(Button a_Button)
{
	pressed[a_Button] = true;
}

void Mouse::Release(Button a_Button)
{
	pressed[a_Button] = false;
	released[a_Button] = true;
}

void Mouse::DoubleClick(Button a_Button, float a_DeltaTime)
{
	if (a_Button == button_lastclick && a_DeltaTime < delta_doubleclick)
	{
		double_clicked[a_Button] = true;
	}

	button_lastclick = a_Button;
}

bool Mouse::Pressed(Button a_Button)
{
	return pressed[a_Button];
}

bool Mouse::Released(Button a_Button)
{
	return released[a_Button];
}