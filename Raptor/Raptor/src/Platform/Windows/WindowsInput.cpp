#include "rtpch.h"
#include "Raptor/Core/Input.h"
#include "Raptor/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Raptor {


	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>( Application::Get().GetWindow().GetNativeWindow());
		auto state =  glfwGetKey(window, keycode);

		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}
	std::pair<float, float> Input::GetMousePosition() 
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos,(float)yPos };
	}
	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}
	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}