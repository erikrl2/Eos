#include <Eos.h>
#include <Eos/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Eos {

	class EosEditor : public Application
	{
	public:
		EosEditor(WindowProps props, ApplicationCommandLineArgs args)
			: Application(props, args)
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

		WindowProps properties;
		properties.Title = "EosEditor";
		properties.Width = 1280;
		properties.Height = 720;

		return new EosEditor(properties, args);
	}

}
