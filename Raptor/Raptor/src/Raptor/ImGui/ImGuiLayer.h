#pragma once
#include "Raptor/core.h"
#include "Raptor/Layer.h"

namespace Raptor {
	class RAPTOR_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		float m_Time = 0.0f;
	};
}

