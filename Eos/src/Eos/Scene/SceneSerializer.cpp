#include "eospch.h"
#include "Eos/Scene/SceneSerializer.h"

#include "Eos/Scene/Entity.h"
#include "Eos/Scripting/ScriptEngine.h"

#include "Eos/Project/Project.h"

#include <fstream>

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Eos::UUID>
	{
		static Node encode(const Eos::UUID& uuid)
		{
			Node node;
			node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, Eos::UUID& uuid)
		{
			uuid = node.as<uint64_t>();
			return true;
		}
	};

}

namespace Eos {

#define WRITE_SCRIPT_FIELD(FieldType, Type)   \
	case ScriptFieldType::FieldType:          \
		out << scriptField.GetValue<Type>();  \
		break

#define READ_SCRIPT_FIELD(FieldType, Type)            \
	case ScriptFieldType::FieldType:                  \
	{                                                 \
		Type data = scriptField["Data"].as<Type>();   \
		fieldInstance.SetValue(data);                 \
		break;                                        \
	}

	static void SerializeAllEntityComponents(YAML::Emitter& out, Entity entity);

	static void DeserializeAllEntityComponents(YAML::detail::iterator_value& entity, Entity& deserializedEntity);

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static std::string RigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static:    return "Static";
			case Rigidbody2DComponent::BodyType::Dynamic:   return "Dynamic";
			case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		EOS_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")    return Rigidbody2DComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")   return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

		EOS_CORE_ASSERT(false, "Unknown body type");
		return Rigidbody2DComponent::BodyType::Static;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		EOS_CORE_ASSERT(entity.HasComponent<IDComponent>());

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<IDComponent>().ID;

		SerializeAllEntityComponents(out, entity); // Serialize components (except IDComponent)

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		auto idView = m_Scene->m_Registry.view<IDComponent>();
		for (auto it = idView.rbegin(); it != idView.rend(); it++)
		{
			Entity entity = { *it, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::filesystem::path& filepath)
	{
		EOS_CORE_ASSERT(false, "Not implemented");
	}

	bool SceneSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::ParserException e)
		{
			EOS_CORE_ERROR("Failed to load .eos file '{0}'\n	{1}", filepath, e.what());
			return false;
		}
		
		std::string sceneName = filepath.stem().string();
		EOS_CORE_TRACE("Deserializing scene '{0}'", sceneName);
		m_Scene->SetName(sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

				DeserializeAllEntityComponents(entity, deserializedEntity); // Deserialize components (except IDComponent and TagComponent)
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::filesystem::path& filepath)
	{
		// Not implemented
		EOS_CORE_ASSERT(false);
		return false;
	}

	template<typename Component>
	static void SerializeEntityComponent(YAML::Emitter&, Entity) = delete;

	template<typename Component>
	void DeserializeEntityComponent(YAML::detail::iterator_value& entity, Entity&) = delete;

	template<>
	static void SerializeEntityComponent<TagComponent>(YAML::Emitter& out, Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}
	}

	template<>
	static void SerializeEntityComponent<TransformComponent>(YAML::Emitter& out, Entity entity)
	{
		out << YAML::Key << "TransformComponent";
		out << YAML::BeginMap;

		auto& tc = entity.GetComponent<TransformComponent>();
		out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
		out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
		out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

		out << YAML::EndMap;
	}

	template<>
	static void DeserializeEntityComponent<TransformComponent>(YAML::detail::iterator_value& entity, Entity& deserializedEntity)
	{
		auto transformComponent = entity["TransformComponent"];
		if (transformComponent)
		{
			auto& tc = deserializedEntity.GetComponent<TransformComponent>();
			tc.Translation = transformComponent["Translation"].as<glm::vec3>();
			tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
			tc.Scale = transformComponent["Scale"].as<glm::vec3>();
		}
	}

	template<>
	static void SerializeEntityComponent<CameraComponent>(YAML::Emitter& out, Entity entity)
	{
		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap;
		}
	}

	template<>
	static void SerializeEntityComponent<ScriptComponent>(YAML::Emitter& out, Entity entity)
	{
		if (entity.HasComponent<ScriptComponent>())
		{
			auto& scriptComponent = entity.GetComponent<ScriptComponent>();

			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "ClassName" << YAML::Value << scriptComponent.ClassName;

			// Fields
			Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(scriptComponent.ClassName);
			const auto& fields = entityClass->GetFields();
			if (fields.size() > 0)
			{
				out << YAML::Key << "ScriptFields" << YAML::Value;
				auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
				out << YAML::BeginSeq;
				for (const auto& [name, field] : fields)
				{
					if (entityFields.find(name) == entityFields.end())
						continue;

					out << YAML::BeginMap;
					out << YAML::Key << "Name" << YAML::Value << name;
					out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.Type);

					out << YAML::Key << "Data" << YAML::Value;
					ScriptFieldInstance& scriptField = entityFields.at(name);

					switch (field.Type)
					{
						WRITE_SCRIPT_FIELD(Float, float);
						WRITE_SCRIPT_FIELD(Double, double);
						WRITE_SCRIPT_FIELD(Bool, bool);
						WRITE_SCRIPT_FIELD(Char, char);
						WRITE_SCRIPT_FIELD(Byte, int8_t);
						WRITE_SCRIPT_FIELD(Short, int16_t);
						WRITE_SCRIPT_FIELD(Int, int32_t);
						WRITE_SCRIPT_FIELD(Long, int64_t);
						WRITE_SCRIPT_FIELD(UByte, uint8_t);
						WRITE_SCRIPT_FIELD(UShort, uint16_t);
						WRITE_SCRIPT_FIELD(UInt, uint32_t);
						WRITE_SCRIPT_FIELD(ULong, uint64_t);
						WRITE_SCRIPT_FIELD(Vector2, glm::vec2);
						WRITE_SCRIPT_FIELD(Vector3, glm::vec3);
						WRITE_SCRIPT_FIELD(Vector4, glm::vec4);
						WRITE_SCRIPT_FIELD(Entity, UUID);
					}
					out << YAML::EndMap;
				}
				out << YAML::EndSeq;
			}

			out << YAML::EndMap;
		}
	}

	template<>
	static void DeserializeEntityComponent<ScriptComponent>(YAML::detail::iterator_value& entity, Entity& deserializedEntity)
	{
		auto scriptComponent = entity["ScriptComponent"];
		if (scriptComponent)
		{
			auto& sc = deserializedEntity.AddComponent<ScriptComponent>().GetComponent<ScriptComponent>();
			sc.ClassName = scriptComponent["ClassName"].as<std::string>();

			if (auto scriptFields = scriptComponent["ScriptFields"])
			{
				if (Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(sc.ClassName))
				{
					const auto& fields = entityClass->GetFields();
					auto& entityFields = ScriptEngine::GetScriptFieldMap(deserializedEntity);

					for (auto scriptField : scriptFields)
					{
						std::string name = scriptField["Name"].as<std::string>();
						std::string typeString = scriptField["Type"].as<std::string>();
						ScriptFieldType type = Utils::ScriptFieldTypeFromString(typeString);

						ScriptFieldInstance& fieldInstance = entityFields[name];

						// TODO: turn this assert into log warning
						// EOS_CORE_ASSERT(fields.find(name) != fields.end());

						if (fields.find(name) == fields.end())
							continue;

						fieldInstance.Field = fields.at(name);

						switch (type)
						{
							READ_SCRIPT_FIELD(Float, float);
							READ_SCRIPT_FIELD(Double, double);
							READ_SCRIPT_FIELD(Bool, bool);
							READ_SCRIPT_FIELD(Char, char);
							READ_SCRIPT_FIELD(Byte, int8_t);
							READ_SCRIPT_FIELD(Short, int16_t);
							READ_SCRIPT_FIELD(Int, int32_t);
							READ_SCRIPT_FIELD(Long, int64_t);
							READ_SCRIPT_FIELD(UByte, uint8_t);
							READ_SCRIPT_FIELD(UShort, uint16_t);
							READ_SCRIPT_FIELD(UInt, uint32_t);
							READ_SCRIPT_FIELD(ULong, uint64_t);
							READ_SCRIPT_FIELD(Vector2, glm::vec2);
							READ_SCRIPT_FIELD(Vector3, glm::vec3);
							READ_SCRIPT_FIELD(Vector4, glm::vec4);
							READ_SCRIPT_FIELD(Entity, UUID);
						}
					}
				}
			}
		}
	}

	template<>
	static void DeserializeEntityComponent<CameraComponent>(YAML::detail::iterator_value& entity, Entity& deserializedEntity)
	{
		auto cameraComponent = entity["CameraComponent"];
		if (cameraComponent)
		{
			auto& cc = deserializedEntity.AddComponent<CameraComponent>().GetComponent<CameraComponent>();
			
			auto cameraProps = cameraComponent["Camera"];
			cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
			cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
			cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
			cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

			cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
			cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
			cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

			cc.Primary = cameraComponent["Primary"].as<bool>();
			cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
		}
	}

	template<>
	static void SerializeEntityComponent<SpriteRendererComponent>(YAML::Emitter& out, Entity entity)
	{
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& src = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << src.Color;
			if (auto& texture = src.Texture)
			{
				std::string texturePath = std::filesystem::relative(texture->GetPath(), Project::GetAssetDirectory()).string();
				out << YAML::Key << "TexturePath" << YAML::Value << texturePath;
				out << YAML::Key << "FlipXY" << YAML::Value << glm::vec2(src.FlipX, src.FlipY); // TEMP
				out << YAML::Key << "TilingFactor" << YAML::Value << src.TilingFactor;

				// TODO: Refactor
				out << YAML::Key << "Atlas" << YAML::Value << src.Atlas;
				if (src.Atlas)
				{
					out << YAML::Key << "Coords" << YAML::Value << src.Coords;
					out << YAML::Key << "CellSize" << YAML::Value << src.CellSize;
					out << YAML::Key << "SpriteSize" << YAML::Value << src.SpriteSize;
				}
			}

			out << YAML::EndMap;
		}
	}

	template<>
	static void DeserializeEntityComponent<SpriteRendererComponent>(YAML::detail::iterator_value& entity, Entity& deserializedEntity)
	{
		auto spriteRendererComponent = entity["SpriteRendererComponent"];
		if (spriteRendererComponent)
		{
			auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>().GetComponent<SpriteRendererComponent>();
			src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
			if (auto texture = spriteRendererComponent["TexturePath"])
			{
				std::string textureFilePath = texture.as<std::string>();
				textureFilePath = Project::GetAssetFileSystemPath(textureFilePath).string();
				if (std::filesystem::exists(textureFilePath))
				{
					src.Texture = Texture2D::Create(textureFilePath);
					glm::vec2 flipXY = spriteRendererComponent["FlipXY"].as<glm::vec2>(); // TODO: Refactor
					src.FlipX = (bool)flipXY.x;
					src.FlipY = (bool)flipXY.y;
					src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
					// ------ TEMP -------
					if (src.Atlas = spriteRendererComponent["Atlas"].as<bool>())
					{
						src.Coords = spriteRendererComponent["Coords"].as<glm::vec2>();
						src.CellSize = spriteRendererComponent["CellSize"].as<glm::vec2>();
						src.SpriteSize = spriteRendererComponent["SpriteSize"].as<glm::vec2>();
					}
					// ------ TEMP -------
				}
				else
					EOS_CORE_ERROR("Texture '{0}' was not found", textureFilePath);
			}
		}
	}

	template<>
	static void SerializeEntityComponent<CircleRendererComponent>(YAML::Emitter& out, Entity entity)
	{
		if (entity.HasComponent<CircleRendererComponent>())
		{
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap;

			auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
			out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;

			out << YAML::EndMap;
		}
	}

	template<>
	static void DeserializeEntityComponent<CircleRendererComponent>(YAML::detail::iterator_value& entity, Entity& deserializedEntity)
	{
		auto circleRendererComponent = entity["CircleRendererComponent"];
		if (circleRendererComponent)
		{
			auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>().GetComponent<CircleRendererComponent>();
			crc.Color = circleRendererComponent["Color"].as<glm::vec4>();
			crc.Thickness = circleRendererComponent["Thickness"].as<float>();
			crc.Fade = circleRendererComponent["Fade"].as<float>();
		}
	}

	template<>
	static void SerializeEntityComponent<Rigidbody2DComponent>(YAML::Emitter& out, Entity entity)
	{
		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap;

			auto& rb2dComponent = entity.GetComponent<Rigidbody2DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(rb2dComponent.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2dComponent.FixedRotation;

			out << YAML::EndMap;
		}
	}

	template<>
	static void DeserializeEntityComponent<Rigidbody2DComponent>(YAML::detail::iterator_value& entity, Entity& deserializedEntity)
	{
		auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
		if (rigidbody2DComponent)
		{
			auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>().GetComponent<Rigidbody2DComponent>();
			rb2d.Type = RigidBody2DBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
			rb2d.FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
		}
	}

	template<>
	static void SerializeEntityComponent<BoxCollider2DComponent>(YAML::Emitter& out, Entity entity)
	{
		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;

			auto& bc2dComponent = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << bc2dComponent.Offset;
			out << YAML::Key << "Size" << YAML::Value << bc2dComponent.Size;
			out << YAML::Key << "Density" << YAML::Value << bc2dComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2dComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2dComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2dComponent.RestitutionThreshold;

			out << YAML::EndMap;
		}
	}

	template<>
	static void DeserializeEntityComponent<BoxCollider2DComponent>(YAML::detail::iterator_value& entity, Entity& deserializedEntity)
	{
		auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
		if (boxCollider2DComponent)
		{
			auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>().GetComponent<BoxCollider2DComponent>();
			bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
			bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
			bc2d.Density = boxCollider2DComponent["Density"].as<float>();
			bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
			bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
			bc2d.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
		}
	}

	template<>
	static void SerializeEntityComponent<CircleCollider2DComponent>(YAML::Emitter& out, Entity entity)
	{
		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap;

			auto& cc2dComponent = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << cc2dComponent.Offset;
			out << YAML::Key << "Radius" << YAML::Value << cc2dComponent.Radius;
			out << YAML::Key << "Density" << YAML::Value << cc2dComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << cc2dComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << cc2dComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2dComponent.RestitutionThreshold;

			out << YAML::EndMap;
		}
	}

	template<>
	static void DeserializeEntityComponent<CircleCollider2DComponent>(YAML::detail::iterator_value& entity, Entity& deserializedEntity)
	{
		auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
		if (circleCollider2DComponent)
		{
			auto& cc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>().GetComponent<CircleCollider2DComponent>();
			cc2d.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
			cc2d.Radius = circleCollider2DComponent["Radius"].as<float>();
			cc2d.Density = circleCollider2DComponent["Density"].as<float>();
			cc2d.Friction = circleCollider2DComponent["Friction"].as<float>();
			cc2d.Restitution = circleCollider2DComponent["Restitution"].as<float>();
			cc2d.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
		}
	}

	template<>
	static void SerializeEntityComponent<NativeScriptComponent>(YAML::Emitter& out, Entity entity)
	{
		// TODO
	}

	template<>
	static void DeserializeEntityComponent<NativeScriptComponent>(YAML::detail::iterator_value&, Entity&)
	{
		// TODO
	}

	template<typename... Component>
	static void SerializeEntityComponents(YAML::Emitter& out, Entity entity)
	{
		(SerializeEntityComponent<Component>(out, entity), ...);
	}

	template<typename... Component>
	static void DeserializeEntityComponents(YAML::detail::iterator_value& entity, Entity& deserializedEntity)
	{
		(DeserializeEntityComponent<Component>(entity, deserializedEntity), ...);
	}

	template<typename... PrefixComponent, typename... Component>
	static void SerializeEntityComponents(ComponentGroup<Component...>, YAML::Emitter& out, Entity entity)
	{
		SerializeEntityComponents<PrefixComponent..., Component...>(out, entity);
	}

	template<typename... PrefixComponent, typename... Component>
	static void DeserializeEntityComponents(ComponentGroup<Component...>, YAML::detail::iterator_value& entity, Entity& deserializedEntity)
	{
		DeserializeEntityComponents<PrefixComponent..., Component...>(entity, deserializedEntity);
	}

	static void SerializeAllEntityComponents(YAML::Emitter& out, Entity entity)
	{
		SerializeEntityComponents<TagComponent>(AllComponents{}, out, entity);
	}

	static void DeserializeAllEntityComponents(YAML::detail::iterator_value& entity, Entity& deserializedEntity)
	{
		DeserializeEntityComponents(AllComponents{}, entity, deserializedEntity);
	}

}
