#include <Eos.h>
#include <Eos/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Eos::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}
 ~Sandbox()
	{
	}
};

Eos::Application* Eos::CreateApplication(ApplicationCommandLineArgs args)
{
	return new Sandbox();
}
