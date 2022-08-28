#pragma once

#include <imgui/imgui.h>

#include "IconsFontAwesome5.h"

namespace Eos {

	class Style
	{
	public:
		enum Theme : uint16_t
		{
			Dark1, Dark2, Dark3
		};

		enum Font : uint16_t
		{
			OpenSansBold = 1, OpenSansRegular, RobotoMedium, RudaRegular
		};

		static void LoadFonts();

		static void SetDefaultFont(Font font);
		static void SetTheme(Theme theme);

		static ImFont* GetImGuiFont(Font font);
	private:
		static void Style::LoadFont(const char* filepath, float fontsize);
		static void Style::LoadFontWithIcons(const char* filepath, float fontsize);

		static void SetEosDark1();
		static void SetEosDark2();
		static void SetEosDark3();
	};
}
