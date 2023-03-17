#pragma once
#include "Raptor/Core/Base.h"

#ifdef RT_PLATFORM_WINDOWS

extern Raptor::Application* Raptor::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc,char** argv) {
	Raptor::Log::Init();
	
	RT_PROFILE_BEGIN_SESSION("Startup", "RaptorProfile-Startup.json");
	auto app = Raptor::CreateApplication({ argc, argv });
	RT_PROFILE_END_SESSION();

	RT_PROFILE_BEGIN_SESSION("Runtime", "RaptorProfile-Runtime.json");
	app->Run();
	RT_PROFILE_END_SESSION();

	RT_PROFILE_BEGIN_SESSION("Shutdown", "RaptorProfile-Shutdown.json");
	delete app;
	RT_PROFILE_END_SESSION();
}

#endif // RT_PLATFORM_WINDOWS
