#include "eospch.h"
#include "Eos/Renderer/SubTexture2D.h"

namespace Eos {

	// note: min & max are between 0 and 1; min is bottom left, max top right
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		const glm::vec2 min = { coords.x * cellSize.x / texture->GetWidth(), coords.y * cellSize.y / texture->GetHeight() };
		const glm::vec2 max = { (coords.x + spriteSize.x) * cellSize.x / texture->GetWidth(), (coords.y + spriteSize.y) * cellSize.y / texture->GetHeight() };
		return CreateRef<SubTexture2D>(texture, min, max);
	}

}
