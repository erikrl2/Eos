#include "SceneHierarchyPanel.h"

#include "Eos/Scene/Components.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

namespace Eos {

	extern const std::filesystem::path g_AssetPath;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarachy");

		if (m_Context)
		{
			m_Context->m_Registry.each([&](const auto e) { DrawEntityNode({ e, *m_Context }); });

			//if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			//	m_SelectionContext = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				if (ImGui::MenuItem("Create Empty"))
					m_SelectionContext = m_Context->CreateEntity();

				ImGui::EndPopup();
			}
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tagComponent = entity.GetComponent<TagComponent>();
		auto& tag = tagComponent.Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			// TODO: Add move up/down MenuItem

			if (ImGui::MenuItem("Create Empty"))
				m_SelectionContext = m_Context->CreateEntity();

			if (ImGui::MenuItem("Create Empty Child"))
			{
				// TODO: Add child entity
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Rename"))
				tagComponent.renaming = true;

			if (ImGui::MenuItem("Duplicate"))
				m_Context->DuplicateEntity(entity);

			if (ImGui::MenuItem("Delete"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (tagComponent.renaming)
		{
			char buffer[128];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				tag = buffer;

			if ((ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) || ImGui::IsKeyPressed(ImGuiKey_Enter, false))
				tagComponent.renaming = false;
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 75.0f)
	{
		auto& style = ImGui::GetStyle();
		style.FrameRounding = 0.0f;

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.7f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.6f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.6f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.7f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.6f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.6f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.7f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.6f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		style.FrameRounding = 4.0f;
	}

	static void DrawLabelLeft(const char* label, float nextCursorPosX = 110.0f)
	{
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::SetCursorPosX(nextCursorPosX);
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		static Ref<Texture2D> settingsIcon = Texture2D::Create("Resources/Icons/Hierarchy/SettingsIcon.png");

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			static char imguiPopupID[64];
			sprintf_s(imguiPopupID, 64, "ComponentSettings%s", typeid(T).name());
			ImGui::PushID(imguiPopupID);

			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t)settingsIcon->GetRendererID()), { lineHeight, lineHeight }, { 0, 1 }, { 1, 0 }, 0, { 0, 0, 0, 0 }, { 0.8, 0.8, 0.8, 1 }))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}
			ImGui::PopID();

			if (open)
			{
				uiFunction(component);

				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[128];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.c_str());
			ImGui::SetNextItemWidth(140.0f);
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
			ImGui::SameLine();
		}

		std::string uid = std::to_string(entity.GetComponent<IDComponent>().ID);
		char idBuffer[32];
		std::strncpy(idBuffer, uid.c_str(), sizeof(idBuffer));
		ImGui::PushStyleColor(ImGuiCol_Text, { 0.5, 0.5, 0.5, 1 });
		ImGui::Text(idBuffer, 0);
		//ImGui::SetNextItemWidth(150.0f);
		//ImGui::InputText("##UID", idBuffer, sizeof(idBuffer), ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);
		ImGui::PopStyleColor();

		static Ref<Texture2D> addIcon = Texture2D::Create("Resources/Icons/Hierarchy/AddIcon.png");

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0, 0, 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0, 0, 0, 0 });
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 42.0f);
		if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t)addIcon->GetRendererID()), { 23, 23 }, { 0, 1 }, { 1, 0 }, 0))
			ImGui::OpenPopup("AddComponent");
		ImGui::PopStyleColor(3);

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<TransformComponent>("Transform");
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<CircleRendererComponent>("Circle Renderer");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec3Control("Position", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
			});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				auto& camera = component.Camera;

				DrawLabelLeft("Primary");
				ImGui::Checkbox("##Primary", &component.Primary);

				DrawLabelLeft("Projection");
				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("##Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					DrawLabelLeft("Vertical FOV");
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("##VerticalFOV", &perspectiveVerticalFov, 0.1f, 0.0f, 180.0f, "%.1f"))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					DrawLabelLeft("Near");
					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("##Near", &perspectiveNear, 0.1f, 0.0f, 0.0f, "%.1f"))
						camera.SetPerspectiveNearClip(perspectiveNear);

					DrawLabelLeft("Far");
					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("##Far", &perspectiveFar, 0.1f, 0.0f, 0.0f, "%.1f"))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					DrawLabelLeft("Size");
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("##Size", &orthoSize, 0.1f, 0.1f, 10000.0f, "%.1f"))
						camera.SetOrthographicSize(orthoSize);

					DrawLabelLeft("Near");
					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("##Near", &orthoNear, 0.1f, 0.0f, 0.0f, "%.1f"))
						camera.SetOrthographicNearClip(orthoNear);

					DrawLabelLeft("Far");
					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("##Far", &orthoFar, 0.1f, 0.0f, 0.0f, "%.1f"))
						camera.SetOrthographicFarClip(orthoFar);
				}

				//ImGui::Checkbox("##FixedAspectRatio", &component.FixedAspectRatio);
			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [&](auto& component)
			{
				DrawLabelLeft("Color");
				glm::vec4& color = component.Color;
				ImGui::ColorEdit4("##Color", glm::value_ptr(color));

				static Ref<Texture2D> checkerboard = Texture2D::Create("Resources/Icons/Hierarchy/Checkerboard.png");

				uint32_t textureID = component.Texture ? component.Texture->GetRendererID() : checkerboard->GetRendererID();
				ImVec4 tintColor = component.Texture ? ImVec4{ color.x, color.y, color.z, color.a } : ImVec4{ 1, 1, 1, 1 };

				DrawLabelLeft("Texture");
				ImGui::Image(reinterpret_cast<ImTextureID>((uint64_t)textureID), { 50.0f, 23.0f }, { 0, 0 }, { 1, 1 }, tintColor, { 1, 1, 1, 1 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
							component.Texture = texture;
						else
							EOS_WARN("Could not load texture {0}", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}

				if (component.Texture)
				{
					ImGui::SameLine();
					if (ImGui::Button("del"))
						component.Texture = {};
				}

				ImGui::SameLine();
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 95.0f);
				ImGui::Checkbox("Atlas", &component.Atlas);
				if (component.Atlas)
				{
					DrawLabelLeft("Coords");
					if (ImGui::DragFloat2("##Coords", glm::value_ptr(component.Coords), 1.0f, 0.0f, 100.0f, "%.0f"));
					DrawLabelLeft("CellSize");
					if (ImGui::DragFloat2("##CellSize", glm::value_ptr(component.CellSize), 0.01f, 0.0f, 500.0f, "%.2f"));
					DrawLabelLeft("SpriteSize");
					if (ImGui::DragFloat2("##SpriteSize", glm::value_ptr(component.SpriteSize), 1.0f, 1.0f, 100.0f, "%.0f"));
				}

				DrawLabelLeft("Tiling Factor");
				ImGui::DragFloat("##TilingFactor", &component.TilingFactor, 0.01f, 0.0f, 100.0f);
			});

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
			{
				// TODO: DrawLabelLeft("Color");

				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
			});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
			{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							component.Type = (Rigidbody2DComponent::BodyType)i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
			});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
			{
				// TODO: Left aligned label
				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.01f);
				ImGui::DragFloat2("Size", glm::value_ptr(component.Size), 0.01f, 0.01f, 10000.0f);

				float rotation = glm::degrees(component.Rotation);
				ImGui::DragFloat("Rotation", &rotation, 0.1f);
				component.Rotation = glm::radians(rotation);

				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f, 10000.0f);
			});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component)
			{
				// TODO: Left aligned label
				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.01f);
				ImGui::DragFloat("Radius", &component.Radius, 0.01f, 0.01f, 10000.0f);
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f, 10000.0f);
			});
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName)
	{
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

}