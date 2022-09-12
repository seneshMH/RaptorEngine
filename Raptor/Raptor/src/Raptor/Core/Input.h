#pragma once

#include "Raptor/Core/Base.h"

namespace Raptor {
	class Input {
	public:
		 static bool IsKeyPressed(int keycode);

		 static bool IsMouseButtonPressed(int button);
		 static std::pair<float, float> GetMousePosition();
		 static float GetMouseX();
		 static float GetMouseY();

	};
}
