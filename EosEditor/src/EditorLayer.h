#pragma once

#include <Eos.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Eos/Renderer/EditorCamera.h"

#include "EditorStyle.h"

namespace Eos {

	struct EditorSettings
	{
		Style::Theme Theme = Style::Theme::Dark1;
		Style::Font Font = Style::Font::RobotoMedium;
	};

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);

		bool CanSelectEntity();

		void OnOverlayRender();

		void NewScene();
		void OpenScene();
		bool OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneSimulate();
		void OnSceneStop();

		void SetEditorScene(const Ref<Scene>& scene);
		void SyncWindowTitle();

		void SetEditorTheme(Style::Theme theme);
		void SetEditorFont(Style::Font font);

		void SaveEditorSettings();
		void LoadEditorSettings();

		void DuplicateEntity();

		// UI Panels
		void UI_MenuBar();
		void UI_Toolbar();
		void UI_Viewport();
		void UI_Gizmos();
		void UI_Settings();
		void UI_RendererStats();
	private:
		EditorCamera m_EditorCamera;

		Ref<Framebuffer> m_MainFramebuffer;
		Ref<Framebuffer> m_CameraPreviewFramebuffer;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;
		Entity m_HoveredEntity;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2] = {};

		int m_GizmoType = -1;

		glm::vec4 m_EntityOutlineColor = { 1, 1, 1, 1 };
		bool m_ShowPhysicsColliders = true;
		glm::vec4 m_PhysicsVisualizationColor = { 0, 1, 0, 1 };

		bool m_ShowCameraPreview = false;

		enum class SceneState
		{
			Edit = 0, Play = 1, Simulate = 2
		};
		SceneState m_SceneState = SceneState::Edit;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		// Settings
		EditorSettings m_Settings;
		bool m_ThemeSelection[16] = {};
		bool m_FontSelection[16] = {};

		const char* m_ImGuiConfigFilepath = "config/eos.ini";
		const char* m_UserConfigFilepath = "config/user.ini";
	};

}
