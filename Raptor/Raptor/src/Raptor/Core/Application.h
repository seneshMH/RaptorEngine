#pragma once
#include "Base.h"
#include "Window.h"
#include "Raptor/Event/Event.h"
#include "Raptor/Event/ApplicationEvent.h"
#include "LayerStack.h"

#include "Raptor/ImGui/ImGuiLayer.h"

namespace Raptor {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }

		void Close();

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

