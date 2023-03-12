#include "eospch.h"
#include "Eos/Renderer/Texture.h"

#include "Eos/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Eos {

	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	EOS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(specification);
		}

		EOS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	EOS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(path);
		}

		EOS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
