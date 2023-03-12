#pragma once

#include <filesystem>

#include "Eos/Core/Base.h"
#include "Eos/Renderer/Texture.h"

namespace Eos {

	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }
	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	};

}
