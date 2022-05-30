#pragma once

#include "Hazel/Scene/Scene.h"

#include <filesystem>

namespace Hazel {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);
		~SceneSerializer() = default;

		void Serialize(const std::filesystem::path& filepath);
		void SerializeRuntime(const std::filesystem::path& filepath); // TODO: implement

		bool Deserialize(const std::filesystem::path& filepath);
		bool DeserializeRuntime(const std::filesystem::path& filepath);
	private:
		Ref<Scene> m_Scene;
	};

}
