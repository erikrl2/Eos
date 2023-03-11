#pragma once

#include "Project.h"

namespace Eos {

	class ProjectSerializer
	{
	public:
		ProjectSerializer(Ref<Project> project);

		void Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);
	private:
		Ref<Project> m_Project;
	};

}
