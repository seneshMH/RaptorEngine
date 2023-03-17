#include <Raptor.h>
#include <Raptor/Core/EntryPoint.h>
#include "Sandbox2D.h"
#include "ExampleLayer.h"

// enable optimus!
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

class SandBox : public Raptor::Application
{
public:
	SandBox(const Raptor::ApplicationSpecification& specification)
		: Raptor::Application(specification)
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~SandBox(){}
};

Raptor::Application* Raptor::CreateApplication(Raptor::ApplicationCommandLineArgs args)
{
	Raptor::ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Raptor-Editor";
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}