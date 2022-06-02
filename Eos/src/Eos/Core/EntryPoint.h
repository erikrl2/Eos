#pragma once
#include "Eos/Core/Base.h"
#include "Eos/Core/Application.h"

#ifdef EOS_PLATFORM_WINDOWS

extern Eos::Application* Eos::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Eos::Log::Init();

	EOS_PROFILE_BEGIN_SESSION("Startup", "EosProfile-Startup.json");
	auto app = Eos::CreateApplication({ argc, argv });
	EOS_PROFILE_END_SESSION();

	EOS_PROFILE_BEGIN_SESSION("Runtime", "EosProfile-Runtime.json");
	app->Run();
	EOS_PROFILE_END_SESSION();

	EOS_PROFILE_BEGIN_SESSION("Shutdown", "EosProfile-Shutdown.json");
	delete app;
	EOS_PROFILE_END_SESSION();
}

#endif
