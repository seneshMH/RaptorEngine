#include <Raptor.h>

class ExampleLayer : public Raptor::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{}

	void OnUpdate() override
	{
		//RT_INFO("layer update");

		if (Raptor::Input::IsKeyPressed(RT_KEY_A))
		{
			RT_TRACE("Tab key is presed");
		}
	}

	void OnEvent(Raptor::Event& event) override
	{
		//RT_TRACE("{0}", event);
	}
};


class SandBox : public Raptor::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}
	~SandBox(){}
};

Raptor::Application* Raptor::CreateApplication()
{
	return new SandBox();
}