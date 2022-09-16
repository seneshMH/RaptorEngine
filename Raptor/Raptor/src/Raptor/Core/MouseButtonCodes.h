#pragma once

namespace Raptor
{
	using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			// From glfw3.h
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Button6 = 6,
			Button7 = 7,

			ButtonLast = Button7,
			ButtonLeft = Button0,
			ButtonRight = Button1,
			ButtonMiddle = Button2
		};
	}
}


//glfw3.h
#define RT_MOUSE_BUTTON_1         0
#define RT_MOUSE_BUTTON_2         1
#define RT_MOUSE_BUTTON_3         2
#define RT_MOUSE_BUTTON_4         3
#define RT_MOUSE_BUTTON_5         4
#define RT_MOUSE_BUTTON_6         5
#define RT_MOUSE_BUTTON_7         6
#define RT_MOUSE_BUTTON_8         7
#define RT_MOUSE_BUTTON_LAST      RT_MOUSE_BUTTON_8
#define RT_MOUSE_BUTTON_LEFT      RT_MOUSE_BUTTON_1
#define RT_MOUSE_BUTTON_RIGHT     RT_MOUSE_BUTTON_2
#define RT_MOUSE_BUTTON_MIDDLE    RT_MOUSE_BUTTON_3