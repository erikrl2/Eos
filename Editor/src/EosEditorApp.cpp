#include <Eos.h>
#include <Eos/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Eos {

	class EosEditor : public Application
	{
	public:
		EosEditor(const ApplicationSpecification& specification)
			: Application(specification)
		{
			PushLayer(new EditorLayer());
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "EosEditor";
		spec.CommandLineArgs = args;

		// For testing purposes (TODO: Remove)
		spec.CommandLineArgs.Count++;
		spec.CommandLineArgs.Args[1] = "SandboxProject/Sandbox.eosproj";

		return new EosEditor(spec);
	}

}
