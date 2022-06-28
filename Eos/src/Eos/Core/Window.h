#pragma once

#include <sstream>

#include "Eos/Core/Base.h"
#include "Eos/Events/Event.h"

namespace Eos {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool Maximized;

		WindowProps(const std::string& title = "Eos Engine", uint32_t width = 1280, uint32_t height = 720, bool maximized = false)
			: Title(title), Width(width), Height(height), Maximized(maximized)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;
		
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetTitle(const std::string& title) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	public:
		static float s_HighDPIScaleFactor;
	};

}
