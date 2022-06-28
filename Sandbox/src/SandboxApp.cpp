#include <Eos.h>
#include <Eos/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Eos::Application
{
public:
	Sandbox(Eos::ApplicationCommandLineArgs args)
		: Application({ "Sandbox" }, args)
	{
		PushLayer(new Sandbox2D());
	}
 ~Sandbox()
	{
	}
};

Eos::Application* Eos::CreateApplication(Eos::ApplicationCommandLineArgs args)
{
	return new Sandbox(args);
}
