#pragma once

#ifdef RT_PLATFORM_WINDOWS

extern Raptor::Application* Raptor::CreateApplication();

int main(int argc,char** argv) {
	
	auto app = Raptor::CreateApplication();
	app->Run();
	delete app;
}

#endif // RT_PLATFORM_WINDOWS
