#include "rtpch.h"

#include "WindowsWindow.h"

#include "Raptor/Event/ApplicationEvent.h"
#include "Raptor/Event/KeyEvent.h"
#include "Raptor/Event/MouseEvent.h"
#include "Raptor/Renderer/Renderer.h"

#include "Platform/OpenGl/OpenGlContext.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <dwmapi.h>

namespace Raptor {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error,const char* discription) 
	{
		RT_CORE_ERROR("GLFW ERROR ({0}) : {1}", error, discription);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		RT_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		RT_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		RT_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.width;
		m_Data.Height = props.height;

		RT_CORE_INFO("Creating Window {0} ({1},{2})",props.Title, props.width, props.height);


		if (s_GLFWWindowCount == 0) 
		{
			RT_PROFILE_SCOPE("glfw Init");

			int success = glfwInit();
			RT_CORE_ASSERT(success,"Could not initalize GLFW {0}");

			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			RT_PROFILE_SCOPE("glfw create window");

			#if defined(RT_DEBUG)
				if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
					glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			#endif


			m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.Title.c_str(), nullptr, nullptr);
			auto hwnd = glfwGetWin32Window(m_Window);

			BOOL dark = TRUE;
			DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

			++s_GLFWWindowCount;
		}

		m_Context =GraphicsContext::Create(m_Window);
		m_Context->Init();
		
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVsync(true);

		//Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowResizeEvent event(width, height);
				
				data.Width = width;
				data.Height = height;
				data.EventCallback(event);

			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key,0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, true);
						data.EventCallback(event);
						break;
					}
					default:
					{
						break;
					}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window,unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window,int button,int action,int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window,double xOffset,double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window,double xPos,double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});

	}

	void WindowsWindow::Shutdown()
	{ 
		RT_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);

		s_GLFWWindowCount -= 1;

		if (s_GLFWWindowCount == 0)
		{
			RT_CORE_INFO("Terminateing window");
			glfwTerminate();
		}
	}


	void WindowsWindow::OnUpdate()
	{
		RT_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}


	void WindowsWindow::SetVsync(bool enabled)
	{
		RT_PROFILE_FUNCTION();

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