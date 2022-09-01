#include "rtpch.h"
#include "OpenGlContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Raptor {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		RT_CORE_ASSERT(windowHandle, "Window handel is null");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RT_CORE_ASSERT(success, "Could not initalize Glad {0}");
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}