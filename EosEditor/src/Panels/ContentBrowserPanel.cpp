#include "eospch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace Eos {

	// TODO: Once we have projects, change this to the projects assets directory
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
		{
			if (ImGui::Button("back"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		columnCount = std::max(columnCount, 1);

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : GetFileIcon(directoryEntry.path());
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0, 0, 0, 0 });
			ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t)icon->GetRendererID()), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				auto relativePath = std::filesystem::relative(path, g_AssetPath);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) // double or single click?
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		//ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		//ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar
		ImGui::End();
	}

	Ref<Texture2D>& ContentBrowserPanel::GetFileIcon(const std::filesystem::path& filepath)
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

}
