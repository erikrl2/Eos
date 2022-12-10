#include "EditorLayer.h"

#include "EditorSerializer.h"
#include "Eos/Scene/SceneSerializer.h"

#include "Eos/Utils/PlatformUtils.h"

#include "Eos/Math/Math.h"

#include "Eos/Scripting/ScriptEngine.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ImGuizmo/ImGuizmo.h>

namespace Eos {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		EOS_PROFILE_FUNCTION();

		Style::LoadFonts();
		Style::LoadIcons();
		LoadEditorSettings();

		Application& app = Application::Get();
		app.GetWindow().MaximizeWindow();
		app.GetImGuiLayer()->ConsumeEvents(false);

		bool isProjectLoaded = false;
		auto commandLineArgs = app.GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
			isProjectLoaded = OpenProject(commandLineArgs[1]);
		else
			isProjectLoaded = OpenProject();

		if (!isProjectLoaded)
		{
			Application::Get().Close(); // TODO: Remove
			//NewProject();
		}

		m_MainFramebuffer =	Framebuffer::Create({ 1280, 720, {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth} });
		m_CameraPreviewFramebuffer = Framebuffer::Create({ 1280, 720, {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth} });

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		Renderer2D::SetLineWidth(3.0f);
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
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		if (Eos::FramebufferSpecification spec = m_MainFramebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_MainFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
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
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}
		else m_HoveredEntity = Entity();

		OnOverlayRender();

		m_MainFramebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		EOS_PROFILE_FUNCTION();

		UpdateCursor();

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
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(-1.0f, 5.0f));
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
		m_ContentBrowserPanel->OnImGuiRender();

		ImGui::End();
	}

	void EditorLayer::UI_MenuBar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::BeginMainMenuBar();
		ImGui::PopStyleVar();

		if (ImGui::BeginMenu(ICON_FA_FILE_ALT "  File "))
		{
			if (ImGui::BeginMenu(ICON_FA_PLUS "   New"))
			{
				if (ImGui::MenuItem(ICON_FA_FOLDER "  Project"))
					NewProject();
				if (ImGui::MenuItem(ICON_FA_FILE "   Scene", "Ctrl+N"))
					NewScene();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(ICON_FA_FOLDER_OPEN "  Open.."))
			{
				if (ImGui::MenuItem(ICON_FA_FOLDER "  Project"))
					OpenProject();
				if (ImGui::MenuItem(ICON_FA_FILE "   Scene", "Ctrl+O"))
					OpenScene();
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem(ICON_FA_SAVE "   Save Scene", "Ctrl+S"))
				SaveScene();
			if (ImGui::MenuItem(ICON_FA_SAVE "   Save Scene As..", "Ctrl+Shift+S"))
				SaveSceneAs();
			if (ImGui::MenuItem(ICON_FA_TOOLBOX "  Project Settings"))
			{
				// TODO
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
				Application::Get().Close();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(ICON_FA_FILE_CODE "  Script"))
		{
			if (ImGui::MenuItem(ICON_FA_FILE_UPLOAD "  Reload assembly", "Ctrl+R"))
			{
				if (m_SceneState == SceneState::Edit)
					ScriptEngine::ReloadAssembly();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(ICON_FA_EDIT "  Edit "))
		{
			if (ImGui::BeginMenu(ICON_FA_PALETTE "  Themes"))
			{
				if (ImGui::MenuItem("Eos Dark 1", 0, m_ThemeSelection[Theme_Dark1]))
					SetEditorTheme(Theme_Dark1);
				if (ImGui::MenuItem("Eos Dark 2", 0, m_ThemeSelection[Theme_Dark2]))
					SetEditorTheme(Theme_Dark2);
				if (ImGui::MenuItem("Eos Dark 3", 0, m_ThemeSelection[Theme_Dark3]))
					SetEditorTheme(Theme_Dark3);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu(ICON_FA_FONT "   Fonts"))
			{
				if (ImGui::MenuItem("OpenSans-Regular", 0, m_FontSelection[Font_OpenSansRegular]))
					SetEditorFont(Font_OpenSansRegular);
				if (ImGui::MenuItem("Roboto-Medium", 0, m_FontSelection[Font_RobotoMedium]))
					SetEditorFont(Font_RobotoMedium);
				if (ImGui::MenuItem("Ruda-Regular", 0, m_FontSelection[Font_RudaRegular]))
					SetEditorFont(Font_RudaRegular);
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::BeginMenuBar();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

		static const char* labelPlay = ICON_FA_PLAY "  Play";
		static const char* labelSim = ICON_FA_PLAY_CIRCLE "  Simulate";
		static const char* labelPause = ICON_FA_PAUSE "  Pause";
		static const char* labelStep = ICON_FA_STEP_FORWARD "  Step";

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - 200.0f);

		if (ImGui::Button(labelPlay, ImVec2(60, 0))) // PLay button
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
			labelPause = ICON_FA_PAUSE "  Pause";
		}

		if (ImGui::Button(labelSim, ImVec2(90, 0))) // Simulation button
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
			labelPause = ICON_FA_PAUSE "  Pause";
		}

		bool isPaused = m_ActiveScene->IsPaused();

		ImGui::BeginDisabled(m_SceneState == SceneState::Edit);
		if (ImGui::Button(labelPause, ImVec2(90, 0))) // Pause button
		{
			m_ActiveScene->SetPaused(!isPaused);

			if (isPaused)
				labelPause = ICON_FA_PAUSE "  Pause";
			else
				labelPause = ICON_FA_PLAY "  Continue";
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(!isPaused);
		if (ImGui::Button(labelStep, ImVec2(90, 0))) // Step button
		{
			m_ActiveScene->Step();
		}
		ImGui::EndDisabled();

		ImGui::PopStyleVar();
		ImGui::PopStyleColor(2);

		ImGui::EndMenuBar();
	}

	void EditorLayer::UI_Viewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.5f, 4.5f));
		ImGui::Begin("   " ICON_FA_GLOBE "  Scene   ");
		ImGui::PopStyleVar();
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		m_EditorCamera.SetViewportHovered(m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint64_t textureID = m_MainFramebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				std::filesystem::path path = (const wchar_t*)payload->Data;

				if (path.extension() == ".eos")
				{
					m_HoveredEntity = Entity();
					OpenScene(path);
				}
				else if (path.extension() == ".png" || path.extension() == ".jpeg")
				{
					if (m_HoveredEntity && m_HoveredEntity.HasComponent<SpriteRendererComponent>())
					{
						Ref<Texture2D> texture = Texture2D::Create(path.string());
						if (texture->IsLoaded())
							m_HoveredEntity.GetComponent<SpriteRendererComponent>().Texture = texture;
						else
							EOS_WARN("Could not load texture {0}", path.filename().string());
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
		ImGui::Begin("   " ICON_FA_WRENCH "  Settings   ");
		if (ImGui::Checkbox("Lock camera rotation", &m_LockCameraRotation))
			m_EditorCamera.SetRotationLocked(m_LockCameraRotation);
		ImGui::Checkbox("Show primary camera view", &m_ShowCameraPreview);
		ImGui::ColorEdit3("Selected entity outline color", glm::value_ptr(m_EntityOutlineColor), ImGuiColorEditFlags_NoInputs);
		ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
		ImGui::SameLine();
		ImGui::ColorEdit3("##PhysicsVisualization", glm::value_ptr(m_PhysicsVisualizationColor), ImGuiColorEditFlags_NoInputs);
		ImGui::End();
	}

	void EditorLayer::UI_RendererStats()
	{
		ImGui::Begin("   " ICON_FA_INFO "  Stats   ");

		static float lastFrameTime = 0.0f;
		float time = (float)ImGui::GetTime();
		float delta = time - lastFrameTime;
		lastFrameTime = time;
		ImGui::Text("FPS: %.0f", 1.0f / delta);

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

	static void DrawCustomCursor(Icon cursor)
	{
		Ref<Texture2D> image = Style::GetIconImage(cursor);
		ImVec2 pos = ImGui::GetMousePos();

		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
		ImGui::GetForegroundDrawList()->AddImage(reinterpret_cast<ImTextureID>((uint64_t)image->GetRendererID()),
			pos, ImVec2(pos.x + 24.0f, pos.y + 24.0f), ImVec2(0, 1), ImVec2(1, 0));
	}

	static void DrawCustomCursor(const char* cursor)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
		ImGui::GetForegroundDrawList()->AddText(Style::GetImGuiFont(Font_RobotoMedium), 18.0f, ImGui::GetMousePos(), 0xffffffff, cursor);
	}

	void EditorLayer::UpdateCursor()
	{
		if (!m_EditorCamera.IsViewingToolActive() || !m_ViewportHovered)
			return; // Default cursor is drawn

		switch (m_EditorCamera.GetCameraState())
		{
			case CameraState::Default:
			{
				DrawCustomCursor(ICON_FA_EYE);
				break;
			}
			case CameraState::Pan:
			{
				DrawCustomCursor(Icon_GrabHand);
				break;
			}
			case CameraState::Rotate:
			{
				DrawCustomCursor(ICON_FA_EYE);
				break;
			}
			case CameraState::Zoom:
			{
				DrawCustomCursor(ICON_FA_SEARCH);
				break;
			}
		}
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

		bool editing = m_SceneState == SceneState::Edit;
		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		bool canChangeGizmoType = !ImGuizmo::IsUsing() && editing;

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
					OpenProject();
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
				if (editing && control)
					DuplicateSelectedEntity();
				break;
			}
			case Key::Delete:
			{
				if (editing)
					DeleteSelectedEntity();
				break;
			}

			// Gizmo
			case Key::Q:
			{
				if (canChangeGizmoType)
					m_GizmoType = -1;
				break;
			}
			case Key::W:
			{
				if (canChangeGizmoType)
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::E:
			{
				if (canChangeGizmoType)
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Key::R:
			{
				if (control)
				{
					if (m_SceneState == SceneState::Edit)
						ScriptEngine::ReloadAssembly();
				}
				else
					if (canChangeGizmoType)
						m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
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

		if (m_ShowPhysicsColliders)
		{
			glm::vec3 forwardDir = m_EditorCamera.GetForwardDirection() * glm::vec3(0.001f);

			// Box Colliders
			for (auto [e, tc, bc2d] : m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>().each())
			{
				glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
					* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
					* glm::translate(glm::mat4(1.0f), glm::vec3(bc2d.Offset, -forwardDir.z))
					* glm::scale(glm::mat4(1.0f), scale);

				Renderer2D::DrawRect(transform, m_PhysicsVisualizationColor);
			}

			// Circle Colliders
			for (auto [e, tc, cc2d] : m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>().each())
			{
				glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
					* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
					* glm::translate(glm::mat4(1.0f), glm::vec3(cc2d.Offset, -forwardDir.z))
					* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.x, scale.z));

				Renderer2D::DrawCircle(transform, m_PhysicsVisualizationColor, 0.02f);
			}
		}

		// Selected entity outline 
		if (Entity selection = m_SceneHierarchyPanel.GetSelectedEntity())
		{
			glm::vec3 forwardDir = m_EditorCamera.GetForwardDirection() * glm::vec3(0.002f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -forwardDir.z))
				* selection.GetComponent<TransformComponent>().GetTransform();
			Renderer2D::DrawRect(transform, m_EntityOutlineColor);
		}

		Renderer2D::EndScene();

		// Camera Preview
		if (m_SceneState == SceneState::Edit && m_ShowCameraPreview)
		{
			m_CameraPreviewFramebuffer->Bind();
			RenderCommand::SetClearColor({ 0, 0, 0, 0 });
			RenderCommand::Clear(); // TODO: Fix Program/shader state performance warning

			if (Entity cameraEntity = m_EditorScene->GetPrimaryCameraEntity())
			{
				auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
				camera.SetViewportSize(16, 9);

				m_EditorScene->RenderScene(camera, cameraEntity.GetComponent<TransformComponent>().GetTransform());

				camera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			}

			m_CameraPreviewFramebuffer->Unbind();
		}
	}

	void EditorLayer::NewProject()
	{
		// TODO: Prompt user to select a directory
		//Project::New();
		//NewScene();
		//m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>();
	}

	bool EditorLayer::OpenProject()
	{
		std::string filepath = FileDialogs::OpenFile("Eos Project (*.eosproj)\0*.eosproj\0");
		if (filepath.empty())
			return false;

		return OpenProject(filepath);
	}

	bool EditorLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			auto startScenePath = Project::GetAssetFileSystemPath(Project::GetActive()->GetConfig().StartScene);
			auto assemblyPath = Project::GetAssetFileSystemPath(Project::GetActive()->GetConfig().ScriptModulePath);
			EOS_ASSERT(std::filesystem::exists(startScenePath) && std::filesystem::exists(assemblyPath));

			ScriptEngine::SetNewAppAssembly(assemblyPath);
			OpenScene(startScenePath);
			m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>();
			return true;
		}
		return false;
	}

	void EditorLayer::SaveProject()
	{
		// TODO: Implement
		//Project::SaveActive();
	}

	void EditorLayer::NewScene()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		SetEditorScene(CreateRef<Scene>());
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
		if (!serializer.Deserialize(path))
			return false;

		SetEditorScene(newScene);
		m_EditorScenePath = path;
		return true;
	}

	void EditorLayer::SaveScene()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		if (!m_EditorScenePath.empty())
			SerializeScene(m_EditorScene, m_EditorScenePath);
		else
			SaveSceneAs();
	}

	void EditorLayer::SaveSceneAs()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		std::filesystem::path filepath = FileDialogs::SaveFile("Eos Scene (*.eos)\0*.eos\0");
		if (!filepath.empty())
		{
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

		OnSceneStateChange();
	}

	void EditorLayer::OnSceneSimulate()
	{
		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		m_SceneState = SceneState::Simulate;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();

		OnSceneStateChange();
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

		OnSceneStateChange();
	}

	void EditorLayer::OnScenePause()
	{
		if (m_SceneState == SceneState::Edit)
			return;

		m_ActiveScene->SetPaused(true);
	}

	void EditorLayer::OnSceneStateChange()
	{
		Entity selectedEntityNew;
		if (Entity selectedEntityOld = m_SceneHierarchyPanel.GetSelectedEntity())
			selectedEntityNew = m_ActiveScene->GetEntityByUUID(selectedEntityOld.GetUUID());

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_SceneHierarchyPanel.SetSelectedEntity(selectedEntityNew);
	}

	void EditorLayer::SetEditorScene(const Ref<Scene>& scene)
	{
		EOS_ASSERT(scene, "EditorLayer ActiveScene cannot be null");

		m_EditorScene = scene;
		m_SceneHierarchyPanel.SetContext(m_EditorScene);

		m_ActiveScene = m_EditorScene;

		SyncWindowTitle();
	}

	void EditorLayer::SyncWindowTitle()
	{
		std::string& projectName = Project::GetActive()->GetConfig().Name;
		std::string title = projectName + " - " + m_ActiveScene->GetName() + " - EosEditor";
		Application::Get().GetWindow().SetTitle(title);
	}

	void EditorLayer::SetEditorTheme(Theme newTheme)
	{
		Style::SetTheme(newTheme);
		m_ThemeSelection[m_Settings.Theme] = false;
		m_ThemeSelection[newTheme] = true;
		m_Settings.Theme = newTheme;
	}

	void EditorLayer::SetEditorFont(Font newFont)
	{
		Style::SetFont(newFont);
		m_FontSelection[m_Settings.Font] = false;
		m_FontSelection[newFont] = true;
		m_Settings.Font = newFont;
	}

	void EditorLayer::SaveEditorSettings()
	{
		EditorSerializer serializer(&m_Settings);
		serializer.Serialize("config/user.ini");
	}

	void EditorLayer::LoadEditorSettings()
	{
		if (std::filesystem::exists("config/user.ini"))
		{
			EditorSerializer serializer(&m_Settings);
			serializer.Deserialize("config/user.ini");
		}
		SetEditorTheme(m_Settings.Theme);
		SetEditorFont(m_Settings.Font);

		ImGui::GetIO().IniFilename = "config/eos.ini";
	}

	void EditorLayer::DuplicateSelectedEntity()
	{
		if (Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity())
		{
			Entity duplicate = m_ActiveScene->DuplicateEntity(selectedEntity);
			m_SceneHierarchyPanel.SetSelectedEntity(duplicate);
		}
	}

	void EditorLayer::DeleteSelectedEntity()
	{
		if (Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity())
		{
			m_ActiveScene->DestroyEntity(selectedEntity);
			m_SceneHierarchyPanel.SetSelectedEntity({});
		}
	}

}
