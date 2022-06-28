#pragma once

#include <imgui/imgui.h>

#include "IconsFontAwesome5.h"

namespace Eos::Style {

	enum class Theme : int
	{
		Dark1, Dark2, Dark3, Dark4, Dark5, Dark6, Dark7,
		Photoshop, SonicRiders, Unreal, VisualStudio, DarkRuda,
		ImGuiClassic, ImGuiDark, ImGuiLight
	};

	enum class Font : int
	{
		OpenSansRegular = 1, OpenSansBold, RobotoMedium, RudaRegular
	};

	void SetTheme(Theme theme);

	ImFont* GetImGuiFont(Font font);
	void SetDefaultFont(Font font);

	// to be called once at program start
	void LoadFonts();

}
