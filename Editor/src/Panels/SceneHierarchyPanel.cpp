#include "Panels/SceneHierarchyPanel.h"

#include "EditorStyle.h"

#include "Eos/Scene/Components.h"

#include "Eos/Scripting/ScriptEngine.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

namespace Eos {

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
		ImGui::Begin("   " ICON_FA_LIST "  Hierarachy   ");

		if (m_Context)
		{
			ImVec4& bg = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
			ImVec4 bg_light = ImVec4(bg.x + 0.1f, bg.y + 0.1f, bg.z + 0.1f, 1.0f);

			ImGui::PushStyleColor(ImGuiCol_Header, bg_light);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, bg_light);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
			m_Context->m_Registry.each([&](const auto e) { DrawEntityNode({ e, m_Context.get() }); });
			ImGui::PopStyleColor(2);
			ImGui::PopStyleVar();

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, ImGuiMouseButton_Right, false))
			{
				if (ImGui::MenuItem("Create Empty"))
				{
					m_SelectionContext = m_Context->CreateEntity();
				}
				else if (ImGui::MenuItem("Create Sprite"))
				{
					m_SelectionContext = m_Context->CreateEntity("Sprite");
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
				}
				else if (ImGui::MenuItem("Create Camera"))
				{
					m_SelectionContext = m_Context->CreateEntity("Camera");
					m_SelectionContext.AddComponent<CameraComponent>();
				}

				ImGui::EndPopup();
			}
			ImGui::InvisibleButton("##EmptySpace", ImVec2(1, 50));
		}
		ImGui::End();

		ImGui::Begin("   " ICON_FA_INFO_CIRCLE "  Inspector   ");
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

			if (ImGui::MenuItem("Duplicate"))
				m_SelectionContext = m_Context->DuplicateEntity(entity);

			if (ImGui::MenuItem("Delete"))
				entityDeleted = true;

			ImGui::PopStyleColor();
			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawLabelLeft(const char* label, float nextCursorPosX = 105.0f)
	{
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::SetCursorPosX(nextCursorPosX);
		float itemWidth = ImGui::GetContentRegionAvail().x;
		ImGui::SetNextItemWidth(itemWidth);
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 70.0f)
	{
		ImFont* boldFont = Style::GetImGuiFont(Font_OpenSansBold);

		ImGui::PushID(label.c_str());

		ImGui::Text(label.c_str());
		ImGui::SameLine();
		ImGui::SetCursorPosX(105.0f);

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight, lineHeight };
		float dragFloatWidth = (ImGui::GetContentRegionAvail().x / 3.0f) - lineHeight - 3.0f;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 3));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.82f, 0.16f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.92f, 0.26f, 0.22f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.82f, 0.16f, 0.12f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::SetNextItemWidth(dragFloatWidth);
		ImGui::DragFloat("##X", &values.x, 0.05f, 0.0f, 0.0f, "%.2f");
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
		ImGui::SetNextItemWidth(dragFloatWidth);
		ImGui::DragFloat("##Y", &values.y, 0.05f, 0.0f, 0.0f, "%.2f");
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
		ImGui::SetNextItemWidth(dragFloatWidth);
		ImGui::DragFloat("##Z", &values.z, 0.05f, 0.0f, 0.0f, "%.2f");

		ImGui::PopStyleVar();

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			static char imguiPopupID[64];
			sprintf_s(imguiPopupID, 64, "ComponentSettings%s", typeid(T).name());
			ImGui::PushID(imguiPopupID);

			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.70f);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::Button(ICON_FA_ELLIPSIS_V, ImVec2(lineHeight, lineHeight)))
				ImGui::OpenPopup("ComponentSettings");
			ImGui::PopStyleColor();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				bool enabled = typeid(T) != typeid(TransformComponent);
				if (ImGui::MenuItem("Remove Component", (const char*)0, false, enabled))
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
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

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

		DrawComponent<ScriptComponent>("Script", entity, [entity, scene = m_Context](auto& component) mutable
			{
				bool scriptClassExists = ScriptEngine::EntityClassExists(component.ClassName);

				static char buffer[64];
				strcpy_s(buffer, sizeof(buffer), component.ClassName.c_str());

				if (!scriptClassExists)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));

				if (ImGui::InputText("Class", buffer, sizeof(buffer), ImGuiInputTextFlags_CharsNoBlank))
					component.ClassName = buffer;

				// Fields
				bool sceneRunning = scene->IsRunning();
				if (sceneRunning)
				{
					Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity.GetUUID());
					if (scriptInstance)
					{
						const auto& fields = scriptInstance->GetScriptClass()->GetFields();
						for (const auto& [name, field] : fields)
						{
							if (field.Type == ScriptFieldType::Float)
							{
								float data = scriptInstance->GetFieldValue<float>(name);
								if (ImGui::DragFloat(name.c_str(), &data, 0.1f))
									scriptInstance->SetFieldValue(name, data);
							}
							else if (field.Type == ScriptFieldType::Int)
							{
								int data = scriptInstance->GetFieldValue<int>(name);
								if (ImGui::DragInt(name.c_str(), &data))
									scriptInstance->SetFieldValue(name, data);
							}
						}
					}
				}
				else
				{
					Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(component.ClassName);
					if (entityClass)
					{
						const auto& fields = entityClass->GetFields();

						auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
						for (const auto& [name, field] : fields)
						{
							// Field has been set in editor
							if (entityFields.find(name) != entityFields.end())
							{
								ScriptFieldInstance& scriptField = entityFields.at(name);

								if (field.Type == ScriptFieldType::Float)
								{
									float data = scriptField.GetValue<float>();
									if (ImGui::DragFloat(name.c_str(), &data, 0.1f))
										scriptField.SetValue(data);
								}
								else if (field.Type == ScriptFieldType::Int)
								{
									int data = scriptField.GetValue<int>();
									if (ImGui::DragInt(name.c_str(), &data))
										scriptField.SetValue(data);
								}
							}
							else
							{
								if (field.Type == ScriptFieldType::Float)
								{
									float data = 0.0f;
									if (ImGui::DragFloat(name.c_str(), &data, 0.1f))
									{
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Int)
								{
									int data = 0;
									if (ImGui::DragInt(name.c_str(), &data))
									{
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
								}
							}
						}
					}
				}

				if (!scriptClassExists)
					ImGui::PopStyleColor();
			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [&](auto& component)
			{
				DrawLabelLeft("Color");
				glm::vec4& color = component.Color;
				ImGui::ColorEdit4("##Color", glm::value_ptr(color));

				uint32_t textureID = component.Texture ? component.Texture->GetRendererID() : Style::GetIconImage(Icon_Checkerboard)->GetRendererID();
				ImVec4 tintColor = component.Texture ? ImVec4{ color.x, color.y, color.z, color.a } : ImVec4{ 1, 1, 1, 1 };

				DrawLabelLeft("Sprite");
				ImGui::Image(reinterpret_cast<ImTextureID>((uint64_t)textureID), { 50.0f, 23.0f }, { 0, 0 }, { 1, 1 }, tintColor, { 1, 1, 1, 1 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = path;
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
							component.Texture = texture;
						else
							EOS_WARN("Could not load sprite {0}", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}

				if (component.Texture)
				{
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
					if (ImGui::Button(ICON_FA_TRASH))
						component.Texture = {};
					ImGui::PopStyleColor();

					ImGui::SameLine(0.0f, 30.0f);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);
					ImGui::Text("Flip X/Y");
					ImGui::SameLine();
					ImGui::Checkbox("##FlipX", &component.FlipX);
					ImGui::SameLine();
					ImGui::Checkbox("##FlipY", &component.FlipY);

					DrawLabelLeft("Tiling Factor", 115.0f);
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
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12.0f, 7.0f));
		if (ImGui::Button(ICON_FA_PLUS "  Add Component"))
			ImGui::OpenPopup("AddComponent");
		ImGui::PopStyleVar();

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<TransformComponent>("Transform");
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<ScriptComponent>("Script");
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
