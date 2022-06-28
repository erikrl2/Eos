#pragma once

#include <imgui/imgui.h>

#include "IconsFontAwesome5.h"

namespace Eos {

	class Style
	{
	public:
		enum class Theme
		{
			Dark1, Dark2, Dark3, Dark4, Dark5, Dark6, Dark7,
			Photoshop, SonicRiders, Unreal, VisualStudio, DarkRuda,
			ImGuiClassic, ImGuiDark, ImGuiLight
		};

		enum class Font
		{
			OpenSansRegular = 1, OpenSansBold, RobotoMedium, RudaRegular
		};

		static void LoadFonts();

		static void SetDefaultFont(Font font);
		static void SetTheme(Theme theme);

		static ImFont* GetImGuiFont(Font font);
	private:
		static void Style::LoadFontWithIcons(const char* filepath, float fontsize);

		static void SetEosDark1();
		static void SetEosDark2();
		static void SetEosDark3();
		static void SetEosDark4();
		static void SetEosDark5();
		static void SetEosDark6();
		static void SetEosDark7();
		static void SetPhotoshop();
		static void SetSonicRiders();
		static void SetUnreal();
		static void SetVisualStudio();
		static void SetDarkRuda();
	};
}
