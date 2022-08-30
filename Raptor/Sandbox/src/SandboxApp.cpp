#include <Raptor.h>

class ExampleLayer : public Raptor::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{}

	void OnUpdate() override
	{
		RT_INFO("layer update");
	}

	void OnEvent(Raptor::Event& event) override
	{
		RT_TRACE("{0}", event);
	}
};


class SandBox : public Raptor::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Raptor::ImGuiLayer());
	}
	~SandBox(){}
};

Raptor::Application* Raptor::CreateApplication()
{
	return new SandBox();
}