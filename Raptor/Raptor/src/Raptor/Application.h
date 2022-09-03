#pragma once
#include "core.h"
#include "Window.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "LayerStack.h"

#include "Raptor/ImGui/ImGuiLayer.h"
#include "Raptor/Renderer/Shader.h"
#include "Raptor/Renderer/Buffer.h"
#include "Raptor/Renderer/VertexArray.h"

namespace Raptor {
	class RAPTOR_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SqureVA;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

