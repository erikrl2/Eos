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
		~EosEditor()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		// for debugging
		args.Args[1] = "assets/scenes/Test.eos";
		args.Count++;

		ApplicationSpecification spec;
		spec.Name = "EosEditor";
		spec.Width = 1280;
		spec.Height = 720;
		spec.CommandLineArgs = args;

		return new EosEditor(spec);
	}

}
