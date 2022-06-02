#include "eospch.h"
#include "Eos/Core/Window.h"

#ifdef EOS_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Eos
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
		#ifdef EOS_PLATFORM_WINDOWS
			return CreateScope<WindowsWindow>(props);
		#else
			EOS_CORE_ASSERT(false, "Unknown platform!");
			return nullptr;
		#endif
	}

}