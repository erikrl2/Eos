#pragma once

#include <Eos.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Eos/Renderer/EditorCamera.h"

#include "EditorStyle.h"

namespace Eos {

	struct EditorSettings
	{
		Theme Theme = Theme_Dark1;
		Font Font = Font_RobotoMedium;
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

		void NewProject();
		bool OpenProject();
		bool OpenProject(const std::filesystem::path& path);
		void SaveProject();

		void NewScene();
		void OpenScene();
		bool OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneSimulate();
		void OnSceneStop();
		void OnScenePause();

		void OnSceneStateChange();

		void SetEditorScene(const Ref<Scene>& scene);
		void SyncWindowTitle();

		void SetEditorTheme(Theme newTheme);
		void SetEditorFont(Font newFont);

		void SaveEditorSettings();
		void LoadEditorSettings();

		void DuplicateSelectedEntity();
		void DeleteSelectedEntity();

		void UI_MenuBar();
		void UI_Toolbar();
		void UI_Viewport();
		void UI_Gizmos();
		void UI_Settings();
		void UI_RendererStats();

		void UpdateCursor();
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

		bool m_LockCameraRotation = false;
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
		Scope<ContentBrowserPanel> m_ContentBrowserPanel;

		// Settings
		EditorSettings m_Settings;
		bool m_ThemeSelection[3] = {};
		bool m_FontSelection[5] = {};
	};

}
