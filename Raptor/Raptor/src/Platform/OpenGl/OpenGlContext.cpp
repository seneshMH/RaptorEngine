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
		RT_CORE_ASSERT(success, "Could not initalize Glad");

		RT_CORE_INFO("OpenGL Info :");
		RT_CORE_INFO("   Vendor   :   {0}", glGetString(GL_VENDOR));
		RT_CORE_INFO("   Renderer :   {0}", glGetString(GL_RENDERER));
		RT_CORE_INFO("   Version  :   {0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}