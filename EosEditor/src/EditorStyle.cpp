#include "eospch.h"
#include "EditorStyle.h"

namespace Eos {


		void Style::SetTheme(Theme theme)
		{
			static ImGuiStyle imguiStyle = ImGui::GetStyle(); // Backup of original style

			switch (theme)
			{
				case Theme::Dark1:			SetEosDark1(); break;
				case Theme::Dark2:			SetEosDark2(); break;
				case Theme::Dark3:			SetEosDark3(); break;
				case Theme::Dark4:			SetEosDark4(); break;
				case Theme::Dark5:			SetEosDark5(); break;
				case Theme::Dark6:			SetEosDark6(); break;
				case Theme::Dark7:			SetEosDark7(); break;
				case Theme::Photoshop:		SetPhotoshop(); break;
				case Theme::SonicRiders:	SetSonicRiders(); break;
				case Theme::Unreal:			SetUnreal(); break;
				case Theme::VisualStudio:	SetVisualStudio(); break;
				case Theme::DarkRuda:		SetDarkRuda(); break;
				case Theme::ImGuiClassic:	ImGui::GetStyle() = imguiStyle; ImGui::StyleColorsClassic(); break;
				case Theme::ImGuiDark:		ImGui::GetStyle() = imguiStyle; ImGui::StyleColorsDark(); break;
				case Theme::ImGuiLight:		ImGui::GetStyle() = imguiStyle; ImGui::StyleColorsLight(); break;
			}
		}

		ImFont* Style::GetImGuiFont(Font font)
		{
			return ImGui::GetIO().Fonts->Fonts[(int)font];
		}

		void Style::SetDefaultFont(Font font)
		{
			ImGui::GetIO().FontDefault = GetImGuiFont(font);
		}

		void Style::LoadFonts()
		{
			LoadFontWithIcons("assets/fonts/opensans/OpenSans-Regular.ttf", 17.0f);
			ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 17.0f);

			LoadFontWithIcons("assets/fonts/Roboto/Roboto-Medium.ttf", 16.0f);
			LoadFontWithIcons("assets/fonts/Ruda/Ruda-Regular.ttf", 16.0f);
		}

		void Style::LoadFontWithIcons(const char* filepath, float fontsize)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontFromFileTTF(filepath, fontsize);

			// merge in icons from Font Awesome
			static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
			ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
			io.Fonts->AddFontFromFileTTF("assets/fonts/fontawesome/" FONT_ICON_FILE_NAME_FAS, 13.0f, &icons_config, icons_ranges);
		}

	void Style::SetEosDark1()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding								= ImVec2(7.0f, 7.0f);
		style.WindowMinSize								= ImVec2(200.0f, 40.0f);
		style.WindowTitleAlign							= ImVec2(0.5f, 0.5f);
		style.FramePadding								= ImVec2(4.0f, 4.0f);
		style.ItemSpacing								= ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing							= ImVec2(4.0f, 4.0f);
		style.CellPadding								= ImVec2(4.0f, 2.0f);
		style.ButtonTextAlign							= ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign						= ImVec2(0.0f, 0.0f);
		style.WindowMenuButtonPosition					= ImGuiDir_None;
		style.ColorButtonPosition						= ImGuiDir_Right;
		style.Alpha										= 1.0f;
		style.DisabledAlpha								= 0.60f;
		style.WindowRounding							= 0.0f;
		style.WindowBorderSize							= 1.0f;
		style.ChildRounding								= 0.0f;
		style.ChildBorderSize							= 1.0f;
		style.PopupRounding								= 0.0f;
		style.PopupBorderSize							= 1.0f;
		style.FrameRounding								= 6.0f;
		style.FrameBorderSize							= 0.0f;
		style.IndentSpacing								= 21.0f;
		style.ColumnsMinSpacing							= 6.0f;
		style.ScrollbarSize								= 13.0f;
		style.ScrollbarRounding							= 12.0f;
		style.GrabMinSize								= 7.0f;
		style.GrabRounding								= 3.0f;
		style.TabRounding								= 4.0f;
		style.TabBorderSize								= 0.0f;
		style.TabMinWidthForCloseButton					= 0.0f;

		style.Colors[ImGuiCol_Text]						= ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.17f, 0.17f, 0.17f, 1.00f); // also sets: Header TabHovered, TabActive, TabUnfocusedActive
		style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_Border]					= ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
		style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.06f, 0.06f, 0.06f, 1.00f); // also sets: TitleBgActive, TitleBgCollapsed, MenuBarBg, Separator, SeparatorHovered, SeparatorActive
		style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_CheckMark]				= ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_Button]					= ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_Tab]						= ImVec4(0.00f, 0.00f, 0.00f, 0.00f); // also sets: TabUnfocused
		style.Colors[ImGuiCol_DockingPreview]			= ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(0.68f, 0.68f, 0.68f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.30f, 0.30f, 0.34f, 1.00f);
		style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.00f, 1.00f, 1.00f, 0.05f);
		style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(1.00f, 1.00f, 1.00f, 0.15f);
		style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_NavHighlight]				= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.00f, 0.00f, 0.00f, 0.58f);
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.00f, 0.00f, 0.00f, 0.58f);
	}

	void Style::SetEosDark2()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding								= ImVec2(8.00f, 8.00f);
		style.WindowMinSize								= ImVec2(32.00f, 32.00f);
		style.WindowTitleAlign							= ImVec2(0.50f, 0.50f);
		style.FramePadding								= ImVec2(5.00f, 2.00f);
		style.CellPadding								= ImVec2(6.00f, 6.00f);
		style.ItemSpacing								= ImVec2(6.00f, 6.00f);
		style.ItemInnerSpacing							= ImVec2(6.00f, 6.00f);
		style.ButtonTextAlign							= ImVec2(0.50f, 0.50f);
		style.SelectableTextAlign						= ImVec2(0.00f, 0.00f);
		style.WindowMenuButtonPosition					= ImGuiDir_Right;
		style.ColorButtonPosition						= ImGuiDir_Right;
		style.DisabledAlpha								= 0.6f;
		style.Alpha										= 1;
		style.IndentSpacing								= 25;
		style.ColumnsMinSpacing							= 6;
		style.ScrollbarSize								= 15;
		style.GrabMinSize								= 10;
		style.WindowBorderSize							= 1;
		style.ChildBorderSize							= 1;
		style.PopupBorderSize							= 1;
		style.FrameBorderSize							= 1;
		style.TabBorderSize								= 1;
		style.WindowRounding							= 7;
		style.ChildRounding								= 4;
		style.FrameRounding								= 3;
		style.PopupRounding								= 4;
		style.ScrollbarRounding							= 9;
		style.GrabRounding								= 3;
		style.TabMinWidthForCloseButton					= 0;
		style.TabRounding								= 4;

		style.Colors[ImGuiCol_Text]						= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // also sets: Header TabHovered, TabActive, TabUnfocusedActive
		style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_Border]					= ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
		style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
		style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.00f, 0.00f, 0.00f, 1.00f); // also sets: TitleBgActive, TitleBgCollapsed, MenuBarBg, Separator, SeparatorHovered, SeparatorActive
		style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		style.Colors[ImGuiCol_CheckMark]				= ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		style.Colors[ImGuiCol_Button]					= ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		style.Colors[ImGuiCol_Tab]						= ImVec4(0.00f, 0.00f, 0.00f, 0.52f); // also sets: TabUnfocused
		style.Colors[ImGuiCol_DockingPreview]			= ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLines]				= ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_NavHighlight]				= ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
	}

	void Style::SetEosDark3()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding								= ImVec2(8.0f, 8.0f);
		style.WindowMinSize								= ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign							= ImVec2(0.5f, 0.5f);
		style.FramePadding								= ImVec2(4.0f, 3.0f);
		style.ItemSpacing								= ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing							= ImVec2(4.0f, 4.0f);
		style.CellPadding								= ImVec2(4.0f, 2.0f);
		style.ButtonTextAlign							= ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign						= ImVec2(0.0f, 0.0f);
		style.WindowMenuButtonPosition					= ImGuiDir_Right;
		style.ColorButtonPosition						= ImGuiDir_Right;
		style.Alpha										= 1.0f;
		style.DisabledAlpha								= 0.6f;
		style.WindowRounding							= 0.0f;
		style.WindowBorderSize							= 1.0f;
		style.ChildRounding								= 0.0f;
		style.ChildBorderSize							= 1.0f;
		style.PopupRounding								= 0.0f;
		style.PopupBorderSize							= 1.0f;
		style.FrameRounding								= 3.0f;
		style.FrameBorderSize							= 0.0f;
		style.IndentSpacing								= 21.0f;
		style.ColumnsMinSpacing							= 6.0f;
		style.ScrollbarSize								= 14.0f;
		style.ScrollbarRounding							= 9.0f;
		style.GrabMinSize								= 10.0f;
		style.GrabRounding								= 3.0f;
		style.TabRounding								= 4.0f;
		style.TabBorderSize								= 0.0f;
		style.TabMinWidthForCloseButton					= 0.0f;

		style.Colors[ImGuiCol_Text]						= ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.00f, 0.00f, 0.00f, 1.00f); // also sets: Header TabHovered, TabActive, TabUnfocusedActive
		style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_Border]					= ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
		style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.37f, 0.14f, 0.14f, 0.67f);
		style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.39f, 0.20f, 0.20f, 0.67f);
		style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.48f, 0.16f, 0.16f, 1.00f); // also sets: TitleBgActive, TitleBgCollapsed, MenuBarBg, Separator, SeparatorHovered, SeparatorActive
		style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_CheckMark]				= ImVec4(0.56f, 0.10f, 0.10f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab]				= ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
		style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
		style.Colors[ImGuiCol_Button]					= ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
		style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.80f, 0.17f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_Tab]						= ImVec4(0.07f, 0.07f, 0.07f, 0.21f); // also sets: TabUnfocused
		style.Colors[ImGuiCol_DockingPreview]			= ImVec4(0.47f, 0.47f, 0.47f, 0.47f);
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
		style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_NavHighlight]				= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	void Style::SetEosDark4()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding								= ImVec2(8.00f, 8.00f);
		style.WindowMinSize								= ImVec2(32.00f, 32.00f);
		style.WindowTitleAlign							= ImVec2(0.50f, 0.50f);
		style.FramePadding								= ImVec2(5.00f, 2.00f);
		style.CellPadding								= ImVec2(6.00f, 6.00f);
		style.ItemSpacing								= ImVec2(6.00f, 6.00f);
		style.ItemInnerSpacing							= ImVec2(6.00f, 6.00f);
		style.ButtonTextAlign							= ImVec2(0.50f, 0.50f);
		style.SelectableTextAlign						= ImVec2(0.00f, 0.00f);
		style.WindowMenuButtonPosition					= ImGuiDir_Right;
		style.ColorButtonPosition						= ImGuiDir_Right;
		style.DisabledAlpha								= 0.6f;
		style.Alpha										= 1;
		style.IndentSpacing								= 25;
		style.ColumnsMinSpacing							= 6;
		style.ScrollbarSize								= 15;
		style.GrabMinSize								= 10;
		style.WindowBorderSize							= 1;
		style.ChildBorderSize							= 1;
		style.PopupBorderSize							= 1;
		style.FrameBorderSize							= 0;
		style.TabBorderSize								= 1;
		style.WindowRounding							= 7;
		style.ChildRounding								= 4;
		style.FrameRounding								= 3;
		style.PopupRounding								= 4;
		style.ScrollbarRounding							= 9;
		style.GrabRounding								= 3;
		style.TabMinWidthForCloseButton					= 0;
		style.TabRounding								= 4;

		style.Colors[ImGuiCol_Text]						= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.06f, 0.06f, 0.06f, 1.00f); // also sets: Header TabHovered, TabActive, TabUnfocusedActive
		style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_Border]					= ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.44f, 0.44f, 0.44f, 0.60f);
		style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.57f, 0.57f, 0.57f, 0.70f);
		style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.76f, 0.76f, 0.76f, 0.80f);
		style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.04f, 0.04f, 0.04f, 1.00f); // also sets: TitleBgActive, TitleBgCollapsed, MenuBarBg, Separator, SeparatorHovered, SeparatorActive
		style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_CheckMark]				= ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
		style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
		style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
		style.Colors[ImGuiCol_Button]					= ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
		style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
		style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
		style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
		style.Colors[ImGuiCol_Tab]						= ImVec4(0.18f, 0.18f, 0.18f, 1.00f); // also sets: TabUnfocused
		style.Colors[ImGuiCol_DockingPreview]			= ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.13f, 0.13f, 0.13f, 0.80f);
		style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
		style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(0.13f, 0.75f, 0.75f, 0.90f);
		style.Colors[ImGuiCol_NavHighlight]				= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	void Style::SetEosDark5()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding								= ImVec2(8.00f, 8.00f);
		style.WindowMinSize								= ImVec2(32.00f, 32.00f);
		style.WindowTitleAlign							= ImVec2(0.50f, 0.50f);
		style.FramePadding								= ImVec2(5.00f, 2.00f);
		style.CellPadding								= ImVec2(6.00f, 6.00f);
		style.ItemSpacing								= ImVec2(6.00f, 6.00f);
		style.ItemInnerSpacing							= ImVec2(6.00f, 6.00f);
		style.ButtonTextAlign							= ImVec2(0.50f, 0.50f);
		style.SelectableTextAlign						= ImVec2(0.00f, 0.00f);
		style.WindowMenuButtonPosition					= ImGuiDir_Right;
		style.ColorButtonPosition						= ImGuiDir_Right;
		style.DisabledAlpha								= 0.6f;
		style.Alpha										= 1;
		style.IndentSpacing								= 25;
		style.ColumnsMinSpacing							= 6;
		style.ScrollbarSize								= 15;
		style.GrabMinSize								= 10;
		style.WindowBorderSize							= 1;
		style.ChildBorderSize							= 1;
		style.PopupBorderSize							= 1;
		style.FrameBorderSize							= 0;
		style.TabBorderSize								= 1;
		style.WindowRounding							= 7;
		style.ChildRounding								= 4;
		style.FrameRounding								= 2.3f;
		style.PopupRounding								= 4;
		style.ScrollbarRounding							= 9;
		style.GrabRounding								= 2.3f;
		style.TabMinWidthForCloseButton					= 0;
		style.TabRounding								= 4;

		style.Colors[ImGuiCol_Text]						= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.13f, 0.14f, 0.15f, 1.00f); // also sets: Header TabHovered, TabActive, TabUnfocusedActive
		style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_Border]					= ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.08f, 0.08f, 0.09f, 1.00f); // also sets: TitleBgActive, TitleBgCollapsed, MenuBarBg, Separator, SeparatorHovered, SeparatorActive
		style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_CheckMark]				= ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
		style.Colors[ImGuiCol_Button]					= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_Tab]						= ImVec4(0.08f, 0.08f, 0.09f, 0.83f); // also sets: TabUnfocused
		style.Colors[ImGuiCol_DockingPreview]			= ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.30f, 0.30f, 0.34f, 1.00f);
		style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.00f, 1.00f, 1.00f, 0.05f);
		style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		style.Colors[ImGuiCol_NavHighlight]				= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	void Style::SetEosDark6()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding								= ImVec2(8.0f, 8.0f);
		style.WindowMinSize								= ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign							= ImVec2(0.5f, 0.5f);
		style.FramePadding								= ImVec2(4.0f, 3.0f);
		style.ItemSpacing								= ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing							= ImVec2(4.0f, 4.0f);
		style.CellPadding								= ImVec2(4.0f, 2.0f);
		style.ButtonTextAlign							= ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign						= ImVec2(0.0f, 0.0f);
		style.WindowMenuButtonPosition					= ImGuiDir_Right;
		style.ColorButtonPosition						= ImGuiDir_Right;
		style.Alpha										= 1.0f;
		style.DisabledAlpha								= 0.6f;
		style.WindowRounding							= 0.0f;
		style.WindowBorderSize							= 0.0f;
		style.ChildRounding								= 0.0f;
		style.ChildBorderSize							= 1.0f;
		style.PopupRounding								= 0.0f;
		style.PopupBorderSize							= 0.0f;
		style.FrameRounding								= 4.0f;
		style.FrameBorderSize							= 0.0f;
		style.IndentSpacing								= 21.0f;
		style.ColumnsMinSpacing							= 6.0f;
		style.ScrollbarSize								= 14.0f;
		style.ScrollbarRounding							= 9.0f;
		style.GrabMinSize								= 10.0f;
		style.GrabRounding								= 4.0f;
		style.TabRounding								= 4.0f;
		style.TabBorderSize								= 0.0f;
		style.TabMinWidthForCloseButton					= 0.0f;

		style.Colors[ImGuiCol_Text]						= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
		style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.09f, 0.09f, 0.09f, 1.00f); // also sets: Header TabHovered, TabActive, TabUnfocusedActive
		style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_Border]					= ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
		style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.84f, 0.66f, 0.66f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.84f, 0.66f, 0.66f, 0.67f);
		style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.47f, 0.22f, 0.22f, 0.67f); // also sets: TitleBgActive, TitleBgCollapsed, MenuBarBg, Separator, SeparatorHovered, SeparatorActive
		style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_CheckMark]				= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.71f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.84f, 0.66f, 0.66f, 1.00f);
		style.Colors[ImGuiCol_Button]					= ImVec4(0.47f, 0.22f, 0.22f, 0.65f);
		style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.71f, 0.39f, 0.39f, 0.65f);
		style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
		style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
		style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
		style.Colors[ImGuiCol_Tab]						= ImVec4(0.71f, 0.39f, 0.39f, 0.24f); // also sets: TabUnfocused
		style.Colors[ImGuiCol_DockingPreview]			= ImVec4(0.47f, 0.22f, 0.22f, 0.67f);
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.30f, 0.30f, 0.34f, 1.00f);
		style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.00f, 1.00f, 1.00f, 0.05f);
		style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(1.0f, 1.0f, 1.0f, 0.90f);
		style.Colors[ImGuiCol_NavHighlight]				= ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	void Style::SetEosDark7()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding								= ImVec2(8.0f, 8.0f);
		style.WindowMinSize								= ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign							= ImVec2(0.5f, 0.5f);
		style.FramePadding								= ImVec2(4.0f, 3.0f);
		style.ItemSpacing								= ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing							= ImVec2(4.0f, 4.0f);
		style.CellPadding								= ImVec2(4.0f, 2.0f);
		style.ButtonTextAlign							= ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign						= ImVec2(0.0f, 0.0f);
		style.WindowMenuButtonPosition					= ImGuiDir_Right;
		style.ColorButtonPosition						= ImGuiDir_Right;
		style.Alpha										= 1.0f;
		style.DisabledAlpha								= 0.6f;
		style.WindowRounding							= 0.0f;
		style.WindowBorderSize							= 1.0f;
		style.ChildRounding								= 0.0f;
		style.ChildBorderSize							= 1.0f;
		style.PopupRounding								= 0.0f;
		style.PopupBorderSize							= 1.0f;
		style.FrameRounding								= 4.0f;
		style.FrameBorderSize							= 0.0f;
		style.IndentSpacing								= 21.0f;
		style.ColumnsMinSpacing							= 6.0f;
		style.ScrollbarSize								= 14.0f;
		style.ScrollbarRounding							= 9.0f;
		style.GrabMinSize								= 10.0f;
		style.GrabRounding								= 4.0f;
		style.TabRounding								= 4.0f;
		style.TabBorderSize								= 0.0f;
		style.TabMinWidthForCloseButton					= 0.0f;

		style.Colors[ImGuiCol_Text]						= ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
		style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.11f, 0.15f, 0.17f, 1.00f); // also sets: Header TabHovered, TabActive, TabUnfocusedActive
		style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
		style.Colors[ImGuiCol_Border]					= ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
		style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.08f, 0.10f, 0.12f, 1.00f); // also sets: TitleBgActive, TitleBgCollapsed, MenuBarBg, Separator, SeparatorHovered, SeparatorActive
		style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_CheckMark]				= ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_Button]					= ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_Tab]						= ImVec4(0.11f, 0.15f, 0.17f, 1.00f); // also sets: TabUnfocused
		style.Colors[ImGuiCol_DockingPreview]			= ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.30f, 0.30f, 0.34f, 1.00f);
		style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.00f, 1.00f, 1.00f, 0.05f);
		style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(0.28f, 0.56f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_NavHighlight]				= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	void Style::SetPhotoshop()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding								= ImVec2(8.0f, 8.0f);
		style.WindowMinSize								= ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign							= ImVec2(0.5f, 0.5f);
		style.FramePadding								= ImVec2(4.0f, 3.0f);
		style.ItemSpacing								= ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing							= ImVec2(4.0f, 4.0f);
		style.CellPadding								= ImVec2(4.0f, 2.0f);
		style.ButtonTextAlign							= ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign						= ImVec2(0.0f, 0.0f);
		style.WindowMenuButtonPosition					= ImGuiDir_Right;
		style.ColorButtonPosition						= ImGuiDir_Right;
		style.Alpha										= 1.0f;
		style.DisabledAlpha								= 0.6f;
		style.WindowRounding							= 4.0f;
		style.WindowBorderSize							= 1.0f;
		style.ChildRounding								= 4.0f;
		style.ChildBorderSize							= 1.0f;
		style.PopupRounding								= 2.0f;
		style.PopupBorderSize							= 1.0f;
		style.FrameRounding								= 2.0f;
		style.FrameBorderSize							= 0.0f;
		style.IndentSpacing								= 21.0f;
		style.ColumnsMinSpacing							= 6.0f;
		style.ScrollbarSize								= 13.0f;
		style.ScrollbarRounding							= 12.0f;
		style.GrabMinSize								= 7.0f;
		style.GrabRounding								= 0.0f;
		style.TabRounding								= 0.0f;
		style.TabBorderSize								= 1.0f;
		style.TabMinWidthForCloseButton					= 0.0f;

		style.Colors[ImGuiCol_Text]						= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.17f, 0.17f, 0.17f, 1.00f); // also sets: Header TabHovered, TabActive, TabUnfocusedActive
		style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.27f, 0.27f, 0.27f, 0.00f);
		style.Colors[ImGuiCol_Border]					= ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.14f, 0.14f, 0.14f, 1.00f); // also sets: TitleBgActive, TitleBgCollapsed, MenuBarBg, Separator, SeparatorHovered, SeparatorActive
		style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_CheckMark]				= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_Button]					= ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
		style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(1.00f, 1.00f, 1.00f, 0.15f);
		style.Colors[ImGuiCol_ButtonActive]				= ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
		style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(1.00f, 1.00f, 1.00f, 0.25f);
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(1.00f, 1.00f, 1.00f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_Tab]						= ImVec4(0.09f, 0.09f, 0.09f, 1.00f); // also sets: TabUnfocused
		style.Colors[ImGuiCol_DockingPreview]			= ImVec4(1.00f, 0.38f, 0.00f, 0.70f);
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.30f, 0.30f, 0.34f, 1.00f);
		style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.00f, 1.00f, 1.00f, 0.05f);
		style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(1.00f, 1.00f, 1.00f, 0.15f);
		style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_NavHighlight]				= ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.00f, 0.00f, 0.00f, 0.58f);
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.00f, 0.00f, 0.00f, 0.58f);
	}

	void Style::SetSonicRiders()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding								= ImVec2(8.0f, 8.0f);
		style.WindowMinSize								= ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign							= ImVec2(0.5f, 0.5f);
		style.FramePadding								= ImVec2(4.0f, 3.0f);
		style.ItemSpacing								= ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing							= ImVec2(4.0f, 4.0f);
		style.CellPadding								= ImVec2(4.0f, 2.0f);
		style.ButtonTextAlign							= ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign						= ImVec2(0.0f, 0.0f);
		style.WindowMenuButtonPosition					= ImGuiDir_Right;
		style.ColorButtonPosition						= ImGuiDir_Right;
		style.Alpha										= 1.0f;
		style.DisabledAlpha								= 0.6f;
		style.WindowRounding							= 0.0f;
		style.WindowBorderSize							= 0.0f;
		style.ChildRounding								= 0.0f;
		style.ChildBorderSize							= 1.0f;
		style.PopupRounding								= 0.0f;
		style.PopupBorderSize							= 0.0f;
		style.FrameRounding								= 4.0f;
		style.FrameBorderSize							= 0.0f;
		style.IndentSpacing								= 21.0f;
		style.ColumnsMinSpacing							= 6.0f;
		style.ScrollbarSize								= 14.0f;
		style.ScrollbarRounding							= 9.0f;
		style.GrabMinSize								= 10.0f;
		style.GrabRounding								= 4.0f;
		style.TabRounding								= 4.0f;
		style.TabBorderSize								= 0.0f;
		style.TabMinWidthForCloseButton					= 0.0f;

		style.Colors[ImGuiCol_Text]						= ImVec4(1.000f, 1.000f, 1.000f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.729f, 0.749f, 0.737f, 1.00f);
		style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.086f, 0.086f, 0.086f, 1.00f); // also sets: Header TabHovered, TabActive, TabUnfocusedActive
		style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.000f, 0.000f, 0.000f, 0.00f);
		style.Colors[ImGuiCol_Border]					= ImVec4(0.200f, 0.200f, 0.200f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.000f, 0.000f, 0.000f, 0.00f);
		style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.709f, 0.388f, 0.388f, 0.54f);
		style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.839f, 0.658f, 0.658f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.839f, 0.658f, 0.658f, 0.67f);
		style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.466f, 0.219f, 0.219f, 0.67f); // also sets: TitleBgActive, TitleBgCollapsed, MenuBarBg, Separator, SeparatorHovered, SeparatorActive
		style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.019f, 0.019f, 0.019f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.309f, 0.309f, 0.309f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.407f, 0.407f, 0.407f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.509f, 0.509f, 0.509f, 1.00f);
		style.Colors[ImGuiCol_CheckMark]				= ImVec4(1.000f, 1.000f, 1.000f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.709f, 0.388f, 0.388f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.839f, 0.658f, 0.658f, 1.00f);
		style.Colors[ImGuiCol_Button]					= ImVec4(0.466f, 0.219f, 0.219f, 0.65f);
		style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.709f, 0.388f, 0.388f, 0.65f);
		style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.200f, 0.200f, 0.200f, 0.50f);
		style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.709f, 0.388f, 0.388f, 0.54f);
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.839f, 0.658f, 0.658f, 0.66f);
		style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.839f, 0.658f, 0.658f, 0.66f);
		style.Colors[ImGuiCol_Tab]						= ImVec4(0.709f, 0.388f, 0.388f, 0.54f); // also sets: TabUnfocused
		style.Colors[ImGuiCol_DockingPreview]			= ImVec4(0.400f, 0.400f, 0.400f, 1.00f);
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.000f, 0.000f, 0.000f, 1.00f);
		style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.607f, 0.607f, 0.607f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.000f, 0.427f, 0.349f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.898f, 0.698f, 0.000f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.000f, 0.600f, 0.000f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.188f, 0.188f, 0.200f, 1.00f);
		style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.309f, 0.309f, 0.349f, 1.00f);
		style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.227f, 0.227f, 0.247f, 1.00f);
		style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.000f, 0.000f, 0.000f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.000f, 1.000f, 1.000f, 0.06f);
		style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.258f, 0.588f, 0.976f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(1.000f, 1.000f, 0.000f, 0.90f);
		style.Colors[ImGuiCol_NavHighlight]				= ImVec4(0.407f, 0.407f, 0.407f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.000f, 1.000f, 1.000f, 0.69f);
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.800f, 0.800f, 0.800f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.800f, 0.800f, 0.800f, 0.35f);
	}

	void Style::SetUnreal()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding								= ImVec2(8.0f, 8.0f);
		style.WindowMinSize								= ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign							= ImVec2(0.5f, 0.5f);
		style.FramePadding								= ImVec2(4.0f, 3.0f);
		style.ItemSpacing								= ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing							= ImVec2(4.0f, 4.0f);
		style.CellPadding								= ImVec2(4.0f, 2.0f);
		style.ButtonTextAlign							= ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign						= ImVec2(0.0f, 0.0f);
		style.WindowMenuButtonPosition					= ImGuiDir_Right;
		style.ColorButtonPosition						= ImGuiDir_Right;
		style.Alpha										= 1.0f;
		style.DisabledAlpha								= 0.6f;
		style.WindowRounding							= 0.0f;
		style.WindowBorderSize							= 1.0f;
		style.ChildRounding								= 0.0f;
		style.ChildBorderSize							= 1.0f;
		style.PopupRounding								= 0.0f;
		style.PopupBorderSize							= 1.0f;
		style.FrameRounding								= 0.0f;
		style.FrameBorderSize							= 0.0f;
		style.IndentSpacing								= 21.0f;
		style.ColumnsMinSpacing							= 6.0f;
		style.ScrollbarSize								= 14.0f;
		style.ScrollbarRounding							= 9.0f;
		style.GrabMinSize								= 10.0f;
		style.GrabRounding								= 0.0f;
		style.TabRounding								= 4.0f;
		style.TabBorderSize								= 0.0f;
		style.TabMinWidthForCloseButton					= 0.0f;

		style.Colors[ImGuiCol_Text]						= ImVec4(1.000f, 1.000f, 1.000f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.498f, 0.498f, 0.498f, 1.00f);
		style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.058f, 0.058f, 0.058f, 1.00f); // also sets: Header TabHovered, TabActive, TabUnfocusedActive
		style.Colors[ImGuiCol_ChildBg]					= ImVec4(1.000f, 1.000f, 1.000f, 0.00f);
		style.Colors[ImGuiCol_Border]					= ImVec4(0.427f, 0.427f, 0.498f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.000f, 0.000f, 0.000f, 0.00f);
		style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.200f, 0.207f, 0.219f, 0.54f);
		style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.400f, 0.400f, 0.400f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.176f, 0.176f, 0.176f, 0.67f);
		style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.286f, 0.286f, 0.286f, 1.00f); // also sets: TitleBgActive, TitleBgCollapsed, MenuBarBg, Separator, SeparatorHovered, SeparatorActive
		style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.019f, 0.019f, 0.019f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.309f, 0.309f, 0.309f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.407f, 0.407f, 0.407f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.509f, 0.509f, 0.509f, 1.00f);
		style.Colors[ImGuiCol_CheckMark]				= ImVec4(0.937f, 0.937f, 0.937f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.509f, 0.509f, 0.509f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.858f, 0.858f, 0.858f, 1.00f);
		style.Colors[ImGuiCol_Button]					= ImVec4(0.439f, 0.439f, 0.439f, 0.40f);
		style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.458f, 0.466f, 0.478f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.419f, 0.419f, 0.419f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.909f, 0.909f, 0.909f, 0.25f);
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.807f, 0.807f, 0.807f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.458f, 0.458f, 0.458f, 0.95f);
		style.Colors[ImGuiCol_Tab]						= ImVec4(0.286f, 0.286f, 0.286f, 1.00f); // also sets: TabUnfocused
		style.Colors[ImGuiCol_DockingPreview]			= ImVec4(0.400f, 0.400f, 0.400f, 1.00f);
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.000f, 0.000f, 0.000f, 1.00f);
		style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.607f, 0.607f, 0.607f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.000f, 0.427f, 0.349f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.729f, 0.600f, 0.149f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.000f, 0.600f, 0.000f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.188f, 0.188f, 0.200f, 1.00f);
		style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.309f, 0.309f, 0.349f, 1.00f);
		style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.227f, 0.227f, 0.247f, 1.00f);
		style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.000f, 0.000f, 0.000f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.000f, 1.000f, 1.000f, 0.06f);
		style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.866f, 0.866f, 0.866f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(0.176f, 0.349f, 0.876f, 0.90f);
		style.Colors[ImGuiCol_NavHighlight]				= ImVec4(0.600f, 0.600f, 0.600f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.000f, 1.000f, 1.000f, 0.69f);
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.800f, 0.800f, 0.800f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.800f, 0.800f, 0.800f, 0.35f);
	}

	void Style::SetVisualStudio()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding								= ImVec2(8.0f, 8.0f);
		style.WindowMinSize								= ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign							= ImVec2(0.5f, 0.5f);
		style.FramePadding								= ImVec2(4.0f, 3.0f);
		style.ItemSpacing								= ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing							= ImVec2(4.0f, 4.0f);
		style.CellPadding								= ImVec2(4.0f, 2.0f);
		style.ButtonTextAlign							= ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign						= ImVec2(0.0f, 0.0f);
		style.WindowMenuButtonPosition					= ImGuiDir_Right;
		style.ColorButtonPosition						= ImGuiDir_Right;
		style.Alpha										= 1.0f;
		style.DisabledAlpha								= 0.6f;
		style.WindowRounding							= 0.0f;
		style.WindowBorderSize							= 1.0f;
		style.ChildRounding								= 0.0f;
		style.ChildBorderSize							= 1.0f;
		style.PopupRounding								= 0.0f;
		style.PopupBorderSize							= 1.0f;
		style.FrameRounding								= 0.0f;
		style.FrameBorderSize							= 0.0f;
		style.IndentSpacing								= 21.0f;
		style.ColumnsMinSpacing							= 6.0f;
		style.ScrollbarSize								= 14.0f;
		style.ScrollbarRounding							= 0.0f;
		style.GrabMinSize								= 10.0f;
		style.GrabRounding								= 0.0f;
		style.TabRounding								= 0.0f;
		style.TabBorderSize								= 0.0f;
		style.TabMinWidthForCloseButton					= 0.0f;

		style.Colors[ImGuiCol_Text]						= ImVec4(1.000f, 1.000f, 1.000f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.592f, 0.592f, 0.592f, 1.00f);
		style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.145f, 0.145f, 0.149f, 1.00f); // also sets: Header TabHovered, TabActive, TabUnfocusedActive
		style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.145f, 0.145f, 0.149f, 1.00f);
		style.Colors[ImGuiCol_Border]					= ImVec4(0.305f, 0.305f, 0.305f, 1.00f);
		style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.305f, 0.305f, 0.305f, 1.00f);
		style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.200f, 0.200f, 0.215f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.113f, 0.592f, 0.925f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.000f, 0.466f, 0.784f, 1.00f);
		style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.145f, 0.145f, 0.149f, 1.00f); // also sets: TitleBgActive, TitleBgCollapsed, MenuBarBg, Separator, SeparatorHovered, SeparatorActive
		style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.200f, 0.200f, 0.215f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.321f, 0.321f, 0.333f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.352f, 0.352f, 0.372f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.352f, 0.352f, 0.372f, 1.00f);
		style.Colors[ImGuiCol_CheckMark]				= ImVec4(0.000f, 0.466f, 0.784f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.113f, 0.592f, 0.925f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.000f, 0.466f, 0.784f, 1.00f);
		style.Colors[ImGuiCol_Button]					= ImVec4(0.200f, 0.200f, 0.215f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.113f, 0.592f, 0.925f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.113f, 0.592f, 0.925f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.145f, 0.145f, 0.149f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.200f, 0.200f, 0.215f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.321f, 0.321f, 0.333f, 1.00f);
		style.Colors[ImGuiCol_Tab]						= ImVec4(0.145f, 0.145f, 0.149f, 1.00f); // also sets: TabUnfocused
		style.Colors[ImGuiCol_DockingPreview]			= ImVec4(0.400f, 0.400f, 0.400f, 1.00f);
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.000f, 0.000f, 0.000f, 1.00f);
		style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.000f, 0.466f, 0.784f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(0.113f, 0.592f, 0.925f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.000f, 0.466f, 0.784f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(0.113f, 0.592f, 0.925f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.188f, 0.188f, 0.200f, 1.00f);
		style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.309f, 0.309f, 0.349f, 1.00f);
		style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.227f, 0.227f, 0.247f, 1.00f);
		style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.000f, 0.000f, 0.000f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.000f, 1.000f, 1.000f, 0.05f);
		style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.000f, 0.466f, 0.784f, 1.00f);
		style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(1.000f, 1.000f, 1.000f, 0.90f);
		style.Colors[ImGuiCol_NavHighlight]				= ImVec4(0.145f, 0.145f, 0.149f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.000f, 1.000f, 1.000f, 0.69f);
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.800f, 0.800f, 0.800f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.145f, 0.145f, 0.149f, 1.00f);
	}

	void Style::SetDarkRuda()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding								= ImVec2(8.0f, 8.0f);
		style.WindowMinSize								= ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign							= ImVec2(0.5f, 0.5f);
		style.FramePadding								= ImVec2(4.0f, 3.0f);
		style.ItemSpacing								= ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing							= ImVec2(4.0f, 4.0f);
		style.CellPadding								= ImVec2(4.0f, 2.0f);
		style.ButtonTextAlign							= ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign						= ImVec2(0.0f, 0.0f);
		style.WindowMenuButtonPosition					= ImGuiDir_Right;
		style.ColorButtonPosition						= ImGuiDir_Right;
		style.Alpha										= 1.0f;
		style.DisabledAlpha								= 0.6f;
		style.WindowRounding							= 0.0f;
		style.WindowBorderSize							= 1.0f;
		style.ChildRounding								= 0.0f;
		style.ChildBorderSize							= 1.0f;
		style.PopupRounding								= 0.0f;
		style.PopupBorderSize							= 1.0f;
		style.FrameRounding								= 4.0f;
		style.FrameBorderSize							= 0.0f;
		style.IndentSpacing								= 21.0f;
		style.ColumnsMinSpacing							= 6.0f;
		style.ScrollbarSize								= 14.0f;
		style.ScrollbarRounding							= 9.0f;
		style.GrabMinSize								= 10.0f;
		style.GrabRounding								= 4.0f;
		style.TabRounding								= 4.0f;
		style.TabBorderSize								= 0.0f;
		style.TabMinWidthForCloseButton					= 0.0f;

		style.Colors[ImGuiCol_Text]						= ImVec4(0.949f, 0.956f, 0.976f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.356f, 0.419f, 0.466f, 1.00f);
		style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.109f, 0.149f, 0.168f, 1.00f); // also sets: Header TabHovered, TabActive, TabUnfocusedActive
		style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.149f, 0.176f, 0.219f, 1.00f);
		style.Colors[ImGuiCol_Border]					= ImVec4(0.078f, 0.098f, 0.117f, 1.00f);
		style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.000f, 0.000f, 0.000f, 0.00f);
		style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.200f, 0.247f, 0.286f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.117f, 0.200f, 0.278f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.086f, 0.117f, 0.137f, 1.00f);
		style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.078f, 0.098f, 0.117f, 1.00f); // also sets: TitleBgActive, TitleBgCollapsed, MenuBarBg, Separator, SeparatorHovered, SeparatorActive
		style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.019f, 0.019f, 0.019f, 0.38f);
		style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.200f, 0.247f, 0.286f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.176f, 0.219f, 0.247f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.086f, 0.207f, 0.309f, 1.00f);
		style.Colors[ImGuiCol_CheckMark]				= ImVec4(0.278f, 0.556f, 1.000f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.278f, 0.556f, 1.000f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.368f, 0.607f, 1.000f, 1.00f);
		style.Colors[ImGuiCol_Button]					= ImVec4(0.200f, 0.247f, 0.286f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.278f, 0.556f, 1.000f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.058f, 0.529f, 0.976f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.258f, 0.588f, 0.976f, 0.25f);
		style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.258f, 0.588f, 0.976f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.258f, 0.588f, 0.976f, 0.95f);
		style.Colors[ImGuiCol_Tab]						= ImVec4(0.109f, 0.149f, 0.168f, 1.00f); // also sets: TabUnfocused
		style.Colors[ImGuiCol_DockingPreview]			= ImVec4(0.400f, 0.400f, 0.400f, 1.00f);
		style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.000f, 0.000f, 0.000f, 1.00f);
		style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.607f, 0.607f, 0.607f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.000f, 0.427f, 0.349f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.898f, 0.698f, 0.000f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.000f, 0.600f, 0.000f, 1.00f);
		style.Colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.188f, 0.188f, 0.200f, 1.00f);
		style.Colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.309f, 0.309f, 0.349f, 1.00f);
		style.Colors[ImGuiCol_TableBorderLight]			= ImVec4(0.227f, 0.227f, 0.247f, 1.00f);
		style.Colors[ImGuiCol_TableRowBg]				= ImVec4(0.000f, 0.000f, 0.000f, 0.00f);
		style.Colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.000f, 1.000f, 1.000f, 0.06f);
		style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.258f, 0.588f, 0.976f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(1.000f, 1.000f, 0.000f, 0.89f);
		style.Colors[ImGuiCol_NavHighlight]				= ImVec4(0.258f, 0.588f, 0.976f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.000f, 1.000f, 1.000f, 0.69f);
		style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.800f, 0.800f, 0.800f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.800f, 0.800f, 0.800f, 0.35f);
	}
}
