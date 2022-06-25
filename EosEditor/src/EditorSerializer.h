#pragma once

#include "EditorLayer.h"

namespace Eos {

	class EditorSerializer
	{
	public:
		EditorSerializer(EditorSettings* settings);
		~EditorSerializer() = default;

		void Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);
	private:
		EditorSettings* m_Settings;
	};

}
