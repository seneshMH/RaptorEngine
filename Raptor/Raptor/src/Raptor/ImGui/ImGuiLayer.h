#pragma once
#include "Raptor/Core/Base.h"
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

		void BlockEvents(bool block) { m_BlockEvents = block; }
	
		void SetDarkTheamColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}

