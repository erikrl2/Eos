#include <Eos.h>
#include <Eos/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Eos {

	class EosEditor : public Application
	{
	public:
		EosEditor(ApplicationCommandLineArgs args)
			: Application({ "EosEditor", 1280, 720 }, args)
		{
			PushLayer(new EditorLayer());
		}
		~EosEditor()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		// for testing
		args.Args[1] = "assets/scenes/Test.eos";
		args.Count++;

		return new EosEditor(args);
	}

}
