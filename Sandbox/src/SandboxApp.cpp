#include <Eos.h>
#include <Eos/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Eos::Application
{
public:
	Sandbox(const Eos::ApplicationSpecification& specification)
		: Application(specification)
	{
		PushLayer(new Sandbox2D());
	}
 ~Sandbox()
	{
	}
};

Eos::Application* Eos::CreateApplication(Eos::ApplicationCommandLineArgs args)
{
	Eos::ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.CommandLineArgs = args;
#ifdef EOS_DEBUG // and release?
	spec.WorkingDirectory = "../EosEditor";
#endif

	return new Sandbox(spec);
}
