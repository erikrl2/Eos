#include <Eos.h>
#include <Eos/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Eos {

	class EosEditor : public Application
	{
	public:
		EosEditor(ApplicationCommandLineArgs args)
			: Application("EosEditor", args)
		{
			PushLayer(new EditorLayer());
		}
		~EosEditor()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		//args.Args[1] = "assets/scenes/Test.eos";
		//args.Count++;
		return new EosEditor(args);
	}

}
