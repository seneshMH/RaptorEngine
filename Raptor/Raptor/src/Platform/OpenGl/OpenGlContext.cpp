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
		RT_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RT_CORE_ASSERT(success, "Could not initalize Glad");

		RT_CORE_INFO("OpenGL Info :");
		RT_CORE_INFO("   Vendor   :   {0}", glGetString(GL_VENDOR));
		RT_CORE_INFO("   Renderer :   {0}", glGetString(GL_RENDERER));
		RT_CORE_INFO("   Version  :   {0}", glGetString(GL_VERSION));

		#ifdef RT_ENABLE_ASSERTS
			int versionMajor;
			int versionMinor;
			glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
			glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

			RT_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Raptor Engine requires at least OpenGL version 4.5!");
		#endif
	}
	void OpenGLContext::SwapBuffers()
	{
		RT_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}