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
		// for debugging
		args.Args[1] = "assets/scenes/Test.eos";
		args.Count++;

		ApplicationSpecification spec;
		spec.Name = "EosEditor";
		spec.CommandLineArgs = args;

		return new EosEditor(spec);
	}

}
