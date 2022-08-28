#pragma once
#include "core.h"

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

