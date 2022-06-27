#include "eospch.h"
#include "Panels/ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace Eos {

	// TODO: Once we have projects, change this to the projects assets directory
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
		//m_SearchIcon = Texture2D::Create("Resources/Icons/ContentBrowser/SearchIcon.png");

		std::memset(m_SearchBuffer, 0, sizeof(m_SearchBuffer));
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Browser");

		bool searching = m_SearchBuffer[0];

		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
		{
			if (ImGui::Button("back"))
			{
				if (searching)
					std::memset(m_SearchBuffer, 0, sizeof(m_SearchBuffer));
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
			ImGui::SameLine();
			DrawSearchbar();
		}
		else
			DrawSearchbar();

		static float padding = 16.0f;
		static float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		columnCount = std::max(columnCount, 1);
		
		ImGui::Columns(columnCount, 0, false);

		if (!searching)
		{
			for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
				DrawDirectoryEntry(directoryEntry, thumbnailSize);
		}
		else
		{
			for (auto& directoryEntry : std::filesystem::recursive_directory_iterator(g_AssetPath))
			{
				// Case insensitive check if filename contains the search string
				auto& path = directoryEntry.path();
				std::string filenameString = path.filename().string();
				std::string searchText = m_SearchBuffer;
				std::transform(filenameString.begin(), filenameString.end(), filenameString.begin(), ::tolower);
				std::transform(searchText.begin(), searchText.end(), searchText.begin(), ::tolower);
				if (filenameString.find(searchText) != std::string::npos)
				{
					if (!directoryEntry.is_directory() ? path.extension() != ".ttf" && path.extension() != ".frag" && path.extension() != ".vert" && path.extension() != ".glsl" : true)
						DrawDirectoryEntry(directoryEntry, thumbnailSize);
				}
			}
		}

		ImGui::Columns();

		ImGui::SetNextItemWidth(100.0f);
		ImGui::SetCursorPos({ ImGui::GetWindowWidth() - 120.0f, 31.0f });
		ImGui::SliderFloat("##Size", &thumbnailSize, 40, 170, "%.0f");

		ImGui::End();
	}

	void ContentBrowserPanel::DrawDirectoryEntry(const std::filesystem::directory_entry& directoryEntry, float thumbnailSize)
	{
		const auto& path = directoryEntry.path();
		std::string filenameString = path.filename().string();

		ImGui::PushID(filenameString.c_str());
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : GetFileIcon(directoryEntry.path());
		ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t)icon->GetRendererID()), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
		ImGui::PopStyleColor();

		if (!directoryEntry.is_directory() && ImGui::BeginDragDropSource())
		{
			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			const wchar_t* itemPath = relativePath.c_str();
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			if (directoryEntry.is_directory())
			{
				if (m_SearchBuffer[0])
					std::memset(m_SearchBuffer, 0, sizeof(m_SearchBuffer));
				m_CurrentDirectory = path;
			}
		ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();
		ImGui::PopID();
	}

	Ref<Texture2D> ContentBrowserPanel::GetFileIcon(const std::filesystem::path& filepath)
	{
		if (m_ImageIcons.find(filepath) != m_ImageIcons.end())
			return m_ImageIcons[filepath];

		std::string extention = filepath.extension().string();
		if (extention == ".png" || extention == ".jpg")
		{
			auto imageIcon = Texture2D::Create(filepath.string());
			if (imageIcon->IsLoaded())
			{
				m_ImageIcons[filepath] = imageIcon;
				return imageIcon;
			}
			else
				EOS_WARN("Could not load texture {0}", filepath.filename().string());
		}
		return m_FileIcon;
	}

	void ContentBrowserPanel::DrawSearchbar()
	{
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - 124.0f);
		ImGui::SetNextItemWidth(130.0f);
		ImGui::InputTextWithHint("##Search", "Search...", m_SearchBuffer, sizeof(m_SearchBuffer));
		//ImGui::SameLine(ImGui::GetWindowWidth() / 2.0f + 10.0f, 0);
		//ImVec4& buttonColor = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
		//ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
		//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonColor);
		//ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonColor);
		//ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t)m_SearchIcon->GetRendererID()), { 17, 17 }, { 0, 1 }, { 1, 0 }, -1, { 0, 0, 0, 0 }, { 0.6f, 0.6f, 0.6f, 1.0f });
		//ImGui::PopStyleColor(3);
		DrawClearSearchbarButton();
		ImGui::Separator();
	}

	void ContentBrowserPanel::DrawClearSearchbarButton()
	{
		if (m_SearchBuffer[0])
		{
			ImGui::SameLine();
			if (ImGui::Button("clear"))
				std::memset(m_SearchBuffer, 0, sizeof(m_SearchBuffer));
		}
	}

}
