#include "eospch.h"
#include "EditorStyle.h"

namespace Eos {

	void Style::SetTheme(Theme theme)
	{
		switch (theme)
		{
			case Dark1: SetEosDark1(); break;
			case Dark2: SetEosDark2(); break;
			case Dark3: SetEosDark3(); break;
		}
	}

	ImFont* Style::GetImGuiFont(Font font)
	{
		return ImGui::GetIO().Fonts->Fonts[font];
	}

	void Style::SetDefaultFont(Font font)
	{
		ImGui::GetIO().FontDefault = GetImGuiFont(font);
	}

	void Style::LoadFonts()
	{
		LoadFont("assets/fonts/opensans/OpenSans-Bold.ttf", 17.0f);

		LoadFontWithIcons("assets/fonts/opensans/OpenSans-Regular.ttf", 17.0f);
		LoadFontWithIcons("assets/fonts/Roboto/Roboto-Medium.ttf", 16.0f);
		LoadFontWithIcons("assets/fonts/Ruda/Ruda-Regular.ttf", 16.0f);
	}

	void Style::LoadFont(const char* filepath, float fontsize)
	{
		ImGui::GetIO().Fonts->AddFontFromFileTTF(filepath, fontsize);
	}

	void Style::LoadFontWithIcons(const char* filepath, float fontsize)
	{
		LoadFont(filepath, fontsize);

		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/fontawesome/" FONT_ICON_FILE_NAME_FAS, 13.0f, &icons_config, icons_ranges);
	}

	void Style::SetEosDark1()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		style.WindowPadding							= ImVec2(8.0f, 8.0f);
		style.WindowMinSize							= ImVec2(200.0f, 40.0f);
		style.WindowTitleAlign						= ImVec2(0.5f, 0.5f);
		style.FramePadding							= ImVec2(4.0f, 4.0f);
		style.ItemSpacing							= ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing						= ImVec2(4.0f, 4.0f);
		style.CellPadding							= ImVec2(4.0f, 2.0f);
		style.ButtonTextAlign						= ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign					= ImVec2(0.0f, 0.0f);
		style.WindowMenuButtonPosition				= ImGuiDir_None;
		style.ColorButtonPosition					= ImGuiDir_Right;
		style.Alpha									= 1.0f;
		style.DisabledAlpha							= 0.60f;
		style.WindowRounding						= 0.0f;
		style.WindowBorderSize						= 1.0f;
		style.ChildRounding							= 0.0f;
		style.ChildBorderSize						= 1.0f;
		style.PopupRounding							= 0.0f;
		style.PopupBorderSize						= 1.0f;
		style.FrameRounding							= 6.0f;
		style.FrameBorderSize						= 0.0f;
		style.IndentSpacing							= 21.0f;
		style.ColumnsMinSpacing						= 6.0f;
		style.ScrollbarSize							= 13.0f;
		style.ScrollbarRounding						= 12.0f;
		style.GrabMinSize							= 7.0f;
		style.GrabRounding							= 3.0f;
		style.TabRounding							= 4.0f;
		style.TabBorderSize							= 0.0f;
		style.TabMinWidthForCloseButton				= 0.0f;

		colors[ImGuiCol_Text]						= ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_TextDisabled]				= ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_WindowBg]					= ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_PopupBg]					= ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		colors[ImGuiCol_ChildBg]					= ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_Border]						= ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		colors[ImGuiCol_BorderShadow]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg]					= ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_FrameBgHovered]				= ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		colors[ImGuiCol_FrameBgActive]				= ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
		colors[ImGuiCol_TitleBg]					= ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab]				= ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		colors[ImGuiCol_CheckMark]					= ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_SliderGrab]					= ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_Button]						= ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
		colors[ImGuiCol_ButtonHovered]				= ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		colors[ImGuiCol_ButtonActive]				= ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_ResizeGrip]					= ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered]			= ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_Tab]						= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_DockingPreview]				= ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_DockingEmptyBg]				= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotLines]					= ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered]			= ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
		colors[ImGuiCol_PlotHistogram]				= ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(0.68f, 0.68f, 0.68f, 1.00f);
		colors[ImGuiCol_TableHeaderBg]				= ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong]			= ImVec4(0.30f, 0.30f, 0.34f, 1.00f);
		colors[ImGuiCol_TableBorderLight]			= ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
		colors[ImGuiCol_TableRowBg]					= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt]				= ImVec4(1.00f, 1.00f, 1.00f, 0.05f);
		colors[ImGuiCol_TextSelectedBg]				= ImVec4(1.00f, 1.00f, 1.00f, 0.15f);
		colors[ImGuiCol_DragDropTarget]				= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_NavHighlight]				= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight]		= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg]			= ImVec4(0.00f, 0.00f, 0.00f, 0.58f);
		colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.00f, 0.00f, 0.00f, 0.58f);
		colors[ImGuiCol_HeaderActive]				= colors[ImGuiCol_PopupBg];
		colors[ImGuiCol_HeaderHovered]				= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_Header]						= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_TitleBgActive]				= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_TitleBgCollapsed]			= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_MenuBarBg]					= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_SeparatorActive]			= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_SeparatorHovered]			= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_Separator]					= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_TabHovered]					= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_TabActive]					= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_TabUnfocusedActive]			= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_TabUnfocused]				= colors[ImGuiCol_Tab];
	}

	void Style::SetEosDark2()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		style.WindowPadding							= ImVec2(8.00f, 8.00f);
		style.WindowMinSize							= ImVec2(200.0f, 40.0f);
		style.WindowTitleAlign						= ImVec2(0.50f, 0.50f);
		style.FramePadding							= ImVec2(5.00f, 4.00f);
		style.CellPadding							= ImVec2(6.00f, 6.00f);
		style.ItemSpacing							= ImVec2(6.00f, 6.00f);
		style.ItemInnerSpacing						= ImVec2(6.00f, 6.00f);
		style.ButtonTextAlign						= ImVec2(0.50f, 0.50f);
		style.SelectableTextAlign					= ImVec2(0.00f, 0.00f);
		style.WindowMenuButtonPosition				= ImGuiDir_Right;
		style.ColorButtonPosition					= ImGuiDir_Right;
		style.DisabledAlpha							= 0.6f;
		style.Alpha									= 1;
		style.IndentSpacing							= 25;
		style.ColumnsMinSpacing						= 6;
		style.ScrollbarSize							= 15;
		style.GrabMinSize							= 10;
		style.WindowBorderSize						= 1;
		style.ChildBorderSize						= 1;
		style.PopupBorderSize						= 1;
		style.FrameBorderSize						= 0;
		style.TabBorderSize							= 0;
		style.WindowRounding						= 7;
		style.ChildRounding							= 4;
		style.FrameRounding							= 2.3f;
		style.PopupRounding							= 4;
		style.ScrollbarRounding						= 9;
		style.GrabRounding							= 2.3f;
		style.TabMinWidthForCloseButton				= 0;
		style.TabRounding							= 4;

		colors[ImGuiCol_Text]						= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled]				= ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg]					= ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		colors[ImGuiCol_PopupBg]					= ImVec4(0.23f, 0.24f, 0.25f, 1.00f);
		colors[ImGuiCol_ChildBg]					= ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
		colors[ImGuiCol_Border]						= ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg]					= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_FrameBgHovered]				= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_FrameBgActive]				= ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		colors[ImGuiCol_TitleBg]					= ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab]				= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark]					= ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		colors[ImGuiCol_SliderGrab]					= ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
		colors[ImGuiCol_Button]						= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_ButtonHovered]				= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_ButtonActive]				= ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
		colors[ImGuiCol_ResizeGrip]					= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_ResizeGripHovered]			= ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
		colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab]						= ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
		colors[ImGuiCol_DockingPreview]				= ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
		colors[ImGuiCol_DockingEmptyBg]				= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotLines]					= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram]				= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg]				= ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong]			= ImVec4(0.30f, 0.30f, 0.34f, 1.00f);
		colors[ImGuiCol_TableBorderLight]			= ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
		colors[ImGuiCol_TableRowBg]					= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt]				= ImVec4(1.00f, 1.00f, 1.00f, 0.05f);
		colors[ImGuiCol_TextSelectedBg]				= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget]				= ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
		colors[ImGuiCol_NavHighlight]				= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight]		= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg]			= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		colors[ImGuiCol_HeaderActive]				= colors[ImGuiCol_PopupBg];
		colors[ImGuiCol_HeaderHovered]				= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_Header]						= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_TitleBgActive]				= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_TitleBgCollapsed]			= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_MenuBarBg]					= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_SeparatorActive]			= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_SeparatorHovered]			= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_Separator]					= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_TabHovered]					= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_TabActive]					= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_TabUnfocusedActive]			= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_TabUnfocused]				= colors[ImGuiCol_Tab];
	}

	void Style::SetEosDark3()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		style.WindowPadding							= ImVec2(8.0f, 8.0f);
		style.WindowMinSize							= ImVec2(200.0f, 40.0f);
		style.WindowTitleAlign						= ImVec2(0.5f, 0.5f);
		style.FramePadding							= ImVec2(4.0f, 3.0f);
		style.ItemSpacing							= ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing						= ImVec2(4.0f, 4.0f);
		style.CellPadding							= ImVec2(4.0f, 2.0f);
		style.ButtonTextAlign						= ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign					= ImVec2(0.0f, 0.0f);
		style.WindowMenuButtonPosition				= ImGuiDir_Right;
		style.ColorButtonPosition					= ImGuiDir_Right;
		style.Alpha									= 1.0f;
		style.DisabledAlpha							= 0.6f;
		style.WindowRounding						= 0.0f;
		style.WindowBorderSize						= 1.0f;
		style.ChildRounding							= 0.0f;
		style.ChildBorderSize						= 1.0f;
		style.PopupRounding							= 0.0f;
		style.PopupBorderSize						= 1.0f;
		style.FrameRounding							= 0.0f;
		style.FrameBorderSize						= 0.0f;
		style.IndentSpacing							= 21.0f;
		style.ColumnsMinSpacing						= 6.0f;
		style.ScrollbarSize							= 14.0f;
		style.ScrollbarRounding						= 9.0f;
		style.GrabMinSize							= 10.0f;
		style.GrabRounding							= 0.0f;
		style.TabRounding							= 4.0f;
		style.TabBorderSize							= 0.0f;
		style.TabMinWidthForCloseButton				= 0.0f;

		colors[ImGuiCol_Text]						= ImVec4(1.000f, 1.000f, 1.000f, 1.00f);
		colors[ImGuiCol_TextDisabled]				= ImVec4(0.498f, 0.498f, 0.498f, 1.00f);
		colors[ImGuiCol_WindowBg]					= ImVec4(0.058f, 0.058f, 0.058f, 1.00f);
		colors[ImGuiCol_PopupBg]					= ImVec4(0.120f, 0.120f, 0.120f, 1.00f);
		colors[ImGuiCol_ChildBg]					= ImVec4(1.000f, 1.000f, 1.000f, 0.00f);
		colors[ImGuiCol_Border]						= ImVec4(0.427f, 0.427f, 0.498f, 0.50f);
		colors[ImGuiCol_BorderShadow]				= ImVec4(0.000f, 0.000f, 0.000f, 0.00f);
		colors[ImGuiCol_FrameBg]					= ImVec4(0.200f, 0.207f, 0.219f, 0.54f);
		colors[ImGuiCol_FrameBgHovered]				= ImVec4(0.400f, 0.400f, 0.400f, 0.40f);
		colors[ImGuiCol_FrameBgActive]				= ImVec4(0.176f, 0.176f, 0.176f, 0.67f);
		colors[ImGuiCol_TitleBg]					= ImVec4(0.286f, 0.286f, 0.286f, 1.00f);
		colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.019f, 0.019f, 0.019f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab]				= ImVec4(0.309f, 0.309f, 0.309f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.407f, 0.407f, 0.407f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.509f, 0.509f, 0.509f, 1.00f);
		colors[ImGuiCol_CheckMark]					= ImVec4(0.937f, 0.937f, 0.937f, 1.00f);
		colors[ImGuiCol_SliderGrab]					= ImVec4(0.509f, 0.509f, 0.509f, 1.00f);
		colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.858f, 0.858f, 0.858f, 1.00f);
		colors[ImGuiCol_Button]						= ImVec4(0.439f, 0.439f, 0.439f, 0.40f);
		colors[ImGuiCol_ButtonHovered]				= ImVec4(0.458f, 0.466f, 0.478f, 1.00f);
		colors[ImGuiCol_ButtonActive]				= ImVec4(0.419f, 0.419f, 0.419f, 1.00f);
		colors[ImGuiCol_ResizeGrip]					= ImVec4(0.909f, 0.909f, 0.909f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered]			= ImVec4(0.807f, 0.807f, 0.807f, 0.67f);
		colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.458f, 0.458f, 0.458f, 0.95f);
		colors[ImGuiCol_Tab]						= ImVec4(0.286f, 0.286f, 0.286f, 1.00f);
		colors[ImGuiCol_DockingPreview]				= ImVec4(0.400f, 0.400f, 0.400f, 1.00f);
		colors[ImGuiCol_DockingEmptyBg]				= ImVec4(0.000f, 0.000f, 0.000f, 1.00f);
		colors[ImGuiCol_PlotLines]					= ImVec4(0.607f, 0.607f, 0.607f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.000f, 0.427f, 0.349f, 1.00f);
		colors[ImGuiCol_PlotHistogram]				= ImVec4(0.729f, 0.600f, 0.149f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.000f, 0.600f, 0.000f, 1.00f);
		colors[ImGuiCol_TableHeaderBg]				= ImVec4(0.188f, 0.188f, 0.200f, 1.00f);
		colors[ImGuiCol_TableBorderStrong]			= ImVec4(0.309f, 0.309f, 0.349f, 1.00f);
		colors[ImGuiCol_TableBorderLight]			= ImVec4(0.227f, 0.227f, 0.247f, 1.00f);
		colors[ImGuiCol_TableRowBg]					= ImVec4(0.000f, 0.000f, 0.000f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt]				= ImVec4(1.000f, 1.000f, 1.000f, 0.06f);
		colors[ImGuiCol_TextSelectedBg]				= ImVec4(0.866f, 0.866f, 0.866f, 0.35f);
		colors[ImGuiCol_DragDropTarget]				= ImVec4(0.176f, 0.349f, 0.876f, 0.90f);
		colors[ImGuiCol_NavHighlight]				= ImVec4(0.600f, 0.600f, 0.600f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight]		= ImVec4(1.000f, 1.000f, 1.000f, 0.69f);
		colors[ImGuiCol_NavWindowingDimBg]			= ImVec4(0.800f, 0.800f, 0.800f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.800f, 0.800f, 0.800f, 0.35f);
		colors[ImGuiCol_HeaderActive]				= colors[ImGuiCol_PopupBg];
		colors[ImGuiCol_HeaderHovered]				= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_Header]						= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_TitleBgActive]				= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_TitleBgCollapsed]			= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_MenuBarBg]					= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_SeparatorActive]			= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_SeparatorHovered]			= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_Separator]					= colors[ImGuiCol_TitleBg];
		colors[ImGuiCol_TabHovered]					= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_TabActive]					= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_TabUnfocusedActive]			= colors[ImGuiCol_WindowBg];
		colors[ImGuiCol_TabUnfocused]				= colors[ImGuiCol_Tab];
	}

}
