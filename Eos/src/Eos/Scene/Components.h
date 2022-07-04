#pragma once

#include "Eos/Scene/SceneCamera.h"
#include "Eos/Core/UID.h"
#include "Eos/Renderer/Texture.h"

#include "Eos/Scene/NativeScript.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <functional>
#include <memory>
#include <unordered_map>

namespace Eos {

	struct IDComponent
	{
		UID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Translation) * glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);
		};
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		// ------ TEMP -------
		bool FlipX = false, FlipY = false;
		bool Atlas = false;
		glm::vec2 Coords = { 0, 0 };
		glm::vec2 CellSize = { 32, 32 };
		glm::vec2 SpriteSize = { 1, 1 };
		// ------ TEMP -------

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
		SpriteRendererComponent(const Ref<Texture2D>& texture, const glm::vec4& tintColor = { 1, 1, 1, 1 }, float tilingFactor = 1.0f)
			: Texture(texture), Color(tintColor), TilingFactor(tilingFactor) {}
	};

	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = false; // TODO: think about moving to scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		std::unordered_map<entt::id_type, std::function<std::shared_ptr<NativeScript>(Entity entity)>> InstantiateScripts;
		std::unordered_map<entt::id_type, std::shared_ptr<NativeScript>> Instances;

		template<typename T, typename... Args>
		NativeScriptComponent& Bind(Args... args)
		{
			InstantiateScripts.try_emplace(entt::type_id<T>().hash(), [args...](Entity entity)->std::shared_ptr<NativeScript> { return std::make_shared<T>(entity, args...); });
			return *this;
		}

		NativeScriptComponent& Instantiate(Entity entity)
		{
			for (const auto& [id, instantiateScript] : InstantiateScripts)
			{
				Instances.try_emplace(id, instantiateScript(entity));
			}
			return *this;
		}

		template<typename T>
		T* GetInstance()
		{
			return Instances.at[entt::type_hash<T>];
		}

		void OnUpdate(Timestep ts)
		{
			for (auto& [id, instance] : Instances)
			{
				instance->OnUpdate(ts);
			}
		}
	};

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };
		float Rotation = 0.0f;

		// TODO: move into physics material in the future
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		// TODO: move into physics material in the future
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};

	template<typename... Component>
	struct ComponentGroup {};

	using AllComponents = ComponentGroup<TransformComponent, SpriteRendererComponent, CircleRendererComponent, CameraComponent, NativeScriptComponent, Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent>;

}
