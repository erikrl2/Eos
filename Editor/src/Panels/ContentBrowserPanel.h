#pragma once

#include "Eos/Renderer/Texture.h"

#include <filesystem>
#include <unordered_map>

namespace Eos {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		void DrawDirectoryEntry(const std::filesystem::directory_entry& directoryEntry, float thumbnailSize);
		Ref<Texture2D> GetFileIcon(const std::filesystem::path& filepath);

		void DrawSearchbar();
		void DrawClearSearchbarButton();
	private:
		std::filesystem::path m_CurrentDirectory;

		std::unordered_map<std::filesystem::path, Ref<Texture2D>> m_ImageIcons;

		Ref<Texture2D> m_SearchIcon;
		char m_SearchBuffer[128] = {};
	};

}
