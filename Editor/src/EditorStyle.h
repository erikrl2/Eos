#pragma once

#include "Eos/Renderer/Texture.h"

#include "IconsFontAwesome5.h"

#include <map>

#include <imgui/imgui.h>

namespace Eos {

	enum Theme : uint16_t
	{
		Theme_Dark1 = 0, Theme_Dark2, Theme_Dark3
	};

	enum Font : uint16_t
	{
		Font_OpenSansBold = 1, Font_OpenSansRegular, Font_RobotoMedium, Font_RudaRegular
	};

	enum Icon : uint16_t
	{
		Icon_Directory = 0, Icon_File, Icon_GrabHand, Icon_Checkerboard
	};

	class Style
	{
	public:
		static void LoadFonts();
		static void LoadIcons();

		static void SetFont(Font font);
		static void SetTheme(Theme theme);

		static ImFont* GetImGuiFont(Font font);
		static Ref<Texture2D> GetIcon(Icon icon);
	private:
		static void Style::LoadFont(const char* filepath, float fontsize);
		static void Style::LoadFontMergedWithIconFont(const char* filepath, float fontsize);

		static void SetEosDark1();
		static void SetEosDark2();
		static void SetEosDark3();
	private:
		inline static std::unordered_map<Icon, Ref<Texture2D>> s_Icons;
	};
}
