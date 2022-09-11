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
		RaptorEditor()
			:Application("Raptor Editor")
		{
			PushLayer(new EditorLayer());
		}
		~RaptorEditor() {}
	};

	Raptor::Application* Raptor::CreateApplication()
	{
		return new RaptorEditor();
	}
}