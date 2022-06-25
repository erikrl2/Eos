#pragma once

namespace Eos::Style {

	enum class Theme : int
	{
		Dark1, Dark2, Dark3, Dark4, Dark5, Dark6, Dark7,
		Photoshop, SonicRiders, Unreal, VisualStudio, DarkRuda,
		ImGuiClassic, ImGuiDark, ImGuiLight
	};

	enum class Font : int
	{
		OpenSansRegular, OpenSansBold, RobotoMedium, RudaRegular, RudaBold
	};

	void SetTheme(Theme theme);
	void SetFont(Font font);

}
