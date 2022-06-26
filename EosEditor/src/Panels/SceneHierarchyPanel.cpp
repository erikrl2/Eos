#include "SceneHierarchyPanel.h"

#include "../EditorStyle.h"

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
		ImGui::Begin("Hierarachy");

		if (m_Context)
		{
			ImVec4& bg = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
			ImVec4 bg_light = ImVec4(bg.x + 0.1f, bg.y + 0.1f, bg.z + 0.1f, 1.0f);

			ImGui::PushStyleColor(ImGuiCol_Header, bg_light);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, bg_light);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
			m_Context->m_Registry.each([&](const auto e) { DrawEntityNode({ e, *m_Context }); });
			ImGui::PopStyleColor(2);
			ImGui::PopStyleVar();

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

		ImGui::Begin("Inspector");
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
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);

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

			ImGui::PopStyleColor();
			ImGui::EndPopup();
		}

		if (tagComponent.renaming)
		{
			char buffer[128];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, tag.c_str(), sizeof(buffer));
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

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 70.0f)
	{
		ImFont* boldFont = Style::GetImGuiFont(Style::Font::OpenSansBold);

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 3));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.82f, 0.16f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.92f, 0.26f, 0.22f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.82f, 0.16f, 0.12f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.60f, 0.58f, 0.18f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.70f, 0.68f, 0.28f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.60f, 0.58f, 0.18f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.52f, 0.53f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.62f, 0.63f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.52f, 0.53f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	static void DrawLabelLeft(const char* label, float nextCursorPosX = 105.0f)
	{
		//ImGui::Separator();
		//ImGui::SetCursorPosX(20.0f);
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::SetCursorPosX(nextCursorPosX);
		//float itemWidth = std::min(ImGui::GetContentRegionAvail().x - 2.0f, 355.0f - nextCursorPosX);
		float itemWidth = ImGui::GetContentRegionAvail().x;
		ImGui::SetNextItemWidth(itemWidth);
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

			static ImVec4 iconTintColor = { 0.6f, 0.6f, 0.6f, 1.0f };
			ImVec4& buttonColor = ImGui::GetStyle().Colors[ImGuiCol_Header]; // TODO: change to bg color
			ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonColor);
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t)settingsIcon->GetRendererID()), { lineHeight, lineHeight }, { 0, 1 }, { 1, 0 }, 0, { 0, 0, 0, 0 }, iconTintColor))
				ImGui::OpenPopup("ComponentSettings");
			iconTintColor = ImGui::IsItemHovered() ? ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f } : ImVec4{ 0.6f, 0.6f, 0.6f, 1.0f };
			ImGui::PopStyleColor(3);

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
			//ImGui::SetNextItemWidth(std::min(ImGui::GetWindowWidth() - 140.0f, 210.0f));
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		/*
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 127.0f);
		//static Ref<Texture2D> addIcon = Texture2D::Create("Resources/Icons/Hierarchy/AddIcon.png");
		//if (ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t)addIcon->GetRendererID()), { 17, 17 }, { 0, 1 }, { 1, 0 }, -1, { 0, 0, 0, 0 }, { 0.7f, 0.7f, 0.7f, 1.0f }))
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

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
		*/

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

				DrawLabelLeft("Fixed AspectRatio", 150.0f);
				ImGui::Checkbox("##FixedAspectRatio", &component.FixedAspectRatio);
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
					if (ImGui::BeginPopupContextItem("TexRemove"))
					{
						if (ImGui::MenuItem("Remove"))
							component.Texture = {};
						ImGui::EndPopup();
					}

					ImGui::SameLine(0.0f, 30.0f);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);
					ImGui::Text("Flip X/Y");
					ImGui::SameLine();
					ImGui::Checkbox("##FlipX", &component.FlipX);
					ImGui::SameLine();
					ImGui::Checkbox("##FlipY", &component.FlipY);

					DrawLabelLeft("Tiling Factor");
					ImGui::DragFloat("##TilingFactor", &component.TilingFactor, 0.01f, 0.0f, 100.0f, "%.2f");

					DrawLabelLeft("Is Atlas");
					ImGui::Checkbox("##Atlas", &component.Atlas);
					if (component.Atlas)
					{
						DrawLabelLeft("Coords");
						ImGui::DragFloat2("##Coords", glm::value_ptr(component.Coords), 1.0f, 0.0f, 100.0f, "%.0f");
						DrawLabelLeft("CellSize");
						ImGui::DragFloat2("##CellSize", glm::value_ptr(component.CellSize), 0.01f, 0.0f, 500.0f, "%.2f");
						DrawLabelLeft("SpriteSize");
						ImGui::DragFloat2("##SpriteSize", glm::value_ptr(component.SpriteSize), 1.0f, 1.0f, 100.0f, "%.0f");
					}
				}
			});

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
			{
				DrawLabelLeft("Color");
				ImGui::ColorEdit4("##Color", glm::value_ptr(component.Color));
				DrawLabelLeft("Thickness");
				ImGui::DragFloat("##Thickness", &component.Thickness, 0.01f, 0.0f, 1.0f, "%.2f");
				DrawLabelLeft("Fade");
				ImGui::DragFloat("##Fade", &component.Fade, 0.001f, 0.0f, 1.0f, "%.3f");
			});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
			{
				DrawLabelLeft("Body Type");
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
				if (ImGui::BeginCombo("##BodyType", currentBodyTypeString))
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

				DrawLabelLeft("Fixed Rotation", 130.0f);
				ImGui::Checkbox("##FixedRotation", &component.FixedRotation);
			});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
			{
				DrawLabelLeft("Offset");
				ImGui::DragFloat2("##Offset", glm::value_ptr(component.Offset), 0.01f, 0.0f, 0.0f, "%.2f");
				DrawLabelLeft("Size");
				ImGui::DragFloat2("##Size", glm::value_ptr(component.Size), 0.01f, 0.01f, 10000.0f, "%.2f");

				DrawLabelLeft("Rotation");
				float rotation = glm::degrees(component.Rotation);
				ImGui::DragFloat("##Rotation", &rotation, 0.1f, 0.0f, 0.0f, "%.2f");
				component.Rotation = glm::radians(rotation);

				DrawLabelLeft("Density");
				ImGui::DragFloat("##Density", &component.Density, 0.005f, 0.0f, 1.0f, "%.3f");
				DrawLabelLeft("Friction");
				ImGui::DragFloat("##Friction", &component.Friction, 0.005f, 0.0f, 1.0f, "%.3f");
				DrawLabelLeft("Restitution");
				ImGui::DragFloat("##Restitution", &component.Restitution, 0.005f, 0.0f, 1.0f, "%.3f");
				DrawLabelLeft("Restitution Threshold", 173.0f);
				ImGui::DragFloat("##RestitutionThreshold", &component.RestitutionThreshold, 0.01f, 0.0f, 10000.0f, "%.2f");
			});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component)
			{
				DrawLabelLeft("Offset");
				ImGui::DragFloat2("##Offset", glm::value_ptr(component.Offset), 0.01f, 0.0f, 0.0f, "%.2f");
				DrawLabelLeft("Radius");
				ImGui::DragFloat("##Radius", &component.Radius, 0.005f, 0.01f, 10000.0f, "%.2f");
				DrawLabelLeft("Density");
				ImGui::DragFloat("##Density", &component.Density, 0.005f, 0.0f, 1.0f, "%.3f");
				DrawLabelLeft("Friction");
				ImGui::DragFloat("##Friction", &component.Friction, 0.005f, 0.0f, 1.0f, "%.3f");
				DrawLabelLeft("Restitution");
				ImGui::DragFloat("##Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f, "%.3f");
				DrawLabelLeft("Restitution Threshold", 173.0f);
				ImGui::DragFloat("##RestitutionThreshold", &component.RestitutionThreshold, 0.01f, 0.0f, 10000.0f, "%.2f");
			});

		// Add new components here


		ImGui::Separator();
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2.0f - 65.0f, ImGui::GetCursorPos().y + 5.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 6.0f));
		if (ImGui::Button("Add Component", ImVec2(130.0f, 0.0f)))
			ImGui::OpenPopup("AddComponent");
		ImGui::PopStyleVar();

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

		ImGui::InvisibleButton("##EmptySpace", ImVec2(1, 50));
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
