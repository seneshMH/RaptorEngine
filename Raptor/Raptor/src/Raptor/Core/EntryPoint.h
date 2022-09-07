#pragma once

#ifdef RT_PLATFORM_WINDOWS

extern Raptor::Application* Raptor::CreateApplication();

int main(int argc,char** argv) {
	Raptor::Log::Init();
	RT_CORE_WARN("Initalize Log");
	RT_INFO("Hello world");
	
	auto app = Raptor::CreateApplication();
	app->Run();
	delete app;
}

#endif // RT_PLATFORM_WINDOWS
