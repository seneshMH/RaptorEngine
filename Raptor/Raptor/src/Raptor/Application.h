#pragma once
#include "core.h"
#include "Event/Event.h"

namespace Raptor {
	class RAPTOR_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}

