#include "EditorLayer.h"

#include "EditorSerializer.h"
#include "Eos/Scene/SceneSerializer.h"

#include "Eos/Utils/PlatformUtils.h"

#include "Eos/Math/Math.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ImGuizmo/ImGuizmo.h>

namespace Eos {

	extern const std::filesystem::path g_AssetPath; // Serialize?

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		EOS_PROFILE_FUNCTION();

		Style::LoadFonts();
		LoadEditorSettings();
		ImGui::GetIO().IniFilename = m_ImGuiConfigFilepath;

		Application& app = Application::Get();
		app.GetWindow().MaximizeWindow();
		app.GetImGuiLayer()->BlockEvents(false);

		bool sceneLoaded = false;
		auto commandLineArgs = app.GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
			sceneLoaded = OpenScene(commandLineArgs[1]);

		if (!sceneLoaded)
			NewScene();

		m_MainFramebuffer =	Framebuffer::Create({ 1280, 720, {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth} });
		m_CameraPreviewFramebuffer = Framebuffer::Create({ 1280, 720, {FramebufferTextureFormat::RGBA8,  FramebufferTextureFormat::Depth} });

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
	}

	void EditorLayer::OnDetach()
	{
		EOS_PROFILE_FUNCTION();

		SaveEditorSettings();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		EOS_PROFILE_FUNCTION();

		// Resize
		if (Eos::FramebufferSpecification spec = m_MainFramebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_MainFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Render
		Renderer2D::ResetStats();
		m_MainFramebuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		m_MainFramebuffer->ClearAttachment(1, -1);

		switch (m_SceneState)
		{
			case SceneState::Edit:
			{
				m_EditorCamera.OnUpdate(ts);

				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
				break;
			}
			case SceneState::Simulate:
			{
				m_EditorCamera.OnUpdate(ts);

				m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
				break;
			}
			case SceneState::Play:
			{
				m_ActiveScene->OnUpdateRuntime(ts);
				break;
			}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		my = m_ViewportSize.y - my - 1;
		int mouseX = (int)mx;
		int mouseY = (int)my;
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)m_ViewportSize.x && mouseY < (int)m_ViewportSize.y)
		{
			int pixelData = m_MainFramebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, *m_ActiveScene);
		}
		else m_HoveredEntity = Entity();

		OnOverlayRender();

		m_MainFramebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		EOS_PROFILE_FUNCTION();

		// Dockspace
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
		ImGui::PopStyleVar(3);

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 minWinSize = style.WindowMinSize;
		style.WindowMinSize = { 350.0f, 150.0f };
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12.0f, 5.0f));
		ImGui::DockSpace(ImGui::GetID("MyDockSpace"));
		ImGui::PopStyleVar();
		style.WindowMinSize = minWinSize;

		// Child panels
		UI_MenuBar();
		UI_Toolbar();
		UI_Viewport();
		UI_RendererStats();
		UI_Settings();
		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::End();
	}

	void EditorLayer::UI_MenuBar()
	{
		static bool renamingScene = false;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::BeginMainMenuBar();
		ImGui::PopStyleVar();

		if (ImGui::BeginMenu(ICON_FA_FILE_ALT "  File "))
		{
			if (ImGui::MenuItem(ICON_FA_FILE "   New", "Ctrl+N"))
				NewScene();
			if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN "  Open..", "Ctrl+O"))
				OpenScene();
			if (ImGui::MenuItem(ICON_FA_SAVE "   Save", "Ctrl+S"))
				SaveScene();
			if (ImGui::MenuItem(ICON_FA_SAVE "   Save As..", "Ctrl+Shift+S"))
				SaveSceneAs();

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
				Application::Get().Close();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(ICON_FA_EDIT "  Edit "))
		{
			if (ImGui::MenuItem(ICON_FA_PEN "  Rename Scene"))
				renamingScene = true;

			ImGui::Separator();

			if (ImGui::BeginMenu(ICON_FA_PALETTE "  Themes"))
			{
				if (ImGui::MenuItem("Eos Dark 1", 0, m_ThemeSelection[(int)Style::Theme::Dark1]))
					SetEditorTheme(Style::Theme::Dark1);
				if (ImGui::MenuItem("Eos Dark 2", 0, m_ThemeSelection[(int)Style::Theme::Dark2]))
					SetEditorTheme(Style::Theme::Dark2);
				if (ImGui::MenuItem("Eos Dark 3", 0, m_ThemeSelection[(int)Style::Theme::Dark3]))
					SetEditorTheme(Style::Theme::Dark3);
				if (ImGui::MenuItem("Eos Dark 4", 0, m_ThemeSelection[(int)Style::Theme::Dark4]))
					SetEditorTheme(Style::Theme::Dark4);
				if (ImGui::MenuItem("Eos Dark 5", 0, m_ThemeSelection[(int)Style::Theme::Dark5]))
					SetEditorTheme(Style::Theme::Dark5);
				if (ImGui::MenuItem("Eos Dark 6", 0, m_ThemeSelection[(int)Style::Theme::Dark6]))
					SetEditorTheme(Style::Theme::Dark6);
				if (ImGui::MenuItem("Eos Dark 7", 0, m_ThemeSelection[(int)Style::Theme::Dark7]))
					SetEditorTheme(Style::Theme::Dark7);

				if (ImGui::MenuItem("Unreal", 0, m_ThemeSelection[(int)Style::Theme::Unreal]))
					SetEditorTheme(Style::Theme::Unreal);
				if (ImGui::MenuItem("Visual Studio", 0, m_ThemeSelection[(int)Style::Theme::VisualStudio]))
					SetEditorTheme(Style::Theme::VisualStudio);
				if (ImGui::MenuItem("Photoshop", 0, m_ThemeSelection[(int)Style::Theme::Photoshop]))
					SetEditorTheme(Style::Theme::Photoshop);
				if (ImGui::MenuItem("Sonic Riders", 0, m_ThemeSelection[(int)Style::Theme::SonicRiders]))
					SetEditorTheme(Style::Theme::SonicRiders);
				if (ImGui::MenuItem("Dark Ruda", 0, m_ThemeSelection[(int)Style::Theme::DarkRuda]))
					SetEditorTheme(Style::Theme::DarkRuda);

				if (ImGui::BeginMenu("Dear ImGui"))
				{
					if (ImGui::MenuItem("Classic", 0, m_ThemeSelection[(int)Style::Theme::ImGuiClassic]))
						SetEditorTheme(Style::Theme::ImGuiClassic);
					if (ImGui::MenuItem("Dark", 0, m_ThemeSelection[(int)Style::Theme::ImGuiDark]))
						SetEditorTheme(Style::Theme::ImGuiDark);
					if (ImGui::MenuItem("Light", 0, m_ThemeSelection[(int)Style::Theme::ImGuiLight]))
						SetEditorTheme(Style::Theme::ImGuiLight);
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu(ICON_FA_FONT "   Fonts"))
			{
				if (ImGui::MenuItem("OpenSans-Regular", 0, m_FontSelection[(int)Style::Font::OpenSansRegular]))
					SetEditorFont(Style::Font::OpenSansRegular);
				if (ImGui::MenuItem("Roboto-Medium", 0, m_FontSelection[(int)Style::Font::RobotoMedium]))
					SetEditorFont(Style::Font::RobotoMedium);
				if (ImGui::MenuItem("Ruda-Regular", 0, m_FontSelection[(int)Style::Font::RudaRegular]))
					SetEditorFont(Style::Font::RudaRegular);
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();

		if (renamingScene)
		{
			ImGui::Begin("Rename Scene", &renamingScene, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 6.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));

			char sceneName[64];
			strncpy_s(sceneName, m_EditorScene->GetName().c_str(), sizeof(sceneName));
			if (ImGui::InputText("##RenameInput", sceneName, sizeof(sceneName), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				m_EditorScene->SetName(sceneName);
				SyncWindowTitle();
				renamingScene = false;
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
			ImGui::End();
		}
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::BeginMenuBar();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
		{
			static const char* labelPlay = ICON_FA_PLAY "  Play";
			static const char* labelSim = ICON_FA_PLAY_CIRCLE "  Simulate";

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - 100.0f);
			if (ImGui::Button(labelPlay, ImVec2(60, 0)))
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
				{
					OnScenePlay();
					labelPlay = ICON_FA_STOP "  Stop";
					labelSim = ICON_FA_PLAY_CIRCLE "  Simulate";
				}
				else if (m_SceneState == SceneState::Play)
				{
					OnSceneStop();
					labelPlay = ICON_FA_PLAY "  Play";
				}
			}
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - 20.0f);
			if (ImGui::Button(labelSim, ImVec2(90, 0)))
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
				{
					OnSceneSimulate();
					labelSim = ICON_FA_STOP_CIRCLE "  Stop";
					labelPlay = ICON_FA_PLAY "  Play";
				}
				else if (m_SceneState == SceneState::Simulate)
				{
					OnSceneStop();
					labelSim = ICON_FA_PLAY_CIRCLE "  Simulate";
				}
			}
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(2);

		ImGui::EndMenuBar();
	}

	void EditorLayer::UI_Viewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.5f, 4.5f));
		ImGui::Begin(ICON_FA_GLOBE "  Scene");
		ImGui::PopStyleVar();
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint64_t textureID = m_MainFramebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				std::filesystem::path path = (const wchar_t*)payload->Data;

				if (path.extension() == ".eos")	// Load scene
				{
					m_HoveredEntity = Entity();
					OpenScene(std::filesystem::path(g_AssetPath) / path);
				}
				else if (path.extension() == ".png" || path.extension() == ".jpeg") // Load texture
				{
					if (m_HoveredEntity && m_HoveredEntity.HasComponent<SpriteRendererComponent>())
					{
						std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
							m_HoveredEntity.GetComponent<SpriteRendererComponent>().Texture = texture;
						else
							EOS_WARN("Could not load texture {0}", texturePath.filename().string());
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		UI_Gizmos();

		ImGui::End();

		// Camera preview window
		if (m_SceneState == SceneState::Edit && m_ShowCameraPreview)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 1));
			ImGui::SetNextWindowBgAlpha(0.9f);
			ImGui::SetNextWindowPos(ImVec2(m_ViewportBounds[0].x, m_ViewportBounds[0].y));
			ImGui::Begin("CameraPreview", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoSavedSettings);
			ImGui::PopStyleVar(3);
			ImGui::PopStyleColor();
			uint64_t textureID = m_CameraPreviewFramebuffer->GetColorAttachmentRendererID();
			float width = std::min(m_ViewportSize.y / 3.0f * 1.78f, m_ViewportSize.x / 2.0f);
			float height = std::min(m_ViewportSize.y / 3.0f, m_ViewportSize.x / 2.0f / 1.78f);
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
		}
	}

	void EditorLayer::UI_Gizmos()
	{
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1 && m_SceneState == SceneState::Edit)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Editor Camera
			const glm::mat4 cameraProjection = m_EditorCamera.GetProjection();
			const glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity Transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}
	}

	void EditorLayer::UI_Settings()
	{
		ImGui::Begin(ICON_FA_WRENCH "  Settings");
		ImGui::Checkbox("2D editor camera", &EditorCamera::s_RotationLocked);
		ImGui::Checkbox("Show primary camera view", &m_ShowCameraPreview);
		ImGui::Checkbox("Highlight selection", &m_ShowEntityOutline);
		ImGui::SameLine();
		ImGui::ColorEdit3("##Ouline", glm::value_ptr(m_EntityOutlineColor), ImGuiColorEditFlags_NoInputs);
		ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
		ImGui::SameLine();
		ImGui::ColorEdit3("##PhysicsVisualization", glm::value_ptr(m_PhysicsVisualizationColor), ImGuiColorEditFlags_NoInputs);
		ImGui::End();
	}

	void EditorLayer::UI_RendererStats()
	{
		ImGui::Begin(ICON_FA_INFO "  Stats");

		static float lastFrameTime = 0.0f;
		float time = (float)ImGui::GetTime();
		float delta = time - lastFrameTime;
		lastFrameTime = time;
		ImGui::Text("FPS: %.0f", 1.0f / delta);
		ImGui::Text("Uptime: %ds", (int)time);

		const char* entityTag = "None";
		if (m_HoveredEntity) entityTag = m_HoveredEntity.GetComponent<TagComponent>().Tag.c_str();
		ImGui::Text("Hovered Entity: %s", entityTag);

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		if (m_SceneState != SceneState::Play && m_ViewportHovered)
			m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(EOS_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(EOS_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(EOS_BIND_EVENT_FN(EditorLayer::OnMouseButtonReleased));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.IsRepeat())
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		bool editing = m_ViewportHovered && m_SceneState == SceneState::Edit;
		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();
				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();
				break;
			}
			case Key::S:
			{
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}
				break;
			}

			// Scene Commands
			case Key::D:
			{
				if (m_SceneState == SceneState::Edit)
					DuplicateEntity();
				break;
			}

			// Gizmo
			case Key::Q:
				if (!ImGuizmo::IsUsing() && editing)
					m_GizmoType = -1;
				break;
			case Key::W:
				if (!ImGuizmo::IsUsing() && editing)
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:
				if (!ImGuizmo::IsUsing() && editing)
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:
				if (!ImGuizmo::IsUsing() && editing)
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}
		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::Button0)
		{
			if (CanSelectEntity())
			{
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
				ImGuizmo::Enable(false);
			}

			if (Input::IsKeyPressed(Key::LeftAlt) && m_ViewportHovered && m_SceneState == SceneState::Edit)
				ImGuizmo::Enable(false);
		}
		return false;
	}

	bool EditorLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::Button0)
		{
			ImGuizmo::Enable(true);
		}
		return false;
	}

	bool EditorLayer::CanSelectEntity()
	{
		return m_ViewportHovered && !Input::IsKeyPressed(Key::LeftAlt) && (!ImGuizmo::IsOver() || !m_SceneHierarchyPanel.GetSelectedEntity() || m_GizmoType == -1) && m_HoveredEntity != m_SceneHierarchyPanel.GetSelectedEntity();
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			if (Entity camera = m_ActiveScene->GetPrimaryCameraEntity())
				Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
			else
				return;
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		// Hightlight selected entity
		if (Entity selection = m_SceneHierarchyPanel.GetSelectedEntity(); selection && m_ShowEntityOutline)
		{
			//Renderer2D::SetLineWidth(3.0f);

			if (selection.HasComponent<TransformComponent>())
			{
				float zIndex = 0.001f;
				glm::vec3 cameraForwardDirection = m_EditorCamera.GetForwardDirection();
				glm::vec3 projectionCollider = cameraForwardDirection * glm::vec3(zIndex);

				auto& tc = selection.GetComponent<TransformComponent>();

				if (selection.HasComponent<SpriteRendererComponent>())
				{
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -projectionCollider.z)) * tc.GetTransform();
					Renderer2D::DrawRect(transform, m_EntityOutlineColor);
				}

				if (selection.HasComponent<CircleRendererComponent>())
				{
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(tc.Translation.x, tc.Translation.y, tc.Translation.z - projectionCollider.z))
						* glm::toMat4(glm::quat(tc.Rotation))
						* glm::scale(glm::mat4(1.0f), tc.Scale + 0.03f);
					Renderer2D::DrawCircle(transform, m_EntityOutlineColor, 0.03f);
				}
			}
		}

		if (m_ShowPhysicsColliders)
		{
			//if (Renderer2D::GetLineWidth() != 2.0f)
			//{
			//	Renderer2D::Flush();
			//	Renderer2D::SetLineWidth(2.0f);
			//}

			// Box Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();

				float zIndex = 0.001f;
				glm::vec3 cameraForwardDirection = m_EditorCamera.GetForwardDirection();
				glm::vec3 projectionCollider = cameraForwardDirection * glm::vec3(zIndex);

				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
						* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::translate(glm::mat4(1.0f), glm::vec3(bc2d.Offset, -projectionCollider.z))
						* glm::rotate(glm::mat4(1.0f), bc2d.Rotation, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawRect(transform, m_PhysicsVisualizationColor);
				}
			}

			// Circle Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();

				float zIndex = 0.001f;
				glm::vec3 cameraForwardDirection = m_EditorCamera.GetForwardDirection();
				glm::vec3 projectionCollider = cameraForwardDirection * glm::vec3(zIndex);

				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
						* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::translate(glm::mat4(1.0f), glm::vec3(cc2d.Offset, -projectionCollider.z))
						* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.x, scale.z));

					Renderer2D::DrawCircle(transform, m_PhysicsVisualizationColor, 0.02f);
				}
			}
		}

		Renderer2D::EndScene();

		// Camera Preview
		if (m_SceneState == SceneState::Edit && m_ShowCameraPreview)
		{
			if (Entity ce = m_EditorScene->GetPrimaryCameraEntity(); ce && ce.HasComponent<TransformComponent>())
			{
				auto& camera = ce.GetComponent<CameraComponent>().Camera;
				camera.SetViewportSize(16, 9);

				m_CameraPreviewFramebuffer->Bind();
				RenderCommand::SetClearColor({ 0, 0, 0, 0 });
				RenderCommand::Clear(); // TODO: Fix Program/shader state performance warning
				m_EditorScene->RenderScene(camera, ce.GetComponent<TransformComponent>().GetTransform());
				m_CameraPreviewFramebuffer->Unbind();

				camera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			}
		}
	}

	void EditorLayer::NewScene()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Ref<Scene> newScene = CreateRef<Scene>();
		SetEditorScene(newScene);
		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Eos Scene (*.eos)\0*.eos\0");
		if (!filepath.empty())
			OpenScene(filepath);
	}

	bool EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		if (path.extension().string() != ".eos")
		{
			EOS_WARN("Could not load {0} - not a scene file", path.filename().string());
			return false;
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (!serializer.Deserialize(path.string()))
			return false;

		SetEditorScene(newScene);
		m_EditorScenePath = path;
		return true;
	}

	void EditorLayer::SaveScene()
	{
		if (!m_EditorScenePath.empty())
			SerializeScene(m_EditorScene, m_EditorScenePath);
		else
			SaveSceneAs();
	}

	void EditorLayer::SaveSceneAs()
	{
		std::filesystem::path filepath = FileDialogs::SaveFile("Eos Scene (*.eos)\0*.eos\0");
		if (!filepath.empty())
		{
			if (m_EditorScene->GetName() == "Untitled")
				m_EditorScene->SetName(filepath.stem().string());
			m_EditorScenePath = filepath;

			SerializeScene(m_EditorScene, filepath);
			SyncWindowTitle();
		}
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(path);
	}

	void EditorLayer::OnScenePlay()
	{
		if (m_SceneState == SceneState::Simulate)
			OnSceneStop();

		m_SceneState = SceneState::Play;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneSimulate()
	{
		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		m_SceneState = SceneState::Simulate;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		EOS_CORE_ASSERT(m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate);

		if (m_SceneState == SceneState::Play)
			m_ActiveScene->OnRuntimeStop();
		else if (m_SceneState == SceneState::Simulate)
			m_ActiveScene->OnSimulationStop();

		m_SceneState = SceneState::Edit;

		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::SetEditorScene(const Ref<Scene>& scene)
	{
		EOS_ASSERT(scene, "EditorLayer ActiveScene cannot be null");

		m_EditorScene = scene;
		m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_EditorScene);

		m_ActiveScene = m_EditorScene;

		SyncWindowTitle();
	}

	void EditorLayer::SyncWindowTitle()
	{
		Application::Get().GetWindow().SetTitle("Eos Editor - " + m_EditorScene->GetName());
	}

	void EditorLayer::SetEditorTheme(Style::Theme theme)
	{
		if (!m_ThemeSelection[(int)theme])
		{
			Style::SetTheme(theme);
			m_ThemeSelection[(int)m_Settings.Theme] = false;
			m_ThemeSelection[(int)theme] = true;
			m_Settings.Theme = theme;
		}

		// General color adjustments
		ImVec4* colors = ImGui::GetStyle().Colors;
		ImVec4& bg = colors[ImGuiCol_WindowBg];
		ImVec4 bg_light = ImVec4(bg.x + 0.1f, bg.y + 0.1f, bg.z + 0.1f, 0.9f);
		ImVec4 titleBg = colors[ImGuiCol_TitleBg];

		colors[ImGuiCol_PopupBg]				= bg_light;
		colors[ImGuiCol_HeaderActive]			= bg_light;
		colors[ImGuiCol_HeaderHovered]			= bg;
		colors[ImGuiCol_Header]					= bg;
		colors[ImGuiCol_TitleBgActive]			= titleBg;
		colors[ImGuiCol_TitleBgCollapsed]		= titleBg;
		colors[ImGuiCol_MenuBarBg]				= titleBg;
		colors[ImGuiCol_SeparatorActive]		= titleBg;
		colors[ImGuiCol_SeparatorHovered]		= titleBg;
		colors[ImGuiCol_Separator]				= titleBg;
		colors[ImGuiCol_TabHovered]				= bg;
		colors[ImGuiCol_TabActive]				= bg;
		colors[ImGuiCol_TabUnfocusedActive]		= bg;
		colors[ImGuiCol_TabUnfocused]			= colors[ImGuiCol_Tab];
	}

	void EditorLayer::SetEditorFont(Style::Font font)
	{
		if (!m_FontSelection[(int)font])
		{
			Style::SetDefaultFont(font);
			m_FontSelection[(int)m_Settings.Font] = false;
			m_FontSelection[(int)font] = true;
			m_Settings.Font = font;
		}
	}

	void EditorLayer::SaveEditorSettings()
	{
		EditorSerializer serializer(&m_Settings);
		serializer.Serialize(m_UserConfigFilepath);
	}

	void EditorLayer::LoadEditorSettings()
	{
		if (std::filesystem::exists(m_UserConfigFilepath))
		{
			EditorSerializer serializer(&m_Settings);
			serializer.Deserialize(m_UserConfigFilepath);
		}
		SetEditorTheme(m_Settings.Theme);
		SetEditorFont(m_Settings.Font);
	}

	void EditorLayer::DuplicateEntity()
	{
		//if (m_SceneState != SceneState::Edit)
		//	return;

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
			m_EditorScene->DuplicateEntity(selectedEntity);
	}

}
