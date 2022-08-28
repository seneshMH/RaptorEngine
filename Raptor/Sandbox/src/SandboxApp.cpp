#include <Raptor.h>

class SandBox : public Raptor::Application
{
public:
	SandBox(){}
	~SandBox(){}
};

Raptor::Application* Raptor::CreateApplication()
{
	return new SandBox();
}