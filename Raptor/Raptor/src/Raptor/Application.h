#pragma once
#include "core.h"
#include "Event/Event.h"
#include "Window.h"
#include "Event/ApplicationEvent.h"

namespace Raptor {
	class RAPTOR_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication();
}

