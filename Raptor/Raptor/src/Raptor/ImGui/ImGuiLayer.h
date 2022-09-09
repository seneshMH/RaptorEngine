#pragma once
#include "Raptor/Core/core.h"
#include "Raptor/Core/Layer.h"

#include "Raptor/Event/ApplicationEvent.h"
#include "Raptor/Event/KeyEvent.h"
#include "Raptor/Event/MouseEvent.h"

namespace Raptor {
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(Event& e) override;
		
		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}

