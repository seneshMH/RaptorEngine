#include <Raptor.h>
#include <Raptor/Core/EntryPoint.h>
#include "EditorLayer.h"

// enable optimus!
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace Raptor {

	class RaptorEditor : public Application
	{
	public:
		RaptorEditor(const ApplicationSpecification& spec)
			:Application(spec)
		{
			PushLayer(new EditorLayer());
		}
		~RaptorEditor() {}
	};

	Raptor::Application* Raptor::CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "RaptorEditor";
		spec.CommandLineArgs = args;

		return new RaptorEditor(spec);
	}
}