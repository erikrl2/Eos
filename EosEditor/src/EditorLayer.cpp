#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Eos/Scene/SceneSerializer.h"

#include "Eos/Utils/PlatformUtils.h"

#include <ImGuizmo/ImGuizmo.h>

#include "Eos/Math/Math.h"

namespace Eos {

	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
		memset(m_ViewportBounds, 0, sizeof(m_ViewportBounds));
	}

	void EditorLayer::OnAttach()
	{
		EOS_PROFILE_FUNCTION();

		m_IconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
		m_IconSimulate = Texture2D::Create("Resources/Icons/SimulateButton.png");
		m_IconStop = Texture2D::Create("Resources/Icons/StopButton.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		Application::Get().GetImGuiLayer()->BlockEvents(false);

		bool sceneLoaded = false;
		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
			sceneLoaded = OpenScene(commandLineArgs[1]);

		if (!sceneLoaded)
			NewScene();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
	}

	void EditorLayer::OnDetach()
	{
		EOS_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		EOS_PROFILE_FUNCTION();

		// Resize
		if (Eos::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Render
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		m_Framebuffer->ClearAttachment(1, -1);

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
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, *m_ActiveScene);
		}
		else m_HoveredEntity = Entity();

		OnOverlayRender();

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		EOS_PROFILE_FUNCTION();

		// Dockspace
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", 0, window_flags);
		ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 minWinSize = style.WindowMinSize;
		style.WindowMinSize = { 350.0f, 50.0f };

		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
		style.WindowMinSize = minWinSize;

		UI_MenuBar();
		UI_Viewport();
		UI_Toolbar();
		UI_ChildPanels();
		UI_Settings();
		UI_RendererStats();

		ImGui::End();
	}

	void EditorLayer::UI_MenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				// TODO: Implement
				if (ImGui::MenuItem("Shortcuts"))
				{
				}
				if (ImGui::MenuItem("About"))
				{
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::UI_Viewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
		ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});

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
		ImGui::PopStyleVar();
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

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

		ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)m_ActiveScene; // Should always be true

		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
			tintColor.w = 0.4f;

		float size = ImGui::GetContentRegionAvail().y - 4.0f;
		{
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_IconPlay : m_IconStop;
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t)icon->GetRendererID()), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
					OnScenePlay();
				else if (m_SceneState == SceneState::Play)
					OnSceneStop();
			}
		}
		ImGui::SameLine();
		{
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t)icon->GetRendererID()), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
					OnSceneSimulate();
				else if (m_SceneState == SceneState::Simulate)
					OnSceneStop();
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		ImGui::End();
	}

	void EditorLayer::UI_ChildPanels()
	{
		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();
	}

	void EditorLayer::UI_Settings()
	{
		ImGui::Begin("Settings");
		ImGui::Checkbox("2D editor camera", &EditorCamera::s_RotationLocked);
		ImGui::Checkbox("Hightlight selection", &m_ShowSelectionHightlighting);
		ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
		ImGui::End();
	}

	void EditorLayer::UI_RendererStats()
	{
		ImGui::Begin("Stats");
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
		if (m_SceneState == SceneState::Edit && m_ViewportHovered)
			m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(EOS_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(EOS_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(EOS_BIND_EVENT_FN(EditorLayer::OnMouseButtonReleased));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
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
				if (control && editing)
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

		// Entity outline
		Entity selection = m_SceneHierarchyPanel.GetSelectedEntity();
		if (m_ShowSelectionHightlighting && selection)
		{
			Renderer2D::SetLineWidth(4.0f);

			if (selection.HasComponent<TransformComponent>())
			{
				// Calculate z index for translation
				float zIndex = 0.001f;
				glm::vec3 cameraForwardDirection = m_EditorCamera.GetForwardDirection();
				glm::vec3 projectionCollider = cameraForwardDirection * glm::vec3(zIndex);

				auto& tc = selection.GetComponent<TransformComponent>();

				if (selection.HasComponent<SpriteRendererComponent>())
				{
					glm::mat4 transform = tc.GetTransform();
					transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -projectionCollider.z)) * transform;

					Renderer2D::DrawRect(transform, glm::vec4(1, 1, 1, 1));
				}

				if (selection.HasComponent<CircleRendererComponent>())
				{
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), { tc.Translation.x, tc.Translation.y, tc.Translation.z - projectionCollider.z })
						* glm::toMat4(glm::quat(tc.Rotation))
						* glm::scale(glm::mat4(1.0f), tc.Scale + 0.03f);
					Renderer2D::DrawCircle(transform, glm::vec4(1, 1, 1, 1), 0.03f);
				}

				if (selection.HasComponent<CameraComponent>())
				{
					// TODO: Show camera view in corner (create texture?)
				}
			}
		}

		if (m_ShowPhysicsColliders)
		{
			if (Renderer2D::GetLineWidth() != -2.0f)
			{
				Renderer2D::Flush();
				Renderer2D::SetLineWidth(2.0f);
			}

			// Box Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();

				// Calculate z index for translation
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

					Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}

			// Circle Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();

				// Calculate z index for translation
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
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.02f);
				}
			}
		}

		Renderer2D::EndScene();
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

	void EditorLayer::DuplicateEntity()
	{
		//if (m_SceneState != SceneState::Edit)
		//	return;

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
			m_EditorScene->DuplicateEntity(selectedEntity);
	}

}
