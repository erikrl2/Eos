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
		Ref<Texture2D>& GetFileIcon(const std::filesystem::path& filepath);
		void DrawSearchbar();
	private:
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		std::unordered_map<std::filesystem::path, Ref<Texture2D>> m_ImageIcons;

		char m_SearchBuffer[128];
	};

}
