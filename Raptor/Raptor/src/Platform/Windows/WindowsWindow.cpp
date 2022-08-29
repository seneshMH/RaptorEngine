#include "rtpch.h"

#include "WindowsWindow.h"


namespace Raptor {
	static bool s_GLFWInitalized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}


	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.width;
		m_Data.Height = props.height;

		RT_CORE_INFO("Creating Window {0} ({1},{2})",props.Title, props.width, props.height);

		if (!s_GLFWInitalized) {
			int success = glfwInit();
			RT_CORE_ASSERT(success,"Could not initalize GLFW {0}");

			s_GLFWInitalized = true;
		}

		m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVsync(true);

	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}


	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}


	void WindowsWindow::SetVsync(bool enabled)
	{
		if(enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
	}

	bool WindowsWindow::IsVsync() const
	{
		return m_Data.Vsync;
	}

}